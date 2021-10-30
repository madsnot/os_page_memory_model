#pragma once

class Page
{
public:
	int handle;
	int logical;
	int time;
	bool busy;
	Page()
	{
		handle = 0;
		logical = 0;
		time = 0;
		busy = false;
	}
	void del();
};

