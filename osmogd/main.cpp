/*
*Title:         OSMOGDcompare
*Author:        Michael Raspotnig
*University:    TU Graz, Geomatics Engineering
*Work-frame:    Bachelor thesis
*Department:    Geoinformation
*Semester:      WS/SS 2013/14
*/
//YCFHQ-9DWCY-DKV88-T2TMH-G7BHP

#include <iostream>
#include "ogr_api.h"
#include "ogr_spatialref.h"
#include "getShape.h"
#include "getLayerName.h"
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
    double x, y;

    char* shapename1;
    char* shapename2;
    char* lName1;
    char* lName2;

    //output files variable declaration

    const char *pszDriverName = "ESRI Shapefile";
    OGRSFDriverH hDriver;

    OGRDataSourceH all_equal;
    OGRDataSourceH coord_equal;
    OGRDataSourceH ref_unique;
    OGRDataSourceH new_unique;

    OGRLayerH hLayer_allequ;
    OGRLayerH hLayer_coordequ;
    OGRLayerH hLayer_refun;
    OGRLayerH hLayer_newun;

    OGRFieldDefnH hFieldDefn_input;

    //register all format drivers built into GDAL/OGR -logrsf_frmts, -fexceptions -logrsf_frmts
    OGRRegisterAll();

    ///creation of output files

    hDriver = OGRGetDriverByName( pszDriverName );

    if( hDriver == NULL )
    {
        printf( "%s Driver not available.\n", pszDriverName );
        exit( 1 );
    }

    //creation of output folders

    all_equal = OGR_Dr_CreateDataSource( hDriver, "1_all_equal", NULL );
    coord_equal = OGR_Dr_CreateDataSource( hDriver, "2_coordinates_equal", NULL );
    ref_unique = OGR_Dr_CreateDataSource( hDriver, "3_reference_unique", NULL );
    new_unique = OGR_Dr_CreateDataSource( hDriver, "4_new_unique", NULL );

    if( all_equal == NULL || coord_equal == NULL || ref_unique == NULL || new_unique == NULL)
    {
        printf( "Creation of output file failed.\n" );
        exit( 1 );
    }

    //creation of the shapefile layers

    hLayer_allequ = OGR_DS_CreateLayer( all_equal, "all_equal", NULL, wkbPoint, NULL );
    hLayer_coordequ = OGR_DS_CreateLayer( coord_equal, "coordinates_equal", NULL, wkbPoint, NULL );
    hLayer_refun = OGR_DS_CreateLayer( ref_unique, "reference_unique", NULL, wkbPoint, NULL );
    hLayer_newun = OGR_DS_CreateLayer( new_unique, "new_unique", NULL, wkbPoint, NULL );

    if( hLayer_allequ == NULL || hLayer_coordequ == NULL || hLayer_refun == NULL || hLayer_newun == NULL)
    {
        printf( "Layer creation failed.\n" );
        exit( 1 );
    }

    ///read input files

    //get names of the input shapefiles
    shapename1 = shapeIn(iCounter++);
    shapename2 = shapeIn(iCounter);

    //open shapefile
    referenceShape_source = OGROpen(shapename1, FALSE, NULL); //FALSE: no update access
    newShape_source = OGROpen(shapename2, FALSE, NULL);

    //check if opening shapefile failed
    if( referenceShape_source == NULL || newShape_source == NULL)
    {
        printf( "Open failed.\n" );
        exit( 1 );
    }

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


    for( iField = 0; iField < OGR_FD_GetFieldCount(hFDefn_reference); iField++ )
    {

            hFieldDefn_reference = OGR_FD_GetFieldDefn( hFDefn_reference, iField );

            hFieldDefn_input = OGR_Fld_Create( OGR_Fld_GetNameRef(hFieldDefn_reference), OFTString );

            OGR_Fld_SetWidth( hFieldDefn_input, OGR_Fld_GetWidth(hFieldDefn_reference));

            if( OGR_L_CreateField( hLayer_allequ, hFieldDefn_input, TRUE ) != OGRERR_NONE || OGR_L_CreateField(hLayer_refun, hFieldDefn_input, TRUE) != OGRERR_NONE)
            {
                printf( "Creating field failed.\n" );
                exit( 1 );
            }

            OGR_Fld_Destroy(hFieldDefn_input);
    }

    for(iField2 = 0; iField2 < OGR_FD_GetFieldCount(hFDefn_new); iField2++)
    {
            hFieldDefn_new = OGR_FD_GetFieldDefn(hFDefn_new, iField2);

            hFieldDefn_input = OGR_Fld_Create(OGR_Fld_GetNameRef(hFieldDefn_new), OFTString);

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

    //Vergleichsalgorithmus startet hier

    while( (referenceShape_feature = OGR_L_GetNextFeature(referenceShape_layer)) != NULL )
    {

        //geometry from feature
        hGeometry_1 = OGR_F_GetGeometryRef(referenceShape_feature);
        hFDefn_reference = OGR_L_GetLayerDefn(referenceShape_layer);

        OGR_L_ResetReading(newShape_layer);
        iEqual = 0;
        iFlag = 0;

        while((newShape_feature = OGR_L_GetNextFeature(newShape_layer)) != NULL)
        {
            //geometry from feature
            hGeometry_2 = OGR_F_GetGeometryRef(newShape_feature);
            hFDefn_new = OGR_L_GetLayerDefn(newShape_layer);

            if(hGeometry_1 != NULL && hGeometry_2 != NULL && wkbFlatten(OGR_G_GetGeometryType(hGeometry_1))== wkbPoint && wkbFlatten(OGR_G_GetGeometryType(hGeometry_2))== wkbPoint)
            {

                if(OGR_G_Within(hGeometry_2, OGR_G_Buffer(hGeometry_1, 0.000001, 30)))
                {
                    iFlag = 1;
                    //iFlag2 = 1;

                    OGRFieldDefnH hFieldDefn_reference;
                    OGRFieldDefnH hFieldDefn_new;

                    for( iField = 0; iField < OGR_FD_GetFieldCount(hFDefn_reference); iField++ )
                    {
                        hFieldDefn_reference = OGR_FD_GetFieldDefn( hFDefn_reference, iField );
                        //cout <<  OGR_Fld_GetNameRef(hFieldDefn_reference) << endl;


                        for(iField2 = 0; iField2 < OGR_FD_GetFieldCount(hFDefn_new); iField2++)
                        {
                            hFieldDefn_new = OGR_FD_GetFieldDefn( hFDefn_new, iField2 );

                            // problem !!
                            if(!strcmp(OGR_F_GetFieldAsString(referenceShape_feature, iField),OGR_F_GetFieldAsString(newShape_feature, iField2)))
                            {
                                //utf8 konvertieren
                                //double für alle zahlen
                                iEqual++;
                                break;
                            }
                        }
                    }

                    if(OGR_FD_GetFieldCount(hFDefn_reference) == OGR_FD_GetFieldCount(hFDefn_new) && iEqual == OGR_FD_GetFieldCount(hFDefn_reference))
                    {
                        //in exaktgleich schreiben

                        OGRFeatureH hFeature_allnew;
                        OGRGeometryH hPt;

                        hFeature_allnew = OGR_F_Create(OGR_L_GetLayerDefn(hLayer_allequ));

                        for(iField = 0; iField < OGR_FD_GetFieldCount(hFDefn_reference); iField++)
                        {
                            OGR_F_SetFieldString(hFeature_allnew, iField, OGR_F_GetFieldAsString(referenceShape_feature, iField));
                        }

                        hPt = OGR_G_CreateGeometry(wkbPoint);
                        OGR_G_SetPoint_2D(hPt, 0, OGR_G_GetX(hGeometry_1, 0), OGR_G_GetY(hGeometry_1, 0));

                        OGR_F_SetGeometry(hFeature_allnew, hPt);
                        OGR_G_DestroyGeometry(hPt);

                        if(OGR_L_CreateFeature(hLayer_allequ, hFeature_allnew) != OGRERR_NONE)
                        {
                            printf("Failed to create feature in shapefile\n");
                            exit(1);
                        }

                        OGR_F_Destroy(hFeature_allnew);

                    }
                    else
                    {
                        //in gleicheKoordinaten_unterschiedlicheAttribute schreiben

                        OGRFeatureH hFeature_coordequ;
                        OGRGeometryH hPt;

                        hFeature_coordequ = OGR_F_Create(OGR_L_GetLayerDefn(hLayer_coordequ));

                        for(iField = 0; iField < OGR_FD_GetFieldCount(hFDefn_new); iField++)
                        {
                            OGR_F_SetFieldString(hFeature_coordequ, iField, OGR_F_GetFieldAsString(newShape_feature, iField));
                        }

                        hPt = OGR_G_CreateGeometry(wkbPoint);
                        OGR_G_SetPoint_2D(hPt, 0, OGR_G_GetX(hGeometry_2, 0), OGR_G_GetY(hGeometry_2, 0));

                        OGR_F_SetGeometry(hFeature_coordequ, hPt);
                        OGR_G_DestroyGeometry(hPt);

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
                cout << "Error. point geometry only" << endl;
            }

        }
        if(iFlag == 0)
        {

            //in ref_unique schreiben

            OGRFeatureH hFeature_refun;
            OGRGeometryH hPt;

            hFeature_refun = OGR_F_Create(OGR_L_GetLayerDefn(hLayer_refun));

            for(iField = 0; iField < OGR_FD_GetFieldCount(hFDefn_reference); iField++)
            {
                OGR_F_SetFieldString(hFeature_refun, iField, OGR_F_GetFieldAsString(referenceShape_feature, iField));
            }

            hPt = OGR_G_CreateGeometry(wkbPoint);
            OGR_G_SetPoint_2D(hPt, 0, OGR_G_GetX(hGeometry_1, 0), OGR_G_GetY(hGeometry_1, 0));

            OGR_F_SetGeometry(hFeature_refun, hPt);
            OGR_G_DestroyGeometry(hPt);

            if(OGR_L_CreateFeature(hLayer_refun, hFeature_refun) != OGRERR_NONE)
            {
                printf("Failed to create feature in shapefile\n");
                exit(1);
            }

            OGR_F_Destroy(hFeature_refun);

        }

    }

    //Vergleich 2

    OGR_L_ResetReading(newShape_layer);

    while( (newShape_feature = OGR_L_GetNextFeature(newShape_layer)) != NULL )
    {
        //geometry from feature
        hGeometry_1 = OGR_F_GetGeometryRef(newShape_feature);
        hFDefn_reference = OGR_L_GetLayerDefn(newShape_layer);

        OGR_L_ResetReading(referenceShape_layer);

        iFlag2 = 0;

        while((referenceShape_feature = OGR_L_GetNextFeature(referenceShape_layer)) != NULL)
        {
            //geometry from feature
            hGeometry_2 = OGR_F_GetGeometryRef(referenceShape_feature);
            hFDefn_new = OGR_L_GetLayerDefn(referenceShape_layer);

            if(hGeometry_1 != NULL && hGeometry_2 != NULL && wkbFlatten(OGR_G_GetGeometryType(hGeometry_1))== wkbPoint && wkbFlatten(OGR_G_GetGeometryType(hGeometry_2))== wkbPoint)
            {

                if(OGR_G_Within(hGeometry_2, OGR_G_Buffer(hGeometry_1, 0.000001, 30)))
                {
                    iFlag2 = 1;
                }
            }
            else
            {
                cout << "Error. point geometry only" << endl;
            }

        }
        if(iFlag2 == 0)
        {
            //in new_unique schreiben

            OGRFeatureH hFeature_newun;
            OGRGeometryH hPt;

            hFeature_newun = OGR_F_Create(OGR_L_GetLayerDefn(hLayer_newun));

            for(iField = 0; iField < OGR_FD_GetFieldCount(hFDefn_new); iField++)
            {
                OGR_F_SetFieldString(hFeature_newun, iField, OGR_F_GetFieldAsString(newShape_feature, iField));
            }

            hPt = OGR_G_CreateGeometry(wkbPoint);
            OGR_G_SetPoint_2D(hPt, 0, OGR_G_GetX(hGeometry_1, 0), OGR_G_GetY(hGeometry_1, 0));

            OGR_F_SetGeometry(hFeature_newun, hPt);
            OGR_G_DestroyGeometry(hPt);

            if(OGR_L_CreateFeature(hLayer_newun, hFeature_newun) != OGRERR_NONE)
            {
                printf("Failed to create feature in shapefile\n");
                exit(1);
            }

            OGR_F_Destroy(hFeature_newun);

        }
    }

    OGR_F_Destroy(referenceShape_feature);
    OGR_F_Destroy(newShape_feature);

    OGR_DS_Destroy(referenceShape_source);
    OGR_DS_Destroy(newShape_source);
    OGR_DS_Destroy(all_equal);
    OGR_DS_Destroy(coord_equal);
    OGR_DS_Destroy(ref_unique);
    OGR_DS_Destroy(new_unique);

    return 0;
}
