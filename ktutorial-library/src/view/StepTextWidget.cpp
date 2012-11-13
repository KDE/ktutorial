/***************************************************************************
 *   Copyright (C) 2010 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#include "StepTextWidget.h"

#include <QContextMenuEvent>
#include <QMenu>

#include <KDebug>
#include <KLocalizedString>

#include "../KTutorial.h"
#include "../extendedinformation/WidgetHighlighterManager.h"

using ktutorial::extendedinformation::WidgetHighlighterManager;

namespace ktutorial {
extern int debugArea();
}

namespace ktutorial {
namespace view {

//public:

StepTextWidget::StepTextWidget(QWidget* parent /*= 0*/):
        KTextEdit(parent),
    mLastReferencedWidget(0),
    mWidgetBeingHighlighted(0) {
    setReadOnly(true);
    setFrameShape(QFrame::NoFrame);
    setFrameShadow(QFrame::Plain);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    viewport()->setCursor(Qt::ArrowCursor);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Base, Qt::transparent);
    setPalette(palette);

    setAlignment(Qt::AlignJustify | Qt::AlignVCenter);

    //Set a explicit text width to avoid some strange behavior: if the text
    //width is set to QWIDGETSIZE_MAX without a previous text width set, the
    //size returned by the document has weird values. Anyway, sizeHint is
    //usually called before minimumSizeHint, so a text width will be already set
    //when minimumSizeHint is called, but just in case.
    document()->setTextWidth(0);

    connect(this, SIGNAL(textChanged()), this, SLOT(updateText()));
}

StepTextWidget::~StepTextWidget() {
    if (mWidgetBeingHighlighted) {
        stopHighlightingCurrentWidget();
    }
}

bool StepTextWidget::eventFilter(QObject* watched, QEvent* event) {
    if (watched != mWidgetBeingHighlighted) {
        return false;
    }

    if (event->type() != QEvent::FocusIn) {
        return false;
    }

    stopHighlightingCurrentWidget();

    return false;
}

int StepTextWidget::heightForWidth(int width) const {
    return sizeForWidth(width).height();
}

QSize StepTextWidget::minimumSizeHint() const {
    QSize size;
    size.setHeight(sizeForWidth(QWIDGETSIZE_MAX).height());
    size.setWidth(sizeForWidth(0).width());

    return size;
}

QSize StepTextWidget::sizeHint() const {
    return sizeForWidth(-1);
}

//protected:

void StepTextWidget::contextMenuEvent(QContextMenuEvent* event) {
    QString anchor = anchorAt(event->pos());
    if (!anchor.startsWith(QLatin1String("widget:"))) {
        KTextEdit::contextMenuEvent(event);
        return;
    }

    QMenu* menu = new QMenu(this);

    mLastReferencedWidget = widgetForAnchor(anchor);
    if (mLastReferencedWidget != mWidgetBeingHighlighted) {
        menu->addAction(i18nc("@item:inmenu", "Highlight"),
                        this, SLOT(highlightLastReferencedWidget()));
    } else {
        menu->addAction(i18nc("@item:inmenu", "Stop highlighting"),
                        this, SLOT(stopHighlightingCurrentWidget()));
    }

    menu->exec(event->globalPos());
    delete menu;
}

void StepTextWidget::mouseMoveEvent(QMouseEvent* event) {
    KTextEdit::mouseMoveEvent(event);

    if (anchorAt(event->pos()).startsWith(QLatin1String("widget:"))) {
        viewport()->setCursor(Qt::PointingHandCursor);
    } else {
        viewport()->setCursor(Qt::ArrowCursor);
    }
}

void StepTextWidget::mousePressEvent(QMouseEvent* event) {
    QString anchor = anchorAt(event->pos());
    if (event->button() != Qt::LeftButton ||
            !anchor.startsWith(QLatin1String("widget:"))) {
        KTextEdit::mousePressEvent(event);
        return;
    }

    mLastReferencedWidget = widgetForAnchor(anchor);
    if (mLastReferencedWidget != mWidgetBeingHighlighted) {
        highlightLastReferencedWidget();
    } else {
        stopHighlightingCurrentWidget();
    }
}

//private:

QSize StepTextWidget::sizeForWidth(int width) const {
    const qreal oldTextWidth = document()->textWidth();

    if (width >= 0) {
        document()->setTextWidth(width);
    } else {
        document()->adjustSize();
    }

    QSize size = document()->size().toSize();

    document()->setTextWidth(oldTextWidth);

    return size;
}

QWidget* StepTextWidget::widgetForAnchor(const QString& anchor) {
    QString widgetName = anchor.mid(QString("widget:").length());
    return KTutorial::self()->findObject<QWidget*>(widgetName);
}

//private slots:

void StepTextWidget::updateText() {
    updateGeometry();

    if (mWidgetBeingHighlighted) {
        stopHighlightingCurrentWidget();
    }
}

void StepTextWidget::highlightLastReferencedWidget() {
    if (!mLastReferencedWidget) {
        kWarning(debugArea()) << "The widget to highlight was not found!";
        return;
    }

    if (mWidgetBeingHighlighted) {
        stopHighlightingCurrentWidget();
    }

    WidgetHighlighterManager::self()->highlight(mLastReferencedWidget);

    mLastReferencedWidget->installEventFilter(this);
    mWidgetBeingHighlighted = mLastReferencedWidget;
}

void StepTextWidget::stopHighlightingCurrentWidget() {
    if (!mWidgetBeingHighlighted) {
        kWarning(debugArea()) << "The widget to stop highlighting was not"
                              << "found!";
        return;
    }

    WidgetHighlighterManager::self()->stopHighlighting(mWidgetBeingHighlighted);

    mWidgetBeingHighlighted->removeEventFilter(this);
    mWidgetBeingHighlighted = 0;
}

}
}
