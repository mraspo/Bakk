#include "getBuffer.h"
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

/*
the function getBuffer reads the user entry for the coordinate buffer
the user-entry has the unit meters
the buffer is recalculated to degree, because the function says the buffer has to be in the same unit as the coordinates
*/

double getBuffer()
{
    double dBuff;
    double exp;
    int iUnit = 1;

    //read buffer
    cout << "Enter buffer [m]: ";
    cin >> dBuff;
    //cout << "Enter coordinate unit [1=deg, 2=m]: ";
    //cin >> iUnit;

    //recalculate m --> °
    //the if statement has no effect, it is a leftover
    if(dBuff) //Grad
    {
        exp = pow(10,-5);
        dBuff = dBuff * exp;
        return dBuff;

    }
    else
    {
        return 0;
    }


}
