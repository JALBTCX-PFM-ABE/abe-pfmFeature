
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



QString orderText = 
  optionPage::tr ("Select the IHO order that will be used to make the feature selections.  IHO special order is "
                  "based on a one meter cube object in less than 40 meters of water and 5 percent of depth over "
                  "40 meters.  IHO order 1 is based on a two meter cube object in less than 40 meters of water "
                  "and 10 percent of depth over 40 meters.");

QString zeroText = 
  optionPage::tr ("Set this if you want to select objects above the zero level of the data.  Note that if you "
                  "haven't corrected the data to the final datum this could be meaningless.");

QString hpcText = 
  optionPage::tr ("Set this if you want to compute the confidence value of selected features using the "
                  "<b>Hockey Puck of Confidence (TM)</b>.  This will set the confidence level of the feature based on the "
                  "number of points within a hockey puck shaped region around the feature point whose dimensions are twice the horizontal "
                  "uncertainty of the feature point in X/Y and twice the vertical uncertainty of the feature point in Z.  The criteria "
                  "is that there must be at least 2 other points in the HPC area and the points must be from more than one "
                  "line to set the confidence to 5 (verified).  Otherwise, the confidence level is set to 3.<br><br>"
                  "<b>IMPORTANT NOTE: If you do not have multiple line coverage in an area this is just a waste of computation time.</b>");

QString threshText = 
  optionPage::tr ("Select the number of deep points that exceed the IHO object size threshold that are required in each octant in order for the shoal "
                  "point to be flagged as a possible feature.  Setting this to 1 means that the algorithm only has to find one point that is at least the "
                  "IHO object size threshhold below the shoal point in each octant in order to flag the shoal point as a possible feature.  If your data "
                  "is very clean (and that includes in the downward direction) then using 1 is fine.  Since, as we all know, hydrographers are lazy (like "
                  "most smart people), your data is probably not very clean in the downward direction.  If this is the case then you probably want to "
                  "increase this value so that you don't flag shoal points due to downward outliers in your data.  If you're a hydrographer and your data "
                  "is not very clean in the upward, or shoal, direction then you probably shouldn't be allowed to play with sharp objects.<br><br>"
                  "<b>IMPORTANT NOTE: Increasing this value will increase the amount of time it will take to run pfmFeature.  On the bright side, "
                  "since you don't have to sit and watch it run it probably doesn't make any difference.</b>");

QString milliText = 
  optionPage::tr ("This is the millimeters value that is used for the final deconflict pass.  The millimeters field is combined with the chart scale "
                  "field to compute a radius.  The features are taken, shoalest to deepest, and compared.  If a deeper feature is in the radius "
                  "of a shoaler feature, it will be marked as a child of the shoaler feature.  The formula used to compute the radius is:<br><br>"
                  "radius = (millimeters / 1000.0) * chart_scale.<br><br>"
                  "<b>IMPORTANT NOTE: Parent features will be marked in the PFM as PFM_SELECTED_FEATURE.  Child features will be marked as "
                  "PFM_DESIGNATED_SOUNDING.  Regardless of this marking, all of the features will be added to the tracking list if used in pfmBag.</b>");

QString scaleText = 
  optionPage::tr ("This is the chart scale that is used for the final deconflict pass.  The millimeters field is combined with the chart scale "
                  "field to compute a radius.  The features are taken, shoalest to deepest, and compared.  If a deeper feature is in the radius "
                  "of a shoaler feature, it will be marked as a child of the shoaler feature.  The formula used to compute the radius is:<br><br>"
                  "radius = (millimeters / 1000.0) * chart_scale.<br><br>"
                  "<b>IMPORTANT NOTE: Parent features will be marked in the PFM as PFM_SELECTED_FEATURE.  Child features will be marked as "
                  "PFM_DESIGNATED_SOUNDING.  Regardless of this marking, all of the features will be added to the tracking list if used in pfmBag.</b>");

QString descriptionText = 
  optionPage::tr ("This is the default description used for all of the selected features.  You may leave this "
                  "blank, choose one of the standard descriptions using the buttons, or set a description manually.  "
                  "<b>All</b> of the features will have this description but they can be individually modified in "
                  "pfmView or pfmEdit(3D).");

QString typeBoxText = 
  optionPage::tr ("Select an item then click on OK to use the selected item for the feature description.");

QString offsetText = 
  optionPage::tr ("Set an offset value for the input depth values.  This is useful if you have not applied a final datum shift to your "
                  "data but want to use pfmFeature to create a feature file to help in editing the data.<br><br>"
                  "<b>IMPORTANT NOTE: The offset value is <i>added</i> to the Z value.</b>");

QString egmText = 
  optionPage::tr ("Press this button to retrieve the EGM08 model value of the ellipsoid to geoid (approximate mean sea "
                  "level) datum offset for the point nearest the center of the minimum bounding rectangle of the displayed "
                  "area.  This will be placed in the <b>Offset</b> field to be used to shift the input depth values.");

