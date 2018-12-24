/*
 * This file is part of Gen4IDs
 * Copyright (C) 2018 by Admiral_Fish
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef IDRESULT_HPP
#define IDRESULT_HPP

#include <QtGlobal>

class IDResult
{

public:
    IDResult() = default;
    IDResult(quint16 tid, quint16 sid, quint32 seed);
    quint16 getTID() const;
    quint16 getSID() const;
    quint32 getSeed() const;

private:
    quint16 tid;
    quint16 sid;
    quint32 seed;

};

#endif // IDRESULT_HPP
