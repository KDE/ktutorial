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

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <QString>

/**
 * Base class for exceptions.
 */
class Exception: public std::exception {
public:

    explicit Exception(const QString& message = QString());
    virtual ~Exception() throw();

    /**
     * Returns the exception message.
     *
     * @return The exception message.
     */
    virtual const char* what() const throw();

    /**
     * Returns the exception message.
     *
     * @return The exception message.
     */
    QString message() const throw();

private:

    QString mMessage;

};

#endif
