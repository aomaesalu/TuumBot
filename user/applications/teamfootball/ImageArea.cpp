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
   std::cout << "B1" << std::endl;
   filteredImage = gui->getImage()->copy(); // TODO: Copy only where is necessary (?)
   std::cout << "B2" << std::endl;

   guint8 *pixels = filteredImage->get_pixels();
   std::cout << "B3" << std::endl;
   unsigned int channels = filteredImage->get_n_channels();
   std::cout << "B4" << std::endl;
   unsigned int stride = filteredImage->get_rowstride();
   std::cout << "B5" << std::endl;

   guint8 *actualPixels = gui->getFrame()->data;
   std::cout << "B6" << std::endl;
   unsigned int actualChannels = 3;
   std::cout << "B7" << std::endl;
   unsigned int actualStride = gui->getFrame()->width * actualChannels;
   std::cout << "B8" << std::endl;

   // Color pixels
   for (unsigned int x = 0; x < CAMERA_WIDTH; ++x) {
     for (unsigned int y = 0; y < CAMERA_HEIGHT; ++y) {
       guint8 *pixel = pixels + x * channels + y * stride;
       guint8 *actualPixel = actualPixels + x * actualChannels + y * actualStride;
       if (Vision::isColored(*(gui->getFrame()), Visioning::filter, actualPixel[0], actualPixel[1], actualPixel[2], 0)) {
         pixel[0] *= 0.2;
         pixel[1] *= 0.2;
         pixel[2] *= 0.2;
       }
     }
   }
   std::cout << "B9" << std::endl;

   return true;
 }

 bool ImageArea::drawImage(const Cairo::RefPtr<Cairo::Context> &cairo) {
   if (!filteredImage)
     return false;

   Gdk::Cairo::set_source_pixbuf(cairo, filteredImage, 0, 0);

   return true;
 }

 bool ImageArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cairo) {
   std::cout << "A1" << std::endl;
   if (!applyFilter())
     return false;
   std::cout << "A2" << std::endl;

   if (!drawImage(cairo))
     return false;
   std::cout << "A3" << std::endl;

   cairo->paint();
   std::cout << "A4" << std::endl;

   return true;
 }

}
