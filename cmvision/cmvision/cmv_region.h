/*========================================================================
    cmv_region.h : Region and connected component support for CMVision2
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

#ifndef __CMV_REGION_H__
#define __CMV_REGION_H__

#include "cmv_types.h"

namespace CMVision{

//==== Utility Functions ===========================================//

// sum of integers over range [x,x+w)
inline int range_sum(int x,int w)
{
  return(w*(2*x + w-1) / 2);
}

// sum of integer squares over range [x,x+w)
// S(n) = n*(n+1)*(2*n+1) / 6
// R(x,w) = S(x+w-1) - S(x-1)
// ref: http://mathworld.wolfram.com/SquarePyramidalNumber.html
// note: if x+w > 1024, you must use 64-bit ints for correct results
inline int range_sum_sq(int x,int w)
{
  int y = x + w;
  int rs = y*(y-1)*(2*y-1) - x*(x-1)*(2*x-1);
  return(rs / 6);
}

#if 0
// table used by bottom_bit
const int log2modp[37] = {
  0, 1, 2,27, 3,24,28, 0, 4,17,25,31,29,12, 0,14, 5, 8,18,
  0,26,23,32,16,30,11,13, 7, 0,22,15,10, 6,21, 9,20,19
};

// returns index of least significant set bit
template <class num>
inline int bottom_bit(num n)
{
  return(log2modp[(n & -n) % 37]);
}

// returns index of most significant set bit
template <class num>
inline num top_bit(num n)
{
  /*
  n = n | (n >>  1);
  n = n | (n >>  2);
  n = n | (n >>  4);
  n = n | (n >>  8);
  n = n | (n >> 16);
  n = n - (n >>  1);
  return(log2modp[n % 37]);
  */

  // I'm unconvinced of the correctness of this
  n |= (n >> 1) | (n >> 2) | (n >>  3);
  n |= (n >> 4) | (n >> 8) | (n >> 12);
  n |= (n >> 16);
  n -= (n >>  1);
  return(log2modp[n % 37]);

  /*
  int i = 1;
  if(!n) return(0);
  while(n>>i) i++;
  return(i);
  */
}
#endif

//==== Main Library Functions ======================================//

#include "colors.h"

template <class rle_t,class pixel>
yuv AverageColor(pixel *buf,int width,int height,
		 rle_t *rmap,int start_index)
{
  static const bool debug = false;
  yuvi sum;
  yuv avg;
  rle_t r;
  pixel p;
  int i,x,lx,area,a2;

  sum.y = sum.u = sum.v = 0;
  area = a2 = 0;
  r.next = start_index;

  do{
    i = r.next;
    r = rmap[i];
    area += r.width;
    x = r.x;
    lx = r.x + r.width - 1;

    if(debug) printf("    %d-%d:%d:",x,lx,r.width);

    // if starting on an odd pixel, add last half of YUYV pixel
    if((x & 1) == 1){
      p = buf[(r.y*width + x)/2];
      sum.y += p.y2;
      sum.u += p.u;
      sum.v += p.v;
      if(debug){
	printf("[%d]",x);
        a2++;
      }
      x++;
    }

    // add up the color sums for a run
    for(; x<lx; x+=2){
      p = buf[(r.y*width + x)/2];
      sum.y += p.y1 + p.y2;
      sum.u += 2 * p.u;
      sum.v += 2 * p.v;
      if(debug){
	printf("[%d,%d]",x,x+1);
	a2 += 2;
      }
    }

    // if last pixel is an even pixel, add first half of YUYV pixel
    if((lx & 1) == 0){
      p = buf[(r.y*width + lx)/2];
      sum.y += p.y1;
      sum.u += p.u;
      sum.v += p.v;
      if(debug){
	printf("[%d]",lx);
	a2++;
      }
    }
    if(debug) printf("\n");

    // a back link indicates a run list terminator
  }while(r.next > i);

  avg.y = sum.y / area;
  avg.u = sum.u / area;
  avg.v = sum.v / area;

  if(debug){
    printf("%d=%d : %d %d %d (%d %d %d)\n",
	   area,a2,
	   avg.y,avg.u,avg.v,
	   sum.y,sum.u,sum.v);
  }

  return(avg);
}

