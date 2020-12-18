
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



#ifndef PFMFEATURE_H
#define PFMFEATURE_H

#include "pfmFeatureDef.hpp"
#include "startPage.hpp"
#include "optionPage.hpp"
#include "runPage.hpp"
#include "selectThread.hpp"


class pfmFeature : public QWizard
{
  Q_OBJECT


public:

  pfmFeature (int32_t *argc = 0, char **argv = 0, QWidget *parent = 0);
  ~pfmFeature ();


protected:

  void initializePage (int id);
  void cleanupPage (int id);

  void load_arrays (int32_t layer_type, int32_t count, BIN_RECORD bin_record[], float data[]);

  void envin (OPTIONS *options);
  void envout (OPTIONS *options);



  OPTIONS          options;

  RUN_PROGRESS     progress;

  QListWidget      *checkList;

  QString          pfm_file, feature_file, area_file, ex_file;

  optionPage       *oPage;

  int32_t          features_count[SELECT_PASSES], min_percent, pfm_handle, l_pfm_handle[SELECT_PASSES];

  uint8_t          pass_complete[SELECT_PASSES], complete;

  FEATURE          **features;

  PFM_OPEN_ARGS    open_args;


protected slots:

  void slotHelpClicked ();
  void slotCustomButtonClicked (int id);
  void slotPercentValue (int32_t percent, int32_t pass);
  void slotCompleted (int32_t num_features, int32_t pass);

};

#endif
