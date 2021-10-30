#include <iostream>
#include "Page.h"
using namespace std;

void Page::del()
{
	handle = 0;
	logical = 0;
	time = 0;
	busy = false;
}