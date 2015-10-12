/**
  Tuum
  Robotex 2015
  University of Tartu
  Team:
    Kristjan Kanarbik
    Meelik Kiik
    Ants-Oskar Mäesalu
    Mart Müllerbeck
    Kaur Viiklaid
    Indrek Virro
    Mihkel Väljaots

  Camera class
    Class for communication with the robot's cameras, based on the V4L2 API.
    The video capture example provided with the V4L2 API has been used as a
    model for the class.

  Contributors:
    Ants-Oskar Mäesalu

  Copyright (c) 2015 Ants-Oskar Mäesalu
*/

#include "camera.h"             // The class header

#include <fcntl.h>              // File control operations (open)
#include <sys/stat.h>           // File characteristics header (stat)
#include <sys/ioctl.h>          // I/O control device header (ioctl)
#include <sys/mman.h>           // Memory management declarations (mmap)
#include <unistd.h>             // OS API header (close)
#include <cstring>              // C string header (memset, strerror)
#include <stdexcept>            // Exception header (runtime_error)
#include <linux/videodev2.h>    // V4L2 header

// Macro to set the memory of a variable to zero
#define CLEAR(x) memset(&(x), 0, sizeof(x))

/**
  Handles ioctl function calls. Calls the ioctl multiple times until the ioctl
  function returns an appropriate response.
  If the ioctl function returns -1, either it failed because of a reasonable
  argument, or it failed because of some blocking function. In this piece of
  software, only the first option is considered to be an appropriate response
  upon failure.
  The first argument must be an open file descriptor.
  The second argument is a device-dependent request code.
  The third argument is an untyped pointer to memory.
  Usually, on success zero is returned.  A few ioctl requests use the return
  value as an output parameter and return a nonnegative value on success. On
  error, -1 is returned, and errno is set appropriately.
*/
static int xioctl(int fileDescriptor, unsigned long int request, void *arg) {
  int result;
  do {
    // The ioctl function manipulates the underlying device parameters of
    // special files. Many operating characteristics of character special files
    // (in this case, the video device file) may be controlled with ioctl
    // requests.  The file descriptor used as the first argument of the function
    // must be an open file descriptor.
    result = ioctl(fileDescriptor, request, arg);
  } while (result == -1 && errno == EINTR);
  return result;
}

Camera::Camera(const std::string &device, const int &width, const int &height):
device{device},
width{width},
height{height}
{
  fileDescriptor = -1;
  openDevice();
  initialiseDevice();
  startCapturing();
}

Camera::~Camera() {
  stopCapturing();
  uninitialiseDevice();
  closeDevice();
}

std::string Camera::getDevice() const {
  return device;
}

int Camera::getWidth() const {
  return width;
}

int Camera::getHeight() const {
  return height;
}

void Camera::openDevice() {
  // File status structure
  struct stat status;

  // Get device file status.
  // Upon successful completion, 0 shall be returned. Otherwise, -1 shall be
  // returned and errno set to indicate the error.
  if (stat(device.c_str(), &status) == -1)
    throw std::runtime_error(device + ": cannot identify! " +
                             std::to_string(errno) + ": " +
                             std::strerror(errno));

  // Test if the device file is a character special file. The test returns a
  // non-zero value if the test is true; 0 if the test is false.
  if (!S_ISCHR(status.st_mode))
    throw std::runtime_error(device + " is no device");

  // Try to open the device.
  // Upon successful completion, the open function shall open the file and
  // return a non-negative integer representing the lowest numbered unused file
  // descriptor. Otherwise, -1 shall be returned and errno set to indicate the
  // error. No files shall be created or modified if the function returns -1.
  // O_RDWR sets the file to be open for reading and writing.
  // O_NONBLOCK sets the function to return without blocking for the device to
  // be ready or available.
  if ((fileDescriptor = open(device.c_str(), O_RDWR | O_NONBLOCK, 0)) == -1)
    throw std::runtime_error(device + ": cannot open! " + std::to_string(errno)
                             + ": " + std::strerror(errno));
}

