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
    OGRDataSourceH hDS_1;
    OGRDataSourceH hDS_2;

    //layer of OGR datasource
    OGRLayerH hLayer_1;
    OGRLayerH hLayer_2;

    //features from layer
    OGRFeatureH hFeature_1;
    OGRFeatureH hFeature_2;

    int iCounter = 1;
    int iLength = 0;
    double d;

    char* shapename1;
    char* shapename2;
    char* lName1;
    char* lName2;

    shapename1 = shapeIn(iCounter++);
    shapename2 = shapeIn(iCounter);

    //open shapefile
    hDS_1 = OGROpen(shapename1, FALSE, NULL); //FALSE: no update access
    hDS_2 = OGROpen(shapename2, FALSE, NULL);

    //check if opening shapefile failed
    if( hDS_1 == NULL || hDS_2 == NULL)
    {
        printf( "Open failed.\n" );
        exit( 1 );
    }

    lName1 = layerName(shapename1);
    lName2 = layerName(shapename2);

    //get the layer, shown in QuantumGIS
    hLayer_1 = OGR_DS_GetLayerByName(hDS_1, lName1);
    hLayer_2 = OGR_DS_GetLayerByName(hDS_2, lName2);

    //to start at beginning of the layer
    OGR_L_ResetReading(hLayer_1);

    OGRGeometryH hGeometry_1;
    OGRGeometryH hGeometry_2;

    while( (hFeature_1 = OGR_L_GetNextFeature(hLayer_1)) != NULL)
    {
        //geometry from feature
        hGeometry_1 = OGR_F_GetGeometryRef(hFeature_1);
        OGR_L_ResetReading(hLayer_2);

        while((hFeature_2 = OGR_L_GetNextFeature(hLayer_2)) != NULL)
        {
            //geometry from feature
            hGeometry_2 = OGR_F_GetGeometryRef(hFeature_2);

            if(hGeometry_1 != NULL && hGeometry_2 != NULL && wkbFlatten(OGR_G_GetGeometryType(hGeometry_1))== wkbPoint && wkbFlatten(OGR_G_GetGeometryType(hGeometry_2))== wkbPoint)
            {
                //puffer fehlt noch!!
                cout << OGR_G_Buffer(hGeometry_1, 1, 30) << endl;

                if(OGR_G_GetX(hGeometry_1, 0) == OGR_G_GetX(hGeometry_2, 0) && OGR_G_GetY(hGeometry_1, 0) == OGR_G_GetY(hGeometry_2, 0))
                {
                    cout << "equal" << endl;
                }
                //{
                    //OGR_G_GetX(hGeometry, 0), OGR_G_GetY(hGeometry, 0)
                    //d = OGR_G_GetX(hGeometry_1, 0);
                    //cout << d << endl;
                //}
            }
            else
            {
                cout << "no point geometry" << endl;
            }


        /*

        //if its a point print it
        if( hGeometry != NULL && wkbFlatten(OGR_G_GetGeometryType(hGeometry)) == wkbPoint )
        {
            printf( "%.7f,%.7f\n", OGR_G_GetX(hGeometry, 0), OGR_G_GetY(hGeometry, 0) );
        }
        */
        }

    }
    OGR_F_Destroy(hFeature_1);
    OGR_F_Destroy(hFeature_2);

    OGR_DS_Destroy(hDS_1);
    OGR_DS_Destroy(hDS_2);

    return 0;
}



