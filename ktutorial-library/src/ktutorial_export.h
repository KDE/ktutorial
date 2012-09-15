/***************************************************************************
 *   Copyright (C) 2008 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#ifndef KTUTORIAL_EXPORT_H
#define KTUTORIAL_EXPORT_H

/* needed for KDE_EXPORT and KDE_IMPORT macros */
#include <kdemacros.h>

#ifndef KTUTORIAL_EXPORT
# if defined(MAKE_KTUTORIAL_LIB)
   /* We are building this library */
#  define KTUTORIAL_EXPORT KDE_EXPORT
# else
   /* We are using this library */
#  define KTUTORIAL_EXPORT KDE_IMPORT
# endif
#endif

# ifndef KTUTORIAL_EXPORT_DEPRECATED
#  define KTUTORIAL_EXPORT_DEPRECATED KDE_DEPRECATED KTUTORIAL_EXPORT
# endif

#endif
