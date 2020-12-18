
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



#include "startPage.hpp"
#include "startPageHelp.hpp"

startPage::startPage (int32_t *argc, char **argv, QWidget *parent, OPTIONS *op):
  QWizardPage (parent)
{
  options = op;


  setPixmap (QWizard::WatermarkPixmap, QPixmap(":/icons/pfmFeatureWatermark.png"));


  setTitle (tr ("Introduction"));

  setWhatsThis (ridiculousText);

  QLabel *label = new QLabel (tr ("pfmFeature is a tool for selecting features in a hydrographic PFM "
                                  "structure.  The selected features will be written to a new or pre-existing "
                                  "feature file.  The feature file will be placed in the input PFM's .data folder.  "
                                  "Feature selection criteria may be either IHO special order or order 1.  "
                                  "Help is available by clicking on the Help button and then clicking "
                                  "on the item for which you want help.  For a ridiculously involved "
                                  "explanation of the selection algorithm click on the help button below "
                                  "and then click on this text.<br><br>"
                                  "<b>IMPORTANT NOTE: This program assumes that the PFM has been very well "
                                  "cleaned.  That is, there should be no outliers in either the up or down "
                                  "direction.</b><br><br>"
                                  "Select a PFM file below.  You may then change the default output file "
                                  "name and, optionally, select an area file to limit the feature selection "
                                  "area and/or an area file to exclude from feature selection.  Click "
                                  "<b>Next</b> to continue or <b>Cancel</b> to exit."));
  label->setWordWrap (true);


  QVBoxLayout *vbox = new QVBoxLayout (this);
  vbox->addWidget (label);
  vbox->addStretch (10);


  QHBoxLayout *pfm_file_box = new QHBoxLayout (0);
  pfm_file_box->setSpacing (8);

  vbox->addLayout (pfm_file_box);


  QLabel *pfm_file_label = new QLabel (tr ("PFM File"), this);
  pfm_file_box->addWidget (pfm_file_label, 1);

  pfm_file_edit = new QLineEdit (this);
  pfm_file_edit->setReadOnly (true);
  pfm_file_box->addWidget (pfm_file_edit, 10);

  QPushButton *pfm_file_browse = new QPushButton (tr ("Browse..."), this);
  pfm_file_box->addWidget (pfm_file_browse, 1);

  pfm_file_label->setWhatsThis (pfm_fileText);
  pfm_file_edit->setWhatsThis (pfm_fileText);
  pfm_file_browse->setWhatsThis (pfm_fileBrowseText);

  connect (pfm_file_browse, SIGNAL (clicked ()), this, SLOT (slotPFMFileBrowse ()));



  QHBoxLayout *output_box = new QHBoxLayout (0);
  output_box->setSpacing (8);

  vbox->addLayout (output_box);


  QLabel *output_file_label = new QLabel (tr ("Output Feature File"), this);
  output_box->addWidget (output_file_label, 1);

  output_file_edit = new QLineEdit (this);
  output_box->addWidget (output_file_edit, 10);

  QPushButton *output_file_browse = new QPushButton (tr ("Browse..."), this);
  output_box->addWidget (output_file_browse, 1);

  output_file_label->setWhatsThis (output_fileText);
  output_file_edit->setWhatsThis (output_fileText);
  output_file_browse->setWhatsThis (output_fileBrowseText);

  connect (output_file_browse, SIGNAL (clicked ()), this, SLOT (slotOutputFileBrowse ()));


  QHBoxLayout *area_box = new QHBoxLayout (0);
  area_box->setSpacing (8);

  vbox->addLayout (area_box);


  QLabel *area_file_label = new QLabel (tr ("Optional Area File"), this);
  area_box->addWidget (area_file_label, 1);

  area_file_edit = new QLineEdit (this);
  area_file_edit->setReadOnly (true);
  area_box->addWidget (area_file_edit, 10);

  QPushButton *area_file_browse = new QPushButton (tr ("Browse..."), this);
  area_box->addWidget (area_file_browse, 1);


  area_file_label->setWhatsThis (area_fileText);
  area_file_edit->setWhatsThis (area_fileText);
  area_file_browse->setWhatsThis (area_fileBrowseText);

  connect (area_file_browse, SIGNAL (clicked ()), this, SLOT (slotAreaFileBrowse ()));


  QHBoxLayout *ex_box = new QHBoxLayout (0);
  ex_box->setSpacing (8);

  vbox->addLayout (ex_box);


  QLabel *ex_file_label = new QLabel (tr ("Optional Exclusion Area File"), this);
  ex_box->addWidget (ex_file_label, 1);

  ex_file_edit = new QLineEdit (this);
  ex_file_edit->setReadOnly (true);
  ex_box->addWidget (ex_file_edit, 10);

  QPushButton *ex_file_browse = new QPushButton (tr ("Browse..."), this);
  ex_box->addWidget (ex_file_browse, 1);


  ex_file_label->setWhatsThis (ex_fileText);
  ex_file_edit->setWhatsThis (ex_fileText);
  ex_file_browse->setWhatsThis (ex_fileBrowseText);

  connect (ex_file_browse, SIGNAL (clicked ()), this, SLOT (slotExFileBrowse ()));


  if (*argc == 2)
    {
      PFM_OPEN_ARGS open_args;
      int32_t pfm_handle = -1;

      QString pfm_file_name = QString (argv[1]);

      strcpy (open_args.list_path, argv[1]);

      open_args.checkpoint = 0;
      pfm_handle = open_existing_pfm_file (&open_args);

      if (pfm_handle >= 0)
        {
          if (!open_args.head.proj_data.projection)
            {
              pfm_file_edit->setText (pfm_file_name);


              //  Set the output feature filename.

              QString output_file_name = pfm_file_name + ".data" + SEPARATOR + QFileInfo (pfm_file_name).baseName () + ".bfd";


              //  If this file already exists we need to see if we are going to replace it or append to it.

              FILE *fp;
              char name[1024];
              strcpy (name, output_file_name.toLatin1 ());

              if ((fp = fopen (name, "r")) != NULL)
                {
                  QMessageBox msgBox (this);
                  msgBox.setIcon (QMessageBox::Question);
                  msgBox.setText (tr ("The output file %1 already exists").arg (output_file_name));
                  msgBox.setInformativeText (tr ("Do you wish to replace it or append to it?"));
                  msgBox.setStandardButtons (QMessageBox::Yes | QMessageBox::No);
                  msgBox.setDefaultButton (QMessageBox::Yes);
                  msgBox.setButtonText (QMessageBox::Yes, tr ("Replace"));
                  msgBox.setButtonText (QMessageBox::No, tr ("Append"));
                  int32_t ret = msgBox.exec ();

                  if (ret == QMessageBox::Yes)
                    {
                      options->replace = NVTrue;
                    }
                  else
                    {
                      options->replace = NVFalse;
                    }

                  fclose (fp);
                }

              output_file_edit->setText (output_file_name);
            }

          close_pfm_file (pfm_handle);
        }
    }


  if (!pfm_file_edit->text ().isEmpty ())
    {
      registerField ("pfm_file_edit", pfm_file_edit);
    }
  else
    {
      registerField ("pfm_file_edit*", pfm_file_edit);
    }


  registerField ("output_file_edit", output_file_edit);
  registerField ("area_file_edit", area_file_edit);
  registerField ("ex_file_edit", ex_file_edit);
}



