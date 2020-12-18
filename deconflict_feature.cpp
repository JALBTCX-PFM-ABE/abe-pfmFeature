
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



#include "pfmFeature.hpp"


//  This is the sort function for qsort.

static int32_t compare_depths (const void *a, const void *b)
{
    NV_F64_COORD3 *sa = (NV_F64_COORD3 *)(a);
    NV_F64_COORD3 *sb = (NV_F64_COORD3 *)(b);

    return (sa->z < sb->z ? 0 : 1);
}



//  Deconflict the possible features.

int32_t deconflict_feature (NV_F64_COORD2 order, int32_t passes, int32_t pfm_handle, OPTIONS *options,
                            int32_t *features_count, FEATURE **features, RUN_PROGRESS *progress)
{
  int32_t              percent = 0, old_percent = -1;
  double               bin_size[2], cut;
  QString              string;


  //  IMPORTANT ASSUMPTION: For the 1.5 or 3.0 meter bin size we are assuming that the object may be as small as a one or 
  //  two meter cube.  Given this assumption we will search for conflicts at either 1.414 or 2.83 meters.  This is the 
  //  diagonal of a one or two meter square.  For the larger features we will use the diagonal of the bin size.  Is this 
  //  correct?  What does blue sound like?  How hot is tweed?  Who the hell knows!


  int32_t decon_count = 0;


  //  We need to sort our "passes" arrays of possible features by depth.

  for (int32_t i = 0 ; i < passes ; i++)
    {
      if (features_count[i]) qsort (features[i], features_count[i], sizeof (FEATURE), compare_depths);
    }



  //  If we had an area file we want to invalidate any points that fall outside our area.

  if (options->polygon_count)
    {
      for (int32_t i = 0 ; i < passes ; i++)
        {
          for (int32_t j = 0 ; j < features_count[i] ; j++)
            {
              if (!inside (options->polygon_x, options->polygon_y, options->polygon_count, features[i][j].x, features[i][j].y))
                {
                  features[i][j].z = -14000.0;
                  decon_count++;
                }
            }
        }
    }


  //  Now deconflict the features in each array with the other features in the same array.  The rationale is:
  //
  //  1) take shoalest feature
  //  2) compare to all other features
  //  3) if another feature is within bin_size * 1.414 meters (3, 6, or 12 meters) of the shoaler feature, mark the feature as invalid
  //  4) wash, rinse, repeat

  for (int32_t i = 0 ; i < passes ; i++)
    {
      bin_size[0] = pow (2.0, (double) i) * 1.5 * order.x;


      //  For the first pass of a special order or order one deconfliction we want to limit the search radius
      //  to the diagonal of the cube.  For subsequent passes or order two we use twice the diagonal of the bin size.

      double radius = bin_size[0] * 1.414;
      if (!i && order.x <= 2.0) radius = order.x * 1.414;


      string = pfmFeature::tr ("Feature deconfliction, pass %1 of %2, %L3 meter search radius").arg (i + 1).arg (passes).arg (radius, 0, 'f', 3);
      progress->dbox->setTitle (string);
      qApp->processEvents ();


      if (features_count[i] > 1)
        {
          for (int32_t j = 0 ; j < features_count[i] ; j++)
            {
              if (features[i][j].z > -13000.0)
                {
                  for (int32_t k = j + 1 ; k < features_count[i] ; k++)
                    {
                      if (features[i][k].z > -13000.0)
                        {
                          pfm_geo_distance (pfm_handle, features[i][j].y, features[i][j].x, features[i][k].y, features[i][k].x, &cut);

                          if (cut < radius)
                            {
                              decon_count++;
                              features[i][k].z = -14000.0;
                            }
                        }
                    }
                }


              percent = NINT (((float) j / (float) features_count[i]) * 100.0);
              if (percent != old_percent)
                {
                  progress->dbar->setValue (percent);
                  old_percent = percent;
                }

              qApp->processEvents ();
            }

          progress->dbar->setValue (100);
          qApp->processEvents ();
        }
    }


  //  Deconflict all bin sizes against all other bin sizes.

  for (int32_t i = 0 ; i < passes ; i++)
    {
      //  Make sure we have some possible features in this bin size.

      if (features_count[i])
        {
          //  Compute the bin size (starts at 1.5 or 3.0 and doubles going up - 1.5, 3, 6, 12, 24...)

          bin_size[0] = pow (2.0, (double) i) * 1.5 * order.x;


          //  Inner loop.

          for (int32_t j = 0 ; j < passes ; j++)
            {
              //  Make sure we have some possible features in this bin size.

              if (j != i && features_count[j])
                {
                  //  Compute the bin size (starts at 1.5 or 3.0 and doubles going up - 3, 6, 12, 24...)

                  bin_size[1] = pow (2.0, (double) j) * 1.5 * order.x;


                  //  We only want to compare smaller bin possible features to larger bin possible features.

                  if (bin_size[0] < bin_size[1])
                    {
                      string = pfmFeature::tr ("Feature deconfliction, pass %1 of %2, %L3m bin vs %L4m bin").arg
                        (i + 1).arg (passes).arg (bin_size[0], 0, 'f', 1).arg (bin_size[1], 0, 'f', 1);
                      progress->dbox->setTitle (string);
                      qApp->processEvents ();


                      //  For the first pass of a special order or order one deconfliction we want to limit the search radius
                      //  to the diagonal of the cube.  For subsequent passes or order two we use twice the diagonal of the bin
                      //  size.  Note that we're using the [i] loop to determine the search radius.

                      double radius = bin_size[0] * 1.414;
                      if (!i && order.x <= 2.0) radius = order.x * 1.414;


                      //  Loop through the first possible feature list (remember, it's sorted shoalest to deepest).

                      for (int32_t k = 0 ; k < features_count[i] ; k++)
                        {
                          //  Make sure the possible feature is valid (we may have invalidated it in a previous pass).

                          if (features[i][k].z > -13000.0)
                            {
                              //  Loop through the second possible feature list.

                              for (int32_t m = 0 ; m < features_count[j] ; m++)
                                {
                                  //  Make sure the possible feature is valid.

                                  if (features[j][m].z > -13000.0)
                                    {
                                      //  Get the distance between the two possible features.  We use pfm_geo_distance because it is
                                      //  about 8 times faster than computing the actual distance.  The difference in most cases is
                                      //  on the order of a millimeter.

                                      pfm_geo_distance (pfm_handle, features[i][k].y, features[i][k].x, features[j][m].y,
                                                        features[j][m].x, &cut);


                                      //  If the distance is less than our deconflict radius we might need to get rid of one of these.

                                      if (cut < radius)
                                        {
                                          //  If the second list feature is deeper we want to invalidate it otherwise we do nothing.

                                          decon_count++;
                                          if (features[j][m].z >= features[i][k].z)
                                            {
                                              features[j][m].z = -14000.0;
                                            }
                                          else
                                            {
                                              features[i][k].z = -14000.0;
                                            }
                                        }
                                    }
                                }
                            }

                          percent = NINT (((float) k / (float) features_count[i]) * 100.0);
                          if (percent != old_percent)
                            {
                              progress->dbar->setValue (percent);
                              old_percent = percent;
                            }

                          qApp->processEvents ();
                        }
                    }

                  progress->dbar->setValue (100);
                  qApp->processEvents ();
                }
            }
        }
    }


  //  If we had an exclusion area file we want to invalidate any points that fall inside our exclusion area.

  if (options->ex_polygon_count)
    {
      for (int32_t i = 0 ; i < passes ; i++)
        {
          for (int32_t j = 0 ; j < features_count[i] ; j++)
            {
              if (features[i][j].z > -13000.0 && inside (options->ex_polygon_x, options->ex_polygon_y, options->ex_polygon_count,
                                                         features[i][j].x, features[i][j].y))
                {
                  features[i][j].z = -14000.0;
                  decon_count++;
                }
            }
        }
    }


  return (decon_count);
}
