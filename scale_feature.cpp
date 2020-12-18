
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



//  Deconflict the features based on the user's selected chart scale.

int32_t scale_feature (int32_t pfm_handle, OPTIONS *options, int32_t total_count, FEATURE *features, RUN_PROGRESS *progress)
{
  int32_t              percent = 0, old_percent = -1;
  double               cut;
  QString              string;


  int32_t scale_count = 0;


  //  We need to sort our features by depth.

  qsort (features, total_count, sizeof (FEATURE), compare_depths);


  //  Now deconflict the features in against all other features within half of millimeters at chart scale distance.  The rationale is:
  //
  //  1) take shoalest feature
  //  2) compare to all other features
  //  3) if another feature is within the computed radius of the shoaler feature, mark the feature as a child of the shoaler feature
  //  4) wash, rinse, repeat

  double radius = (((double) options->millimeters / 1000.0) * options->chart_scale) / 2.0;


  for (int32_t i = 0 ; i < total_count ; i++)
    {
      progress->fbox->setTitle (pfmFeature::tr ("Scaled feature deconfliction"));
      qApp->processEvents ();


      //  Only check valid features and features that aren't already children of another feature.

      if (features[i].z > -13000.0 && !features[i].parent_record)
        {
          int32_t start = i + 1;

          for (int32_t j = start ; j < total_count ; j++)
            {
              //  Only check valid features.

              if (features[j].z > -13000.0)
                {
                  pfm_geo_distance (pfm_handle, features[i].y, features[i].x, features[j].y, features[j].x, &cut);

                  if (cut < radius)
                    {
                      scale_count++;
                      features[j].parent_record = i + 1;
                    }
                }
            }
        }


      percent = NINT (((float) i / (float) total_count) * 100.0);
      if (percent != old_percent)
        {
          progress->fbar->setValue (percent);
          old_percent = percent;
        }

      qApp->processEvents ();
    }

  progress->fbar->setValue (100);
  qApp->processEvents ();


  return (scale_count);
}
