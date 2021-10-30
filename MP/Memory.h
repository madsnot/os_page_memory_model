#pragma once

void Init();
int AllocMem(int sz);
void FreeMem(int handle);
void SwapPages(int notvisPage, int visPage);
int WriteMem(int handle, int offset, int size, const char* data);
int ReadMem(int handle, int offset, int size, char* data);
void Dump(void);