/***************************************************************************
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

#include "ActionListWidget.h"

#include <QAction>
#include <QActionEvent>
#include <QToolButton>
#include <QVBoxLayout>

/**
 * A button which always leave room for an icon, even if there is none, so that
 * all button texts are correctly aligned.
 *
 * Code got from Gwenview's app/sidebar.cpp file, licensed as GPL 2 or later
 * (at the time of KDE SVN revision 968729).
 */
class ForcedIconToolButton: public QToolButton {
protected:

    virtual void paintEvent(QPaintEvent* event) {
        forceIcon();
        QToolButton::paintEvent(event);
    }

private:

    void forceIcon() {
        if (!icon().isNull()) {
            return;
        }

        // Assign an empty icon to the button if there is no icon associated
        // with the action so that all button texts are correctly aligned.
        QSize wantedSize = iconSize();
        if (mEmptyIcon.isNull() ||
                mEmptyIcon.actualSize(wantedSize) != wantedSize) {
            QPixmap pix(wantedSize);
            pix.fill(Qt::transparent);
            mEmptyIcon.addPixmap(pix);
        }
        setIcon(mEmptyIcon);
    }

    QIcon mEmptyIcon;

};

//public:

ActionListWidget::ActionListWidget(QWidget* parent): QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->setMargin(0);
    layout->setSpacing(0);

    setLayout(layout);
}

//protected:

void ActionListWidget::actionEvent(QActionEvent* event) {
    if (event->type() == QEvent::ActionAdded) {
        addToolButtonForAction(event->action(),
                               toolButtonForAction(event->before()));
        return;
    }

    if (event->type() == QEvent::ActionRemoved) {
        QToolButton* button = toolButtonForAction(event->action());
        layout()->removeWidget(button);
        button->deleteLater();
        return;
    }
}

//private:

QToolButton* ActionListWidget::toolButtonForAction(QAction* action) {
    for (int i=0; i<layout()->count(); i++ ) {
        QWidget* widget = layout()->itemAt(i)->widget();
        if (widget) {
            QToolButton* button = static_cast<QToolButton*>(widget);
            if (button->defaultAction() == action) {
                return button;
            }
        }
    }

    return 0;
}

void ActionListWidget::addToolButtonForAction(QAction* action,
                                              QToolButton* before) {
    Q_ASSERT(action);

    QToolButton* button = new ForcedIconToolButton();
    button->setDefaultAction(action);
    button->setObjectName(action->objectName() + "ToolButton");
    button->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    button->setAutoRaise(true);
    button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    int index;
    if (before == 0) {
        index = layout()->count() - 1;
    } else {
        index = layout()->indexOf(before);
    }

    static_cast<QVBoxLayout*>(layout())->insertWidget(index, button);
}
