/*========================================================================
    capture.h : Video4Linux2 raw video capture class for CMVision2
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

#ifndef __CAPTURE_H__
#define __CAPTURE_H__

#include <stdlib.h>

#include "v4l_helper.h"


#define DEFAULT_VIDEO_DEVICE  "/dev/video"
#define VIDEO_STANDARD        "NTSC"
#define DEFAULT_IMAGE_WIDTH   640
#define DEFAULT_IMAGE_HEIGHT  240

// if you get a message like "DQBUF returned error", "DQBUF error: invalid"
// then you need to use a higher value for STREAMBUFS or process frames faster
#define STREAMBUFS            3

#define DEFAULT_VIDEO_FORMAT  V4L2_PIX_FMT_YUYV

class Capture {
public:
  class Image{
  public:
    char *data;        // image data
    size_t length;     // data buffer size
    int width,height;  // image dimensions
    int bytesperline;  // image pitch, usually width*sizeof(pixel)
    timeval timestamp; // timestamp for when the frame was captured
    char field;        // which field of interlaced video this is (0,1)
  public:
    Image()
      {data=NULL; length=0;}
    int size() const
      {return(bytesperline * height);}
    double getTimeSec() const
      {return(timestamp.tv_sec + timestamp.tv_usec*1E-6);}
    bool copy(const Image &img);
  private:
    char index;
    // v4l2_buffer vidbuf;
    friend class Capture;
  };

  enum ImageType{
    ImageTypeUnknown = 0,
    ImageTypeRawYUV  = 1,
  };

  class RawImageFileHdr{
  public:
    uint16_t type;         // from ImageType enum
    uint8_t  reserved;     // reserved, default to zero
    uint8_t  field;        // which field of video this is from {0,1}
    uint16_t width,height; // image dimensions
    timeval  timestamp;
  };

protected:
  VideoDevice vid;        // video device
  Image img[STREAMBUFS];  // buffers for images
  int pixelformat;        // format of image data
  struct v4l2_buffer tempbuf;
  bool *run_flag;

  // log data if reading log
  char *log_data;
  int log_idx,log_num_images;
  int log_bytes_per_image;
  bool log_has_header;

  // log file if writing log
  int out_log_fd;
public:
  Capture()
    {run_flag=NULL; log_data=NULL; out_log_fd=-1;}
  ~Capture()
    {close();}

  bool init(const char *device,int input_idx,
            int nwidth,int nheight,int nfmt);
  bool initLog(const char *device,int nwidth,int nheight,int nfmt);
  void close();

  const Image *captureFrame(int step=1);
  bool releaseFrame(const Image *_img);

  bool isLog()
    {return(log_data != NULL);}

  int getFd()
    {return(vid.fd);}
  void setRunFlag(bool *_run_flag)
    {run_flag=_run_flag;}

  bool writeFrame(const Image *image,int out_fd);

  bool openWriteLog(const char *logfile,bool append=false);
  bool closeWriteLog();
  bool writeFrame(const Image *image)
    {return(writeFrame(image,out_log_fd));}
  bool isWriteLogOpen()
    {return(out_log_fd>=0);}

  bool saveLog(const char *logfile,int num_frames,int skip=1);
  int getLogNumFrames()
    {return(log_num_images);}
  int getLogFrameIdx()
    {return(log_idx);}
};

#endif // __CAPTURE_H__
