/***************************************************************************
 *   Copyright (C) 2011 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#include "SemanticMarkupLinkWidget.h"

#include "ui_SemanticMarkupLinkWidget.h"

#ifdef QT_QTDBUS_FOUND
#include "RemoteObjectNameWidget.h"
#endif

//public:

SemanticMarkupLinkWidget::SemanticMarkupLinkWidget(QWidget* parent):
        QWidget(parent) {

    ui = new Ui::SemanticMarkupLinkWidget();
    ui->setupUi(this);

#ifdef QT_QTDBUS_FOUND
    mRemoteObjectNameWidget = new RemoteObjectNameWidget(this);

    //Replace ui->widgetLinkLineEdit with mRemoteObjectNameWidget
    ui->valueVerticalLayout->removeWidget(ui->widgetLinkLineEdit);
    delete ui->widgetLinkLineEdit;

    ui->valueVerticalLayout->insertWidget(1, mRemoteObjectNameWidget);
#endif

    ui->genericLinkRadioButton->setChecked(true);

    connect(ui->genericLinkRadioButton, SIGNAL(toggled(bool)),
            ui->genericLinkLineEdit, SLOT(setEnabled(bool)));

#ifdef QT_QTDBUS_FOUND
    mRemoteObjectNameWidget->setEnabled(false);

    connect(ui->widgetLinkRadioButton, SIGNAL(toggled(bool)),
            mRemoteObjectNameWidget, SLOT(setEnabled(bool)));
#else
    ui->widgetLinkLineEdit->setEnabled(false);

    connect(ui->widgetLinkRadioButton, SIGNAL(toggled(bool)),
            ui->widgetLinkLineEdit, SLOT(setEnabled(bool)));
#endif
}

SemanticMarkupLinkWidget::~SemanticMarkupLinkWidget() {
    delete ui;
}

QString SemanticMarkupLinkWidget::url() const {
    if (ui->genericLinkRadioButton->isChecked()) {
        return ui->genericLinkLineEdit->text();
    }

#ifdef QT_QTDBUS_FOUND
    return "widget:" + mRemoteObjectNameWidget->name();
#else
    return "widget:" + ui->widgetLinkLineEdit->text();
#endif
}

void SemanticMarkupLinkWidget::setUrl(const QString& url) {
    if (!url.startsWith(QLatin1String("widget:"))) {
        ui->genericLinkRadioButton->setChecked(true);
        ui->genericLinkLineEdit->setText(url);
        return;
    }

    ui->widgetLinkRadioButton->setChecked(true);
    QString widgetName = url.mid(QString("widget:").length());
#ifdef QT_QTDBUS_FOUND
    mRemoteObjectNameWidget->setName(widgetName);
#else
    ui->widgetLinkLineEdit->setText(widgetName);
#endif
}
