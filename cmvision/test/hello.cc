// hello.cc: A simple test program for CMVision2

// (C) 2005-2006 James R. Bruce, Carnegie Mellon University
// Licenced under the GNU General Public License (GPL) version 2,
//   or alternately by a specific written agreement.

#include <getopt.h>

#include "capture.h"
#include "vision.h"


#define PIXEL_FORMAT V4L2_PIX_FMT_YUYV


int main(int argc,char **argv)
{
  Capture cap;
  LowVision vision;

  const char *video_device = "/dev/video2";
  const int input_idx = 1;
  const int width  = 320;
  const int height = 240;
  const int num_frames = 60;

  // initialize
  cap.init(video_device,input_idx,width,height,PIXEL_FORMAT);
  char tmap_file[64];
  snprintf(tmap_file,64,"config/thresh.%d%d%d.tmap.gz",bits_y,bits_u,bits_v);
  vision.init("config/colors.txt",tmap_file,width,height);

  // main loop
  for(int i=0; i<num_frames; i++){
    // capture and process a frame
    const Capture::Image *img = cap.captureFrame();
    if(img != NULL){
      vision_image cmv_img;
      cmv_img.buf    = (pixel*)(img->data);
      cmv_img.width  = img->width;
      cmv_img.height = img->height;
      cmv_img.pitch  = img->bytesperline;
      cmv_img.field  = img->field;

      vision.processFrame(cmv_img);

      // do something with the data here
      printf("."); fflush(stdout);

      cap.releaseFrame(img);
    }
  }
  printf("\n");

  // shutdown
  vision.close();
  cap.close();

  return(0);
}
