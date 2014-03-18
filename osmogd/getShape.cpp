#include "getShape.h"
#include <iostream>
#include <string>

using namespace std;

char* shapeIn(int iCounter)
{
    char* cShapename;
    string s;

    cout << "Enter name of Shapefile " << iCounter << ": ";
    getline(cin,s);

    //cast from string to char*
    cShapename = strdup(s.c_str());

    return cShapename;
}

