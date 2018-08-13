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

IDSearcher::IDSearcher()
{
    isSearching = false;
    cancel = false;
}

void IDSearcher::run()
{
    isSearching = true;
    cancel = false;
    progress = 0;

    if (infinite)
        maxResults = 0x100000000;
    else
        maxResults = 256 * 24 * (maxDelay - minDelay);

    quint32 hour = infinite ? 256 : 24;
    quint32 delay = infinite ? 0x10000 : maxDelay;

    for (quint32 ab = 0; ab < 256; ab++)
    {
        for (quint32 cd = 0; cd < hour; cd++)
        {
            for (quint32 efgh = infinite ? 0 : minDelay; efgh < delay; efgh++)
            {
                quint32 seed = ((ab << 24) | (cd << 16)) + efgh;
                quint32 id = getID(seed);

                if ((id & 0xffff) == tid && (id >> 16) == sid)
                    emit result(seed, id);

                if (cancel)
                {
                    isSearching = false;
                    return;
                }

                progress++;
            }
        }
    }
    isSearching = false;
}

quint32 IDSearcher::getID(quint32 seed)
{
    // Gen 4 TID/SID use the 2nd call of Mersenne Twister
    // This method avoids making an array of 624 numbers and doing about half the math to get that full table
    quint32 s1 = 0x6c078965 * (seed ^ (seed >> 30)) + 1;
    quint32 s2 = 0x6c078965 * (s1 ^ (s1 >> 30)) + 2;

    quint32 s398 = s2;
    for (quint32 i = 3; i < 399; i++)
        s398 = 0x6c078965 * (s398 ^ (s398 >> 30)) + i;

    quint32 y = (s1 & 0x80000000) | (s2 & 0x7fffffff);
    s1 = s398 ^ (y >> 1) ^ MAG[y & 1];

    y = s1 ^ (s1 >> 11);
    y ^= ((y << 7) & 0x9d2c5680);
    y ^= ((y << 15) & 0xefc60000);
    y ^= (y >> 18);

    return y;
}

void IDSearcher::setTID(const quint32 &value)
{
    tid = value;
}

void IDSearcher::setSID(const quint32 &value)
{
    sid = value;
}

void IDSearcher::setMinDelay(const quint32 &value)
{
    minDelay = value;
}

void IDSearcher::setMaxDelay(const quint32 &value)
{
    maxDelay = value;
}

void IDSearcher::setInfinite(bool value)
{
    infinite = value;
}

bool IDSearcher::getIsSearching() const
{
    return isSearching;
}

void IDSearcher::setIsSearching(bool value)
{
    isSearching = value;
}

bool IDSearcher::getCancel() const
{
    return cancel;
}

void IDSearcher::setCancel(bool value)
{
    cancel = value;
}

int IDSearcher::calcProgress() const
{
    return static_cast<int>((progress * 100) / maxResults);
}
