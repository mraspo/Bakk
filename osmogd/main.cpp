/*
*Title:         OSMOGDcompare
*Author:        Michael Raspotnig
*University:    TU Graz, Geomatics Engineering
*Work-frame:    Bachelor thesis
*Department:    Geoinformation
*Semester:      WS/SS 2013/14
*/

#include <iostream>
#include "ogr_api.h"


using namespace std;


int main()
{
    //register all format drivers built into GDAL/OGR -logrsf_frmts, -fexceptions -logrsf_frmts
    OGRRegisterAll();

    //input OGR datasource
    OGRDataSourceH hDS;

    //layer of OGR datasource
    OGRLayerH hLayer;

    //features from layer
    OGRFeatureH hFeature;

    int iField;

    //open shapefile
    hDS = OGROpen( "points.shp", FALSE, NULL ); //FALSE: no update access

    //check if opening shapefile failed
    if( hDS == NULL )
    {
        printf( "Open failed.\n" );
        exit( 1 );
    }

    //get the layer, shown in QuantumGIS
    hLayer = OGR_DS_GetLayerByName( hDS, "points" );

    //to start at beginning of the layer
    OGR_L_ResetReading(hLayer);

    //iterate through all features in the layer
    while( (hFeature = OGR_L_GetNextFeature(hLayer)) != NULL )
    {
        //to dump all the attribute fields of the feature
        OGRFeatureDefnH hFDefn = OGR_L_GetLayerDefn(hLayer);

        //loop over all fields, fetch and report the attributes based on their type
        for( iField = 0; iField < OGR_FD_GetFieldCount(hFDefn); iField++ )
        {
            OGRFieldDefnH hFieldDefn = OGR_FD_GetFieldDefn( hFDefn, iField );

            if( OGR_Fld_GetType(hFieldDefn) == OFTInteger )
                //printf( "%d,", OGR_F_GetFieldAsInteger( hFeature, iField ) );
                cout << OGR_F_GetFieldAsInteger( hFeature, iField ) << endl;
            else if( OGR_Fld_GetType(hFieldDefn) == OFTReal )
                //printf( "%.3f,", OGR_F_GetFieldAsDouble( hFeature, iField) );
                cout << OGR_F_GetFieldAsDouble( hFeature, iField ) << endl;
            else if( OGR_Fld_GetType(hFieldDefn) == OFTString )
                //printf( "%s,", OGR_F_GetFieldAsString( hFeature, iField) );
                cout << OGR_F_GetFieldAsString( hFeature, iField ) << endl;
            else
                //printf( "%s,", OGR_F_GetFieldAsString( hFeature, iField) );
                cout << OGR_F_GetFieldAsString( hFeature, iField ) << endl;

            //GetFieldAsString wäre für alle Typen möglich, würde die Sache verkürzen
        }

        //geometry from feature
        OGRGeometryH hGeometry;

        hGeometry = OGR_F_GetGeometryRef(hFeature);

        //if its a point print it
        if( hGeometry != NULL && wkbFlatten(OGR_G_GetGeometryType(hGeometry)) == wkbPoint )
        {
            printf( "%.3f,%3.f\n", OGR_G_GetX(hGeometry, 0), OGR_G_GetY(hGeometry, 0) );
        }
        else
        {
            //printf( "no point geometry\n" );
            cout << "no point geometry" << endl;
        }
        OGR_F_Destroy( hFeature );


    }
    OGR_DS_Destroy( hDS );

    return 0;
}



