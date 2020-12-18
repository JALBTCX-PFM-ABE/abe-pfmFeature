
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



#include "runPage.hpp"

runPage::runPage (QWidget *parent, RUN_PROGRESS *prog):
  QWizardPage (parent)
{
  progress = prog;


  setTitle (tr ("Process Page"));

  setPixmap (QWizard::WatermarkPixmap, QPixmap(":/icons/pfmFeatureWatermark.png"));

  setFinalPage (true);

  QVBoxLayout *vbox = new QVBoxLayout (this);
  vbox->setMargin (5);
  vbox->setSpacing (5);


  progress->sbox = new QGroupBox (tr ("Feature selection"), this);
  QVBoxLayout *sboxLayout = new QVBoxLayout;
  progress->sbox->setLayout (sboxLayout);
  sboxLayout->setSpacing (10);


  progress->sbar = new QProgressBar (this);
  progress->sbar->setRange (0, 100);
  progress->sbar->setToolTip (tr ("Progress of the feature selection process."));
  progress->sbar->setWhatsThis (tr ("Progress of the feature selection process."));
  sboxLayout->addWidget (progress->sbar);


  vbox->addWidget (progress->sbox);


  progress->dbox = new QGroupBox (tr ("Feature deconfliction"), this);
  QVBoxLayout *dboxLayout = new QVBoxLayout;
  progress->dbox->setLayout (dboxLayout);
  dboxLayout->setSpacing (10);


  progress->dbar = new QProgressBar (this);
  progress->dbar->setRange (0, 100);
  progress->dbar->setToolTip (tr ("Progress of the feature deconfliction process."));
  progress->dbar->setWhatsThis (tr ("Progress of the feature deconfliction process."));
  dboxLayout->addWidget (progress->dbar);


  vbox->addWidget (progress->dbox);


  progress->fbox = new QGroupBox (tr ("Chart scale feature deconfliction"), this);
  QVBoxLayout *fboxLayout = new QVBoxLayout;
  progress->fbox->setLayout (fboxLayout);
  fboxLayout->setSpacing (10);


  progress->fbar = new QProgressBar (this);
  progress->fbar->setRange (0, 100);
  progress->fbar->setToolTip (tr ("Progress of the chart scale feature deconfliction process."));
  progress->fbar->setWhatsThis (tr ("Progress of the chart scale feature deconfliction process."));
  fboxLayout->addWidget (progress->fbar);


  vbox->addWidget (progress->fbox);


  progress->wbox = new QGroupBox (tr ("Writing features"), this);
  QVBoxLayout *wboxLayout = new QVBoxLayout;
  progress->wbox->setLayout (wboxLayout);
  wboxLayout->setSpacing (10);


  progress->wbar = new QProgressBar (this);
  progress->wbar->setRange (0, 100);
  progress->wbar->setToolTip (tr ("Progress of the feature writing process."));
  progress->wbar->setWhatsThis (tr ("Progress of the feature writing process."));
  wboxLayout->addWidget (progress->wbar);


  vbox->addWidget (progress->wbox);


  QGroupBox *lbox = new QGroupBox (tr ("Process status"), this);
  QVBoxLayout *lboxLayout = new QVBoxLayout;
  lbox->setLayout (lboxLayout);
  lboxLayout->setSpacing (10);


  progress->list = new QListWidget (this);
  progress->list->setAlternatingRowColors (true);
  progress->list->setMaximumHeight (90);
  lboxLayout->addWidget (progress->list);


  vbox->addWidget (lbox);


  //  Serious cheating here ;-)  I want the finish button to be disabled when you first get to this page
  //  so I set the last progress bar to be a "mandatory" field.  I couldn't disable the damn button in
  //  initializePage in the parent for some unknown reason.

  registerField ("progress_wbar*", progress->wbar, "value");
}
