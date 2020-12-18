
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



#ifndef PFMFEATUREDEF_H
#define PFMFEATUREDEF_H

#include <QtCore>
#include <QtGui>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>


#include "nvutility.h"
#include "nvutility.hpp"
#include "pfm.h"
#include "binaryFeatureData.h"
#include "shapefil.h"


/*  Define this to do memory check debugging.  */

#undef __PFM_FEATURE_DEBUG__


#define SELECT_PASSES    4



typedef struct
{
  int32_t       order;
  uint8_t       zero;
  uint8_t       hpc;
  float         offset;
  int32_t       threshhold;
  int32_t       millimeters;
  double        chart_scale;
  QString       description;
  NV_F64_XYMBR  mbr;
  NV_F64_XYMBR  ex_mbr;
  int32_t       polygon_count;
  double        polygon_x[2000];
  double        polygon_y[2000];
  int32_t       ex_polygon_count;
  double        ex_polygon_x[2000];
  double        ex_polygon_y[2000];
  int32_t       window_x;
  int32_t       window_y;
  int32_t       window_width;
  int32_t       window_height;
  uint8_t       replace;
  QString       input_dir;
  QString       output_dir;
  QString       area_dir;
  QString       exclude_dir;
  char          progname[256];
  QFont         font;                       //  Font used for all ABE GUI applications
} OPTIONS;


typedef struct
{
  QGroupBox           *sbox;
  QGroupBox           *dbox;
  QGroupBox           *fbox;
  QGroupBox           *wbox;
  QProgressBar        *sbar;
  QProgressBar        *dbar;
  QProgressBar        *fbar;
  QProgressBar        *wbar;
  QListWidget         *list;
} RUN_PROGRESS;


//  Place to save X, Y, Z, line number, horizontal uncertainty, vertical uncertainty, and anything else we may need in the future.

typedef struct
{
  NV_F64_COORD3       xyz;          //  X, Y, Z
  uint32_t            l;            //  line number for Hockey Puck of Confidence (TM)
  float               h;            //  horizontal uncertainty
  float               v;            //  vertical uncertainty
} POINT_DATA;


//  This feature structure is overkill but, if I can come up with a better method of determining the
//  shape of the feature, it may be useful in the future.

typedef struct
{
  double              x;
  double              y;
  double              z;
  float               h;            //  horizontal uncertainty
  float               v;            //  vertical uncertainty
  double              bin_size;     //  Based on search radius
  uint8_t             confidence;
  uint8_t             oct_hit[8];   //  Whether there was a point in this octant
  NV_F64_COORD3       oct_pos[8];   //  Position and depth of the point
  uint32_t            parent_record;//  Set to indicate that the feature has a parent.  The value is the parent feature number plus 1.
} FEATURE;


#endif
