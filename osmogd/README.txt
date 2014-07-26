----- README for OSMOGDcompare -----

Precondition: This software was developed and tested on Windows7 Professional, Service Pack 1. 
			  It is very likely that it wouldn't work on Linux or other OS.

Requirements:
- The executable file (osmogd.exe) has to be in the same folder as the OSM and OGD source-datafiles, no subfolders are allowed.
- The folder has to contain the following files:
	-- the executable file of 7zip (7z.exe) and furthermore 7zip has to be installed on the system (only if .bz2 and .gz are used)
	-- the executable file of ogr2ogr (ogr2ogr.exe)
	-- the executable file of osmconvert (osmconvert.exe)
	-- the two .csv files gcs.csv and pcs.csv
	-- the osmconf.ini file
- The user of this software has to have write-permissions on the folder; it would be perfect if he has admin-rights on the software and the folder.
- Maybe a firewall-exception is needed for the software as it uses the "system"-command and could therefore be misinterpreted as malware.

Course of action:
- Start the software by double-clicking the executable file.
- Enter the name of the OSM-sourcefile (i.e. osm_input.shp). The possible formats are .osm, .pbf, .osm.gz, .osm.bz2 and .shp.
  If the name of the entered file does not exist in the folder or the fileformat is not supported the program reports an error and closes down;
  the procedure has to be started from the beginning.
- Then the name of the OGD-sourcefile has to be entered (i.e. ogd_input.geojson). The possible formats are .csv, .geojson and .shp.
  If the name of the entered file does not exist in the folder or the fileformat is not supported the program reports an error and closes down;
  the procedure has to be started from the beginning.
  The format .csv only works when the integer and the decimal places of the coordinates are separated by a dot instead of a comma (i.e. 47.123N,
  15.456E). This circumstance is caused by the conversion-program ogr2ogr.
- If the entered files exist and their formats are supported the coordinate buffer could be entered in the next step. The unit of the entered buffer 
  is meters, the software recalculates it in degree. The integer and the decimal places of the buffer have to be separated by a dot (i.e. 0.1 m).
- In the last step the format for the output files has to be entered. The possible formats are .csv, .geojson and .shp.
  The format has to be entered without a leading dot (i.e. geojson). 
- As a result, the user gets 4 subfolders in the main-folder where the software is running. 
  These folders are: 
	-- "1_all_equal" which contains all points that are completely equal (coordinates and attributes) in both files (OSM and OGD) 
	-- "2_coordinates_equal" which contains all points where the coordinates are equal but the attributes are at least in one case different 
	-- "3_osm_unique" which contains all points that are only present in the OSM file and missing in the OGD file (deleted points)
	-- "4_ogd_unique" which contains all points that are only present in the OGD file and missing in the OSM file (new points)
  The folders contain the files in one/two formats: by default in the .shp format and in the user specified format

IMPORTANT: To start a new cycle the 4 resulting subfolders from the last cycle have to be moved to another folder or have to be deleted.  