void startPage::slotPFMFileBrowse ()
{
  PFM_OPEN_ARGS       open_args;
  QStringList         files, filters;
  QString             file;
  int32_t             pfm_handle = -1;


  QFileDialog *fd = new QFileDialog (this, tr ("pfmFeature Open PFM Structure"));
  fd->setViewMode (QFileDialog::List);


  //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
  //  This function is in the nvutility library.

  setSidebarUrls (fd, options->input_dir);


  filters << QString ("PFM (*.pfm)");

  fd->setNameFilters (filters);
  fd->setFileMode (QFileDialog::ExistingFile);
  fd->selectNameFilter (QString ("PFM (*.pfm)"));

  if (fd->exec () == QDialog::Accepted)
    {
      files = fd->selectedFiles ();

      QString pfm_file_name = files.at (0);


      if (!pfm_file_name.isEmpty())
        {
          strcpy (open_args.list_path, pfm_file_name.toLatin1 ());

          open_args.checkpoint = 0;
          pfm_handle = open_existing_pfm_file (&open_args);

          if (pfm_handle < 0)
            {
              QMessageBox::warning (this, tr ("Open PFM Structure"),
                                    tr ("The file %1 is not a PFM structure or there was an error reading the file.  The error message returned was:\n\n%2").arg 
                                    (QDir::toNativeSeparators (QString (open_args.list_path))).arg (pfm_error_str (pfm_error)));

              if (pfm_error == CHECKPOINT_FILE_EXISTS_ERROR)
                {
                  fprintf (stderr, "\n\n%s\n", pfm_error_str (pfm_error));
                  exit (-1);
                }

              return;
            }
        }

      if (open_args.head.proj_data.projection)
        {
          QMessageBox::warning (this, tr ("Open PFM Structure"), tr ("Sorry, pfmFeature only handles geographic PFM structures."));
          close_pfm_file (pfm_handle);
        }


      pfm_file_edit->setText (pfm_file_name);


      //  If one hasn't been set, set the output feature filename.

      if (output_file_edit->text ().isEmpty ())
        {
          QString output_file_name = pfm_file_name + ".data" + SEPARATOR + QFileInfo (pfm_file_name).baseName () + ".bfd";


          //  If this file already exists we need to see if we are going to replace it, append to it, or make a new file.

          FILE *fp;
          char name[512];
          strcpy (name, output_file_name.toLatin1 ());

          if ((fp = fopen (name, "r")) != NULL)
            {
              QMessageBox msgBox (this);
              msgBox.setIcon (QMessageBox::Question);
              msgBox.setText (tr ("The output file %1 already exists").arg (output_file_name));
              msgBox.setInformativeText (tr ("Do you wish to replace it, append to it, or create a new file?"));
              msgBox.setStandardButtons (QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort);
              msgBox.setDefaultButton (QMessageBox::Yes);
              msgBox.setButtonText (QMessageBox::Yes, tr ("Replace"));
              msgBox.setButtonText (QMessageBox::No, tr ("Append"));
              msgBox.setButtonText (QMessageBox::Abort, tr ("New file"));
              int32_t ret = msgBox.exec ();

              switch (ret)
                {
                default:
                case QMessageBox::Yes:
                  options->replace = NVTrue;
                  output_file_edit->setText (output_file_name);
                  break;

                case QMessageBox::No:
                  options->replace = NVFalse;
                  output_file_edit->setText (output_file_name);
                  break;

                case QMessageBox::Abort:
                  options->replace = NVTrue;
                  break;
                }

              fclose (fp);
            }
          else
            {
              output_file_edit->setText (output_file_name);
            }
        }

      options->input_dir = fd->directory ().absolutePath ();
    }
}