template <class rle_t,class tmap_t>
int EncodeRuns(rle_t *rle,tmap_t *map,int width,int height,int max_runs)
// Changes the flat array version of the thresholded image into a run
// length encoded version, which speeds up later processing since we
// only have to look at the points where values change.
{
  tmap_t m,save;
  tmap_t *row;
  int x,y,j,l;
  rle_t r;

  r.next = 0;

  // initialize terminator restore
  save = map[0];

  j = 0;
  for(y=0; y<height; y++){
    row = &map[y * width];

    // restore previous terminator and store next
    // one in the first pixel on the next row
    row[0] = save;
    save = row[width];
    row[width] = 255;
    r.y = y;

    x = 0;
    while(x < width){
      m = row[x];
      r.x = x;

      l = x;
      while(row[x] == m) x++;

      if(m!=0 || x>=width){
	r.color = m; 
	r.width = x - l;
	r.parent = j;
	rle[j++] = r;

        // printf("run (%d,%d):%d %d\n",r.x,r.y,r.width,r.color);

	if(j >= max_runs){
          row[width] = save;
          return(j);
        }
      }
    }
  }

  return(j);
}

template <class rle_t>
void ConnectComponents(rle_t *map,int num)
// Connect components using four-connecteness so that the runs each
// identify the global parent of the connected region they are a part
// of.  It does this by scanning adjacent rows and merging where
// similar colors overlap.  Used to be union by rank w/ path
// compression, but now is just uses path compression as the global
// parent index is a simpler rank bound in practice.
// WARNING: This code is complicated.  I'm pretty sure it's a correct
//   implementation, but minor changes can easily cause big problems.
//   Read the papers on this library and have a good understanding of
//   tree-based union find before you touch it
{
  int l1,l2;
  rle_t r1,r2;
  int i,j,s;

  // l2 starts on first scan line, l1 starts on second
  l2 = 0;
  l1 = 1;
  while(map[l1].y == 0) l1++; // skip first line

  // Do rest in lock step
  r1 = map[l1];
  r2 = map[l2];
  s = l1;
  while(l1 < num){
    /*
    printf("%6d:(%3d,%3d,%3d) %6d:(%3d,%3d,%3d)\n",
	   l1,r1.x,r1.y,r1.width,
	   l2,r2.x,r2.y,r2.width);
    */

    if(r1.color==r2.color && r1.color){
      // case 1: r2.x <= r1.x < r2.x + r2.width
      // case 2: r1.x <= r2.x < r1.x + r1.width
      if((r2.x<=r1.x && r1.x<r2.x+r2.width) ||
	 (r1.x<=r2.x && r2.x<r1.x+r1.width)){
        if(s != l1){
          // if we didn't have a parent already, just take this one
          map[l1].parent = r1.parent = r2.parent;
          s = l1;
        }else if(r1.parent != r2.parent){
          // otherwise union two parents if they are different

          // find terminal roots of each path up tree
          i = r1.parent;
          while(i != map[i].parent) i = map[i].parent;
          j = r2.parent;
          while(j != map[j].parent) j = map[j].parent;

          // union and compress paths; use smaller of two possible
          // representative indicies to preserve DAG property
          if(i < j){
	    map[j].parent = i;
            map[l1].parent = map[l2].parent = r1.parent = r2.parent = i;
          }else{
            map[i].parent = j;
            map[l1].parent = map[l2].parent = r1.parent = r2.parent = j;
          }
        }
      }
    }

    // Move to next point where values may change
    i = (r2.x + r2.width) - (r1.x + r1.width);
    if(i >= 0) r1 = map[++l1];
    if(i <= 0) r2 = map[++l2];
  }

  // Now we need to compress all parent paths
  for(i=0; i<num; i++){
    j = map[i].parent;
    map[i].parent = map[j].parent;
  }
}

