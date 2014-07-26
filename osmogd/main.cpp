/*
*Title:         OSMOGDcompare
*Author:        Michael Raspotnig
*/

#include <iostream>
#include <fstream>
#include <string>
#include "ogr_api.h"
#include "ogr_spatialref.h"
#include "getShape.h"
#include "getLayerName.h"
#include "getString.h"
#include "getBuffer.h"
#include "getFormat.h"
#include <ogr_spatialref.h>
#include "ogr_core.h"
//#include "geos_c.h"


using namespace std;


int main()
{
    //input file variable declaration

    //input OGR datasource
    OGRDataSourceH referenceShape_source;
    OGRDataSourceH newShape_source;

    //layer of OGR datasource
    OGRLayerH referenceShape_layer;
    OGRLayerH newShape_layer;

    //features from layer
    OGRFeatureH referenceShape_feature;
    OGRFeatureH newShape_feature;

    int iCounter = 1;
    int iLength = 0;
    int iFlag = 0;
    int iFlag2 = 0;
    int iField;
    int iField2;
    int iEqual = 0;
    int ii = 0;

    double d;
    double dBuffer; //coordinate buffer

    char* shapename1;   //name of osm-shapefile
    char* shapename2;   //name of ogd-shapefile
    char* lName1;   //layername shapefile1
    char* lName2;   //layername shapefile2

    //output for user input
    char* osm = "OSM";
    char* pformat_osm = "(possible formats: .osm,.pbf,.osm.gz,.osm.bz2,.shp)";
    char* ogd = "OGD";
    char* pformat_ogd = "(possible formats: .csv,.geojson,.shp)";
    char* pformat_out = "(possible formats: csv,geojson,shp)";

    string str;
    char* str2;
    const char* c;

    //output files variable declaration

    const char *pszDriverName = "ESRI Shapefile";
    OGRSFDriverH hDriver;

    //output shapefile datasources
    OGRDataSourceH all_equal;
    OGRDataSourceH coord_equal;
    OGRDataSourceH ref_unique;
    OGRDataSourceH new_unique;

    //output shapefile layers
    OGRLayerH hLayer_allequ;
    OGRLayerH hLayer_coordequ;
    OGRLayerH hLayer_refun;
    OGRLayerH hLayer_newun;

    OGRFieldDefnH hFieldDefn_input;

    //register all format drivers built into GDAL/OGR -logrsf_frmts, -fexceptions -logrsf_frmts
    OGRRegisterAll();

    ///creation of output files

    //driver definition
    hDriver = OGRGetDriverByName( pszDriverName );

    if( hDriver == NULL )
    {
        printf( "%s Driver not available.\n", pszDriverName );
        exit( 1 );
    }

    //creation of output folders

    all_equal = OGR_Dr_CreateDataSource( hDriver, "1_all_equal", NULL );
    coord_equal = OGR_Dr_CreateDataSource( hDriver, "2_coordinates_equal", NULL );
    ref_unique = OGR_Dr_CreateDataSource( hDriver, "3_osm_unique", NULL );
    new_unique = OGR_Dr_CreateDataSource( hDriver, "4_ogd_unique", NULL );

    if( all_equal == NULL || coord_equal == NULL || ref_unique == NULL || new_unique == NULL)
    {
        printf( "Creation of output file failed.\n" );
        exit( 1 );
    }

    //creation of the shapefile layers

    ///test reference-system, not working
    //OGRSpatialReferenceH csys;
    //OGRErr er;
    //const char* coordn = "WGS 84";
    //const char* coordn = "EPSG:4326";
    //cout << "hallo";
    //cout << OSRSetWellKnownGeogCS(csys, coordn);
    //er = OSRSetWellKnownGeogCS(csys, coordn );

    //creation of output file layers
    hLayer_allequ = OGR_DS_CreateLayer( all_equal, "all_equal", NULL, wkbPoint, NULL );
    hLayer_coordequ = OGR_DS_CreateLayer( coord_equal, "coordinates_equal", NULL, wkbPoint, NULL );
    hLayer_refun = OGR_DS_CreateLayer( ref_unique, "osm_unique", NULL, wkbPoint, NULL );
    hLayer_newun = OGR_DS_CreateLayer( new_unique, "ogd_unique", NULL, wkbPoint, NULL );

    if( hLayer_allequ == NULL || hLayer_coordequ == NULL || hLayer_refun == NULL || hLayer_newun == NULL)
    {
        printf( "Layer creation failed.\n" );
        exit( 1 );
    }

    ///read input files

    //get names of the input shapefiles
    shapename1 = shapeIn(osm, pformat_osm);

    if (!shapename1)
    {
        cout << "Error. File does not exist or could not be opened.\n";
        OGR_DS_Destroy(all_equal);
        OGR_DS_Destroy(coord_equal);
        OGR_DS_Destroy(ref_unique);
        OGR_DS_Destroy(new_unique);
        system("rmdir /s /Q 1_all_equal");
        system("rmdir /s /Q 2_coordinates_equal");
        system("rmdir /s /Q 3_osm_unique");
        system("rmdir /s /Q 4_ogd_unique");
        //system("pause");
        return 0;
    }

    shapename2 = shapeIn(ogd, pformat_ogd);

    if (!shapename2)
    {
        cout << "Error. File does not exist or could not be opened.\n";
        OGR_DS_Destroy(all_equal);
        OGR_DS_Destroy(coord_equal);
        OGR_DS_Destroy(ref_unique);
        OGR_DS_Destroy(new_unique);
        system("rmdir /s /Q 1_all_equal");
        system("rmdir /s /Q 2_coordinates_equal");
        system("rmdir /s /Q 3_osm_unique");
        system("rmdir /s /Q 4_ogd_unique");
        //system("pause");
        return 0;
    }

    //open shapefile
    referenceShape_source = OGROpen(shapename1, FALSE, NULL); //FALSE: no update access
    newShape_source = OGROpen(shapename2, FALSE, NULL);

    //check if opening shapefile failed
    if( referenceShape_source == NULL || newShape_source == NULL)
    {
        printf( "Error. File could not be opened.\n" );
        OGR_DS_Destroy(all_equal);
        OGR_DS_Destroy(coord_equal);
        OGR_DS_Destroy(ref_unique);
        OGR_DS_Destroy(new_unique);
        system("rmdir /s /Q 1_all_equal");
        system("rmdir /s /Q 2_coordinates_equal");
        system("rmdir /s /Q 3_osm_unique");
        system("rmdir /s /Q 4_ogd_unique");
        //system("pause");
        return 0;
        //exit( 1 );
    }

    //get coordinate buffer in [m]
    if(!(dBuffer = getBuffer()))
    {
        cout << "Error. Given buffer impossible" << endl;
        OGR_DS_Destroy(all_equal);
        OGR_DS_Destroy(coord_equal);
        OGR_DS_Destroy(ref_unique);
        OGR_DS_Destroy(new_unique);
        system("rmdir /s /Q 1_all_equal");
        system("rmdir /s /Q 2_coordinates_equal");
        system("rmdir /s /Q 3_osm_unique");
        system("rmdir /s /Q 4_ogd_unique");
        //system("pause");
        return 0;
    }

    //get layername of source shapefiles
    lName1 = layerName(shapename1);
    lName2 = layerName(shapename2);

    //get the layer, shown in QuantumGIS
    referenceShape_layer = OGR_DS_GetLayerByName(referenceShape_source, lName1);
    newShape_layer = OGR_DS_GetLayerByName(newShape_source, lName2);

    OGRGeometryH hGeometry_1;
    OGRGeometryH hGeometry_2;
    OGRFeatureDefnH hFDefn_reference;
    OGRFeatureDefnH hFDefn_new;

    ///get field names for output files

    referenceShape_feature = OGR_L_GetNextFeature(referenceShape_layer);
    newShape_feature = OGR_L_GetNextFeature(newShape_layer);

    hGeometry_1 = OGR_F_GetGeometryRef(referenceShape_feature);
    hGeometry_2 = OGR_F_GetGeometryRef(newShape_feature);

    hFDefn_reference = OGR_L_GetLayerDefn(referenceShape_layer);
    hFDefn_new = OGR_L_GetLayerDefn(newShape_layer);

    OGRFieldDefnH hFieldDefn_reference;
    OGRFieldDefnH hFieldDefn_new;

    //loop through all fields of osm source shapefile
    for( iField = 0; iField < OGR_FD_GetFieldCount(hFDefn_reference); iField++ )
    {

            hFieldDefn_reference = OGR_FD_GetFieldDefn( hFDefn_reference, iField );

            hFieldDefn_input = OGR_Fld_Create( OGR_Fld_GetNameRef(hFieldDefn_reference), OFTString );
            //set field width for output shapefile
            OGR_Fld_SetWidth( hFieldDefn_input, OGR_Fld_GetWidth(hFieldDefn_reference));

            if( OGR_L_CreateField( hLayer_allequ, hFieldDefn_input, TRUE ) != OGRERR_NONE || OGR_L_CreateField(hLayer_refun, hFieldDefn_input, TRUE) != OGRERR_NONE)
            {
                printf( "Creating field failed.\n" );
                exit( 1 );
            }

            OGR_Fld_Destroy(hFieldDefn_input);
    }

    //loop through all fields of ogd source shapefile
    for(iField2 = 0; iField2 < OGR_FD_GetFieldCount(hFDefn_new); iField2++)
    {
            hFieldDefn_new = OGR_FD_GetFieldDefn(hFDefn_new, iField2);

            hFieldDefn_input = OGR_Fld_Create(OGR_Fld_GetNameRef(hFieldDefn_new), OFTString);
            //set field width for output shapefile
            OGR_Fld_SetWidth(hFieldDefn_input, OGR_Fld_GetWidth(hFieldDefn_new));

            if( OGR_L_CreateField( hLayer_coordequ, hFieldDefn_input, TRUE) != OGRERR_NONE || OGR_L_CreateField(hLayer_newun, hFieldDefn_input, TRUE) != OGRERR_NONE)
            {
                printf( "Creating field failed.\n" );
                exit( 1 );
            }

            OGR_Fld_Destroy(hFieldDefn_input);
    }

    //to start at beginning of the layer
    OGR_L_ResetReading(referenceShape_layer);

    //comparing-algorithm starts here

    //read till end of osm file
    while( (referenceShape_feature = OGR_L_GetNextFeature(referenceShape_layer)) != NULL )
    {

        //geometry from feature
        hGeometry_1 = OGR_F_GetGeometryRef(referenceShape_feature);
        hFDefn_reference = OGR_L_GetLayerDefn(referenceShape_layer);

        //start second file (file which is compared with first one) at the beginning for next loop
        OGR_L_ResetReading(newShape_layer);
        iEqual = 0;
        iFlag = 0;

        //read till end of ogd file
        while((newShape_feature = OGR_L_GetNextFeature(newShape_layer)) != NULL)
        {
            //geometry from feature
            hGeometry_2 = OGR_F_GetGeometryRef(newShape_feature);
            hFDefn_new = OGR_L_GetLayerDefn(newShape_layer);

            //check if geometry is existant and geometry is a point-geometry
            if(hGeometry_1 != NULL && hGeometry_2 != NULL && wkbFlatten(OGR_G_GetGeometryType(hGeometry_1))== wkbPoint && wkbFlatten(OGR_G_GetGeometryType(hGeometry_2))== wkbPoint)
            {
                //check if ogd-point is within buffer of osm-point
                if(OGR_G_Within(hGeometry_2, OGR_G_Buffer(hGeometry_1, dBuffer, 30)))
                {
                    iFlag = 1;
                    //iFlag2 = 1;

                    OGRFieldDefnH hFieldDefn_reference;
                    OGRFieldDefnH hFieldDefn_new;

                    //the coordinates are the same, so now the attributes are checked for equality
                    for( iField = 1; iField < OGR_FD_GetFieldCount(hFDefn_reference); iField++ )
                    {
                        hFieldDefn_reference = OGR_FD_GetFieldDefn( hFDefn_reference, iField );

                        for(iField2 = 1; iField2 < OGR_FD_GetFieldCount(hFDefn_new); iField2++)
                        {
                            hFieldDefn_new = OGR_FD_GetFieldDefn( hFDefn_new, iField2 );

                            //check if attribute of osm file is equal to attribute of ogd file
                            if(!strcmp(OGR_F_GetFieldAsString(referenceShape_feature, iField),OGR_F_GetFieldAsString(newShape_feature, iField2)))
                            {
                                iEqual++; //if equal, raise counter
                                break;
                            }
                        }
                    }

                    //check if number of attribute fields of osm and ogd are equal and if number of equal attributes
                    //is equal to number of attribute fields --> all attributes are equal
                    if(OGR_FD_GetFieldCount(hFDefn_reference) == OGR_FD_GetFieldCount(hFDefn_new) && iEqual == (OGR_FD_GetFieldCount(hFDefn_reference)-1))
                    {
                        //write to all_equal

                        OGRFeatureH hFeature_allnew;
                        OGRGeometryH hPt;

                        hFeature_allnew = OGR_F_Create(OGR_L_GetLayerDefn(hLayer_allequ));

                        //remove all not-ascii signs from the attributes
                        for(iField = 0; iField < OGR_FD_GetFieldCount(hFDefn_reference); iField++)
                        {
                            str = convert2Asci(OGR_F_GetFieldAsString(referenceShape_feature, iField));
                            c = str.c_str();
                            OGR_F_SetFieldString(hFeature_allnew, iField, c);
                        }

                        //define point geometry
                        hPt = OGR_G_CreateGeometry(wkbPoint);
                        OGR_G_SetPoint_2D(hPt, 0, OGR_G_GetX(hGeometry_1, 0), OGR_G_GetY(hGeometry_1, 0));

                        OGR_F_SetGeometry(hFeature_allnew, hPt);
                        OGR_G_DestroyGeometry(hPt);

                        //create the entry in output file
                        if(OGR_L_CreateFeature(hLayer_allequ, hFeature_allnew) != OGRERR_NONE)
                        {
                            printf("Failed to create feature in shapefile\n");
                            exit(1);
                        }

                        OGR_F_Destroy(hFeature_allnew);

                    }
                    else
                    {
                        //write to coordinates_equal
                        //coordinates are the same, attributes differe

                        OGRFeatureH hFeature_coordequ;
                        OGRGeometryH hPt;

                        hFeature_coordequ = OGR_F_Create(OGR_L_GetLayerDefn(hLayer_coordequ));

                        //remove all not-ascii signs from the attributes
                        for(iField = 0; iField < OGR_FD_GetFieldCount(hFDefn_new); iField++)
                        {
                            str = convert2Asci(OGR_F_GetFieldAsString(newShape_feature, iField));
                            c = str.c_str();

                            OGR_F_SetFieldString(hFeature_coordequ, iField, c);
                        }

                        //define point geometry
                        hPt = OGR_G_CreateGeometry(wkbPoint);
                        OGR_G_SetPoint_2D(hPt, 0, OGR_G_GetX(hGeometry_2, 0), OGR_G_GetY(hGeometry_2, 0));

                        OGR_F_SetGeometry(hFeature_coordequ, hPt);
                        OGR_G_DestroyGeometry(hPt);

                        //create the entry in output file
                        if(OGR_L_CreateFeature(hLayer_coordequ, hFeature_coordequ) != OGRERR_NONE)
                        {
                            printf("Failed to create feature in shapefile\n");
                            exit(1);
                        }

                        OGR_F_Destroy(hFeature_coordequ);

                    }

                }

            }
            else
            {
                //if the file contains not just point geometries
                cout << "Error. point geometry only" << endl;
            }

        }
        if(iFlag == 0)
        {
            //the coordinates are not the same
            //write to osm_unique

            OGRFeatureH hFeature_refun;
            OGRGeometryH hPt;

            hFeature_refun = OGR_F_Create(OGR_L_GetLayerDefn(hLayer_refun));

            //remove all not-ascii signs from the attributes
            for(iField = 0; iField < OGR_FD_GetFieldCount(hFDefn_reference); iField++)
            {
                str = convert2Asci(OGR_F_GetFieldAsString(referenceShape_feature, iField));
                c = str.c_str();

                OGR_F_SetFieldString(hFeature_refun, iField, c);
            }

            //define point geometry
            hPt = OGR_G_CreateGeometry(wkbPoint);
            OGR_G_SetPoint_2D(hPt, 0, OGR_G_GetX(hGeometry_1, 0), OGR_G_GetY(hGeometry_1, 0));

            OGR_F_SetGeometry(hFeature_refun, hPt);
            OGR_G_DestroyGeometry(hPt);

            //create the entry in output file
            if(OGR_L_CreateFeature(hLayer_refun, hFeature_refun) != OGRERR_NONE)
            {
                printf("Failed to create feature in shapefile\n");
                exit(1);
            }

            OGR_F_Destroy(hFeature_refun);
        }
    }

    //comparison the other way round, ogd with osm
    //makes it easier to find differences in the two files, for ogd_unique

    //start at the beginning of the ogd file
    OGR_L_ResetReading(newShape_layer);

    //read till end of ogd file
    while( (newShape_feature = OGR_L_GetNextFeature(newShape_layer)) != NULL )
    {
        //geometry from feature
        hGeometry_1 = OGR_F_GetGeometryRef(newShape_feature);
        hFDefn_reference = OGR_L_GetLayerDefn(newShape_layer);

        //start at the beginning of the osm file, for the new loop
        OGR_L_ResetReading(referenceShape_layer);

        iFlag2 = 0;

        //read till end of osm file
        while((referenceShape_feature = OGR_L_GetNextFeature(referenceShape_layer)) != NULL)
        {
            //geometry from feature
            hGeometry_2 = OGR_F_GetGeometryRef(referenceShape_feature);
            hFDefn_new = OGR_L_GetLayerDefn(referenceShape_layer);

            //check if geometry is not empty and if it is a point geometry
            if(hGeometry_1 != NULL && hGeometry_2 != NULL && wkbFlatten(OGR_G_GetGeometryType(hGeometry_1))== wkbPoint && wkbFlatten(OGR_G_GetGeometryType(hGeometry_2))== wkbPoint)
            {
                //check if osm-point is within ogd-point-buffer
                if(OGR_G_Within(hGeometry_2, OGR_G_Buffer(hGeometry_1, dBuffer, 30)))
                {
                    iFlag2 = 1;
                }
            }
            else
            {
                cout << "Error. Point geometry only" << endl;
            }

        }
        if(iFlag2 == 0)
        {
            //write to ogd_unique

            OGRFeatureH hFeature_newun;
            OGRGeometryH hPt;

            hFeature_newun = OGR_F_Create(OGR_L_GetLayerDefn(hLayer_newun));

            //remove all not-ascii signs from the attributes
            for(iField = 0; iField < OGR_FD_GetFieldCount(hFDefn_new); iField++)
            {
                str = convert2Asci(OGR_F_GetFieldAsString(newShape_feature, iField));
                c = str.c_str();

                OGR_F_SetFieldString(hFeature_newun, iField, c);
            }

            //define point geometry
            hPt = OGR_G_CreateGeometry(wkbPoint);
            OGR_G_SetPoint_2D(hPt, 0, OGR_G_GetX(hGeometry_1, 0), OGR_G_GetY(hGeometry_1, 0));

            OGR_F_SetGeometry(hFeature_newun, hPt);
            OGR_G_DestroyGeometry(hPt);

            //create the entry in output file
            if(OGR_L_CreateFeature(hLayer_newun, hFeature_newun) != OGRERR_NONE)
            {
                printf("Failed to create feature in shapefile\n");
                exit(1);
            }

            OGR_F_Destroy(hFeature_newun);

        }
    }

    //close all open pointers and references
    OGR_F_Destroy(referenceShape_feature);
    OGR_F_Destroy(newShape_feature);

    OGR_DS_Destroy(referenceShape_source);
    OGR_DS_Destroy(newShape_source);
    OGR_DS_Destroy(all_equal);
    OGR_DS_Destroy(coord_equal);
    OGR_DS_Destroy(ref_unique);
    OGR_DS_Destroy(new_unique);

    //ask user for the output format
    formatOut(pformat_out);

    return 0;

}
