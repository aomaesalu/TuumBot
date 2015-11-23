/**
 *  @file MainWindow.hpp
 *  Perspective calibration application GUI main window.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 24 November 2015
 */

#ifndef RTX_APPLICATIONS_CALIBRATION_PERSPECTIVE_MAIN_WINDOW_H
#define RTX_APPLICATIONS_CALIBRATION_PERSPECTIVE_MAIN_WINDOW_H

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

      Gtk::Scale *getAScale();
      Gtk::Scale *getBScale();
      Gtk::Scale *getCScale();

    protected:
      Gtk::Grid grid;

      // Frames and boxes for the layout
      Gtk::Frame imageAreaFrame;
      Gtk::Box scalesBox;

      // Constant scales
      Gtk::Scale AScale;
      Gtk::Label ALabel;
      Gtk::Scale BScale;
      Gtk::Label BLabel;
      Gtk::Scale CScale;
      Gtk::Label CLabel;

      // Image drawing areas and contexts
      ImageArea imageArea;

    private:
      GUI *gui;

      void setProperties();

      void construct();
      void constructGrid();
      void constructImageAreaFrame();
      void constructScalesBox();
      void constructScale(Gtk::Container&, Gtk::Scale&, Gtk::Label&, const std::string&);

  };

}

#endif // RTX_APPLICATIONS_CALIBRATION_PERSPECTIVE_MAIN_WINDOW_H
