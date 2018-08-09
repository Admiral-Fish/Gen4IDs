/*
 * This file is part of gen4ids
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

#ifndef IDSEARCHER_HPP
#define IDSEARCHER_HPP

#include <QThread>

class IDSearcher : public QThread
{
    Q_OBJECT

signals:
    void result(quint32, quint32);

private:
    const quint32 MAG[2] = { 0, 0x9908b0df };
    quint32 tid;
    quint32 sid;
    quint32 minDelay;
    quint32 maxDelay;
    quint64 progress;
    quint64 maxResults;
    bool infinite;
    bool isSearching;
    bool cancel;

public:
    IDSearcher();
    void run() override;
    quint32 getID(quint32 seed);
    void setTID(const quint32 &value);
    void setSID(const quint32 &value);
    void setMinDelay(const quint32 &value);
    void setMaxDelay(const quint32 &value);
    void setInfinite(bool value);
    bool getIsSearching() const;
    void setIsSearching(bool value);
    bool getCancel() const;
    void setCancel(bool value);
    int calcProgress() const;

};

#endif // IDSEARCHER_HPP
