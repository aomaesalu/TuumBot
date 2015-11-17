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
       if (Vision::isColored(*(gui->getFrame()), Visioning::filter, actualPixel[0], actualPixel[1], actualPixel[2], 0)) {
         pixel[0] *= 0.2;
         pixel[1] *= 0.2;
         pixel[2] *= 0.2;
       }
     }
   }

   // Draw rectangles
   for (Vision::BlobSet::iterator blob = Vision::blobs.begin(); blob != Vision::blobs.end(); ++blob) {
     if (*blob) {
       unsigned int x = (*blob)->getPosition()->getX();
       unsigned int y = (*blob)->getPosition()->getY();
       unsigned int minX = (*blob)->getMinX();
       unsigned int maxX = (*blob)->getMaxX();
       unsigned int minY = (*blob)->getMinY();
       unsigned int maxY = (*blob)->getMaxY();
       if (minX >= CAMERA_WIDTH || maxX >= CAMERA_WIDTH || minY >= CAMERA_HEIGHT || maxY >= CAMERA_HEIGHT) {
         continue;
       }
       //std::cout << (*blob)->getPosition()->getX() << " " << (*blob)->getPosition()->getY() << " " << minX << " " << maxX << " " << minY << " " << maxY << std::endl;
       Color color = (*blob)->getColor();

       unsigned int value = 0;
       for (unsigned int i = minX; i <= maxX; ++i) {
         guint8 *pixel = pixels + i * channels + minY * stride;
         for (unsigned int p = 0; p < 3; ++p) {
           pixel[p] = value;
         }
         pixel = pixels + i * channels + maxY * stride;
         for (unsigned int p = 0; p < 3; ++p) {
           pixel[p] = value;
         }
       }
       for (unsigned int j = minY; j <= maxY; ++j) {
         guint8 *pixel = pixels + minX * channels + j * stride;
         for (unsigned int p = 0; p < 3; ++p) {
           pixel[p] = value;
         }
         pixel = pixels + maxX * channels + j * stride;
         for (unsigned int p = 0; p < 3; ++p) {
           pixel[p] = value;
         }
       }

       for (int dx = -3; dx < 3; ++dx) {
         for (int dy = -3; dy < 3; ++dy) {
           if (x + dx < CAMERA_WIDTH && x + dx >= 0 && y + dy < CAMERA_HEIGHT && y + dy >= 0) {
             guint8 *pixel = pixels + (x + dx) * channels + (y + dy) * stride;
             pixel[0] = 255;
             pixel[1] = 0;
             pixel[2] = 0;
           }
         }
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
