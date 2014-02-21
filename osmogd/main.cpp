#include <iostream>
#include "ogrsf_frmts.h"
//#include "ogr_api.h"

using namespace std;


int main()
{
    //register all format drivers built into GDAL/OGR
    OGRRegisterAll();
    /*
    OGRDataSourceH hDS;
    OGRLayerH hLayer;

    hDS = OGROpen( "points.shp", FALSE, NULL );
    if( hDS == NULL )
    {
        printf( "Open failed.\n" );
        exit( 1 );
    }



    hLayer = OGR_DS_GetLayerByName( hDS, "point" );
    */

    //open the input OGR datasource
    OGRDataSource       *poDS;

    poDS = OGRSFDriverRegistrar::Open( "points.shp", FALSE );

    //access check
    if( poDS == NULL )
    {
        printf( "Open failed.\n" );
        exit( 1 );
    }


    return 0;
}



