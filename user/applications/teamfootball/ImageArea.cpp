/**
 *  @file ImageArea.cpp
 *  Team football application GUI preview area.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 18 November 2015
 */

#include "ImageArea.hpp"

#include "cameraConstants.hpp"
#include "GUI.hpp"

#include <cairomm/context.h>
#include <gdkmm/general.h>
#include <gdkmm/pixbuf.h>
#include <glibmm/refptr.h>
#include <glibmm/fileutils.h>

#include <iostream> // TODO: Remove


namespace rtx {

 ImageArea::ImageArea(GUI *gui) {
   // Attach GUI interface information
   this->gui = gui;
 }

 ImageArea::~ImageArea() {
   // TODO
 }

 void ImageArea::initialise() {
   // Initialise area size
   set_size_request(gui->getImage()->get_width(), gui->getImage()->get_height());
 }

 bool ImageArea::applyFilter() {
   filteredImage = gui->getImage()->copy(); // TODO: Copy only where is necessary (?)

   guint8 *pixels = filteredImage->get_pixels();
   unsigned int channels = filteredImage->get_n_channels();
   unsigned int stride = filteredImage->get_rowstride();

   guint8 *actualPixels = gui->getFrame()->data;
   unsigned int actualChannels = 3;
   unsigned int actualStride = gui->getFrame()->width * actualChannels;

   // Color pixels
   for (unsigned int x = 0; x < CAMERA_WIDTH; ++x) {
     for (unsigned int y = 0; y < CAMERA_HEIGHT; ++y) {
       guint8 *pixel = pixels + x * channels + y * stride;
       guint8 *actualPixel = actualPixels + x * actualChannels + y * actualStride;
       if (gui->isColored(0, actualPixel[0], actualPixel[1], actualPixel[2])) {
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
