
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
#include "version.hpp"


void get_feature_event_time (int32_t pfm_handle, DEPTH_RECORD depth, time_t *tv_sec, long *tv_nsec);


void build_feature (int32_t pfm_handle, PFM_OPEN_ARGS open_args, OPTIONS *options, QString feature_file, int32_t total_count, FEATURE *features,
                    RUN_PROGRESS *progress)
{
  int32_t             bfd_handle, year, day, hour, minute, percent = 0, old_percent = -1;
  float               second;
  QString             title;
  BFDATA_HEADER       bfd_header;
  BFDATA_RECORD       bfd_record;
  BFDATA_POLYGON      bfd_poly;
  NV_I32_COORD2       coord;
  DEPTH_RECORD        *in_depth = NULL;



  //  Make sure that we can open and write to the output .bfd file.

  char ftr[512];
  strcpy (ftr, feature_file.toLatin1 ());

  if (options->replace)
    {
      memset (&bfd_header, 0, sizeof (BFDATA_HEADER));

      strcpy (bfd_header.creation_software, VERSION);

      if ((bfd_handle = binaryFeatureData_create_file (ftr, bfd_header)) < 0)
        {
          QString msg = QString (binaryFeatureData_strerror ());
          QMessageBox::critical (0, "pfmFeature", pfmFeature::tr ("Unable to create feature file.\nReason : %1").arg (msg));
          exit (-1);
        }
    }
  else
    {
      if ((bfd_handle = binaryFeatureData_open_file (ftr, &bfd_header, BFDATA_UPDATE)) < 0)
        {
          QString msg = QString (binaryFeatureData_strerror ());
          QMessageBox::critical (0, "pfmFeature", pfmFeature::tr ("Unable to open feature file.\nReason : %1").arg (msg));
          exit (-1);
        }
    }


  time_t current_time = time (&current_time);

  cvtime ((time_t) current_time, 0, &year, &day, &hour, &minute, &second);
  year += 1900;


  int32_t count = 0;
  for (int32_t i = 0 ; i < total_count ; i++)
    {
      progress->wbox->setTitle (pfmFeature::tr ("Writing features"));
      qApp->processEvents ();


      old_percent = -1;
      percent = 0;


      memset (&bfd_record, 0, sizeof (BFDATA_RECORD));


      //  No matter what, pfmFeature makes only Hydrographic features.

      bfd_record.feature_type = BFDATA_HYDROGRAPHIC;


      if (features[i].z > -13000.0)
        {
          //  Find the event time from the file pointed to in the PFM file.

          NV_F64_COORD2 xy = {features[i].x, features[i].y};
          compute_index_ptr (xy, &coord, &open_args.head);
          int32_t numrecs;

          if (!read_depth_array_index (pfm_handle, coord, &in_depth, &numrecs))
            {
              for (int32_t m = 0 ; m < numrecs ; m++)
                {
                  //  Don't use invalid, deleted, or reference points.

                  if (!(in_depth[m].validity & (PFM_INVAL | PFM_DELETED | PFM_REFERENCE)))
                    {
                      if (fabs (in_depth[m].xyz.z - features[i].z) < 0.005 && fabs (in_depth[m].xyz.x - features[i].x) < 0.000001 && 
                          fabs (in_depth[m].xyz.y - features[i].y) < 0.000001)
                        {
                          bfd_record.parent_record = features[i].parent_record;


                          //  Add PFM_DESIGNATED_SOUNDING to the validity for the selected point if it is a child record.

                          if (features[i].parent_record)
                            {
                              in_depth[m].validity |= (PFM_DESIGNATED_SOUNDING | PFM_MODIFIED);
                            }


                          //  If it's a parent, add PFM_SELECTED_FEATURE to the validity for the selected point.

                          else
                            {
                              in_depth[m].validity |= (PFM_SELECTED_FEATURE | PFM_MODIFIED);
                            }


                          update_depth_record_index (pfm_handle, &in_depth[m]);


                          get_feature_event_time (pfm_handle, in_depth[m], &bfd_record.event_tv_sec, &bfd_record.event_tv_nsec);
                          cvtime (bfd_record.event_tv_sec, bfd_record.event_tv_nsec, &year, &day, &hour, &minute, &second);
                          break;
                        }
                    }
                }
              free (in_depth);
            }
          else
            {
              inv_cvtime (year - 1900, day, hour, minute, 0.0, &bfd_record.event_tv_sec, &bfd_record.event_tv_nsec);
            }


          //  Doing this to suppress a meaningless warning.  The variable count will NEVER exceed 99,999.
          //  If it does, you've screwed the pooch somewhere.

          char no_warning[128];
          sprintf (no_warning, "%02d%03d%02d%02d%05d", year % 100, day, hour, minute, count++);
          strncpy (bfd_record.contact_id, no_warning, 14);
          bfd_record.contact_id[14] = 0;


          //  Subtract off the offset (if any) prior to putting the depth in the feature.

          bfd_record.depth = (float) features[i].z - options->offset;


          strcpy (bfd_record.description, options->description.toLatin1 ());


          //  Find the octant position farthest from the shoal (feature) point.

          double max_dist = -1.0;
          int32_t oct_cnt = 0;

          for (int32_t oct = 0 ; oct < 8 ; oct++)
            {
              if (features[i].oct_hit[oct])
                {
                  double dist;
                  pfm_geo_distance (pfm_handle, features[i].y, features[i].x, features[i].oct_pos[oct].y, features[i].oct_pos[oct].x, &dist);

                  max_dist = qMax (dist, max_dist);

                  oct_cnt++;
                }
            }


          switch (options->order)
            {
            case 0:
              sprintf (bfd_record.remarks, "Created with pfmFeature, IHO Special order, bin size %.0f, octants %d, H/V %.2f/%.2f, #%d, max dist %.1f",
                       features[i].bin_size, oct_cnt, features[i].h, features[i].v, count - 1, max_dist);
              break;

            case 1:
              sprintf (bfd_record.remarks, "Created with pfmFeature, IHO Order 1, bin size %.0f, octants %d, H/V %.2f/%.2f, #%d, max dist %.1f",
                       features[i].bin_size, oct_cnt, features[i].h, features[i].v, count - 1, max_dist);
              break;
            }


          bfd_record.confidence_level = features[i].confidence;


          bfd_record.latitude = features[i].y;
          bfd_record.longitude = features[i].x;


          //  Build a feature polygon from the nearest octant trigger points that exceed the combined horizontal uncertainty.

          bfd_record.poly_count = 0;
          bfd_record.poly_type = 1;
          for (int32_t oct = 0 ; oct < 8 ; oct++)
            {
              if (features[i].oct_hit[oct])
                {
                  bfd_poly.latitude[bfd_record.poly_count] = features[i].oct_pos[oct].y;
                  bfd_poly.longitude[bfd_record.poly_count] = features[i].oct_pos[oct].x;
                  bfd_record.poly_count++;
                }
            }


          strcpy (bfd_record.analyst_activity, "NAVOCEANO BHY");

          bfd_record.equip_type = 3;

          bfd_record.nav_system = 1;

          bfd_record.platform_type = 4;

          bfd_record.sonar_type = 3;

          if (binaryFeatureData_write_record (bfd_handle, BFDATA_NEXT_RECORD, &bfd_record, &bfd_poly, NULL) < 0)
            {
              fprintf (stderr, "%s %s %s %d - Writing BFD record - %s\n", options->progname, __FILE__, __FUNCTION__, __LINE__,
                       binaryFeatureData_strerror ());
              exit (-1);
            }
        }


      percent = NINT (((float) i / (float) total_count) * 100.0);
      if (percent != old_percent)
        {
          progress->wbar->setValue (percent);
          old_percent = percent;
        }

      qApp->processEvents ();
    }


  binaryFeatureData_close_file (bfd_handle);


  //  If we didn't find any features (an admittedly rare occurrence) we need to get rid of the files and put out a message.

  if (!count)
    {
      //  Remove the .bfd file.

      remove (ftr);


      //  Remove the .bfa file.

      ftr[strlen (ftr) - 1] = 'a';
      remove (ftr);


      QMessageBox::information (0, "pfmFeature", pfmFeature::tr ("No features were detected.\nFeature file not created."));
    }
  else
    {
      //  Make this the PFMs feature file.

      update_target_file (pfm_handle, open_args.list_path, ftr);
    }


  progress->wbar->setValue (100);
  qApp->processEvents ();
}
