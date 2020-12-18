
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
#include "pfmFeatureHelp.hpp"

#include <getopt.h>

double settings_version = 1.0;


pfmFeature::pfmFeature (int32_t *argc, char **argv, QWidget *parent)
  : QWizard (parent, 0)
{
  QResource::registerResource ("/icons.rcc");


  strcpy (options.progname, argv[0]);


  //  Set the main icon

  setWindowIcon (QIcon (":/icons/pfmFeatureWatermark.png"));


  //  Get the command line arguments.  This is a placeholder for now.

  int32_t option_index = 0;

  while (NVTrue) 
    {
      static struct option long_options[] = {{"polygon", no_argument, 0, 0},
                                             {0, no_argument, 0, 0}};

      char c = (char) getopt_long (*argc, argv, "p", long_options, &option_index);
      if (c == -1) break;

      switch (c) 
        {
        case 0:

          switch (option_index)
            {
            case 0:
              //  Not used
              break;
            }
          break;

        case 'p':
          //  Not used
          break;
        }
    }


  //  Get the user's defaults if available

  envin (&options);


  // Set the application font

  QApplication::setFont (options.font);


  options.replace = NVTrue;


  setWizardStyle (QWizard::ClassicStyle);


  setOption (HaveHelpButton, true);
  setOption (ExtendedWatermarkPixmap, false);

  connect (this, SIGNAL (helpRequested ()), this, SLOT (slotHelpClicked ()));


  //  Set the window size and location from the defaults

  this->resize (options.window_width, options.window_height);
  this->move (options.window_x, options.window_y);


  setPage (0, new startPage (argc, argv, this, &options));

  setPage (1, oPage = new optionPage (this, &options));

  setPage (2, new runPage (this, &progress));


  setButtonText (QWizard::CustomButton1, tr("&Run"));
  setOption (QWizard::HaveCustomButton1, true);
  button (QWizard::CustomButton1)->setToolTip (tr ("Start selecting the features"));
  button (QWizard::CustomButton1)->setWhatsThis (runText);
  connect (this, SIGNAL (customButtonClicked (int)), this, SLOT (slotCustomButtonClicked (int)));


  setStartId (0);
}



pfmFeature::~pfmFeature ()
{
}



void pfmFeature::initializePage (int id)
{
  button (QWizard::HelpButton)->setIcon (QIcon (":/icons/contextHelp.png"));
  button (QWizard::CustomButton1)->setEnabled (false);


  switch (id)
    {
    case 0:
      break;

    case 1:
      pfm_file = field ("pfm_file_edit").toString ();
      area_file = field ("area_file_edit").toString ();
      oPage->setFiles (pfm_file, area_file);
      break;

    case 2:
      button (QWizard::CustomButton1)->setEnabled (true);

      feature_file = field ("output_file_edit").toString ();
      if (!feature_file.endsWith (".bfd")) feature_file.append (".bfd");


      //  Save the output directory.  It might have been input manually instead of browsed.

      options.output_dir = QFileInfo (feature_file).absoluteDir ().absolutePath ();


      ex_file = field ("ex_file_edit").toString ();

      options.order = field ("order").toInt ();
      options.zero = field ("zero").toBool ();
      options.offset = (float) field ("offset").toDouble ();
      options.hpc = field ("hpc").toBool ();
      options.threshhold = field ("thresh").toInt () + 1;
      options.millimeters = field ("milli").toInt ();
      options.chart_scale = field ("scale").toDouble ();
      options.description = field ("description").toString ();


      //  Use frame geometry to get the absolute x and y.

      QRect tmp = this->frameGeometry ();
      options.window_x = tmp.x ();
      options.window_y = tmp.y ();


      //  Use geometry to get the width and height.

      tmp = this->geometry ();
      options.window_width = tmp.width ();
      options.window_height = tmp.height ();


      //  Save the options.

      envout (&options);


      QString string;

      progress.list->clear ();

      string = tr ("Input PFM file : %1").arg (pfm_file);
      progress.list->addItem (string);

      string = tr ("Feature file : %1").arg (feature_file);
      progress.list->addItem (string);

      if (!area_file.isEmpty ())
        {
          string = tr ("Area file : %1").arg (area_file);
          progress.list->addItem (string);
        }

      if (!ex_file.isEmpty ())
        {
          string = tr ("Exclusion area file : %1").arg (ex_file);
          progress.list->addItem (string);
        }


      switch (options.order)
        {
        case 0:
          string = tr ("IHO special order");
          progress.list->addItem (string);
          break;

        case 1:
          string = tr ("IHO order 1");
          progress.list->addItem (string);
          break;
        }


      switch (options.zero)
        {
        case false:
          string = tr ("Not selecting features above zero level");
          progress.list->addItem (string);
          break;

        case true:
          string = tr ("Selecting features above zero level");
          progress.list->addItem (string);
          break;
        }

      switch (options.hpc)
        {
        case false:
          string = tr ("Not determining confidence");
          progress.list->addItem (string);
          break;

        case true:
          string = tr ("Determining confidence using HPC algorithm", "HPC is Hockey Puck of Confidence");
          progress.list->addItem (string);
          break;
        }


      string = tr ("Writing estimated feature shape polygons to feature file");
      progress.list->addItem (string);
      break;

      if (NINT (options.offset * 1000.0))
        {
          string = tr ("Using artificial offset of %L1").arg (options.offset, 0, 'f', 2);
          progress.list->addItem (string);
          break;
        }

      break;
    }

  QListWidgetItem *cur = new QListWidgetItem (" ");

  progress.list->addItem (cur);
  progress.list->setCurrentItem (cur);
  progress.list->scrollToItem (cur);
}



