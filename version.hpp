
/*********************************************************************************************

    This is public domain software that was developed by or for the U.S. Naval Oceanographic
    Office and/or the U.S. Army Corps of Engineers.

    This is a work of the U.S. Government. In accordance with 17 USC 105, copyright protection
    is not available for any work of the U.S. Government.

    Neither the United States Government, nor any employees of the United States Government,
    nor the author, makes any warranty, express or implied, without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, or assumes any liability or
    responsibility for the accuracy, completeness, or usefulness of any information,
    apparatus, product, or process disclosed, or represents that its use would not infringe
    privately-owned rights. Reference herein to any specific commercial products, process,
    or service by trade name, trademark, manufacturer, or otherwise, does not necessarily
    constitute or imply its endorsement, recommendation, or favoring by the United States
    Government. The views and opinions of authors expressed herein do not necessarily state
    or reflect those of the United States Government, and shall not be used for advertising
    or product endorsement purposes.
*********************************************************************************************/


/****************************************  IMPORTANT NOTE  **********************************

    Comments in this file that start with / * ! or / / ! are being used by Doxygen to
    document the software.  Dashes in these comment blocks are used to create bullet lists.
    The lack of blank lines after a block of dash preceeded comments means that the next
    block of dash preceeded comments is a new, indented bullet list.  I've tried to keep the
    Doxygen formatting to a minimum but there are some other items (like <br> and <pre>)
    that need to be left alone.  If you see a comment that starts with / * ! or / / ! and
    there is something that looks a bit weird it is probably due to some arcane Doxygen
    syntax.  Be very careful modifying blocks of Doxygen comments.

*****************************************  IMPORTANT NOTE  **********************************/




#ifndef VERSION

#define     VERSION     "PFM Software - pfmFeature V5.30 - 08/07/19"

#endif

