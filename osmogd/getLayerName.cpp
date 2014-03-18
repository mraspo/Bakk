#include "getLayerName.h"
#include <string>

using namespace std;

char* layerName(char* shapeName)
{
    int iLength = 0;

    iLength = strlen(shapeName);
    shapeName[iLength-4] = '\0';

    return shapeName;
}

