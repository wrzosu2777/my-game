#include "stdafx.h"
#include "keygen.h"
#include <iostream>
using namespace std;

#define MAX_CHECK_FILES 31


const char KeyFiles[MAX_CHECK_FILES][20] =
{
	{"racer.exe"},
	{"gfx.dll"  },
	{"sfx.dll"  },
	{"cars.dat" },
	{"cup.dat"  },
	{"drivars.dat"},
	{"objects.dat"},
	{"test1.edb"},
	{"test1.pcx"},
	{"test1.jpg"},
	{"test2.edb"},
	{"test2.pcx"},
	{"test2.jpg"},
	{"test3.edb"},
	{"test3.pcx"},
	{"test3.jpg"},
	{"test4.edb"},
	{"test4.pcx"},
	{"test4.jpg"},
	{"test5.edb"},
	{"test5.pcx"},
	{"test5.jpg"},
	{"test6.edb"},
	{"test6.pcx"},
	{"test6.jpg"},
	{"test7.edb"},
	{"test7.pcx"},
	{"test7.jpg"},
	{"test8.edb"},
	{"test8.pcx"},
	{"test8.jpg"},
};


int  GenerateKey(const char *file)
{
	FILE *f = fopen(file, "rb");
	int ret = 0, i = 2;
	if (f) {
		while (fread(&i, sizeof(int), 1, f) == 1) ret += i;
		fclose(f);
	}
	return ret;
}

bool TestKeys()
{
	FILE *f = fopen(KEY_FILE, "rb");

	if (f == NULL) return false;

	int mkey;
	for (int i = 0; i < MAX_CHECK_FILES; i++) {
		if (fread(&mkey, sizeof(int), 1, f) == 0)
        {
            cerr << "fread failed" << endl;
        }
		if (mkey != GenerateKey(KeyFiles[i])) return false;
	}

	fclose(f);

	return true;
}

void CreateKeyFiles()
{
	FILE *f = fopen(KEY_FILE, "wb");

	if (f == NULL) return;

	int mkey;
	for (int i = 0; i < MAX_CHECK_FILES; i++) {
		mkey = GenerateKey(KeyFiles[i]);
		fwrite(&mkey, sizeof(int), 1, f);
	}

	fclose(f);
}
