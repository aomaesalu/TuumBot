/*======================================================================
    Util.h : Numerical utility functions
  ----------------------------------------------------------------------
    Copyright (C) 1999-2002  James R. Bruce
    School of Computer Science, Carnegie Mellon University
  ----------------------------------------------------------------------
    This software is distributed under the GNU General Public License,
    version 2.  If you do not have a copy of this licence, visit
    www.gnu.org, or write: Free Software Foundation, 59 Temple Place,
    Suite 330 Boston, MA 02111-1307 USA.  This program is distributed
    in the hope that it will be useful, but WITHOUT ANY WARRANTY,
    including MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  ======================================================================*/

#ifndef __UTIL_H__
#define __UTIL_H__

#include <math.h>
#include <string.h>
#include <algorithm>

// This seems to be missing from some systems
#ifndef M_2PI
#define M_2PI 6.28318530717958647693
#endif

using std::sort;

#if 1

using std::min;
using std::max;
using std::swap;

#else
template <class num>
inline num max(num a,num b)
{
  return((a > b)? a : b);
}

template <class num>
inline num min(num a,num b)
{
  return((a < b)? a : b);
}

template <class data>
inline void swap(data &a,data &b)
{
  data t;
  t = a;
  a = b;
  b = t;
}
#endif

template <class num1,class num2>
inline num1 bound(num1 x,num2 low,num2 high)
{
  if(x < low ) x = low;
  if(x > high) x = high;
  return(x);
}

template <class num1,class num2>
inline num1 abs_bound(num1 x,num2 range)
// bound absolute value x in [-range,range]
{
  if(x < -range) x = -range;
  if(x >  range) x =  range;
  return(x);
}

template <class num>
inline num abs_max(num a,num b)
{
  return((fabs(a) > fabs(b))? a : b);
}

template <class num>
inline num abs_min(num a,num b)
{
  return((fabs(a) < fabs(b))? a : b);
}

template <class num>
inline num max3(num a,num b,num c)
{
  if(a > b){
    return((a > c)? a : c);
  }else{
    return((b > c)? b : c);
  }
}

template <class num>
inline num min3(num a,num b,num c)
{
  if(a < b){
    return((a < c)? a : c);
  }else{
    return((b < c)? b : c);
  }
}

template <class num>
inline num max4(num a,num b,num c,num d)
{
  num x,y;
  x = max(a,b);
  y = max(c,d);
  return(max(x,y));
}

template <class num>
inline num min4(num a,num b,num c,num d)
{
  num x,y;
  x = min(a,b);
  y = min(c,d);
  return(min(x,y));
}

template <class num>
inline num max_abs(num a,num b)
{
  return((fabs(a) > fabs(b))? a : b);
}

template <class num>
inline num min_abs(num a,num b)
{
  return((fabs(a) < fabs(b))? a : b);
}

template <class data_t>
inline int max_idx(data_t *arr,int num)
{
  int mi = 0;
  for(int i=1; i<num; i++){
    if(arr[i] > arr[mi]) mi = i;
  }
  return(mi);
}

template <class data_t>
inline int min_idx(data_t *arr,int num)
{
  int mi = 0;
  for(int i=1; i<num; i++){
    if(arr[i] < arr[mi]) mi = i;
  }
  return(mi);
}

template <class num>
inline void sort(num &a,num &b,num &c)
{
  if(a > b) swap(a,b);
  if(b > c) swap(b,c);
  if(a > b) swap(a,b);
}

template <class real>
real sq(real x)
{
  return(x * x);
}

template <class real>
real cube(real x)
{
  return(x * x * x);
}

template <class num>
num sign_nz(num x)
{
  return((x >= 0)? 1 : -1);
}

template <class num>
num sign_eq(num a,num b)
{
  return((a >= 0) == (b >= 0));
}

template <class num>
num sign(num x)
{
  return((x >= 0)? (x > 0) : -1);
}

template <class bool_t>
void toggle(bool_t &b)
{
  b = !b;
}

template <class num>
bool one_bit_set(num n)
// returns true if num has only one bit set, i.e. n=2^k for some integer k
{
  return(n!=0 && ((-n) & n) == n);
}

template <class num>
num gcd(num x,num y)
// returns greatest common divisor of x,y
// NOTE: untested
{
  int w;
  while (y != 0) {
    w = x % y;
    x = y;
    y = w;
  }
  return x;
}

template <class num>
num lcm(num x,num y)
// returns least common multiple of x,y
// NOTE: untested
{
  num d = gcd(x,y);
  num r = (x / d) * (y / d);
  // num r = (x * y) / d; // faster but more likely to overflow
  return(r);
}

template <class num>
num mod(num x,num m)
// returns x mod m, where x e [0,m-1]
// note this is different from %, which can return negative numbers
{
  return(((x % m) + m) % m);
}

