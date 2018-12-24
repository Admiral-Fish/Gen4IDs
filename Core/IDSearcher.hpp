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

#ifndef IDSEARCHER_HPP
#define IDSEARCHER_HPP

#include <QMutex>
#include <QThread>
#include <QVector>
#include <Core/IDResult.hpp>

class IDSearcher : public QThread
{
    Q_OBJECT

public:
    IDSearcher(const QVector<quint16> &tidFilter, const QVector<quint16> &sidFilter, quint32 minDelay, quint32 maxDelay, bool infinite, quint64 maxProgress);
    void run() override;
    int currentProgress() const;
    QVector<IDResult> getResults();

public slots:
    void cancelSearch();

private:
    const quint32 MAG[2] = { 0, 0x9908b0df };
    QVector<quint16> tidFilter;
    QVector<quint16> sidFilter;
    quint32 minDelay;
    quint32 maxDelay;
    bool infinite;

    QVector<IDResult> results;
    QMutex mutex;
    bool cancel;
    quint64 progress;
    quint64 maxProgress;

    quint32 getID(quint32 seed);

};

#endif // IDSEARCHER_HPP
