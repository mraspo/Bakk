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
#include "ogr_spatialref.h"


using namespace std;


int main()
{
    /*
    //reading from shapefile

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
            printf( "%.7f,%.7f\n", OGR_G_GetX(hGeometry, 0), OGR_G_GetY(hGeometry, 0) );
        }
        else
        {
            //printf( "no point geometry\n" );
            cout << "no point geometry" << endl;
        }
        OGR_F_Destroy( hFeature );


    }
    OGR_DS_Destroy( hDS );
    */



    //reading from and writing to shapefile

    double x, y;
    char szName[33];

    //shapefile driver
    const char *pszDriverName = "ESRI Shapefile";
    OGRSFDriverH hDriver;

    OGRRegisterAll();

    hDriver = OGRGetDriverByName( pszDriverName );
    if( hDriver == NULL )
    {
        printf( "%s driver not available.\n", pszDriverName );
        exit( 1 );
    }

    //create shapefile
    OGRDataSourceH hDS;

    hDS = OGR_Dr_CreateDataSource( hDriver, "point_out.shp", NULL );
    if( hDS == NULL )
    {
        printf( "Creation of output file failed.\n" );
        exit( 1 );
    }

    //create layer, point, no reference system information
    OGRLayerH hLayer;

    OGRSpatialReference oSRS;

    oSRS.SetWellKnownGeogCS( "WGS84" );
//SRS_PT_CYLINDRICAL_EQUAL_AREA

//(OGRSpatialReferenceH *) oSRS SRS_DN_WGS84
    hLayer = OGR_DS_CreateLayer( hDS, "point_out", (OGRSpatialReferenceH *) oSRS , wkbPoint, NULL );
    if( hLayer == NULL )
    {
        printf( "Layer creation failed.\n" );
        exit( 1 );
    }

    //create field
    OGRFieldDefnH hFieldDefn;

    hFieldDefn = OGR_Fld_Create( "Name", OFTString );

    //field width
    OGR_Fld_SetWidth( hFieldDefn, 254); //32

    if( OGR_L_CreateField( hLayer, hFieldDefn, TRUE ) != OGRERR_NONE )
    {
        printf( "Creating Name field failed.\n" );
        exit( 1 );
    }

    OGR_Fld_Destroy(hFieldDefn);

    //reading lines of the form "x,y,name" from stdin, and parsing them
    while( !feof(stdin) && fscanf( stdin, "%lf,%lf,%32s", &x, &y, szName ) == 3 )
    {
        //write feature to disk, set attributes and attach geometry before trying to write it to the layer
        OGRFeatureH hFeature;
        OGRGeometryH hPt;

        hFeature = OGR_F_Create( OGR_L_GetLayerDefn( hLayer ) );
        OGR_F_SetFieldString( hFeature, OGR_F_GetFieldIndex(hFeature, "Name"), szName );

        //local geometry object, assign copy to the feature
        hPt = OGR_G_CreateGeometry(wkbPoint);
        OGR_G_SetPoint_2D(hPt, 0, x, y);

        OGR_F_SetGeometry( hFeature, hPt );
        OGR_G_DestroyGeometry(hPt);

        //create feature in the file
        if( OGR_L_CreateFeature( hLayer, hFeature ) != OGRERR_NONE )
        {
           printf( "Failed to create feature in shapefile.\n" );
           exit( 1 );
        }

        OGR_F_Destroy( hFeature );
   }

   //close down datasource
   OGR_DS_Destroy( hDS );

    return 0;
}



