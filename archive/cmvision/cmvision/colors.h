/*========================================================================
    colors.h: Color structure definitions for various software
  ------------------------------------------------------------------------
    Copyright (C) 1999-2005  James R. Bruce
    School of Computer Science, Carnegie Mellon University
  ------------------------------------------------------------------------
    This software is distributed under the GNU General Public License,
    version 2.  If you do not have a copy of this licence, visit
    www.gnu.org, or write: Free Software Foundation, 59 Temple Place,
    Suite 330 Boston, MA 02111-1307 USA.  This program is distributed
    in the hope that it will be useful, but WITHOUT ANY WARRANTY,
    including MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  ========================================================================*/

#ifndef __COLORS_H__
#define __COLORS_H__

#include <math.h>

#define RGB_COLOR_NAMES
#define RGB_OPERATORS


//==== Color Classes =================================================//

typedef unsigned char uchar;

#ifndef YUV_STRUCT
#define YUV_STRUCT
struct yuv{
  uchar y,u,v;
};

/* Depricated
struct yuv_int{
  int y,u,v;
};
*/
#endif

#ifndef YUVI_STRUCT
#define YUVI_STRUCT
struct yuvi{
  int y,u,v;
};
#endif

#ifndef YUVF_STRUCT
#define YUVF_STRUCT
struct yuvf{
  float y,u,v;
};
#endif

#ifndef YUYV_STRUCT
#define YUYV_STRUCT
struct yuyv{
  uchar y1,u,y2,v;
};
#endif

struct uyvy{
#ifndef UYVY_STRUCT
#define UYVY_STRUCT
  uchar u,y1,v,y2;
};
#endif

#ifndef RGB_STRUCT
#define RGB_STRUCT
struct rgb{
  uchar red,green,blue;

public:
  bool operator ==(const rgb &c) const
    {return(red==c.red && green==c.green && blue==c.blue);}
  bool operator !=(const rgb &c) const
    {return(red!=c.red || green!=c.green || blue!=c.blue);}

  void set(uchar r,uchar g,uchar b)
    {red=r; green=g; blue=b;}
  void set(uchar y)
    {red = green = blue = y;}

  rgb operator*(float f){
    rgb r;
    r.set((int)rint(red*f),
          (int)rint(green*f),
          (int)rint(blue*f));
    return(r);
  }
};
#endif

#ifndef RGB_INT_STRUCT
#define RGB_INT_STRUCT
struct rgb_int{
  int r,g,b;

  void set(int _r,int _g,int _b)
    {r=_r; g=_g; b=_b;}
};
#endif

#ifndef RGBA_STRUCT
#define RGBA_STRUCT
struct rgba{
  uchar r,g,b,a;

  void set(uchar _r,uchar _g,uchar _b,uchar _a)
    {r=_r; g=_g; b=_b; a=_a;}
};
#endif

#ifndef RGBA_INT_STRUCT
#define RGBA_INT_STRUCT
struct rgba_int{
  int r,g,b,a;

  void set(int _r,int _g,int _b,int _a)
    {r=_r; g=_g; b=_b; a=_a;}
};
#endif

#ifndef ARGB_STRUCT
#define ARGB_STRUCT
struct argb{
  uchar a,r,g,b;

  void set(uchar _r,uchar _g,uchar _b,uchar _a)
    {r=_r; g=_g; b=_b; a=_a;}
};
#endif

#ifndef RGBF_STRUCT
#define RGBF_STRUCT
struct rgbf{
  float red,green,blue;

  rgbf()
    {}
  rgbf(float nred,float ngreen,float nblue)
    {red=nred; green=ngreen; blue=nblue;}
};
#endif

#ifndef RGBAF_STRUCT
#define RGBAF_STRUCT
struct rgbaf{
  float r,g,b,a;

  void set(float _r,float _g,float _b,float _a)
    {r=_r; g=_g; b=_b; a=_a;}
};
#endif

#ifndef ARGBF_STRUCT
#define ARGBF_STRUCT
struct argbf{
  float a,r,g,b;
};
#endif


//==== Color Names ===================================================//

#ifdef RGB_COLOR_NAMES
namespace Rgb{
  static const rgb Black = {  0,  0,  0};
  static const rgb Blue  = {  0,  0,255};
  static const rgb Green = {  0,255,  0};
  static const rgb Red   = {255,  0,  0};
  static const rgb White = {255,255,255};
  static const rgb Gray  = {128,128,128};

  static const rgb Cyan   = {  0,255,255};
  static const rgb Purple = {255,  0,255};
  static const rgb Yellow = {255,255,  0};
  static const rgb Orange = {255,128,  0};

  static const rgb MidBlue  = {  0,  0,192};
  static const rgb MidGreen = {  0,192,  0};
  static const rgb MidRed   = {192,  0,  0};

  static const rgb DarkBlue  = {  0,  0,128};
  static const rgb DarkGreen = {  0,128,  0};
  static const rgb DarkRed   = {128,  0,  0};
  static const rgb DarkPurple = {128,  0,128};

  static const rgb DarkGray = { 32, 32, 48};

  static const rgb FieldGreen     = {  0, 96,  0};
  static const rgb DarkFieldGreen = {  0, 72,  0};
};
#endif


//==== Color Arithmetic ==============================================//

#ifdef RGB_OPERATORS
rgb to_rgb(rgbf c);
rgbf to_rgbf(rgb c);
rgbf clip(rgbf c);
rgbf operator +(const rgbf a,const rgbf b);
rgbf operator *(const rgbf a,const rgbf b);
rgbf operator *(const rgbf a,const double m);
rgbf pow(const rgbf x,const double p);
double sse(rgbf a,rgbf b);
#endif

#endif
