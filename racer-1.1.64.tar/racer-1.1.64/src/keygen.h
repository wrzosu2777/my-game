#ifndef _KEY_GEN_INCLUDED_
#define _KEY_GEN_INCLUDED_

int  GenerateKey(const char *file);
bool TestKeys();
void CreateKeyFiles();

#define KEY_FILE "data/racer.key"

#endif
