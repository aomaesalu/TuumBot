/**
 *  @file MainWindow.hpp
 *  Team football application GUI main window.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 18 November 2015
 */

#ifndef RTX_APPLICATIONS_TEAMFOOTBALL_MAIN_WINDOW_H
#define RTX_APPLICATIONS_TEAMFOOTBALL_MAIN_WINDOW_H

#include <vector>
#include <set>
#include <string>

#include <gtkmm.h>
#include <gtkmm/window.h>

#include "ImageArea.hpp"


namespace rtx {

  class Interace;

  class MainWindow: public Gtk::Window {

    public:
      MainWindow(Interface*);
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
      Interface *interface;

      void setProperties();

      void construct();
      void constructGrid();
      void constructImageAreaFrame();

  };

}

#endif // RTX_APPLICATIONS_TEAMFOOTBALL_MAIN_WINDOW_H
