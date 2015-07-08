/*========================================================================
    capture.cc : Video4Linux2 raw video capture class for CMVision2
  ------------------------------------------------------------------------
    Copyright (C) 1999-2005  James R. Bruce, Anna Helena Reali Costa
    School of Computer Science, Carnegie Mellon University
  ------------------------------------------------------------------------
    This software is distributed under the GNU General Public License,
    version 2.  If you do not have a copy of this licence, visit
    www.gnu.org, or write: Free Software Foundation, 59 Temple Place,
    Suite 330 Boston, MA 02111-1307 USA.  This program is distributed
    in the hope that it will be useful, but WITHOUT ANY WARRANTY,
    including MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  ========================================================================*/

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/poll.h>
#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "capture.h"

bool Capture::Image::copy(const Image &img)
{
  // allocate memory only if needed
  if(length < img.length){
    delete[](data);
    length = 0;
    data = new char[img.length];
    if(!data) return(false);
  }

  length = img.length;
  width  = img.width;
  height = img.height;
  bytesperline = img.bytesperline;
  timestamp = img.timestamp;
  field = img.field;
  index = -1;

  memcpy(data,img.data,length);

  return(true);
}

//==== Capture Class Implementation =======================================//

bool Capture::init(const char *device,int input_idx,
                   int nwidth,int nheight,int nfmt)
{
  if(log_data){
    delete[](log_data);
    log_data = NULL;
  }

  struct v4l2_requestbuffers req;
  int err;

  // Set defaults if not given
  if(!device) device = DEFAULT_VIDEO_DEVICE;
  if(!nfmt) nfmt = DEFAULT_VIDEO_FORMAT;
  if(!nwidth || !nheight){
    nwidth  = DEFAULT_IMAGE_WIDTH;
    nheight = DEFAULT_IMAGE_HEIGHT;
  }

  // Open the video device
  if(!vid.open(device)){
    printf("Could not open video device [%s]\n",device);
    return(false);
  }

  // Set video format
  v4l2_format fmt;
  mzero(fmt);
  fmt.fmt.pix.width       = nwidth;
  fmt.fmt.pix.height      = nheight;
  fmt.fmt.pix.pixelformat = nfmt;
  fmt.fmt.pix.field       = V4L2_FIELD_ALTERNATE; 
  if(!vid.setFormat(fmt)){
    printf("Could not set format\n");
    return(false);
  }
  pixelformat = fmt.fmt.pix.pixelformat;

  // Set Input and Controls
  vid.setInput(input_idx);
  // vid.setInput(1); // Component video
  // vid.setInput(2); // S-video

  vid.setStandard(V4L2_STD_NTSC);
  vid.setControl(V4L2_CID_BRIGHTNESS, 0.50);
  vid.setControl(V4L2_CID_CONTRAST  , 0.50);
  vid.setControl(V4L2_CID_SATURATION, 0.90);
  vid.setControl(V4L2_CID_HUE       , 0.50);

  // Request mmap-able capture buffers
  mzero(req);
  req.count  = STREAMBUFS;
  req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;
  err = ioctl(vid.fd, VIDIOC_REQBUFS, &req);
  if(err < 0 || req.count != STREAMBUFS){
    printf("REQBUFS returned error %d, count %d\n",
	   errno,req.count);
    return(false);
  }

  // set up individual buffers
  mzero(img,STREAMBUFS);
  mzero(tempbuf);
  tempbuf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  tempbuf.memory = V4L2_MEMORY_MMAP;

  for(unsigned i=0; i<req.count; i++){
    tempbuf.index = i;
    err = ioctl(vid.fd, VIDIOC_QUERYBUF, &tempbuf);
    if(err < 0){
      printf("QUERYBUF returned error %d\n",errno);
      return(false);
    }

    img[i].length = tempbuf.length;
    img[i].data = (char*)mmap(NULL, tempbuf.length,
                              PROT_READ | PROT_WRITE, MAP_SHARED,
                              vid.fd, tempbuf.m.offset);

    if(img[i].data == MAP_FAILED){
      printf("mmap() returned error %d (%s)\n",errno,strerror(errno));
      return(false);
    }

    // fill out other fields
    img[i].width        = fmt.fmt.pix.width;
    img[i].height       = fmt.fmt.pix.height;
    img[i].bytesperline = fmt.fmt.pix.bytesperline;
    img[i].index = i;
  }

  for(unsigned i=0; i<req.count; i++){
    tempbuf.index = i;
    if(!vid.enqueueBuffer(tempbuf)){
      printf("Error queueing initial buffers\n");
      return(false);
    }
  }

  bool ok = vid.startStreaming();
  return(ok);
}

