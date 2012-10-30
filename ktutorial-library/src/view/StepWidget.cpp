/***************************************************************************
 *   Copyright (C) 2008 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
 *   Copyright (C) 2009 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
 *   Copyright (C) 2010 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
 *   Copyright (C) 2012 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; If not, see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/

#include "StepWidget.h"

#include <QHBoxLayout>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWhatsThis>

#include <klocalizedstring.h>

#include "ui_StepWidget.h"
#include "WindowOnTopEnforcer.h"
#include "../Option.h"
#include "../Step.h"

namespace ktutorial {
namespace view {

//public:

StepWidget::StepWidget(const QString& tutorialName, QWidget* parent /*= 0*/):
        QFrame(parent, Qt::Dialog | Qt::FramelessWindowHint),
    mDragging(false) {

    setFrameShape(QFrame::Panel);
    setLineWidth(1);

    ui = new Ui::StepWidget();
    ui->setupUi(this);

    setWindowTitle(i18nc("@title:window", "Tutorial: %1", tutorialName));

    ui->closeButton->setIcon(KIcon("dialog-close"));

    QSize buttonSize = ui->closeButton->size();
    buttonSize.setWidth(buttonSize.height());
//     buttonSize.setWidth(ui.closeButton->iconSize().width());
//     buttonSize.setHeight(ui.closeButton->iconSize().height());

    ui->closeButton->setFixedSize(buttonSize);

    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));

    mOptionsLayout = new QHBoxLayout(ui->optionsWidget);
}

StepWidget::~StepWidget() {
    delete ui;
}

void StepWidget::setMainApplicationWindow(QWidget* mainApplicationWindow) {
    WindowOnTopEnforcer* windowOnTopEnforcer = new WindowOnTopEnforcer(this);
    windowOnTopEnforcer->setBaseWindow(mainApplicationWindow);
}

//public slots:

void StepWidget::setStep(Step* step) {
    ui->textWidget->setText(step->text());
    setOptions(step->options());

    adjustSize();

    //Shown after adjusting the size so it is centered on the widget. If shown
    //before, it'll use the ui file size, and it won't be centered
    if (isHidden()) {
        show();
    }
}

//protected:

void StepWidget::closeEvent(QCloseEvent* event) {
    QWidget::closeEvent(event);
    emit finished();
    deleteLater();
}

void StepWidget::keyPressEvent(QKeyEvent* event) {
    if (event->modifiers() == Qt::NoModifier &&
                event->key() == Qt::Key_Escape) {
        ui->closeButton->animateClick();
        event->accept();
        return;
    } else if (event->modifiers() == Qt::ShiftModifier &&
               event->key() == Qt::Key_F1) {
        QWhatsThis::enterWhatsThisMode();
        event->accept();
        return;
    }

    QWidget::keyPressEvent(event);
}

void StepWidget::mouseMoveEvent(QMouseEvent* event) {
    if (!mDragging || this != mouseGrabber()) {
        return;
    }

    move(event->globalPos() - mDragOffset);
}

void StepWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton || mDragging) {
        return;
    }

    mDragOffset = event->pos();

    grabMouse(Qt::SizeAllCursor);
    mDragging = true;
}

void StepWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton || !mDragging) {
        return;
    }

    mDragging = false;
    releaseMouse();
}

void StepWidget::paintEvent(QPaintEvent* event) {
    adjustSize();
    QFrame::paintEvent(event);
}

//private:

void StepWidget::setOptions(const QList<Option*>& options) {
    QList<KPushButton*> buttons = ui->optionsWidget->findChildren<KPushButton*>();
    QListIterator<KPushButton*> itButtons(buttons);
    while (itButtons.hasNext()) {
        KPushButton* button = itButtons.next();
        button->setParent(0);
        delete button;
    }

    QListIterator<Option*> it(options);
    while (it.hasNext()) {
        Option* option = it.next();

        KPushButton* button = new KPushButton(ui->optionsWidget);
        button->setText(option->name());
        mOptionsLayout->addWidget(button);

        connect(button, SIGNAL(clicked()), option, SIGNAL(selected()));
    }
}

}
}
