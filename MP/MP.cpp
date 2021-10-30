#include <iostream>
#include "Memory.h"
using namespace std;

int H;

int main()
{
    Init();
    H = AllocMem(100);
 //   Dump();
    H = AllocMem(1500);
 //   Dump();
    H = AllocMem(3999);
//    Dump();
    H = AllocMem(2000);
 //   Dump();
    H = AllocMem(5500);
 //   Dump();
    H = AllocMem(5500);
 //   Dump();
    H = AllocMem(9500);
    Dump();
    WriteMem(2, 0, 2, "AA");
    Dump();
    WriteMem(5, 991, 10, "mmmmmmmmmm");
    Dump();
    char* str = new char[10];
    ReadMem(5, 991, 10, str);
    cout << "Read: ";
    while (*str)
    {
        cout << *str;
        str++;
    }
    cout << endl;
    WriteMem(6, 0, 10, "FFFFFFFFFF");
    Dump();
    FreeMem(5);
    Dump();
    H = AllocMem(5500);
    Dump();
    WriteMem(H, 5000, 10, "SSSSSSSSSS");
    Dump();
    str = new char[10];
    ReadMem(6, 5000, 10, str);
    cout << "Read: ";
    while (*str)
    {
        cout << *str;
        str++;
    }
    cout << endl;
    FreeMem(H);
    FreeMem(6);
    FreeMem(4);
    FreeMem(1);
    Dump();
    FreeMem(2);
    FreeMem(3);
    Dump();
    H = AllocMem(29500);
    Dump();
    H = AllocMem(500);
    Dump();
    WriteMem(8, 28000, 10, "QQQQQQQQQQ");
    Dump();
    str = new char[10];
    ReadMem(8, 28000, 10, str);
    cout << "Read: ";
    while (*str)
    {
        cout << *str;
        str++;
    }
    cout << endl;
    return 0;
}
