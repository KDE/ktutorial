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

#include "Tutorial.h"

#include <QStringList>

#include "Step.h"

//public:

Tutorial::Tutorial(QObject* parent): QObject(parent) {
}

Tutorial::~Tutorial() {
    qDeleteAll(mSteps);
}

QString Tutorial::id() const {
    return toLowerCamelCase(mName);
}

QString Tutorial::name() const {
    return mName;
}

void Tutorial::setName(const QString& name) {
    mName = name;

    emit dataChanged(this);
}

QString Tutorial::description() const {
    return mDescription;
}

void Tutorial::setDescription(const QString& description) {
    mDescription = description;

    emit dataChanged(this);
}

QString Tutorial::licenseText() const {
    return mLicenseText;
}

void Tutorial::setLicenseText(const QString& licenseText) {
    mLicenseText = licenseText;

    emit dataChanged(this);
}

QString Tutorial::customSetupCode() const {
    return mCustomSetupCode;
}

void Tutorial::setCustomSetupCode(const QString& code) {
    mCustomSetupCode = code;

    emit dataChanged(this);
}

QString Tutorial::customTearDownCode() const {
    return mCustomTearDownCode;
}

void Tutorial::setCustomTearDownCode(const QString& code) {
    mCustomTearDownCode = code;

    emit dataChanged(this);
}

void Tutorial::addStep(Step* step) {
    addStep(step, mSteps.count());
}

void Tutorial::addStep(Step* step, int index) {
    Q_ASSERT(!mSteps.contains(step));

    mSteps.insert(index, step);

    emit stepAdded(step, index);
}

QList<Step*> Tutorial::steps() const {
    return mSteps;
}

void Tutorial::removeStep(Step* step) {
    Q_ASSERT(mSteps.contains(step));

    mSteps.removeOne(step);

    emit stepRemoved(step);
}

//private:

QString Tutorial::toLowerCamelCase(const QString& text) const {
    QStringList words = text.split(' ', QString::SkipEmptyParts);

    if (words.count() == 0) {
        return "";
    }

    QString lowerCamelCase;
    words[0][0] = words[0][0].toLower();
    lowerCamelCase += words[0];

    for (int i=1; i<words.count(); ++i) {
        words[i][0] = words[i][0].toUpper();
        lowerCamelCase += words[i];
    }

    return lowerCamelCase;
}
