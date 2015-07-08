// (C) 2004-2006 James R. Bruce, Carnegie Mellon University
// Licenced under the GNU General Public License (GPL) version 2,
//   or alternately by a specific written agreement.

#ifndef __V4L_HELPER_H__
#define __V4L_HELPER_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/poll.h>

#include <asm/types.h>
#include <linux/videodev2.h>

#include "util.h"

class VideoDevice{
public:
  int fd;
  pollfd pollset;
protected:
  int xioctl(int fd,int request,void *data,
             const char *error_str = NULL);
public:
  VideoDevice()
    {fd = -1;}
  ~VideoDevice()
    {close();}

  bool open(const char *device);
  bool close();
  bool isOpen()
   {return(fd >= 0);}

  // Video input
  bool setInput(int idx);
  bool setInput(const char *str);
  void listInputs(FILE *out = stdout);

  // Video standard (i.e. NTSC/PAL)
  bool setStandard(v4l2_std_id id);

  // Streaming capture control
  bool startStreaming();
  bool stopStreaming();

  // Controls
  struct CtrlVal{
    const char *name;
    int value;
  };
  static const int MaxCtrlVal = (1 << 16);

  bool getControl(int ctrl_id,float &val);
  bool setControl(int ctrl_id,float  val);
  // bool setControls(CtrlVal *arr,int num);
  // bool listControls();

  // Capture formats
  bool getFormat(v4l2_format &fmt);
  bool setFormat(v4l2_format &fmt);

  // Video capture
  bool initCapture();
  bool stopCapture();
  bool waitForFrame(int max_msec);
  bool isFrameReady()
    {return(waitForFrame(0));}

  bool enqueueBuffer(v4l2_buffer &buf);
  bool dequeueBuffer(v4l2_buffer &buf);
};


#endif
