#include <iostream>
#include "ogr_api.h"

using namespace std;


int main()
{
    //register all format drivers built into GDAL/OGR -logrsf_frmts, -fexceptions -logrsf_frmts

    OGRRegisterAll();

    OGRDataSourceH hDS;
    OGRLayerH hLayer;
    OGRFeatureH hFeature;

    int iField;

    hDS = OGROpen( "points.shp", FALSE, NULL );

    if( hDS == NULL )
    {
        printf( "Open failed.\n" );
        exit( 1 );
    }

    hLayer = OGR_DS_GetLayerByName( hDS, "point" );

    //OGR_L_ResetReading(hLayer);
    cout << "1";
    while( (hFeature = OGR_L_GetNextFeature(hLayer)) != NULL )
    {
        /*
        OGRFeatureDefnH hFDefn = OGR_L_GetLayerDefn(hLayer);


        for( iField = 0; iField < OGR_FD_GetFieldCount(hFDefn); iField++ )
        {
            OGRFieldDefnH hFieldDefn = OGR_FD_GetFieldDefn( hFDefn, iField );

            if( OGR_Fld_GetType(hFieldDefn) == OFTInteger )
                printf( "%d,", OGR_F_GetFieldAsInteger( hFeature, iField ) );
            else if( OGR_Fld_GetType(hFieldDefn) == OFTReal )
                printf( "%.3f,", OGR_F_GetFieldAsDouble( hFeature, iField) );
            else if( OGR_Fld_GetType(hFieldDefn) == OFTString )
                printf( "%s,", OGR_F_GetFieldAsString( hFeature, iField) );
            else
                printf( "%s,", OGR_F_GetFieldAsString( hFeature, iField) );
        }*/

        OGRGeometryH hGeometry;

        hGeometry = OGR_F_GetGeometryRef(hFeature);
        if( hGeometry != NULL && wkbFlatten(OGR_G_GetGeometryType(hGeometry)) == wkbPoint )
        {
            printf( "%.3f,%3.f\n", OGR_G_GetX(hGeometry, 0), OGR_G_GetY(hGeometry, 0) );
        }
        else
        {
            printf( "no point geometry\n" );
        }

    }

    return 0;
}



