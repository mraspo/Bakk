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

    //reading from shapefile

    //register all format drivers built into GDAL/OGR -logrsf_frmts, -fexceptions -logrsf_frmts
    OGRRegisterAll();

    //input OGR datasource
    OGRDataSourceH referenceShape_source;
    OGRDataSourceH newShape_source;
    OGRDataSourceH refShape_unique_source;
    OGRDataSourceH newShape_unique_source;
    OGRDataSourceH hDS_5;

    //layer of OGR datasource
    OGRLayerH referenceShape_layer;
    OGRLayerH newShape_layer;
    OGRLayerH hLayer_3;
    OGRLayerH hLayer_4;
    OGRLayerH hLayer_5;

    //features from layer
    OGRFeatureH referenceShape_feature;
    OGRFeatureH newShape_feature;

    int iCounter = 1;
    int iLength = 0;
    int iFlag = 0;
    int iField;
    int iField2;
    int iEqual = 0;
    int iLoop = 1;
    double d;

    char* shapename1;
    char* shapename2;
    char* lName1;
    char* lName2;

    const char *pszDriverName = "ESRI Shapefile";
    OGRSFDriverH hDriver;
    OGRFieldDefnH hFieldDefn;


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

    //to start at beginning of the layer
    OGR_L_ResetReading(referenceShape_layer);

    OGRGeometryH hGeometry_1;
    OGRGeometryH hGeometry_2;
    OGRFeatureDefnH hFDefn_reference;
    OGRFeatureDefnH hFDefn_new;


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

                if(OGR_G_Within(hGeometry_2, OGR_G_Buffer(hGeometry_1, 1, 30)))
                {
                    iFlag = 1;

                    OGRFieldDefnH hFieldDefn_reference;
                    OGRFieldDefnH hFieldDefn_new;

                    for( iField = 0; iField < OGR_FD_GetFieldCount(hFDefn_reference); iField++ )
                    {
                        hFieldDefn_reference = OGR_FD_GetFieldDefn( hFDefn_reference, iField );

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
                        cout << "equal" << endl;
                    }
                    else
                    {
                        //in gleicheKoordinaten_unterschiedlicheAttribute schreiben
                    }
                    //in unverändert schreiben
                }
                else
                {
                    //hGeometry2 neu, in hinzugekommen und

                    if(iFlag == 0)
                    {
                        //hGeometry1 in gelöscht schreiben
                    }

                }


            }
            else
            {
                cout << "no point geometry" << endl;
            }

        }

    }
    OGR_F_Destroy(referenceShape_feature);
    OGR_F_Destroy(newShape_feature);

    OGR_DS_Destroy(referenceShape_source);
    OGR_DS_Destroy(newShape_source);

    return 0;
}



