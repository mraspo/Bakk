#include "getShape.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>

using namespace std;

/*
the function shapeIn reads the name of the osm and ogd shapefile
depending on the file format it converts all objects to point objects and converts the files into the .shp format
the shape-format is the internal working format
the function sets the osm-filename to osm.shp and the ogd-filename to ogd.shp
*/

char* shapeIn(char* cName, char* cpFormat)
{
    char* cShapename;
    string s;

    //get user input
    cout << "Enter name of " << cName << " file " << cpFormat << ": ";
    getline(cin,s);

    //cast from string to char*
    cShapename = strdup(s.c_str());
    ifstream ifile(cShapename);

    //for format conversion
    string nameString(cShapename, strlen(cShapename));
    string delimiter = ".";
    //get filename
    string filename = nameString.substr(0, nameString.find(delimiter));

    //get fileformat
    unsigned pos = nameString.find(".");
    string fileformat = nameString.substr (pos+1);

    //cast from string to char*
    char* c = new char[filename.length()+1];;

    strcpy(c, filename.c_str());

    //check if file exists
    if (ifile)
    {
        //check fileformat
        if(fileformat == "pbf" || fileformat == "osm.pbf")
        {
            //convert all objects to point-objects
            string s1 = "osmconvert.exe ";
            string s2 = " --all-to-nodes -o=osm_nodesonly.pbf --object-type-offset=7777777777";
            string s3 = s1 + nameString + s2;
            char* name_file;
            name_file = strdup(s3.c_str());
            system(name_file);

            //convert pbf-file to shapefile
            system("ogr2ogr.exe -f \"ESRI Shapefile\" osm_nodes osm_nodesonly.pbf -nlt POINT");

            //rename shapefile and remove the folder created by ogr2ogr
            //all files have to be in the same folder/subfolder
            system("cls");
            system("rename osm_nodes\\points.* osm.*");
            system("move osm_nodes\\osm.* .\\");
            system("rmdir /s /Q osm_nodes");
            system("cls");

            //set default shape name
            cShapename = "osm.shp";

        }
        else if (fileformat == "osm")
        {
            //convert all objects to point-objects
            string s1 = "osmconvert.exe ";
            string s2 = " --all-to-nodes -o=osm_nodesonly.osm --object-type-offset=7777777777";
            string s3 = s1 + nameString + s2;
            char* name_file;
            name_file = strdup(s3.c_str());
            system(name_file);

            //convert osm-file to shapefile
            system("ogr2ogr.exe -f \"ESRI Shapefile\" osm_nodes osm_nodesonly.osm -nlt POINT");
            system("cls");

            //rename shapefile and remove the folder created by ogr2ogr
            system("rename osm_nodes\\points.* osm.*");
            system("move osm_nodes\\osm.* .\\");
            system("rmdir /s /Q osm_nodes");
            system("cls");

            //set default shape name
            cShapename = "osm.shp";

        }
        else if (fileformat == "osm.gz" || fileformat == "osm.bz2")
        {
            //extract the zipped file
            //7zip has to be installed
            string s1 = "7z.exe e ";
            string s2 = " -y";
            string s3 = s1 + nameString + s2;
            char* name_file;
            name_file = strdup(s3.c_str());
            system(name_file);
            system("cls");

            //convert all objects to point-objects
            string s4 = "osmconvert.exe ";
            string s5 = " --all-to-nodes -o=osm_nodesonly.osm --object-type-offset=7777777777";
            string s7 = ".osm";
            string s6 = s4 + filename + s7 + s5;
            //cout << s6;
            char* name_file2;
            name_file2 = strdup(s6.c_str());
            system(name_file2);

            //convert osm-file to shapefile
            system("ogr2ogr.exe -f \"ESRI Shapefile\" osm_nodes osm_nodesonly.osm -nlt POINT");
            system("cls");

            //rename shapefile and remove the folder created by ogr2ogr
            system("rename osm_nodes\\points.* osm.*");
            system("move osm_nodes\\osm.* .\\");
            system("rmdir /s /Q osm_nodes");
            system("cls");

            //set default shape name
            cShapename = "osm.shp";

        }
        else if (fileformat == "csv") //funktioniert nur, wenn nachkommastellen der koordinaten durch . gekennzeichnet sind
        {
            //convert csv-file to shapefile
            //1. dbf file is created
            string s1 = "ogr2ogr.exe -f \"ESRI Shapefile\" ogd_nodes ";
            //string s2 = " OGRGeoJSON -nlt POINT ";
            string s3 = s1 + nameString;
            //cout << nameString;
            char* name_file;
            name_file = strdup(s3.c_str());
            system(name_file);

            //2. in the next step a .vrt file has to be created
            ofstream myfile;
            //string s6 = ".vrt";
            //string s7 = filename + s6;
            string s7 = "ogd_nodes.vrt";

            char* fPath = new char[s7.length()+1];;
            strcpy(fPath, s7.c_str());

            myfile.open (fPath);
            myfile << "<OGRVRTDataSource>\n";

            string s8 = "<OGRVRTLayer name=\"";
            string s9 = "\">\n";
            string s10 = s8 + filename + s9;
            myfile << s10;
            string s11 = "<SrcDataSource relativeToVRT=\"1\">";
            string s12 = "</SrcDataSource>\n";
            string s13 = s11 + nameString + s12;
            myfile << s13;
            //myfile << "<SrcLayer>sample1</SrcLayer>\n";
            myfile << "<GeometryType>wkbPoint</GeometryType>\n";
            myfile << "<LayerSRS>WGS84</LayerSRS>\n";
            myfile << "<GeometryField encoding=\"PointFromColumns\" x=\"LAMBDA\" y=\"PHI\"/>\n";
            myfile << "</OGRVRTLayer>\n";
            myfile << "</OGRVRTDataSource>\n";
            myfile.close();

            //3. with the .vrt and .dbf file the shape file is created
            //Caution, the only way this works is when the decimal places are marked with a point instead of a comma
            string s4 = "ogr2ogr.exe -overwrite -f \"ESRI Shapefile\" ogd_nodes ";
            string s14 = " -lco \"SEPARATOR=COMMA\""; //the SEPARATOR commando has no effect
            string s5 = s4 + s7 + s14;// + s6;
            char* name_file2;
            name_file2 = strdup(s5.c_str());
            system(name_file2);

            //rename shapefile and remove the folder created by ogr2ogr
            system("cls");
            string s15 = "rename ogd_nodes\\";
            string s16 = ".* ogd.*";
            string s17 = s15 + filename + s16;
            char* sysvar;
            sysvar = strdup(s17.c_str());
            system(sysvar);
            //system("rename ogd_nodes\\points.* ogd.*");
            system("move ogd_nodes\\ogd.* .\\");
            system("rmdir /s /Q ogd_nodes");
            system("cls");

            //set default shape name
            cShapename = "ogd.shp";

        }
        else if (fileformat == "geojson" || fileformat == "json")
        {
            //convert geojson-file to shapefile
            string s1 = "ogr2ogr.exe -f \"ESRI Shapefile\" ogd_nodes -skipfailures ";
            string s2 = " OGRGeoJSON -nlt POINT ";
            string s3 = s1 + nameString + s2;
            //cout << nameString;
            char* name_file;
            name_file = strdup(s3.c_str());
            system(name_file);

            //rename shapefile and remove the folder created by ogr2ogr
            system("cls");
            system("rename ogd_nodes\\OGRGeoJSON.* ogd.*");
            system("move ogd_nodes\\ogd.* .\\");
            system("rmdir /s /Q ogd_nodes");
            system("cls");

            //set default shape name
            cShapename = "ogd.shp";

        }
        else if (fileformat == "shp")
        {
            //if the input file has shape-format nothing has to be done
            //assuming that it only contains point objects
        }
        else
        {
            cout << "\nError. wrong file format\n" << endl;
            return 0;
        }

        return cShapename;
    }
    else
    {
        return 0;
    }

}

