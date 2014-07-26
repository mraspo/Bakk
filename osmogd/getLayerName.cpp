#include "getLayerName.h"
#include <string>
#include <iostream>

using namespace std;

/*
the function layerName extracts the part of the shapefilename before the separating point
if the filename is osm.shp, it returns osm as a char*
it depends on the fact, that the layer has the same name as the file
*/

char* layerName(char* shapeName)
{
    //convert char* to string
    string s(shapeName, strlen(shapeName));
    string delimiter = ".";
    //get first part
    string token = s.substr(0, s.find(delimiter));

    //get the file format
    unsigned pos = s.find(".");
    string fformat = s.substr (pos+1);

    //convert string back to char*
    char* c = new char[token.length()+1];;

    strcpy(c, token.c_str());

    return c;
}

