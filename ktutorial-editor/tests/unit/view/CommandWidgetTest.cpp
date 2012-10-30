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

#include <QTest>

#include "CommandWidget.h"

#include <QUndoCommand>

class CommandWidgetTest: public QObject {
Q_OBJECT

private slots:

    void testSaveChangesNoParentAndNoStack();
    void testSaveChangesParentAndNoStack();
    void testSaveChangesNoParentAndStack();
    void testSaveChangesNoParentAndStackWithSingleCommand();
    void testSaveChangesNoParentAndStackWithNoCommands();
    void testSaveChangesParentAndStack();
    void testSaveChangesParentAndStackWithNoCommands();

};

class MockCommandWidget;

class MockCommand: public QUndoCommand {
public:

    MockCommandWidget* mWidget;
    int mIndex;

    MockCommand(QUndoCommand* parent, MockCommandWidget* widget, int index):
            QUndoCommand(parent),
        mWidget(widget),
        mIndex(index) {
    }

    virtual void redo();

};

class MockCommandWidget: public CommandWidget {
public:

    MockCommand* mCommand1;
    int mCommand1RedoCallCount;
    MockCommand* mCommand2;
    int mCommand2RedoCallCount;
    MockCommand* mCommand3;
    int mCommand3RedoCallCount;

    int mNumberOfCommands;

    MockCommandWidget(QWidget* parent = 0): CommandWidget(parent),
        mCommand1RedoCallCount(0),
        mCommand2RedoCallCount(0),
        mCommand3RedoCallCount(0),
        mNumberOfCommands(3) {
        setWindowTitle("The window title");
    }

protected:

    virtual QList<QUndoCommand*> createSaveCommands(QUndoCommand* parent) {
        QList<QUndoCommand*> commands;
        if (mNumberOfCommands >= 1) {
            mCommand1 = new MockCommand(parent, this, 0);
            mCommand1->setText("First command");
            commands.append(mCommand1);
        }

        if (mNumberOfCommands >= 2) {
            mCommand2 = new MockCommand(parent, this, 1);
            mCommand2->setText("Second command");
            commands.append(mCommand2);
        }

        if (mNumberOfCommands >= 3) {
            mCommand3 = new MockCommand(parent, this, 2);
            mCommand3->setText("Third command");
            commands.append(mCommand3);
        }

        return commands;
    }

};

//As commands are deleted after being executed when there is no stack and no
//parent the call count must be stored somewhere out of the commands themselves.
//The method must be defined here to avoid the compiler complaining about an
//incomplete type due to using the widget.
void MockCommand::redo() {
    if (mIndex == 0) {
        mWidget->mCommand1RedoCallCount++;
    } else if (mIndex == 1) {
        mWidget->mCommand2RedoCallCount++;
    } else if (mIndex == 2) {
        mWidget->mCommand3RedoCallCount++;
    }
}

void CommandWidgetTest::testSaveChangesNoParentAndNoStack() {
    MockCommandWidget widget;

    widget.saveChanges();

    QCOMPARE(widget.mCommand1RedoCallCount, 1);
    QCOMPARE(widget.mCommand2RedoCallCount, 1);
    QCOMPARE(widget.mCommand3RedoCallCount, 1);
}

void CommandWidgetTest::testSaveChangesParentAndNoStack() {
    MockCommandWidget widget;
    QUndoCommand parent;

    widget.setParentUndoCommand(&parent);
    widget.saveChanges();

    QCOMPARE(parent.child(0), widget.mCommand1);
    QCOMPARE(parent.child(1), widget.mCommand2);
    QCOMPARE(parent.child(2), widget.mCommand3);
    QCOMPARE(widget.mCommand1RedoCallCount, 0);
    QCOMPARE(widget.mCommand2RedoCallCount, 0);
    QCOMPARE(widget.mCommand3RedoCallCount, 0);
}

void CommandWidgetTest::testSaveChangesNoParentAndStack() {
    MockCommandWidget widget;
    QUndoStack stack;

    widget.setUndoStack(&stack);
    widget.saveChanges();

    QCOMPARE(stack.count(), 1);
    QCOMPARE(stack.index(), 1);
    const QUndoCommand* parent = stack.command(0);
    QCOMPARE(parent->text(), QString("The window title"));
    QCOMPARE(parent->child(0), widget.mCommand1);
    QCOMPARE(parent->child(1), widget.mCommand2);
    QCOMPARE(parent->child(2), widget.mCommand3);
    QCOMPARE(widget.mCommand1RedoCallCount, 1);
    QCOMPARE(widget.mCommand2RedoCallCount, 1);
    QCOMPARE(widget.mCommand3RedoCallCount, 1);
}

void CommandWidgetTest::testSaveChangesNoParentAndStackWithSingleCommand() {
    MockCommandWidget widget;
    widget.mNumberOfCommands = 1;
    QUndoStack stack;

    widget.setUndoStack(&stack);
    widget.saveChanges();

    QCOMPARE(stack.count(), 1);
    QCOMPARE(stack.index(), 1);
    const QUndoCommand* parent = stack.command(0);
    QCOMPARE(parent->text(), QString("First command"));
    QCOMPARE(parent->child(0), widget.mCommand1);
    QCOMPARE(widget.mCommand1RedoCallCount, 1);
}

void CommandWidgetTest::testSaveChangesNoParentAndStackWithNoCommands() {
    MockCommandWidget widget;
    widget.mNumberOfCommands = 0;
    QUndoStack stack;

    widget.setUndoStack(&stack);
    widget.saveChanges();

    QCOMPARE(stack.count(), 0);
}

void CommandWidgetTest::testSaveChangesParentAndStack() {
    MockCommandWidget widget;
    QUndoStack stack;
    QUndoCommand* parent = new QUndoCommand();
    parent->setText("Parent command");

    widget.setParentUndoCommand(parent);
    widget.setUndoStack(&stack);
    widget.saveChanges();

    QCOMPARE(stack.count(), 1);
    QCOMPARE(stack.index(), 1);
    QCOMPARE(stack.command(0), parent);
    QCOMPARE(parent->text(), QString("Parent command"));
    QCOMPARE(parent->child(0), widget.mCommand1);
    QCOMPARE(parent->child(1), widget.mCommand2);
    QCOMPARE(parent->child(2), widget.mCommand3);
    QCOMPARE(widget.mCommand1RedoCallCount, 1);
    QCOMPARE(widget.mCommand2RedoCallCount, 1);
    QCOMPARE(widget.mCommand3RedoCallCount, 1);
}

void CommandWidgetTest::testSaveChangesParentAndStackWithNoCommands() {
    MockCommandWidget widget;
    widget.mNumberOfCommands = 0;
    QUndoStack stack;
    QUndoCommand* parent = new QUndoCommand();
    parent->setText("Parent command");

    widget.setParentUndoCommand(parent);
    widget.setUndoStack(&stack);
    widget.saveChanges();

    QCOMPARE(stack.count(), 1);
    QCOMPARE(stack.index(), 1);
    QCOMPARE(stack.command(0), parent);
    QCOMPARE(parent->text(), QString("Parent command"));
}

/////////////////////////////////// Helpers ////////////////////////////////////

QTEST_MAIN(CommandWidgetTest)

#include "CommandWidgetTest.moc"
