/*========================================================================
    cmv_types.h : Base types for CMVision2
  ------------------------------------------------------------------------
    Copyright (C) 1999-2005 James R. Bruce
    School of Computer Science, Carnegie Mellon University
  ------------------------------------------------------------------------
    This software is distributed under the GNU General Public License,
    version 2.  If you do not have a copy of this licence, visit
    www.gnu.org, or write: Free Software Foundation, 59 Temple Place,
    Suite 330 Boston, MA 02111-1307 USA.  This program is distributed
    in the hope that it will be useful, but WITHOUT ANY WARRANTY,
    including MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  ========================================================================*/

#ifndef __CMVISION_TYPES_H__
#define __CMVISION_TYPES_H__

namespace CMVision{

// uncomment if your compiler supports the "restrict" keyword
// #define restrict __restrict__
#define restrict

template <class pixel>
class image{
public:
  pixel *buf;
  int width,height,pitch;
  int field;
};

#define CMV_PARAM <class pixel,class vimage,class threshold>
#define CMV_TEM template CMV_PARAM

#define CMV_NONE ((unsigned)(-1))

/*
// Options for level of processing
//   use enable()/disable() to change 
#define CMV_THRESHOLD      0x01
#define CMV_COLOR_AVERAGES 0x02
#define CMV_DUAL_THRESHOLD 0x04
#define CMV_DENSITY_MERGE  0x08

#define CMV_VALID_OPTIONS  0x0F
*/

template <class cclass>
class run{
public:
  short x,y,width;    // location and width of run
  cclass color;       // which color(s) this run represents
  int parent,next;    // parent run and next run in run list
};

template <class cclass>
class run_mini{
public:
  short x,y,width;    // location and width of run
  cclass color;       // which color(s) this run represents
  short parent,next;  // parent run and next run in run list
};

struct region{
  int color;         // id of the color
  int x1,y1,x2,y2;   // bounding box (x1,y1) - (x2,y2)
  float cen_x,cen_y; // centroid
  int area;          // occupied area in pixels
  int run_start;     // first run index for this region
  int iterator_id;   // id to prevent duplicate hits by an iterator
  region *next;      // next region in list
};

struct region_small{
  short color;       // id of the color
  short x1,y1,x2,y2; // bounding box (x1,y1) - (x2,y2)
  short iterator_id; // id to prevent duplicate hits by an iterator
  float cen_x,cen_y; // centroid
  int area;          // occupied area in pixels
  int run_start;     // first run index for this region
  region *next;      // next region in list
};

struct region_tiny{
  uchar color;       // id of the color
  uchar x1,y1,x2,y2; // bounding box (x1,y1) - (x2,y2)
  float cen_x,cen_y; // centroid
  short area;        // occupied area in pixels
  short run_start;   // first run index for this region
  short iterator_id; // id to prevent duplicate hits by an iterator
  region *next;      // next region in list
};

struct color_class_state{
  region *list;      // head of region list for this color
  int num;           // number of regions of this color
  int min_area;      // minimum area for a meaningful region
  rgb color;         // example color (such as used in test output)
  char *name;        // color's meaningful name (e.g. orange ball, goal)
};

} // namespace

#endif