void startPage::slotOutputFileBrowse ()
{
  QFileDialog *fd = new QFileDialog (this, tr ("pfmFeature Output File"));
  fd->setViewMode (QFileDialog::List);


  //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
  //  This function is in the nvutility library.

  setSidebarUrls (fd, options->output_dir);


  QStringList filters;
  filters << tr ("Feature file (*.bfd)");

  fd->setNameFilters (filters);
  fd->setFileMode (QFileDialog::AnyFile);
  fd->selectNameFilter (tr ("Feature file (*.bfd)"));

  QStringList files;
  if (fd->exec () == QDialog::Accepted)
    {
      files = fd->selectedFiles ();

      QString output_file_name = files.at (0);


      if (!output_file_name.isEmpty())
        {
          //  Add .bfd to filename if not there.
            
          if (!output_file_name.endsWith (".bfd")) output_file_name.append (".bfd");


          //  If this file already exists we need to see if we are going to replace it or append to it.

          FILE *fp;
          char name[512];
          strcpy (name, output_file_name.toLatin1 ());

          if ((fp = fopen (name, "r")) != NULL)
            {
              QMessageBox msgBox (this);
              msgBox.setIcon (QMessageBox::Question);
              msgBox.setText (tr ("The output file %1 already exists").arg (output_file_name));
              msgBox.setInformativeText (tr ("Do you wish to replace it or append to it?"));
              msgBox.setStandardButtons (QMessageBox::Yes | QMessageBox::No);
              msgBox.setDefaultButton (QMessageBox::Yes);
              msgBox.setButtonText (QMessageBox::Yes, tr ("Replace"));
              msgBox.setButtonText (QMessageBox::No, tr ("Append"));
              int32_t ret = msgBox.exec ();

              if (ret == QMessageBox::Yes)
                {
                  options->replace = NVTrue;
                }
              else
                {
                  options->replace = NVFalse;
                }

              fclose (fp);
            }

          output_file_edit->setText (output_file_name);
        }

      options->output_dir = fd->directory ().absolutePath ();
    }
}


