#include <iostream>
#include <fstream>
#include <set>
#include "Page.h"
#include "Memory.h"
using namespace std;

#define ALL_MEM_PAGES 30
#define VIRT_MEM_PAGES 20
#define PAGE_SIZE 1000
#define MEM_SIZE 10000

int Handle;
const int workPages = ALL_MEM_PAGES - VIRT_MEM_PAGES;
char mem[MEM_SIZE];
const char *fileName = "VMemory.txt";
Page page[ALL_MEM_PAGES];

void Init()
{
	ofstream file(fileName);
	if (file.is_open())
	{
		file.width(VIRT_MEM_PAGES * PAGE_SIZE);
		file << " ";
	}
	file.close();
}

int AllocMem(int sz)
{
	int i, j, pages = 1, countBusy = 0, countPages;
	while (sz > pages * PAGE_SIZE)
		pages++;
	for (i = 0; i < ALL_MEM_PAGES; i++)
		if (page[i].busy)
			countBusy++;
	if (ALL_MEM_PAGES - countBusy < pages)
		return 0;
	countPages = 0;
	Handle++;
	for (i = 0; i < ALL_MEM_PAGES; i++)
	{
		if (!page[i].busy)
		{
			page[i].handle = Handle;
			page[i].busy = true;
			countPages++;
			page[i].logical = countPages;
			if (i < workPages)
			{
				for (j = i * PAGE_SIZE; j < (i+1) * PAGE_SIZE; j++)
					mem[j] = ' ';
			}
		}
		if (countPages == pages)
			break;
	}
	return Handle;
}

void SwapPages(int notvisPage, int visPage)
{
	#define STEP -1
	int handle, logical, busy, i;
	char data;
	fstream file(fileName);
	if (file.is_open())
	{
		file.seekg((notvisPage - workPages) * PAGE_SIZE, ios_base::beg);
		for (i = visPage * PAGE_SIZE; i < (visPage + 1) * PAGE_SIZE; i++)
		{
			data = file.get();
			file.seekg(STEP, ios_base::cur);
			file.put(mem[i]);
			mem[i] = data;
		}
		cout << endl;
	}
	file.close();
	handle = page[visPage].handle;
	logical = page[visPage].logical;
	busy = page[visPage].busy;
	page[visPage].handle = page[notvisPage].handle;
	page[visPage].logical = page[notvisPage].logical;
	page[visPage].busy = page[notvisPage].busy;
	page[notvisPage].handle = handle;
	page[notvisPage].logical = logical;
	page[notvisPage].busy = busy;
}

int WriteMem(int handle, int offset, int size, const char* data)
{
	int i = 0, j, usedPage, availSpace = 0, 
		pages = 1, minTime, sharPage;
	bool flag;
	while (data[i])
		i++;
	if (size > i)
		size = i;
	for (i = 0; i < ALL_MEM_PAGES; i++)
		if (page[i].handle == handle)
			availSpace += PAGE_SIZE;
	if (availSpace - offset < size)
		return 1;
	while (size + offset > pages * PAGE_SIZE)
		pages++;
	usedPage = offset / PAGE_SIZE + 1;
	offset = offset - (usedPage - 1) * PAGE_SIZE;
	do
	{
		minTime = page[0].time;
		sharPage = 0;
		for (i = 1; i < workPages; i++)
		{
			if (page[i].time < minTime)
			{
				minTime = page[i].time;
				sharPage = i;
			}
		}
		for (i = 0; i < ALL_MEM_PAGES; i++)
		{
			if (page[i].handle == handle && page[i].logical == usedPage)
			{
				if (i >= workPages)
				{
					SwapPages(i, sharPage);
					i = sharPage;
				}
				page[i].time++;
				for (j = i * PAGE_SIZE + offset; j < (i + 1) * PAGE_SIZE; j++)
				{
					if (size == 0)
						break;
					mem[j] = *data;
					data++;
					size--;
				}
				offset = 0;
			}
			if (size <= 0)
				break;
		}
		usedPage++;
	} while (usedPage <= pages);
	return 0;
}

int ReadMem(int handle, int offset, int size, char* data)
{
	int i, j, availSpace = 0, pages = 1, 
		usedPage, minTime, sharPage;
	for (i = 0; i < ALL_MEM_PAGES; i++)
		if (page[i].handle == handle)
			availSpace += PAGE_SIZE;
	if (availSpace - offset < size)
		return 1;
	while (size + offset > pages * PAGE_SIZE)
		pages++;
	usedPage = offset / PAGE_SIZE + 1;
	offset = offset - (usedPage - 1) * PAGE_SIZE;
	do
	{
		minTime = page[0].time;
		sharPage = 0;
		for (i = 1; i < workPages; i++)
		{
			if (page[i].time < minTime)
			{
				minTime = page[i].time;
				sharPage = i;
			}
		}
		for (i = 0; i < ALL_MEM_PAGES; i++)
		{
			if (page[i].handle == handle && page[i].logical == usedPage)
			{
				if (i >= workPages)
				{
					SwapPages(i, sharPage);
					i = sharPage;
				}
				page[i].time++;
				for (j = i * PAGE_SIZE + offset; j < (i + 1) * PAGE_SIZE; j++)
				{
					if (size == 0)
						break;
					*data = mem[j];
					data++;
					size--;
				}
				usedPage++;
				offset = 0;
			}
			if (size <= 0)
			{
				*data = '\0';
				break;
			}
		}
	} while (usedPage <= pages);
	return 0;
}

void FreeMem(int handle)
{
	int i;
	for (i = 0; i < ALL_MEM_PAGES; i++)
	{
		if (page[i].handle == handle)
			page[i].del();
	}
}

void Dump(void)
{
	int i, j, pagesCount, byte = 10; 
	set<int> usedHandles;
	bool flag, eofBloc;
	for (i = 0; i < ALL_MEM_PAGES; i++)
	{
		flag = true;
		if (page[i].busy && usedHandles.count(page[i].handle) == 0)
		{
			cout << "H:" << page[i].handle << " ";
			cout << "P:";
			pagesCount = 1;
			do
			{
				eofBloc = true;
				for (j = i; j < ALL_MEM_PAGES; j++)
				{
					if (page[i].handle == page[j].handle && page[j].logical == pagesCount)
					{
						if (pagesCount != 1)
							cout << ",";
						if (j < workPages)
							cout << j + 1;
						else
							cout << j + 1 << "*";
						eofBloc = false;
						pagesCount++;
					}
				}
			} while (!eofBloc);
			cout << " S:" << (pagesCount-1) * PAGE_SIZE << " ";
			if (i < workPages && flag)
			{
				for (j = i * PAGE_SIZE; j < i * PAGE_SIZE + byte; j++)
					cout << mem[j];
				flag = false;
			}
			cout << endl;
			usedHandles.insert(page[i].handle);
		}
	}
	cout << endl;
}