#include "getString.h"
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

double getBuffer(int iUnit)
{
    double dBuff;
    double exp;

    cout << "Enter buffer [m]: ";
    cin >> dBuff;

    if(iUnit == 1) //Grad
    {
        exp = pow(10,-5);
        dBuff = dBuff * exp;

    }
    else if(iUnit == 2) //Meter
    {
        dBuff = dBuff;
    }
    else
    {
        cout << "Error. Wrong unit" << endl;
    }

    //getline(cin,s);

    //cast from string to char*


    return dBuff;
}