/*! <pre>

    Version 1.0
    Jan C. Depner
    01/05/09

    First working version.


    Version 1.01
    Jan C. Depner
    01/08/09

    Fixed min_index not set bug.


    Version 1.02
    Jan C. Depner
    01/22/09

    Changed first pass of special order to use 1.5 meter bin size instead of 3.0 meter bin size.  Also,
    added exclusion area file input.


    Version 1.03
    Jan C. Depner
    01/29/09

    Added ability to append to an existing feature file.  We now place the feature file name in the PFM structure.


    Version 2.00
    Jan C. Depner
    02/18/09

    We no longer search for conflicts using the larger bin diagonal against a smaller bin.  There is a pathological
    case that could cause us to invalidate a smaller size object when we don't really want to.  It would look something
    like this:

                                                               X____________________
                      x                                        |                    \________________________________
                     / \                                       |                                                     \
                    /   \                                      |                                                      \
                   /     \                                     |                                                       |
    ______________/       \___________________________________/                                                        |


    where x is the smaller bin feature and X is the larger bin feature.


    Version 2.01
    Jan C. Depner
    02/24/09

    Changed name from pfmTarget to pfmFeature to be compatible with IHO terminology.


    Version 2.10
    Jan C. Depner
    04/05/09

    Replaced support for NAVO standard target file (XML with support for Binary Feature Data (BFD) files.


    Version 2.11
    Jan C. Depner
    04/10/09

    Tries to get the actual event time associated with each feature's selected depth.


    Version 3.00
    Jan C. Depner
    05/01/09

    Where to begin?  The logic (and I use that term loosely) that I was using to determine the bin in which
    to store each of the points read from the PFM depth records was, to put it mildly, flawed.  I must have
    been using bistro math.  In addition, the PFM bin start and end row and column loops were set up as
        for (NV_INT32 i = start_row_or_col ; i < end_row_or_col ; i++)
    They should have been :
        for (NV_INT32 i = start_row_or_col ; i <= end_row_or_col ; i++)
    because the upper or right bound falls into the end_row_or_col PFM bin.  In other words I was ignoring 
    one row or column every time I ran through it.  With small PFM bin sizes this was not evident but when you
    kicked the PFM bin size up to larger than the final search bin size (24 meters for order 1) it became
    painfully apparent.  You will still not get exactly the same features if you run this on the same area using
    different PFM bin sizes.  The reason for this is that in some bins there may be two or more points that
    have the same minimum depth value.  The one that is selected depends on the order that the data is read into
    the program.  Obviously, if you are reading three-by-three one-meter bins from a PFM you will not get the 
    same data ordering you will if you read one ten-meter bin that completely surrounds the three-by-three meter
    search bin.


    Version 3.01
    Jan C. Depner
    05/21/09

    Set all QFileDialogs to use List mode instead of Detail mode.


    Version 3.10
    Jan C. Depner
    05/26/09

    Trying to determine the shape of the object (not very well) we grab the nearest point in each
    octant that is below the cutoff and save these as the feature polygon.


    Version 3.11
    Jan C. Depner
    05/28/09

    The shape stuff (above) didn't work too well so I commented it out.


    Version 3.12
    Jan C. Depner
    06/24/09

    Changed the BFD names to avoid collision with GNU Binary File Descriptor library.


    Version 3.13
    Jan C. Depner
    09/22/09

    Fixed bug in get_feature_event_time.cpp.


    Version 4.00
    Jan C. Depner
    09/25/09

    Added the HPC (Hockey Puck of Confidence (TM)) check.  This will set the confidence level of the feature based on the
    number of points within 1.5 meters in X/Y and 0.5 meters in Z of the selected feature point.  The criteria is that there
    must be at least 2 other points in the HPC and the points must be from more than one line.  This doesn't work well
    with sonar data due to the preponderance of crappy data (like Erik's horns, Kjell's hat, etc.).


    Version 4.01
    Jan C. Depner
    03/25/10

    Added PFM_SELECTED_FEATURE flag to each final selected depth record in the PFM so that we can unload it to the GSF files
    if that's what we're unloading (it doesn't do squat for HOF, TOF, etc).


    Version 4.02
    Jan C. Depner
    04/19/10

    If no features are detected we now get rid of the empty .bfd and .bfa files and issue a message to the user.


    Version 4.10
    Jan C. Depner
    06/03/10

    Added optional chart scale dependent deconfliction pass.


    Version 4.20
    Jan C. Depner
    09/17/10

    Added ability to use horizontal uncertainty to try to eliminate false hits due to X,Y jitter.  Also, added max distance from feature
    point to any of the octant points to the output feature description.  This is used when we build our enhanced surface in pfmBag.


    Version 4.21
    Jan C. Depner
    09/23/10

    Fixed octant ordering bug.  Added number of octants with trigger points to the feature remarks.  Added ability to output a feature
    polygon made up of the nearest points in each octant.  This is a hidden option (--polygon or -p command line option) for
    debugging/documentation purposes ONLY since it can greatly increase the amount of time required to run the program.


    Version 4.22
    Jan C. Depner
    10/07/10

    Changed the Hockey Puck of Confidence (HPC) computation to use the horizontal and vertical uncertainties of the selected feature point
    to define the size of the HPC.  Added the horizontal and vertical uncertainties to the feature remarks.


    Version 5.00
    Jan C. Depner
    10/12/10

    The four selection passes are now run concurrently as QThreads.  Depending on the number of processors on your system,
    this can speed up the process by somewhere between three and four times.  Also, removed horizontal uncertainty as an option.
    It is now hard-wired in (since we've cut the processing time in half this isn't such a big issue).


    Version 5.01
    Jan C. Depner
    01/06/11

    Correct problem with the way I was instantiating the main widget.  This caused an intermittent error on Windows7/XP.


    Version 5.10
    Jan C. Depner
    01/21/11

    Added the ability to use an artificial datum offset for data that hasn't yet been datum shifted.  It also allows the
    user to retrieve the EGM08 value to be used as the offset if desired.


    Version 5.11
    Jan C. Depner
    06/27/11

    Save all directories used by the QFileDialogs.  Add current working directory to the sidebar for all QFileDialogs.


    Version 5.12
    Jan C. Depner
    07/22/11

    Using setSidebarUrls function from nvutility to make sure that current working directory (.) and
    last used directory are in the sidebar URL list of QFileDialogs.


    Version 5.13
    Jan C. Depner
    09/21/11

    Replaced compute_index calls with compute_index_ptr calls.


    Version 5.14
    Jan C. Depner
    11/30/11

    Converted .xpm icons to .png icons.


    Version 5.15
    Jan C. Depner (PFM Software)
    12/09/13

    Switched to using .ini file in $HOME (Linux) or $USERPROFILE (Windows) in the ABE.config directory.  Now
    the applications qsettings will not end up in unknown places like ~/.config/navo.navy.mil/blah_blah_blah on
    Linux or, in the registry (shudder) on Windows.


    Version 5.16
    Jan C. Depner (PFM Software)
    02/26/14

    Cleaned up "Set but not used" variables that show up using the 4.8.2 version of gcc.


    Version 5.17
    Jan C. Depner (PFM Software)
    03/17/14

    Removed WLF support.  Top o' the mornin' to ye!
    Also, added CZMIL support in get_feature_time.cpp.


    Version 5.18
    Jan C. Depner (PFM Software)
    07/01/14

    - Replaced all of the old, borrowed icons with new, public domain icons.  Mostly from the Tango set
      but a few from flavour-extended and 32pxmania.


    Version 5.19
    Jan C. Depner (PFM Software)
    07/23/14

    - Switched from using the old NV_INT64 and NV_U_INT32 type definitions to the C99 standard stdint.h and
      inttypes.h sized data types (e.g. int64_t and uint32_t).


    Version 5.20
    Jan C. Depner (PFM Software)
    07/29/14

    - Fixed errors discovered by cppcheck.


    Version 5.21
    Jan C. Depner (PFM Software)
    02/16/15

    - To give better feedback to shelling programs in the case of errors I've added the program name to all
      output to stderr.


    Version 5.22
    Jan C. Depner (PFM Software)
    03/31/15

    - Added ability to use ESRI Polygon, PolygonZ, PolygonM, PolyLine, PolyLineZ, or PolyLineM geographic shape
      files as area files.


    Version 5.23
    Jan C. Depner (PFM Software)
    09/01/15

    - Fixed bug getting CZMIL time values.


    Version 5.24
    Jan C. Depner (PFM Software)
    08/20/16

    - Added support for Binary Feature Data (BFD) 3.0 feature type field.
    - Changed the "hidden" output polygons option to a user option in the options page.


    Version 5.25
    Jan C. Depner (PFM Software)
    08/21/16

    - Added a chart scale deconflict pass.  This computes a radius based on the user's selected millimeters
      at chart scale values then deconflicts making deeper features within the radius "children" of the 
      shoaler "parent" feature.  Parent features are flagged in the PFM as PFM_SELECTED_FEATURE while
      child features are marked as PFM_DESIGNATED_SOUNDING.  These will be set as NV_GSF_SELECTED_DESIGNATED
      and NV_GSF_SELECTED_SPARE_1 repectively in GSF files when unloaded.  They will be set as
      CZMIL_RETURN_SELECTED_FEATURE and CZMIL_RETURN_DESIGNATED_SOUNDING in CZMIL CPF files.  All 
      features will be placed in the tracking list when the resulting file is run through pfmBAG but the
      parent to child relationship will be maintained in the track_code value and as part of the list_series
      description.
    - The resulting BFD file now goes, by default, into the .pfm.data folder.
    - Use the maximum horizontal and vertical errors (octant hit points and shoal point) to define the
      horizontal and vertical errors of the feature.


    Version 5.26
    Jan C. Depner (PFM Software)
    08/24/16

    - Made program always compute polygons for each feature.
    - Added a "threshhold points" option to the Options page.  This allows the user to set the number of points
      required in all octants.  The number ranges from 1 (this was the way pfmFeature used to work) up to 5.
      Higher values may help if you haven't cleaned your data very well in the downard direction.


    Version 5.27
    Jan C. Depner (PFM Software)
    08/27/16

    - Now uses the same font as all other ABE GUI apps.  Font can only be changed in pfmView Preferences.


    Version 5.28
    Jan C. Depner (PFM Software)
    06/04/17

    - Fixed a possible problem with opening and reading HOF and TOF files.  It probably isn't a problem.


    Version 5.29
    Jan C. Depner (PFM Software)
    09/26/17

    - A bunch of changes to support doing translations in the future.  There is a generic
      pfmFeature_xx.ts file that can be run through Qt's "linguist" to translate to another language.


    Version 5.30
    Jan C. Depner (PFM Software)
    08/07/19

    - Now that get_area_mbr supports shape files we don't need to handle it differently from the other
      area file types.

</pre>*/
