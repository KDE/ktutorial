/***************************************************************************
 *   Copyright (C) 2007 by Matt Williams <matt@milliams.com>               *
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>
#include <KTextEdit>

class MainWindow: public KXmlGuiWindow {
Q_OBJECT
public:

    explicit MainWindow(QWidget* parent = 0);

private:

    KTextEdit* textArea;
    void setupActions();
    QString fileName;

private slots:

    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void saveFileAs(const QString& outputFileName);
};

#endif