void Camera::closeDevice() {
  // Try to close the device, based on its file descriptor
  // The close function returns -1 if the file descriptor is not a valid open
  // device file descriptor of the device.
  if (close(fileDescriptor) == -1)
    throw std::runtime_error("Error upon closing device");

  // Reset the file descriptor to ensure it doesn't refer to any device anymore.
  fileDescriptor = -1;
}

void Camera::initialiseDevice() {
  // Check the V4L2 capabilities of the video device. Throws runtime_error with
  // an appropriate error message if the device is not suitable for usage in our
  // application.
  checkV4L2Capabilities();

  // TODO: Select video input, video standard and tune here.

  // Check the cropping capabilities of the video device. Currently does not
  // throw any kind of exceptions.
  // TODO: Decide on what to do with this functionality.
  checkCroppingCapabilites();

  // Initialise the appropriate video format for the camera device.
  initialiseFormat();

  // TODO
  initialiseBuffer();
}

void Camera::uninitialiseDevice() {
  unsigned int i;

  for (i = 0; i < numberOfBuffers; ++i)
    if (munmap(buffers[i].data, buffers[i].size) == -1)
      throw std::runtime_error("munmap");

  free(buffers);
}

void Camera::startCapturing() {
  unsigned int i;
  enum v4l2_buf_type type;

  for (i = 0; i < numberOfBuffers; ++i) {
    struct v4l2_buffer buf;

    CLEAR(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = i;

    if (xioctl(fileDescriptor, VIDIOC_QBUF, &buf) == -1)
      throw std::runtime_error("VIDIOC_QBUF");
  }
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (xioctl(fileDescriptor, VIDIOC_STREAMON, &type) == -1)
    throw std::runtime_error("VIDIOC_STREAMON");
}

void Camera::stopCapturing() {
  enum v4l2_buf_type type;

  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (xioctl(fileDescriptor, VIDIOC_STREAMOFF, &type) == -1)
    throw std::runtime_error("VIDIOC_STREAMOFF");
}

void Camera::checkV4L2Capabilities() {
  // V4L2 capability structure
  struct v4l2_capability capabilities;

  // Query device capabilities. If the video device driver is not compatible
  // with V4L2 specification, (x)ioctl returns an EINVAL error code.
  if (xioctl(fileDescriptor, VIDIOC_QUERYCAP, &capabilities) == -1) {
    if (errno == EINVAL)
      throw std::runtime_error(device + " is not a V4L2 device");
    else
      throw std::runtime_error("VIDIOC_QUERYCAP");
  }

  // Check if the device supports the Video Capture interface.
  // Video capture devices sample an analog video signal and store the digitized
  // images in memory. Today, nearly all devices can capture at full 25 or 30
  // frames/second. With this interface, applications can control the capture
  // process and move images from the driver into user space.
  if (!(capabilities.capabilities & V4L2_CAP_VIDEO_CAPTURE))
    throw std::runtime_error(device + " is not a video capture device");

  // Check if the device supports the streaming I/O method.
  // Streaming is an I/O method where only pointers to buffers are exchanged
  // between application and driver, the data itself is not copied.
  if (!(capabilities.capabilities & V4L2_CAP_STREAMING))
    throw std::runtime_error(device + " does not support streaming I/O");
}

/**
  TODO: This method isn't necessarily needed upon camera device initialisation.
  Decide on what to do with it, or whether to improve it.
*/
void Camera::checkCroppingCapabilites() {
  // V4L2 cropping capability structure
  struct v4l2_cropcap croppingCapabilities;

  // V4L2 cropping rectangle structure
  struct v4l2_crop croppingRectangle;

  CLEAR(croppingCapabilities);

  // Specify the cropping capability type to be specific for video captures.
  // Other types possible include video outputs, overlays, and custom types.
  croppingCapabilities.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  // Query device cropping capabilities. On success, 0 is returned; on error -1
  // is returned and the errno variable is set appropriately to EINVAL.
  if (xioctl(fileDescriptor, VIDIOC_CROPCAP, &croppingCapabilities) == 0) {
    // Specify the cropping type to be specific for video captures. Other types
    // possible include video outputs, overlays, and custom types.
    croppingRectangle.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    // Reset the cropping rectangle to default
    croppingRectangle.c = croppingCapabilities.defrect;

    // Try to crop the video capture. On success, 0 is returned; on error, -1
    // is returned and the errno variable is set appropriately to EINVAL.
    if (xioctl(fileDescriptor, VIDIOC_S_CROP, &croppingRectangle) == -1) {
      if (errno == EINVAL) {
        // Cropping is not supported. Ignore error (for now?).
      } else {
        // Errors ignored.
      }
    }
  } else {
    // Errors ignored.
  }
}

void Camera::initialiseFormat() {
  // V4l2 data format structure
  struct v4l2_format format;

  CLEAR(format);

  // Assign a format suitable for our application to the camera device.
  // TODO: Check data integrity

  // Type of the data stream - buffer of a video capture stream.
  format.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  // Image width in pixels.
  format.fmt.pix.width       = width;
  // Image height in pixels.
  format.fmt.pix.height      = height;
  // The pixel format or type of compression, set by the application.
  // YUYV is a packed format with 1/2 horizontal chroma resolution, also known
  // as YUV 4:2:2. In this format each four bytes is two pixels. Each four bytes
  // is two Y's, a Cb and a Cr. Each Y goes to one of the pixels, and the Cb and
  // Cr belong to both pixels.
  format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
  // Video images are typically interlaced. Applications can request to capture
  // or output only the top or bottom field, or both fields interlaced or
  // sequentially stored in one buffer or alternating in separate buffers.
  // Drivers return the actual field order selected.
  // TODO: Deinterlace video to remove motion artifacts from data.
  format.fmt.pix.field       = V4L2_FIELD_INTERLACED;

  // Try to set the format according to our specifications. On success, 0 is
  // returned; on error -1 and the errno variable is set appropriately.
  if (xioctl(fileDescriptor, VIDIOC_S_FMT, &format) == -1)
    throw std::runtime_error("VIDIOC_S_FMT");

  // VIDIOC_S_FMT may change resolution width and height.
  width = format.fmt.pix.width;
  height = format.fmt.pix.height;

  // TODO: Add stride?
}

void Camera::initialiseBuffer() {
  // V4L2 buffer request structure
  struct v4l2_requestbuffers request;

  CLEAR(request);

  request.count = 4;
  request.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  request.memory = V4L2_MEMORY_MMAP;

  if (xioctl(fileDescriptor, VIDIOC_REQBUFS, &request) == -1) {
    if (errno == EINVAL) {
      throw std::runtime_error(device + " does not support memory mapping");
    } else {
      throw std::runtime_error("VIDIOC_REQBUFS");
    }
  }

  if (request.count < 2) {
    throw std::runtime_error(std::string("Insufficient buffer memory on ") + device);
  }

  buffers = (buffer*) calloc(request.count, sizeof(*buffers));

  if (!buffers) {
    throw std::runtime_error("Out of memory");
  }

  for (numberOfBuffers = 0; numberOfBuffers < request.count; ++numberOfBuffers) {
    struct v4l2_buffer buffer;

    CLEAR(buffer);

    buffer.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buffer.memory      = V4L2_MEMORY_MMAP;
    buffer.index       = numberOfBuffers;

    if (xioctl(fileDescriptor, VIDIOC_QUERYBUF, &request) == -1)
      throw std::runtime_error("VIDIOC_QUERYBUF");

    buffers[numberOfBuffers].size = buffer.length;
    buffers[numberOfBuffers].data =
      mmap(NULL /* start anywhere */,
           buffer.length,
           PROT_READ | PROT_WRITE /* required */,
           MAP_SHARED /* recommended */,
           fileDescriptor, buffer.m.offset);

    if (buffers[numberOfBuffers].data == MAP_FAILED)
      throw std::runtime_error("mmap");
  }
}
