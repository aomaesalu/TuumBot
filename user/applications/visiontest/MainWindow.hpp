/**
 *  @file MainWindow.hpp
 *  Vision testing application GUI main window.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 21 November 2015
 */

#ifndef RTX_APPLICATIONS_VISION_TEST_MAIN_WINDOW_H
#define RTX_APPLICATIONS_VISION_TEST_MAIN_WINDOW_H

#include <vector>
#include <set>
#include <string>

#include <gtkmm.h>
#include <gtkmm/window.h>

#include "ImageArea.hpp"


namespace rtx {

  class GUI;

  class MainWindow: public Gtk::Window {

    public:
      MainWindow(GUI*);
      virtual ~MainWindow();

      void initialise();

      ImageArea* getImageArea();

    protected:
      Gtk::Grid grid;

      // Frames and boxes for the layout
      Gtk::Frame imageAreaFrame;

      // Image drawing areas and contexts
      ImageArea imageArea;

    private:
      GUI *gui;

      void setProperties();

      void construct();
      void constructGrid();
      void constructImageAreaFrame();

  };

}

#endif // RTX_APPLICATIONS_VISION_TEST_MAIN_WINDOW_H
