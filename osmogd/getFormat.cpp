#include "getFormat.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>


using namespace std;

/*
the function formatOut reads the user entry for the output format
the format has to be written WITHOUT a leading dot
depending on the file format it converts all resulting shapefiles into the user given format
as a result, the user gets the shapefiles and the files in the specified format
each output file group is in a separate folder
*/

void formatOut(char* oFormat)
{
    char* cFormatname;
    string sformat;

    //read user entry for output format
    cout << "Enter the output file format " << oFormat << ": ";
    getline(cin,sformat);
    getline(cin,sformat);

    //check file format
    if (sformat == "csv")
    {
        //move the files in the same folder as the program
        system("cls");
        system("move 1_all_equal\\all_equal.* .\\");
        system("move 2_coordinates_equal\\coordinates_equal.* .\\");
        system("move 3_osm_unique\\osm_unique.* .\\");
        system("move 4_ogd_unique\\ogd_unique.* .\\");
        system("cls");
        //ogr2ogr.exe -update s_srs EPSG:4326 all_equal.shp
        //system("ogr2ogr.exe -f \"CSV\" all_equal.csv all_equal.shp -nlt POINT");
        //system("ogr2ogr.exe -f \"CSV\" coordinates_equal.csv coordinates_equal.shp -nlt POINT");
        //system("ogr2ogr.exe -f \"CSV\" osm_unique.csv osm_unique.shp -nlt POINT");
        //system("ogr2ogr.exe -f \"CSV\" ogd_unique.csv ogd_unique.shp -nlt POINT");

        //convert the files into the specified format
        system("ogr2ogr.exe -f \"CSV\" all_equal.csv all_equal.shp -nlt POINT");
        system("ogr2ogr.exe -f \"CSV\" coordinates_equal.csv coordinates_equal.shp -nlt POINT");
        system("ogr2ogr.exe -f \"CSV\" osm_unique.csv osm_unique.shp -nlt POINT");
        system("ogr2ogr.exe -f \"CSV\" ogd_unique.csv ogd_unique.shp -nlt POINT");
        system("cls");

        //move the files back in their folders
        system("move all_equal.* 1_all_equal");
        system("move coordinates_equal.* 2_coordinates_equal");
        system("move osm_unique.* 3_osm_unique");
        system("move ogd_unique.* 4_ogd_unique");
        system("cls");

    }
    else if (sformat == "geojson" || sformat == "json")
    {
        //move the files in the same folder as the program
        system("cls");
        system("move 1_all_equal\\all_equal.* .\\");
        system("move 2_coordinates_equal\\coordinates_equal.* .\\");
        system("move 3_osm_unique\\osm_unique.* .\\");
        system("move 4_ogd_unique\\ogd_unique.* .\\");
        system("cls");

        //convert the files into the specified format
        system("ogr2ogr.exe -f \"GeoJSON\" all_equal.geojson all_equal.shp -nlt POINT");
        system("ogr2ogr.exe -f \"GeoJSON\" coordinates_equal.geojson coordinates_equal.shp -nlt POINT");
        system("ogr2ogr.exe -f \"GeoJSON\" osm_unique.geojson osm_unique.shp -nlt POINT");
        system("ogr2ogr.exe -f \"GeoJSON\" ogd_unique.geojson ogd_unique.shp -nlt POINT");
        system("cls");

        //move the files back in their folders
        system("move all_equal.* 1_all_equal");
        system("move coordinates_equal.* 2_coordinates_equal");
        system("move osm_unique.* 3_osm_unique");
        system("move ogd_unique.* 4_ogd_unique");
        system("cls");
    }
    else if (sformat == "shp")
    {
        //if the specified format is shape, nothing has to be done as shape is the internal working format
        //additionaly it could be reprojected through ogr2ogr, but for some weird reasons this does not work
        //so the user has to choose the reference system manually when used in qgis
    }
    else
    {
        cout << "Error. wrong file format\n";
    }

}
