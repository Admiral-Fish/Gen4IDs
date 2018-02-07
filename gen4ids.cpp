﻿// gen4ids.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdint.h"
#include <iostream>
using namespace std;
typedef uint32_t u32;

u32 mag[2] = { 0, 0x9908b0df };
void checkCombos(u32, u32, u32, u32);
u32 getIDs(u32);


int main()
{
	cout << "Gen 4 TID/SID calculator by Admiral_Fish" << endl << endl;
	u32 tid, sid, min, max;
	int go = 1;

	while (go == 1)
	{
		cout << "Enter target TID: ";
		cin >> tid;

		cout << "Enter target SID: ";
		cin >> sid;

		cout << "Enter minimum delay: ";
		cin >> min;

		cout << "Enter maximum delay: ";
		cin >> max;

		checkCombos(tid, sid, min, max);

		cout << "Search again? 1/0" << endl;
		cin >> go;
		cout << endl;
	}

	return 0;
}

void checkCombos(u32 tid, u32 sid, u32 min, u32 max)
{
	// Cap max delay, anything above gives duplicate results.
	// Cap determined by cd being range of 0x0-0x17
	// 0xFF - 0x17 = 0xE8
	max = max > 0xEFFFFF ? 0xEFFFFF : max;
	
	for (u32 efgh = min; efgh <= max; efgh++)
	{
		for (u32 ab = 0; ab < 0x100; ab++)
		{
			for (u32 cd = 0; cd < 0x18; cd++)
			{
				u32 seed = (ab << 24) + (cd << 16) + efgh;
				u32 ids = getIDs(seed);

				if ((tid == (ids & 0xffff)) && (sid == (ids >> 16)))
				{
					cout << endl << "This seed will give you the combo: 0x";
					cout << hex << seed << endl << endl;

					cout << "Continue searching? 1/0" << endl;
					int x;
					cin >> x;
					if (x == 0)
						return;
				}
			}
		}
	}

	cout << endl << "Finished" << endl;
}

u32 getIDs(u32 seed)
{
	u32 s0 = seed;
	u32 s1 = 0x6c078965 * (s0 ^ (s0 >> 30)) + 1;
	u32 s2 = 0x6c078965 * (s1 ^ (s1 >> 30)) + 2;

	u32 s398 = s2;
	for (int i = 3; i < 399; i++)
		s398 = 0x6c078965 * (s398 ^ (s398 >> 30)) + i;

	u32 y = (s1 & 0x80000000) | (s2 & 0x7fffffff);
	s1 = s398 ^ (y >> 1) ^ mag[y & 1];

	u32 ret = s1 ^ (s1 >> 11);
	ret ^= ((ret << 7) & 0x9d2c5680);
	ret ^= ((ret << 15) & 0xefc60000);
	ret ^= (ret >> 18);

	return ret;
}
