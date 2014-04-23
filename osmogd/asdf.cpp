int main()
{

    //reading from shapefile

    //register all format drivers built into GDAL/OGR -logrsf_frmts, -fexceptions -logrsf_frmts
    OGRRegisterAll();

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

    //----create output files

    const char *pszDriverName = "ESRI Shapefile";
    OGRSFDriverH hDriver;

    hDriver = OGRGetDriverByName( pszDriverName );
    if( hDriver == NULL )
    {
        printf( "%s Driver not available.\n", pszDriverName );
        exit( 1 );
    }

    //OGRDataSourceH ref_unique;
    //OGRDataSourceH new_unique;
    //OGRDataSourceH coord_equal;
    OGRDataSourceH all_equal;

    //ref_unique = OGR_Dr_CreateDataSource( hDriver, "reference_unique.shp", NULL );
    //new_unique = OGR_Dr_CreateDataSource( hDriver, "new_unique.shp", NULL );
    //coord_equal = OGR_Dr_CreateDataSource( hDriver, "coordinates_equal.shp", NULL );
    all_equal = OGR_Dr_CreateDataSource( hDriver, "all_equal.shp", NULL );

    if( all_equal == NULL)//ref_unique == NULL || new_unique == NULL || coord_equal == NULL ||
    {
        printf( "Creation of output file failed.\n" );
        exit( 1 );
    }

    //OGRLayerH hLayer_ref;
    //OGRLayerH hLayer_new;
    //OGRLayerH hLayer_coord;
    OGRLayerH hLayer_all;

    //hLayer_ref = OGR_DS_CreateLayer( ref_unique, "reference_unique", NULL, wkbPoint, NULL );
    //hLayer_new = OGR_DS_CreateLayer( new_unique, "new_unique", NULL, wkbPoint, NULL );
    //hLayer_coord = OGR_DS_CreateLayer( coord_equal, "coordinates_equal", NULL, wkbPoint, NULL );
    hLayer_all = OGR_DS_CreateLayer( all_equal, "all_equal", NULL, wkbPoint, NULL );

    if( hLayer_all == NULL)//hLayer_ref == NULL || hLayer_new == NULL || hLayer_coord == NULL ||
    {
        printf( "Layer creation failed.\n" );
        exit( 1 );
    }


/*

    if( OGR_L_CreateField( hLayer, hFieldDefn, TRUE ) != OGRERR_NONE )
    {
        printf( "Creating Name field failed.\n" );
        exit( 1 );
    }

    OGR_Fld_Destroy(hFieldDefn);
    //-------
*/
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


    //------------field names ermitteln

    referenceShape_feature = OGR_L_GetNextFeature(referenceShape_layer);
    newShape_feature = OGR_L_GetNextFeature(newShape_layer);

    hGeometry_1 = OGR_F_GetGeometryRef(referenceShape_feature);
    hGeometry_2 = OGR_F_GetGeometryRef(newShape_feature);

    hFDefn_reference = OGR_L_GetLayerDefn(referenceShape_layer);
    hFDefn_new = OGR_L_GetLayerDefn(newShape_layer);

    OGRFieldDefnH hFieldDefn_reference;
    OGRFieldDefnH hFieldDefn_new;
    OGRFieldDefnH hFieldDefn_input;

    for( iField = 0; iField < OGR_FD_GetFieldCount(hFDefn_reference); iField++ )
    {
            hFieldDefn_reference = OGR_FD_GetFieldDefn( hFDefn_reference, iField );
            //cout <<  OGR_Fld_GetNameRef(hFieldDefn_reference) << endl;

            hFieldDefn_input = OGR_Fld_Create( OGR_Fld_GetNameRef(hFieldDefn_reference), OFTString );

            OGR_Fld_SetWidth( hFieldDefn_input, OGR_Fld_GetWidth(hFieldDefn_reference));

            if( OGR_L_CreateField( hLayer_all, hFieldDefn_input, TRUE ) != OGRERR_NONE)//OGR_L_CreateField( hLayer_ref, hFieldDefn_input, TRUE ) != OGRERR_NONE &&
            {
                printf( "Creating field failed.\n" );
                exit( 1 );
            }

            OGR_Fld_Destroy(hFieldDefn_input);
    }
    /*
    for(iField2 = 0; iField2 < OGR_FD_GetFieldCount(hFDefn_new); iField2++)
    {
            hFieldDefn_new = OGR_FD_GetFieldDefn( hFDefn_new, iField2 );
            //cout <<  OGR_Fld_GetNameRef(hFieldDefn_new) << endl;

            OGRFieldDefnH hFieldDefn_input;

            hFieldDefn_input = OGR_Fld_Create( OGR_Fld_GetNameRef(hFieldDefn_new), OFTString );

            OGR_Fld_SetWidth( hFieldDefn_input, 32);

            if( OGR_L_CreateField( hLayer_new, hFieldDefn_input, TRUE ) != OGRERR_NONE && OGR_L_CreateField( hLayer_coord, hFieldDefn_input, TRUE ) != OGRERR_NONE)
            {
                printf( "Creating field failed.\n" );
                exit( 1 );
            }

            //OGR_Fld_Destroy(hFieldDefn_input);
    }*/

    //to start at beginning of the layer
    OGR_L_ResetReading(referenceShape_layer);

while(ii < 3)
{
    OGRFeatureH hFeature_all;
        OGRGeometryH hPt;

        hFeature_all = OGR_F_Create( OGR_L_GetLayerDefn( hLayer_all ) );

                            //hFieldDefn_reference = OGR_FD_GetFieldDefn( hFDefn_reference, iField );

                            //cout << OGR_F_GetFieldIndex(referenceShape_feature, "name") << endl; OGR_F_GetFieldAsString(referenceShape_feature, 0)
                            //for( iField = 0; iField < OGR_FD_GetFieldCount(hFDefn_reference); iField++ )
                            //{
                                OGR_F_SetFieldString( hFeature_all, OGR_F_GetFieldIndex(hFeature_all, "osm_id"), "adsf");
                                //cout <<  OGR_F_GetFieldIndex(hFeature_all, "osm_id") << endl;
                                OGR_F_SetFieldString( hFeature_all, OGR_F_GetFieldIndex(hFeature_all, "timestamp"), "adf");
                                //OGR_F_SetFieldString( hFeature_all, OGR_F_GetFieldIndex(hFeature_all, "name"), "adf");
                                //OGR_F_SetFieldString( hFeature_all, OGR_F_GetFieldIndex(hFeature_all, "type"), "adsf");
                            //}
                            //OGR_F_GetFieldIndex(hFeature_all, OGR_Fld_GetNameRef(hFieldDefn_input))
                            //OGR_F_GetFieldIndex(hFeature_all, "")

                            hPt = OGR_G_CreateGeometry(wkbPoint);


                            OGR_G_SetPoint_2D(hPt, 0, OGR_G_GetX(hGeometry_1, 0), OGR_G_GetY(hGeometry_1, 0));

                            OGR_F_SetGeometry( hFeature_all, hPt );
                            OGR_G_DestroyGeometry(hPt);

                            if( OGR_L_CreateFeature( hLayer_all, hFeature_all ) != OGRERR_NONE )
                            {
                                 printf( "Failed to create feature in shapefile.\n" );
                                 exit( 1 );
                            }

                            OGR_F_Destroy( hFeature_all );
                            ii++;
}


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

                if(OGR_G_Within(hGeometry_2, OGR_G_Buffer(hGeometry_1, 1, 30)))
                {
                    iFlag = 1;

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

                       // for( iField = 0; iField < OGR_FD_GetFieldCount(hFDefn_reference); iField++ )
                         //{
                         /*
                            OGRFeatureH hFeature_all;
                            OGRGeometryH hPt;

                            hFeature_all = OGR_F_Create( OGR_L_GetLayerDefn( hLayer_all ) );

                            //hFieldDefn_reference = OGR_FD_GetFieldDefn( hFDefn_reference, iField );

                            //cout << OGR_F_GetFieldIndex(referenceShape_feature, "name") << endl; OGR_F_GetFieldAsString(referenceShape_feature, 0)
                            for( iField = 0; iField < OGR_FD_GetFieldCount(hFDefn_reference); iField++ )
                            {
                                OGR_F_SetFieldString( hFeature_all, iField, OGR_F_GetFieldAsString(referenceShape_feature, iField));
                            }
                            //OGR_F_GetFieldIndex(hFeature_all, OGR_Fld_GetNameRef(hFieldDefn_input))
                            //OGR_F_GetFieldIndex(hFeature_all, "")

                            hPt = OGR_G_CreateGeometry(wkbPoint);

                            OGR_G_SetPoint_2D(hPt, 0, OGR_G_GetX(hGeometry_1, 0), OGR_G_GetY(hGeometry_1, 0));

                            OGR_F_SetGeometry( hFeature_all, hPt );
                            OGR_G_DestroyGeometry(hPt);

                            if( OGR_L_CreateFeature( hLayer_all, hFeature_all ) != OGRERR_NONE )
                            {
                                 printf( "Failed to create feature in shapefile.\n" );
                                 exit( 1 );
                            }

                            OGR_F_Destroy( hFeature_all );*/
                        //cout <<  OGR_Fld_GetNameRef(hFieldDefn_reference) << endl;
                        //cout << OGR_Fld_GetWidth(hFieldDefn_reference) << endl;

                        //cout << "equal" << endl;
                      //}

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
