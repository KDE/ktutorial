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

#ifndef SEMANTICMARKUPLINKWIDGET_H
#define SEMANTICMARKUPLINKWIDGET_H

#include <QWidget>

#ifdef QT_QTDBUS_FOUND
class RemoteObjectNameWidget;
#endif

namespace Ui {
class SemanticMarkupLinkWidget;
}

/**
 * Helper widget for the semantic markup edition to input the URL of a link.
 * The widget contains a line edit where any URL can be written. However, it
 * also offers support for a specific type of URLs: widget URLs. There are two
 * radio buttons to select between a generic URL and a widget URL.
 *
 * Widget URLs represent a widget in the target application. When DBus module is
 * enabled, the widget to link to can be selected using a RemoteObjectChooser.
 *
 * When the URL is got, the URL returned depends on the current type of URL
 * selected. Widget URLs also contain the "widget:" protocol. When the URL is
 * set, the line edit filled and radio button checked depend on the type of URL
 * set.
 */
class SemanticMarkupLinkWidget: public QWidget {
Q_OBJECT
public:

    /**
     * Creates a new SemanticMarkupLinkWidget.
     *
     * @param parent The parent QWidget.
     */
    explicit SemanticMarkupLinkWidget(QWidget* parent = 0);

    /**
     * Destroys this widget.
     */
    virtual ~SemanticMarkupLinkWidget();

    /**
     * Returns the URL.
     * If the URL is a widget URL, the returned URL contains the "widget:"
     * protocol.
     *
     * @return The URL.
     */
    QString url() const;

    /**
     * Sets the URL.
     * If the URL is a widget URL, the "widget:" protocol is not shown in its
     * line edit.
     *
     * @param url The URL to set.
     */
    void setUrl(const QString& url);

private:

    /**
     * The Ui Designer generated class.
     */
    Ui::SemanticMarkupLinkWidget* ui;

#ifdef QT_QTDBUS_FOUND
    /**
     * The widget to get the name of a remote object.
     */
    RemoteObjectNameWidget* mRemoteObjectNameWidget;
#endif

};

#endif
