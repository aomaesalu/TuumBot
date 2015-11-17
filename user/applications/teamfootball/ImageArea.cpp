/**
 *  @file ImageArea.cpp
 *  Team football application GUI preview area.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 18 November 2015
 */

 /**
  *  @file PreviewArea.cpp
  *  Color calibration application preview area.
  *
  *  @authors Ants-Oskar Mäesalu
  *  @version 0.1
  *  @date 17 November 2015
  */

#include "ImageArea.hpp"

#include "cameraConstants.hpp"
#include "Interface.hpp"

#include <cairomm/context.h>
#include <gdkmm/general.h>
#include <gdkmm/pixbuf.h>
#include <glibmm/refptr.h>
#include <glibmm/fileutils.h>

#include <iostream> // TODO: Remove


namespace rtx {

 ImageArea::ImageArea(Interface *interface) {
   // Set properties
   this->interface = interface;
 }

 ImageArea::~ImageArea() {
   // TODO
 }

 void ImageArea::initialise() {
   // Initialise area size
   set_size_request(application->getImage()->get_width(), application->getImage()->get_height());
 }

 bool ImageArea::applyFilter() {
   filteredImage = interface->getImage()->copy(); // TODO: Copy only where is necessary (?)

   Filter *filter = interface->getFilter();

   guint8 *pixels = filteredImage->get_pixels();
   unsigned int channels = filteredImage->get_n_channels();
   unsigned int stride = filteredImage->get_rowstride();

   guint8 *actualPixels = interface->getFrame()->data;
   unsigned int actualChannels = 3;
   unsigned int actualStride = interface->getFrame()->width * actualChannels;

   // Color pixels
   for (unsigned int x = 0; x < CAMERA_WIDTH; ++x) {
     for (unsigned int y = 0; y < CAMERA_HEIGHT; ++y) {
       guint8 *pixel = pixels + x * channels + y * stride;
       guint8 *actualPixel = actualPixels + x * actualChannels + y * actualStride;
       if (filter->has(mode, actualPixel[0], actualPixel[1], actualPixel[2])) {
         pixel[0] *= 0.2;
         pixel[1] *= 0.2;
         pixel[2] *= 0.2;
       }
     }
   }

   return true;
 }

 bool ImageArea::drawImage(const Cairo::RefPtr<Cairo::Context> &cairo) {
   if (!filteredImage)
     return false;

   Gdk::Cairo::set_source_pixbuf(cairo, filteredImage, 0, 0);

   return true;
 }

 bool ImageArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cairo) {
   if (!applyFilter())
     return false;

   if (!drawImage(cairo))
     return false;

   cairo->paint();

   return true;
 }

}
