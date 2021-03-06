/*
 * This file is part of the KDE project.
 *
 * Copyright (C) 2017 Anthony Fieroni <bvbfan@abv.bg>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef KIO_PATHHELPERS_P_H
#define KIO_PATHHELPERS_P_H

#include <QString>

inline
QString concatPaths(const QString &path1, const QString &path2)
{
    Q_ASSERT(!path2.startsWith(QLatin1Char('/')));

    if (path1.isEmpty()) {
        return path2;
    } else if (!path1.endsWith(QLatin1Char('/'))) {
        return path1 + QLatin1Char('/') + path2;
    } else {
        return path1 + path2;
    }
}

#endif // KIO_PATHHELPERS_P_H