void pfmFeature::cleanupPage (int id)
{
  switch (id)
    {
    case 0:
      break;

    case 1:
      break;

    case 2:
      break;
    }
}



void pfmFeature::slotHelpClicked ()
{
  QWhatsThis::enterWhatsThisMode ();
}



//  This is where the fun stuff happens.

void 
pfmFeature::slotCustomButtonClicked (int id __attribute__ ((unused)))
{
  selectThread        select_thread[SELECT_PASSES];
  double              lat, lon;
  static int32_t      old_percent = -1;
  QString             string;
  NV_F64_COORD2       order[2] = {{1.0, 0.05}, {2.0, 0.10}};


  int32_t deconflict_feature (NV_F64_COORD2 order, int32_t passes, int32_t pfm_handle, OPTIONS *options, int32_t *features_count,
                               FEATURE **features, RUN_PROGRESS *progress);
  int32_t scale_feature (int32_t pfm_handle, OPTIONS *options, int32_t total_count, FEATURE *features, RUN_PROGRESS *progress);
  void build_feature (int32_t pfm_handle, PFM_OPEN_ARGS open_args, OPTIONS *options, QString feature_file, int32_t total_count, FEATURE *features,
                      RUN_PROGRESS *progress);



  QApplication::setOverrideCursor (Qt::WaitCursor);


  button (QWizard::FinishButton)->setEnabled (false);
  button (QWizard::BackButton)->setEnabled (false);
  button (QWizard::CustomButton1)->setEnabled (false);


  strcpy (open_args.list_path, pfm_file.toLatin1 ());


  //  Open the PFM file.

  open_args.checkpoint = 0;
  if ((pfm_handle = open_existing_pfm_file (&open_args)) < 0)
    {
      fprintf (stderr, "%s %s %s %d - %s - %s\n", options.progname, __FILE__, __FUNCTION__, __LINE__, open_args.list_path, pfm_error_str (pfm_error));
      exit (-1);
    }


  features = (FEATURE **) calloc (SELECT_PASSES, sizeof (FEATURE *));

  if (features == NULL)
    {
      fprintf (stderr, "%s %s %s %d - features - %s\n", options.progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
      exit (-1);
    }


  //  Check for an exclusion area file.

  options.ex_polygon_count = 0;
  if (!ex_file.isEmpty ())
    {
      char ef[512];
      strcpy (ef, ex_file.toLatin1 ());

      get_area_mbr (ef, &options.ex_polygon_count, options.ex_polygon_x, options.ex_polygon_y, &options.ex_mbr);
    }


  //  Set the bin size.

  double start_size = 1.5 * pow (2.0, (double) options.order);


  //  Check for an area file.

  if (!area_file.isEmpty ())
    {
      char af[512];
      strcpy (af, area_file.toLatin1 ());

      get_area_mbr (af, &options.polygon_count, options.polygon_x, options.polygon_y, &options.mbr);


      //  Add one bin at largest bin size all around the MBR.

      double size = start_size * pow (2.0, (double) (SELECT_PASSES - 1));


      newgp (options.mbr.min_y, options.mbr.min_x, 180.0, size, &lat, &lon);
      options.mbr.min_y = qMax (lat, open_args.head.mbr.min_y);

      newgp (options.mbr.max_y, options.mbr.min_x, 0.0, size, &lat, &lon);
      options.mbr.max_y = qMin (lat, open_args.head.mbr.max_y);


      //  If we're north of the equator then the south edge of the area will have the largest X bin size.

      if (open_args.head.mbr.min_y >= 0.0)
        {
          newgp (options.mbr.max_y, options.mbr.min_x, 270.0, size, &lat, &lon);
          options.mbr.min_x = qMax (lon, open_args.head.mbr.min_x);

          newgp (options.mbr.max_y, options.mbr.max_x, 90.0, size, &lat, &lon);
          options.mbr.max_x = qMin (lon, open_args.head.mbr.max_x);
        }
      else
        {
          newgp (options.mbr.min_y, options.mbr.min_x, 270.0, size, &lat, &lon);
          options.mbr.min_x = qMax (lon, open_args.head.mbr.min_x);

          newgp (options.mbr.min_y, options.mbr.max_x, 90.0, size, &lat, &lon);
          options.mbr.max_x = qMin (lon, open_args.head.mbr.max_x);
        }
    }
  else
    {
      options.polygon_count = 0;

      options.mbr = open_args.head.mbr;
    }


  //  We're starting all SELECT_PASSES selection passes concurrently using threads.  Note that we're using the Qt::DirectConnection type
  //  for the signal/slot connections.  This causes all of the signals emitted from the threads to be serviced immediately.

  qRegisterMetaType<int32_t> ("int32_t");

  complete = NVFalse;
  for (int32_t i = 0 ; i < SELECT_PASSES ; i++)
    {
      pass_complete[i] = NVFalse;
      connect (&select_thread[i], SIGNAL (percentValue (int32_t, int32_t)), this, SLOT (slotPercentValue (int32_t, int32_t)), Qt::DirectConnection);
      connect (&select_thread[i], SIGNAL (completed (int32_t, int32_t)), this, SLOT (slotCompleted (int32_t, int32_t)), Qt::DirectConnection);
    }


  //  We open a different handle for each thread so that we don't have any collision problems in the PFM library.

  PFM_OPEN_ARGS l_open_args[SELECT_PASSES];

  for (int32_t i = 0 ; i < SELECT_PASSES ; i++)
    {
      strcpy (l_open_args[i].list_path, pfm_file.toLatin1 ());


      //  Open the PFM file.

      l_open_args[i].checkpoint = 0;
      if ((l_pfm_handle[i] = open_existing_pfm_file (&l_open_args[i])) < 0)
        {
          fprintf (stderr, "%s %s %s %d - %s - %s\n", options.progname, __FILE__, __FUNCTION__, __LINE__, l_open_args[i].list_path, pfm_error_str (pfm_error));
          exit (-1);
        }


      select_thread[i].select (&options, l_pfm_handle[i], &l_open_args[i], features, i, start_size);
    }


  //  We can't exit from this method until the threads are complete but we want to keep our progress bar updated.  This is a bit tricky 
  //  because you can't update the progress bar from within the slots connected to the thread signals.  Those slots are considered part
  //  of the selection thread and not part of the GUI thread.  In the percent slot we just update the minimum percentage value (from
  //  any of the four threads) and monitor it here every second (updating the progress bar accordingly).  When all of the threads are
  //  finished we move on to the deconfliction and BFD file creation steps.

  while (!complete)
    {
#ifdef NVWIN3X
      Sleep (1000);
#else
      sleep (1);
#endif

      if (min_percent != old_percent)
        {
          progress.sbar->setValue (min_percent);
          old_percent = min_percent;
        }

      qApp->processEvents ();
    }


  progress.sbar->setValue (100);
  qApp->processEvents ();


  //  Compute our total possible features.

  int32_t total = 0;
  for (int32_t i = 0 ; i < SELECT_PASSES ; i++)
    {
      close_pfm_file (l_pfm_handle[i]);
      total += features_count[i];
    }


  progress.sbar->setRange (0, 100);
  progress.sbar->setValue (100);
  qApp->processEvents ();


  string = tr ("Feature selection, detected %L1 possible features").arg (total);
  progress.sbox->setTitle (string);
  qApp->processEvents ();


  //  Deconflict the possible features.

  int32_t decon_count = deconflict_feature (order[options.order], SELECT_PASSES, pfm_handle, &options, features_count, features, &progress);


  string = tr ("Feature deconfliction, deconflicted or removed %L1 features").arg (decon_count);
  progress.dbox->setTitle (string);
  qApp->processEvents ();


  //  For the chart scale deconflict we need to put all of the features into a single array so it can be sorted by depth.

  int32_t total_count = 0;
  for (int32_t i = 0 ; i < SELECT_PASSES ; i++) total_count += features_count[i];


  FEATURE *scale_features = (FEATURE *) calloc (total_count, sizeof (FEATURE));

  if (scale_features == NULL)
    {
      fprintf (stderr, "%s %s %s %d - features - %s\n", options.progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
      exit (-1);
    }

  int32_t k = 0;

  for (int32_t i = 0 ; i < SELECT_PASSES ; i++)
    {
      for (int32_t j = 0 ; j < features_count[i] ; j++)
        {
          scale_features[k++] = features[i][j];
        }
    }


  //  Now free the features that were stored by pass.

  for (int32_t i = 0 ; i < SELECT_PASSES ; i++) free (features[i]);


  //  This is the chart scale deconflict.  It doesn't invalidate features, it aggregates them into a parent and one or more children.

  int32_t scale_count = scale_feature (pfm_handle, &options, total_count, scale_features, &progress);


  string = tr ("Chart scale deconfliction, deconflicted %L1 features").arg (scale_count);
  progress.fbox->setTitle (string);
  qApp->processEvents ();


  //  Write the valid features to a NAVO standard feature file.

  build_feature (pfm_handle, open_args, &options, feature_file, total_count, scale_features, &progress);


  close_pfm_file (pfm_handle);


  string = tr ("Writing feature file, wrote %L1 features").arg (total - decon_count);
  progress.wbox->setTitle (string);
  qApp->processEvents ();


  button (QWizard::FinishButton)->setEnabled (true);
  button (QWizard::CancelButton)->setEnabled (false);


  QApplication::restoreOverrideCursor ();


  progress.list->addItem (" ");
  QListWidgetItem *cur = new QListWidgetItem (tr ("Feature selection and deconfliction complete, press Finish to exit."));

  progress.list->addItem (cur);
  progress.list->setCurrentItem (cur);
  progress.list->scrollToItem (cur);
}



void 
pfmFeature::slotPercentValue (int32_t percent, int32_t pass)
{
  static int32_t save_percent[SELECT_PASSES];

  save_percent[pass] = percent;

  min_percent = 1001;
  for (int32_t i = 0 ; i < SELECT_PASSES ; i++) min_percent = qMin (min_percent, save_percent[i]);
}



void 
pfmFeature::slotCompleted (int32_t num_features, int32_t pass)
{
  features_count[pass] = num_features;

  pass_complete[pass] = NVTrue;

  uint8_t done = NVTrue;
  for (int32_t i = 0 ; i < SELECT_PASSES ; i++) if (!pass_complete[i]) done = NVFalse;


  if (done) complete = NVTrue;
}



//  Get the users defaults.

void pfmFeature::envin (OPTIONS *options)
{
  //  We need to get the font from the global settings.

#ifdef NVWIN3X
  QString ini_file2 = QString (getenv ("USERPROFILE")) + "/ABE.config/" + "globalABE.ini";
#else
  QString ini_file2 = QString (getenv ("HOME")) + "/ABE.config/" + "globalABE.ini";
#endif

  options->font = QApplication::font ();

  QSettings settings2 (ini_file2, QSettings::IniFormat);
  settings2.beginGroup ("globalABE");


  QString defaultFont = options->font.toString ();
  QString fontString = settings2.value (QString ("ABE map GUI font"), defaultFont).toString ();
  options->font.fromString (fontString);


  settings2.endGroup ();


  double saved_version = 1.0;


  // Set defaults so that if keys don't exist the parameters are defined

  options->order = 1;
  options->zero = NVFalse;
  options->hpc = NVFalse;
  options->threshhold = 3;
  options->millimeters = 5;
  options->chart_scale = 2000.0;
  options->description = "";
  options->input_dir = ".";
  options->output_dir = ".";
  options->area_dir = ".";
  options->exclude_dir = ".";
  options->window_x = 0;
  options->window_y = 0;
  options->window_width = 640;
  options->window_height = 200;


  //  Get the INI file name

#ifdef NVWIN3X
  QString ini_file = QString (getenv ("USERPROFILE")) + "/ABE.config/pfmFeature.ini";
#else
  QString ini_file = QString (getenv ("HOME")) + "/ABE.config/pfmFeature.ini";
#endif

  QSettings settings (ini_file, QSettings::IniFormat);
  settings.beginGroup ("pfmFeature");

  saved_version = settings.value (QString ("settings version"), saved_version).toDouble ();


  //  If the settings version has changed we need to leave the values at the new defaults since they may have changed.

  if (settings_version != saved_version) return;


  options->order = settings.value (QString ("IHO order"), options->order).toInt ();
  options->zero = settings.value (QString ("zero level"), options->zero).toBool ();
  options->hpc = settings.value (QString ("HPC flag"), options->hpc).toBool ();
  options->threshhold = settings.value (QString ("threshhold"), options->threshhold).toInt ();
  options->millimeters = settings.value (QString ("millimeters"), options->millimeters).toInt ();
  options->chart_scale = settings.value (QString ("chart scale"), options->chart_scale).toDouble ();
  options->input_dir = settings.value (QString ("input directory"), options->input_dir).toString ();
  options->output_dir = settings.value (QString ("output directory"), options->output_dir).toString ();
  options->area_dir = settings.value (QString ("area directory"), options->area_dir).toString ();
  options->exclude_dir = settings.value (QString ("exclude directory"), options->exclude_dir).toString ();

  options->window_width = settings.value (QString ("width"), options->window_width).toInt ();
  options->window_height = settings.value (QString ("height"), options->window_height).toInt ();
  options->window_x = settings.value (QString ("x position"), options->window_x).toInt ();
  options->window_y = settings.value (QString ("y position"), options->window_y).toInt ();

  settings.endGroup ();
}




//  Save the users defaults.

void pfmFeature::envout (OPTIONS *options)
{
  //  Get the INI file name

#ifdef NVWIN3X
  QString ini_file = QString (getenv ("USERPROFILE")) + "/ABE.config/pfmFeature.ini";
#else
  QString ini_file = QString (getenv ("HOME")) + "/ABE.config/pfmFeature.ini";
#endif

  QSettings settings (ini_file, QSettings::IniFormat);
  settings.beginGroup ("pfmFeature");


  settings.setValue (QString ("settings version"), settings_version);

  settings.setValue (QString ("IHO order"), options->order);
  settings.setValue (QString ("zero level"), options->zero);
  settings.setValue (QString ("HPC flag"), options->hpc);
  settings.setValue (QString ("threshhold"), options->threshhold);
  settings.setValue (QString ("millimeters"), options->millimeters);
  settings.setValue (QString ("chart scale"), options->chart_scale);
  settings.setValue (QString ("input directory"), options->input_dir);
  settings.setValue (QString ("output directory"), options->output_dir);
  settings.setValue (QString ("area directory"), options->area_dir);
  settings.setValue (QString ("exclude directory"), options->exclude_dir);

  settings.setValue (QString ("width"), options->window_width);
  settings.setValue (QString ("height"), options->window_height);
  settings.setValue (QString ("x position"), options->window_x);
  settings.setValue (QString ("y position"), options->window_y);

  settings.endGroup ();
}
