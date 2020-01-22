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

#include "IDSearcher.hpp"

constexpr quint32 MAG[2] = { 0, 0x9908b0df };

IDSearcher::IDSearcher(const QVector<quint16> &tidFilter, const QVector<quint16> &sidFilter, quint32 minDelay, quint32 maxDelay,
                       bool infinite, quint64 maxProgress) :
    tidFilter(tidFilter),
    sidFilter(sidFilter),
    minDelay(minDelay),
    maxDelay(maxDelay),
    infinite(infinite),
    cancel(false),
    progress(0),
    maxProgress(maxProgress)
{
}

void IDSearcher::run()
{
    uint32_t hour = infinite ? 256 : 24;
    minDelay = infinite ? 0 : minDelay;
    maxDelay = infinite ? 0x10000 : maxDelay;

    for (uint32_t ab = 0; ab < 256; ab++)
    {
        for (uint32_t cd = 0; cd < hour; cd++)
        {
            for (uint32_t efgh = minDelay; efgh < maxDelay; efgh++)
            {
                if (cancel)
                {
                    return;
                }

                uint32_t seed = ((ab << 24) | (cd << 16)) + efgh;
                uint32_t id = getID(seed);

                uint16_t tid = id & 0xffff;
                uint16_t sid = id >> 16;

                if (tidFilter.contains(tid) && sidFilter.contains(sid))
                {
                    IDResult result(tid, sid, seed);

                    QMutexLocker locker(&mutex);
                    results.append(result);
                }

                progress++;
            }
        }
    }
}

int IDSearcher::currentProgress() const
{
    return (progress * 100) / maxProgress;
}

QVector<IDResult> IDSearcher::getResults()
{
    QMutexLocker locker(&mutex);
    auto data(results);
    results.clear();
    return data;
}

void IDSearcher::cancelSearch()
{
    cancel = true;
}

uint32_t IDSearcher::getID(uint32_t seed)
{
    // Gen 4 TID/SID use the 2nd call of Mersenne Twister
    // This method avoids making an array of 624 numbers and doing about half the math to get that full table
    uint32_t s1 = 0x6c078965 * (seed ^ (seed >> 30)) + 1;
    uint32_t s2 = 0x6c078965 * (s1 ^ (s1 >> 30)) + 2;

    uint32_t s398 = s2;
    for (uint16_t i = 3; i < 399; i++)
    {
        s398 = 0x6c078965 * (s398 ^ (s398 >> 30)) + i;
    }

    uint32_t y = (s1 & 0x80000000) | (s2 & 0x7fffffff);
    s1 = s398 ^ (y >> 1) ^ MAG[y & 1];

    y = s1 ^ (s1 >> 11);
    y ^= ((y << 7) & 0x9d2c5680);
    y ^= ((y << 15) & 0xefc60000);
    y ^= (y >> 18);

    return y;
}
