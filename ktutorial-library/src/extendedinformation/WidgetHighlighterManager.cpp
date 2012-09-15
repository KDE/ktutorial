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

#include <QWidget>

#include "WidgetHighlighterManager.h"
#include "WidgetHighlighter.h"

namespace ktutorial {
namespace extendedinformation {

//public:

WidgetHighlighterManager* WidgetHighlighterManager::self() {
    return sSelf;
}

void WidgetHighlighterManager::highlight(QWidget* widget) {
    WidgetHighlighter* highlighter = widget->findChild<WidgetHighlighter*>();
    if (highlighter && !widget->children().contains(highlighter)) {
        stopHighlighting(highlighter->parentWidget());
        highlighter = 0;
    }

    if (!highlighter) {
        highlighter = new WidgetHighlighter(widget);
        connect(highlighter, SIGNAL(stopped(extendedinformation::WidgetHighlighter*)),
                this, SLOT(remove(extendedinformation::WidgetHighlighter*)));
    }

    highlighter->start();
}

void WidgetHighlighterManager::stopHighlighting(QWidget* widget) {
    WidgetHighlighter* highlighter = widget->findChild<WidgetHighlighter*>();
    if (!highlighter || !widget->children().contains(highlighter)) {
        return;
    }

    highlighter->stop();
}

//private:

WidgetHighlighterManager* WidgetHighlighterManager::sSelf =
                                                new WidgetHighlighterManager();

WidgetHighlighterManager::WidgetHighlighterManager(): QObject() {
}

//private slots:

void WidgetHighlighterManager::remove(
                        extendedinformation::WidgetHighlighter* highlighter) {
    highlighter->setParent(0);
    delete highlighter;
}

}
}
