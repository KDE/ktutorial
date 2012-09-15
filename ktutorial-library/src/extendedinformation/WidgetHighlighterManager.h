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

#ifndef KTUTORIAL_EXTENDEDINFORMATION_WIDGETHIGHLIGHTERMANAGER_H
#define KTUTORIAL_EXTENDEDINFORMATION_WIDGETHIGHLIGHTERMANAGER_H

#include <QHash>
#include <QObject>

namespace ktutorial {
namespace extendedinformation {
class WidgetHighlighter;
}
}

namespace ktutorial {
namespace extendedinformation {

/**
 * Utility class to manage WidgetHighlighters.
 * Starting and stopping the highlighting from WidgetHighlighterManager instead
 * of using directly WidgetHighlighter ensures that no overlapping animations
 * will be executed.
 *
 * WidgetHighlighterManager also takes care of deleting the WidgetHighlighter
 * when no longer needed, that is, when they are fully stopped (once the stop
 * animation has ended).
 *
 * @see WidgetHighlighter
 */
class WidgetHighlighterManager: public QObject {
Q_OBJECT
public:

    /**
     * Returns the only instance of this class.
     *
     * @return The only instance of this class.
     */
    static WidgetHighlighterManager* self();

    /**
     * Starts a WidgetHighlighter for the given widget.
     * If the widget was already being highlighted nothing is done. If a child
     * widget was already being highlighted it is stopped, so only the parent
     * highlighting is active.
     *
     * @param widget The widget to highlight.
     */
    void highlight(QWidget* widget);

    /**
     * Stops the WidgetHighlighter of the given widget.
     *
     * @param widget The widget to stop highlighting.
     */
    void stopHighlighting(QWidget* widget);

private:

    /**
     * The instance of this class.
     */
    static WidgetHighlighterManager* sSelf;

    /**
     * Creates a new WidgetHighlighterManager.
     * Private to avoid classes other than self to create instances.
     */
    WidgetHighlighterManager();

private Q_SLOTS:

    /**
     * Removes the highlighter from its widget and destroys it.
     * Called when the highlighter stopped.
     *
     * @param highlighter The highlighter to remove.
     */
    void remove(extendedinformation::WidgetHighlighter* highlighter);

};

}
}

#endif