template <class real>
real fmodt(real x,real m)
// Does a real modulus the *right* way, using
// truncation instead of round to zero.
{
  return(x - floor(x / m)*m);
}

template <class real>
real ramp(real x,real x0,real x1)
// linear ramp from f(x0)=0 to f(x1)=1, with output bounded to [0,1]
// returns f(x)
{
  if(x < x0) return(0);
  if(x > x1) return(1);
  return((x - x0) / (x1 - x0));
}

template <class real>
real ramp(real x, real x0,real y0, real x1,real y1)
// linear ramp from f(x0)=y0 to f(x1)=y1, with output bounded to [y0,y1]
// returns f(x)
{
  if(x < x0) return(y0);
  if(x > x1) return(y1);
  return(y0 + (y1 - y0) * (x - x0) / (x1 - x0));
}

template <class num,class num2>
num bool_sat_count(num cnt,num2 min,num2 max,bool val)
// Saturating counter of a boolean value, clipped to the range
// [min,max].  True values increment the counter (up to max), while
// false values decrement the counter (down to min).
{
  if(val){
    return((cnt < max)? cnt+1 : max);
  }else{
    return((cnt > min)? cnt-1 : min);
  }
}

//==== Angle Utilities ===============================================//

// Returns angle within [-PI,PI]
template <class real>
real angle_mod(real angle)
{
  angle -= M_2PI * rint(angle / M_2PI);

  return(angle);
}

// normalize the supplied angle to lie in (-M_PI, M_PI]
// NOTE: this one passes value and returns the adjusted angle
template <class real>
inline real norm_angle(real angle)
{
  angle -= M_2PI * rint(angle / M_2PI);

  return(angle);
}

// Returns difference of two angles (a-b), within [-PI,PI]
template <class real>
real angle_diff(real a,real b)
{
  real d;

  d = a - b;
  d -= M_2PI * rint(d / M_2PI);

  return(d);
}

// Returns absolute angular distance between two angles
template <class real>
real angle_dist(real a,real b)
{
  return(fabs(angle_mod(a-b)));
}

// return the normalized angle halfway between these two
// assumes the arguments are already normalized to (-M_PI, M_PI].
template <class real>
real avg_angle(real left,real right)
{
  real result;

  if(left < right) left += 2*M_PI;
  result = (left + right)/2;
  if(result > M_PI) result -= 2*M_PI;

  return(result);
}

// bound angle <a> to interval [bound_angle-tolerance, bound_angle+tolerance]
// does the appropriate normalization of angles as long as <tolerance>
// is below M_PI.
template <class real>
real abs_bound_angle(real bound_angle,real tolerance,real a)
{
  real x = angle_mod(a - bound_angle);
  x = abs_bound(x,tolerance);
  return(angle_mod(x + bound_angle));
}

//==== Array Functions ===============================================//

template <class data>
int find_item(const data *arr,int num,data key)
{
  int i = 0;
  while(i<num && !(arr[i]==key)) i++;
  return(i);
}

template <class data,class num>
data *alloc_array(data *arr,num &size,num new_size)
{
  if((arr!=NULL && new_size==size) ||
     (arr==NULL && new_size==0)) return(arr);

  delete[](arr);
  arr = new data[new_size];
  size = (arr != NULL)? new_size : 0;
  return(arr);
}

template <class data,class num>
data *alloc_array2(data *arr,
                   num &w,num &h,
                   int new_w,int new_h)
{
  int size = w * h;
  int new_size = new_w * new_h;

  if((arr!=NULL && new_size==size) ||
     (arr==NULL && new_size==0)) return(arr);

  delete[](arr);
  arr = new data[new_size];
  if(arr){
    w = new_w;
    h = new_w;
  }else{
    w = h = 0;
  }

  return(arr);
}

template <class data>
void set_range(data *arr,int start,int num,const data &val)
{
  num += start;
  for(int i=start; i<num; i++) arr[i] = val;
}

//==== Template-based Memory Operations ==============================//

template <class data>
inline int mcopy(data *dest,data *src,int num)
{
  int i;

  for(i=0; i<num; i++) dest[i] = src[i];

  return(num);
}

template <class data>
inline data mset(data *dest,data val,int num)
{
  int i;

  for(i=0; i<num; i++) dest[i] = val;

  return(val);
}

template <class data>
inline void mzero(data &d)
{
  memset(&d,0,sizeof(d));
}

template <class data>
inline void mzero(data *d,int n)
{
  memset(d,0,sizeof(data)*n);
}

#if __GNUC__ >= 3
# define likely(x) __builtin_expect(!!(x),1)
# define unlikely(x) __builtin_expect(!!(x),0)
#else
# define likely(x) (x)
# define unlikely(x) (x)
#endif

#endif