template <class region_t,class rle_t>
int ExtractRegions(region_t *reg,int max_reg,rle_t *rmap,int num)
// Takes the list of runs and formats them into a region table,
// gathering the various statistics along the way.  num is the number
// of runs in the rmap array, and the number of unique regions in
// reg[] (bounded by max_reg) is returned.  Implemented as a single
// pass over the array of runs.
{
  int b,i,n,a;
  rle_t r;

  n = 0;

  for(i=0; i<num; i++){
    if(rmap[i].color){
      r = rmap[i];
      if(r.parent == i){
        // Add new region if this run is a root (i.e. self parented)
        rmap[i].parent = b = n;  // renumber to point to region id
        reg[b].color = r.color;
        reg[b].area = r.width;
        reg[b].x1 = r.x;
        reg[b].y1 = r.y;
        reg[b].x2 = r.x + r.width;
        reg[b].y2 = r.y;
        reg[b].cen_x = range_sum(r.x,r.width);
        reg[b].cen_y = r.y * r.width;
	reg[b].run_start = i;
	reg[b].iterator_id = i; // temporarily use to store last run
        n++;
        if(n >= max_reg) return(max_reg);
      }else{
        // Otherwise update region stats incrementally
        b = rmap[r.parent].parent;
        rmap[i].parent = b; // update parent to identify region id
        reg[b].area += r.width;
        reg[b].x2 = max(r.x + r.width,reg[b].x2);
        reg[b].x1 = min((int)r.x,reg[b].x1);
        reg[b].y2 = r.y; // last set by lowest run
        reg[b].cen_x += range_sum(r.x,r.width);
        reg[b].cen_y += r.y * r.width;
	// set previous run to point to this one as next
	rmap[reg[b].iterator_id].next = i;
	reg[b].iterator_id = i;
      }
    }
  }

  // calculate centroids from stored sums
  for(i=0; i<n; i++){
    a = reg[i].area;
    reg[i].cen_x = (float)reg[i].cen_x / a;
    reg[i].cen_y = (float)reg[i].cen_y / a;
    rmap[reg[i].iterator_id].next = 0; // -1;
    reg[i].iterator_id = 0;
    reg[i].x2--; // change to inclusive range
  }

  return(n);
}

template <class color_class_state_t,class region_t>
int SeparateRegions(color_class_state_t *color,int colors,
		    region_t *reg,int num)
// Splits the various regions in the region table a separate list for
// each color.  The lists are threaded through the table using the
// region's 'next' field.  Returns the maximal area of the regions,
// which can be used later to speed up sorting.
{
  region_t *p;
  int i; // ,l;
  int c;
  int area,max_area;

  // clear out the region list head table
  for(i=0; i<colors; i++){
    color[i].list = NULL;
    color[i].num  = 0;
  }

  // step over the table, adding successive
  // regions to the front of each list
  max_area = 0;
  for(i=0; i<num; i++){
    p = &reg[i];
    c = p->color;
    area = p->area;

    if(area >= color[c].min_area){
      if(area > max_area) max_area = area;
      color[c].num++;
      p->next = color[c].list;
      color[c].list = p;
    }
  }

  return(max_area);
}

// These are the tweaking values for the radix sort given below
// Feel free to change them, though these values seemed to work well
// in testing.  Don't worry about extra passes to get all 32 bits of
// the area; the implementation only does as many passes as needed to
// touch the most significant set bit (MSB of largest region's area)
#define CMV_RBITS 6
#define CMV_RADIX (1 << CMV_RBITS)
#define CMV_RMASK (CMV_RADIX-1)

template <class region_t>
region_t *SortRegionListByArea(region_t *list,int passes)
// Sorts a list of regions by their area field.
// Uses a linked list based radix sort to process the list.
{
  region_t *tbl[CMV_RADIX],*p,*pn;
  int slot,shift;
  int i,j;

  // Handle trivial cases
  if(!list || !list->next) return(list);

  // Initialize table
  for(j=0; j<CMV_RADIX; j++) tbl[j] = NULL;

  for(i=0; i<passes; i++){
    // split list into buckets
    shift = CMV_RBITS * i;
    p = list;
    while(p){
      pn = p->next;
      slot = ((p->area) >> shift) & CMV_RMASK;
      p->next = tbl[slot];
      tbl[slot] = p;
      p = pn;
    }

    // integrate back into partially ordered list
    list = NULL;
    for(j=0; j<CMV_RADIX; j++){
      p = tbl[j];
      tbl[j] = NULL; // clear out table for next pass
      while(p){
        pn = p->next;
        p->next = list;
        list = p;
        p = pn;
      }
    }
  }

  return(list);
}

template <class color_class_state_t>
void SortRegions(color_class_state_t *color,int colors,int max_area)
// Sorts entire region table by area, using the above
// function to sort each threaded region list.
{
  int i,p;

  // do minimal number of passes sufficient to touch all set bits
  p = 0;
  while(max_area != 0){
    max_area >>= CMV_RBITS;
    p++;
  }

  // sort each list
  for(i=0; i<colors; i++){
    color[i].list = SortRegionListByArea(color[i].list,p);
  }
}

template <class region_t,class rle_t>
int FindStart(rle_t *rmap,int left,int right,int x)
// This function uses binary search to find the leftmost run whose
// interval either contains or is greater than x.
{
  int m;

  while(right > left){
    m = (left + right) / 2;
    if(x > rmap[m].x+rmap[m].width){
      left = m + 1;
    }else if(x < rmap[m].x){
      right = m;
    }else{
      return(m);
    }
  }

  return(m);
}