void startPage::slotAreaFileBrowse ()
{
  SHPHandle shpHandle;
  SHPObject *shape = NULL;
  int32_t type, numShapes;
  double minBounds[4], maxBounds[4];


  QFileDialog *fd = new QFileDialog (this, tr ("pfmFeature Area File"));
  fd->setViewMode (QFileDialog::List);


  //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
  //  This function is in the nvutility library.

  setSidebarUrls (fd, options->area_dir);


  QStringList filters;
  filters << tr ("Area file (*.ARE *.are *.afs *.shp *.SHP)");

  fd->setNameFilters (filters);
  fd->setFileMode (QFileDialog::ExistingFile);
  fd->selectNameFilter (tr ("Area file (*.ARE *.are *.afs *.shp *.SHP)"));


  QStringList files;
  if (fd->exec () == QDialog::Accepted)
    {
      files = fd->selectedFiles ();

      QString area_file_name = files.at (0);


      if (!area_file_name.isEmpty())
        {
          if (area_file_name.endsWith (".shp", Qt::CaseInsensitive))
            {
              char shpname[1024];
              strcpy (shpname, area_file_name.toLatin1 ());


              //  Open shape file

              shpHandle = SHPOpen (shpname, "rb");

              if (shpHandle == NULL)
                {
                  QMessageBox::warning (this, "pfmFeature", tr ("Cannot open shape file %1!").arg (area_file_name));
                  return;
                }
              else
                {
                  //  Get shape file header info

                  SHPGetInfo (shpHandle, &numShapes, &type, minBounds, maxBounds);


                  if (type != SHPT_POLYGON && type != SHPT_POLYGONZ && type != SHPT_POLYGONM &&
                      type != SHPT_ARC && type != SHPT_ARCZ && type != SHPT_ARCM)
                    {
                      QMessageBox::warning (this, "pfmFeature", tr ("Shape file %1 is not a polygon or polyline file!").arg (area_file_name));
                      return;
                    }
                  else
                    {
                      //  Read only the first shape.

                      shape = SHPReadObject (shpHandle, 0);


                      //  Check the number of vertices.

                      if (shape->nVertices < 3)
                        {
                          SHPClose (shpHandle);
                          QMessageBox::warning (this, "pfmFeature", tr ("Number of vertices (%1) of shape file %2 is too few for a polygon!").arg
                                                (shape->nVertices).arg (area_file_name));
                          return;
                        }


                      //  Read the vertices to take a shot at determining that this is a geographic polygon.

                      for (int32_t j = 0 ; j < shape->nVertices ; j++)
                        {
                          if (shape->padfX[j] < -360.0 || shape->padfX[j] > 360.0 || shape->padfY[j] < -90.0 || shape->padfY[j] > 90.0)
                            {
                              SHPDestroyObject (shape);
                              SHPClose (shpHandle);
                              QMessageBox::warning (this, "pfmFeature", tr ("Shape file %1 does not appear to be geographic!").arg (area_file_name));
                              return;
                            }
                        }


                      SHPDestroyObject (shape);
                      SHPClose (shpHandle);
                    }
                }
            }

          area_file_edit->setText (area_file_name);
        }

      options->area_dir = fd->directory ().absolutePath ();
    }
}



void startPage::slotExFileBrowse ()
{
  QFileDialog *fd = new QFileDialog (this, tr ("pfmFeature Exclusion Area File"));
  fd->setViewMode (QFileDialog::List);


  //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
  //  This function is in the nvutility library.

  setSidebarUrls (fd, options->exclude_dir);


  QStringList filters;
  filters << tr ("Area file (*.ARE *.are *.afs)");

  fd->setNameFilters (filters);
  fd->setFileMode (QFileDialog::ExistingFile);
  fd->selectNameFilter (tr ("Area file (*.ARE *.are *.afs)"));


  QStringList files;
  if (fd->exec () == QDialog::Accepted)
    {
      files = fd->selectedFiles ();

      QString ex_file_name = files.at (0);


      if (!ex_file_name.isEmpty())
        {
          ex_file_edit->setText (ex_file_name);
        }

      options->exclude_dir = fd->directory ().absolutePath ();
    }
}
