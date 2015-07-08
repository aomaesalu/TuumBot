// (C) 2004-2006 James R. Bruce, Carnegie Mellon University
// Licenced under the GNU General Public License (GPL) version 2,
//   or alternately by a specific written agreement.

#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>

#include "ansicolor.h"

#include "v4l_helper.h"


int VideoDevice::xioctl(int fd,int request,void *data,
                        const char *error_str)
{
  // try the ioctl, which should succeed in the common case
  int ret = ioctl(fd,request,data);
  if(ret >= 0) return(ret);

  // retry if we were interrupted (up to a few times)
  int n=0;
  while(ret!=0 && errno==EINTR && n<8){
    ret = ioctl(fd,request,data);
    n++;
  }

  // report error
  if(ret != 0){
    if(!error_str) error_str = "";
    AnsiColor::SetFgColor(stderr,AnsiColor::Red);
    AnsiColor::Bold(stderr);
    fprintf(stderr,"VideoDevice: %s returned %d (%s)\n",
            error_str,ret,strerror(errno));
    AnsiColor::Reset(stderr);
  }

  return(ret);
}

bool VideoDevice::open(const char *device)
{
  // open device
  close();
  fd = ::open(device, O_RDWR|O_NONBLOCK);
  if(fd < 0) return(false);

  pollset.fd = fd;
  pollset.events = POLLIN;

  // test capture capabilities
  v4l2_capability cap;
  if(xioctl(fd, VIDIOC_QUERYCAP, &cap, "VideoQueryCap") == 0){
    if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) ||
       !(cap.capabilities & V4L2_CAP_STREAMING)){
      return(false);
    }
  }else{
    return(false);
  }

  // set cropping to default (no effect if not supported)
  if(false){
    v4l2_cropcap cropcap;
    v4l2_crop crop;
    mzero(cropcap);
    mzero(crop);
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    xioctl(fd, VIDIOC_CROPCAP, &cropcap, "CropCap");
    crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    crop.c = cropcap.defrect;
    xioctl(fd, VIDIOC_S_CROP, &crop, "SetCrop");
  }

  return(true);
}

bool VideoDevice::close()
{
  if(fd >= 0) ::close(fd);
  fd = -1;
  pollset.fd = -1;
  return(true);
}

bool VideoDevice::setInput(int idx)
{
  return(xioctl(fd,VIDIOC_S_INPUT,&idx,"SetInput") == 0);
}

bool VideoDevice::setInput(const char *str)
{
  // TODO
  return(false);
}

void VideoDevice::listInputs(FILE *out)
{
  v4l2_input input;
  int current = -1;
  if(xioctl(fd, VIDIOC_G_INPUT, &current) < 0) return;

  int index = 0;
  fprintf(out,"Inputs:\n");
  while(true){
    memset(&input, 0, sizeof (input));
    input.index = index;
    int ret = xioctl(fd, VIDIOC_ENUMINPUT, &input);
    if(ret < 0) break;
    char ch = (index == current)? '#' : ' ';
    fprintf(out,"  %d: (%c) \"%s\"\n",index,ch,input.name);
    index++;
  }
}

bool VideoDevice::setStandard(v4l2_std_id id)
{
  return(xioctl(fd,VIDIOC_S_STD,&id,"SetStandard") == 0);
}

bool VideoDevice::startStreaming()
{
  v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  return(xioctl(fd, VIDIOC_STREAMON, &type, "StreamOn") == 0);
}

bool VideoDevice::stopStreaming()
{
  v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  return(xioctl(fd, VIDIOC_STREAMOFF, &type, "StreamOff") == 0);
}

/*
  V4L2_CID_BRIGHTNESS
  V4L2_CID_CONTRAST
  V4L2_CID_SATURATION
  V4L2_CID_HUE
*/

bool VideoDevice::getControl(int ctrl_id,float &s)
{
  v4l2_control ctrl;
  mzero(ctrl);
  ctrl.id = ctrl_id;

  if(xioctl(fd, VIDIOC_G_CTRL, &ctrl, "GetControl") == 0){
    s = (float)ctrl.value / MaxCtrlVal;
    return(true);
  }else{
    return(false);
  }
}

bool VideoDevice::setControl(int ctrl_id,float s)
{
  v4l2_control ctrl;
  mzero(ctrl);
  ctrl.id = ctrl_id;
  ctrl.value = bound((int)rint(s * MaxCtrlVal),0,MaxCtrlVal-1);
 
  if(xioctl(fd, VIDIOC_S_CTRL, &ctrl, "SetControl") == 0){
    return(true);
  }else{
    return(false);
  }
}

bool VideoDevice::getFormat(v4l2_format &fmt)
{
  memset(&fmt,0,sizeof(fmt));
  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  return(xioctl(fd, VIDIOC_G_FMT, &fmt, "GetFormat") == 0);
}

bool VideoDevice::setFormat(v4l2_format &fmt)
{
  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  return(xioctl(fd, VIDIOC_S_FMT, &fmt, "SetFormat") == 0);
}

bool VideoDevice::initCapture()
{
  // mmap initialization
  // TODO
  return(false);
}

bool VideoDevice::stopCapture()
{
  // TODO
  return(false);
}

bool VideoDevice::waitForFrame(int max_msec)
{
  int n = poll(&pollset,1,max_msec);
  return(n==1 && (pollset.revents & POLLIN)!=0);
}

bool VideoDevice::enqueueBuffer(v4l2_buffer &buf)
{
  // printf("e %d\n",buf.index);
  int ret = xioctl(fd, VIDIOC_QBUF, &buf, "EnqueueBuffer");
  return(ret == 0);
}

bool VideoDevice::dequeueBuffer(v4l2_buffer &buf)
{
  // printf("d %d\n",buf.index);
  int ret = xioctl(fd, VIDIOC_DQBUF, &buf, "DequeueBuffer");
  return(ret == 0);
}
