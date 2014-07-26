#include "getString.h"
#include <string>

using namespace std;

std::string convert2Asci(string s)
{
    int iCounter;

    for(iCounter = 0; iCounter < s.length(); iCounter++)
    {
        if(int(s[iCounter]) >= 128 || int(s[iCounter]) < 0)
        {
           s[iCounter] = ' ';
        }
    }

    return s;

}