template <class rle_t>
int FindRun(rle_t *rmap,int left,int right,int x,int y)
// This function uses binary search to find the leftmost run whose
// interval either contains (x,y), or returns -1 if no such run exists
// (for example when (x,y) is background color)
{
  int m = 0;

  while(right > left){
    m = (left + right) / 2;
    if(y > rmap[m].y || (y == rmap[m].y && x > rmap[m].x+rmap[m].width)){
      left = m + 1;
    }else if(y < rmap[m].y || (y == rmap[m].y && x < rmap[m].x)){
      right = m;
    }else{
      return(m);
    }
  }

  if(y==rmap[m].y && x>=rmap[m].x && x<rmap[m].x+rmap[m].width){
    return(m);
  }else{
    return(-1);
  }
}

template <class region_t,class rle_t>
void CreateRunIndex(int *yindex,rle_t *rmap,int num)
// Creates an index of where rows start in the run map.  This can be
// used to speed up searching over the map.  This function assumes
// there is at least one run in every row.
{
  int y = 0;
  yindex[y] = 0;

  for(int i=0; i<num; i++){
    if(rmap[i].y > y){
      y = rmap[i].y;
      yindex[y] = i;
    }
  }
}

/*
template <class color_class_state_t>
void GetNextRegion(color_class_state_t *color,int colors,int max_area)
// Sorts entire region table by area, using the above
// function to sort each threaded region list.
{
  int i,p;

  // do minimal number of passes sufficient to touch all set bits
  p = (top_bit(max_area) + CMV_RBITS-1) / CMV_RBITS;

  // sort each list
  for(i=0; i<colors; i++){
    color[i].list = SortRegionListByArea(color[i].list,p);
  }
}
*/

/*
template <class rle_t,class region_t,class regtorgb>
void RegionToRgbImage(rgb *img,int width,int height,
		      rle_t *rmap,
		      region_t *reg,int num_reg,
		      regtorgb &map)
{
  rle_t r;
  int i,j,ri;
  rgb c;

  for(i=0; i<num_reg; i++){
    c = map(reg[i]);
    ri = reg[i].run_start;

    do{
      r = rmap[ri];

      for(j=0; j<r.width; j++){
	img[r.y*width + r.x + j] = c;
      }

    }while(ri = r.next);
  }
}
*/

template <class rle_t>
void RegionToRgbImage(rgb *img,int width,int height,
		      rle_t *rmap,int num_runs,
                      rgb *reg_color)
{
  rle_t r;
  int i,j;
  rgb c;

  for(i=0; i<num_runs; i++){
    r = rmap[i];
    if(r.color){
      c = reg_color[r.parent];
    }else{
      c.red = c.green = c.blue = 0;
    }

    for(j=0; j<r.width; j++){
      img[r.y*width + r.x + j] = c;
    }
  }
}

template <class data>
int find(data *arr,int start,int end,data key)
{
  int i;

  for(i=start; i<end; i++){
    if(arr[i] == key) return(i);
  }

  return(end);
}

template <class color_class_state_t>
int LoadColorInformation(color_class_state_t *color,int max,
                         const char *filename)
{
  char buf[512];
  FILE *in;
  int len;
  int sl,sr;
  int num;

  int idx,r,g,b,ms;
  char *name;

  in = fopen(filename,"rt");
  if(!in) return(0);

  memset(color,0,sizeof(color_class_state_t)*max);
  num = 0;
  memset(buf,0,512);

  while(fgets(buf,256,in)){
    len = strlen(buf) - 1;
    buf[len] = 0;

    if(len && buf[0]!='#'){
      sl = find(buf,   0,len,'"');
      sr = find(buf,sl+1,len,'"');
      if(sl<len && sr<len){
	buf[sl] = buf[sr] = 0;
	idx = r = g = b = ms = 0;
	sscanf(buf,"%d (%d %d %d)",&idx,&r,&g,&b);
	name = buf+sl+1;
	sscanf(buf+sr+1,"%d",&ms);

	if(idx>=0 && idx<max && ms>0){
	  color[idx].min_area = ms;
	  color[idx].color.red   = r;
	  color[idx].color.green = g;
	  color[idx].color.blue  = b;
	  color[idx].name = strdup(name);
	  if(idx >= num) num = idx+1;
	}else{
 	  printf("Options error: %2d (%3d %3d %3d) '%s' %d\n",
		 idx,r,g,b,name,ms);
	}
      }
    }
  }

  fclose(in);

  return(num);
}

} // namespace

#endif
