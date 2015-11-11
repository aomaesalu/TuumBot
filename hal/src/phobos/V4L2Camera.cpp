/**
 * @file V4L2Camera.hpp
 * Camera class
 * Class for communication with the robot's cameras, based on the V4L2 API.
 * The video capture example provided with the V4L2 API has been used as a
 * model for the class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include <fcntl.h>              // File control operations (open)
#include <sys/stat.h>           // File characteristics header (stat)
#include <sys/ioctl.h>          // I/O control device header (ioctl)
#include <sys/mman.h>           // Memory management declarations (mmap)
#include <unistd.h>             // OS API header (close)
#include <cstring>              // C string header (memset, strerror)
#include <stdexcept>            // Exception header (runtime_error)
#include <linux/videodev2.h>    // V4L2 header

#include <iostream> // TODO: Remove

#include "V4L2Camera.hpp"       // The class header


namespace rtx {

  // Macro to set the memory of a variable to zero
  #define CLEAR(x) memset(&(x), 0, sizeof(x))

  #define CLIP(color) (unsigned char)(((color) > 0xFF) ? 0xff : (((color) < 0) ? 0 : (color)))

  static void convertYCbCrtoRGB(const unsigned char *src, unsigned char *dest,
                                 int width, int height)
  {
    int j;
    while (--height >= 0) {
      for (j = 0; j < width; ++j) {
        // Y:  src[0]
        // Cb: src[1]
        // Cr: src[2]
        // Red
        *dest++ = CLIP(1.164 * (src[0] - 16) + 1.596 * (src[2] - 128));
        // Green
        *dest++ = CLIP(1.164 * (src[0] - 16) - 0.813 * (src[2] - 128) - 0.391 * (src[1] - 128));
        // Blue
        *dest++ = CLIP(1.164 * (src[0] - 16) + 2.018 * (src[1] - 128));
        src += 3;
      }
    }
  }

  Frame toRGB(const Frame &frame) {
    Frame rgbFrame;
    rgbFrame.data = (unsigned char *) malloc(frame.size * sizeof(char));
    std::copy(frame.data, frame.data + frame.size, rgbFrame.data);
    rgbFrame.width = frame.width;
    rgbFrame.height = frame.height;
    rgbFrame.size = frame.size;
    convertYCbCrtoRGB((unsigned char *) frame.data,
                             rgbFrame.data,
                             rgbFrame.width,
                             rgbFrame.height);
    return rgbFrame;
  }

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

  static void formatFrame(const unsigned char *source, unsigned char *destination,
                          int width, int height, int stride) {
    while (--height >= 0) {
      for (int i = 0; i < width - 1; i += 2) {
        for (int j = 0; j < 2; ++j) {
          *destination++ = source[j * 2];
          *destination++ = source[1];
          *destination++ = source[3];
        }
        source += 4;
      }
      source += stride - width * 2;
    }
  }

  Camera::Camera(const std::string &device, const int &width, const int &height):
    device(device),
    width(width),
    height(height)
  {
    fileDescriptor = -1;
    openDevice();
    initialiseDevice();
    initialiseFrame();
    startCapturing();
  }

  Camera::~Camera() {
    stopCapturing();
    uninitialiseDevice();
    closeDevice();
    free(frame.data);
  }

  std::string Camera::getDevice() const {
    return device;
  }

  size_t Camera::getWidth() const {
    return width;
  }

  size_t Camera::getHeight() const {
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

    // TODO: Crop here (if necessary)

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

  void Camera::initialiseFrame() {
    frame.width = width;
    frame.height = height;
    frame.size = width * height * 3;
    frame.data = (unsigned char *) malloc(frame.size * sizeof(char));
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

    stride = format.fmt.pix.bytesperline;
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

      if (xioctl(fileDescriptor, VIDIOC_QUERYBUF, &buffer) == -1)
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

  bool Camera::readFrame()
  {
    struct v4l2_buffer buffer;

    CLEAR(buffer);

    buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buffer.memory = V4L2_MEMORY_MMAP;

    if (xioctl(fileDescriptor, VIDIOC_DQBUF, &buffer) == -1) {
      switch (errno) {
        case EAGAIN:
          return -1;

        case EIO:
          /* Could ignore EIO, see spec. */

          /* fall through */

        default:
          throw std::runtime_error("VIDIOC_DQBUF");
      }
    }

    //assert(buffer.index < numberOfBuffers);

    // TODO: Process image

    if (xioctl(fileDescriptor, VIDIOC_QBUF, &buffer) == -1)
        throw std::runtime_error("VIDIOC_QBUF");

    return buffer.index;
  }

  const Frame& Camera::getFrame(unsigned int timeout) {
    while (true) {
      fd_set fileDescriptors;
      struct timeval timeValue;

      FD_ZERO(&fileDescriptors);
      FD_SET(fileDescriptor, &fileDescriptors);

      /* Timeout. */
      timeValue.tv_sec = timeout;
      timeValue.tv_usec = 0;

      int returnValue = select(fileDescriptor + 1, &fileDescriptors, NULL, NULL,
                               &timeValue);

      if (returnValue == -1) {
        if (errno == EINTR)
          continue;
        throw std::runtime_error("select");
      }

      if (returnValue == 0) {
        throw std::runtime_error(device + ": select timeout");
      }

      int index = readFrame();
      if (index != -1) {
        formatFrame((unsigned char *) buffers[index].data, frame.data, width,
                    height, stride);
        return frame;
      }
      /* EAGAIN - continue select loop. */
    }
  }

};