bool Capture::initLog(const char *logfile,int nwidth,int nheight,int nfmt)
{
  if(log_data){
    delete[](log_data);
    log_data = NULL;
  }

  FILE *in = fopen(logfile,"rb");
  if(!in) return(false);

  mzero(img,STREAMBUFS);
  img[0].data         = NULL;
  img[0].width        = nwidth;
  img[0].height       = nheight;
  img[0].bytesperline = nwidth*2;
  img[0].index = 0;
  unsigned img_size = img[0].size();

  struct stat st;
  mzero(st);
  fstat(fileno(in),&st);
  unsigned size = st.st_size;
  if(size % (img_size+sizeof(RawImageFileHdr)) == 0){
    // new format, with header
    log_bytes_per_image = img_size + sizeof(RawImageFileHdr);
    log_has_header = true;
  }else if(size % img_size){
    // old format, with no header    
    log_bytes_per_image = img_size;    
    log_has_header = false;
  }else{
    goto error;
  }

  log_idx = -1;
  log_num_images = size / log_bytes_per_image;
  log_data = new char[st.st_size];
  if(!log_data) goto error;
  mzero(log_data,size);
  fread(log_data,img_size,log_num_images,in);

  fclose(in);
  return(log_num_images > 0);
error:
  if(log_data){
    delete[](log_data);
    log_data = NULL;
  }
  if(in) fclose(in);
  return(false);
}

void Capture::close()
{
  if(vid.isOpen()){
    vid.stopStreaming();

    for(int i=0; i<STREAMBUFS; i++){
      if(img[i].data){
        munmap(img[i].data,img[i].length);
      }
    }

    vid.close();
  }

  if(log_data){
    delete[](log_data);
    log_data = NULL;
    log_idx = -1;
    log_num_images = 0;
  }

  closeWriteLog();
}

const Capture::Image *Capture::captureFrame(int step)
{
  if(log_data){
    step += log_num_images;
    log_idx = (log_idx + step) % log_num_images;
    img[0].data = &(log_data[log_bytes_per_image * log_idx]);
    if(log_has_header){
      const RawImageFileHdr &hdr = *((RawImageFileHdr*)img[0].data);
      img[0].data += sizeof(RawImageFileHdr);
      img[0].timestamp = hdr.timestamp;
      img[0].field = hdr.field;
    }
    return(&(img[0]));
  }

  if(!vid.waitForFrame(1000)){
    if(run_flag!=NULL && *run_flag==false){
      printf("stopping!\n");
      return(NULL);
    }
    printf("Capture: error waiting for frame\n");
    return(NULL);
  }

  while(true){
    // get the frame
    vid.dequeueBuffer(tempbuf);

    // poll to see if a another frame is already available
    // if so, break out now
    if(!vid.isFrameReady()) break;

    // otherwise, drop this frame
    vid.enqueueBuffer(tempbuf);
  }

  int i = tempbuf.index;
  img[i].timestamp = tempbuf.timestamp;
  img[i].field = (tempbuf.field == V4L2_FIELD_BOTTOM);

  return(&(img[i]));
}

bool Capture::releaseFrame(const Capture::Image *_img)
{
  if(log_data) return(true);
  if(!_img) return(false);
  tempbuf.index = _img->index;
  return(vid.enqueueBuffer(tempbuf));
}

bool Capture::writeFrame(const Image *image,int out_fd)
{
  if(!image || out_fd<0) return(false);

  // only YUV422 supported for now
  if(image->bytesperline != image->width*2) return(false);

  // create header
  RawImageFileHdr hdr;
  mzero(hdr);
  hdr.type      = ImageTypeRawYUV;
  hdr.field     = image->field;
  hdr.width     = image->width;
  hdr.height    = image->height;
  hdr.timestamp = image->timestamp;

  // write header and contents
  int hw = ::write(out_fd,&hdr,sizeof(hdr));
  int bw = ::write(out_fd,image->data,image->size());

  bool ok = (hw==sizeof(hdr) && bw==image->size());
  return(ok);
}

bool Capture::openWriteLog(const char *logfile,bool append)
{
  if(out_log_fd >= 0) ::close(out_log_fd);
  int a = append? O_APPEND : O_TRUNC;
  out_log_fd = ::open(logfile,O_CREAT|O_WRONLY|a,0644);
  return(out_log_fd >= 0);
}

bool Capture::closeWriteLog()
{
  bool ok = true;
  if(out_log_fd >= 0){
    ok = (::close(out_log_fd) == 0);
    out_log_fd = -1;
  }
  return(ok);
}

bool Capture::saveLog(const char *logfile,int num_frames,int skip)
{
  if(log_data) return(false);
  int out = ::open(logfile,O_CREAT|O_WRONLY|O_TRUNC,0644);
  if(out < 0) return(false);

  for(int i=0; i<num_frames; i++){
    const Image *img = NULL;

    // skip frames if needed
    for(int i=1; i<skip; i++){
      img = captureFrame();
      releaseFrame(img);
    }

    // capture a frame and write it
    img = captureFrame();
    if(img){
      writeFrame(img,out);
      releaseFrame(img);
      fputc('.',stdout);
    }else{
      fputc('x',stdout);
    }
    fflush(stdout);
  }

  return(::close(out) == 0);
}
