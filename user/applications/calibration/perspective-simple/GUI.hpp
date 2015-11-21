/**
 *  @file GUI.hpp
 *  Perspective calibration application GUI interface class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 21 November 2015
 */

#ifndef RTX_APPLICATIONS_CALIBRATION_PERSPECTIVE_GUI_H
#define RTX_APPLICATIONS_CALIBRATION_PERSPECTIVE_GUI_H

#include <vector>
#include <string>

#include "rtxhal.hpp"
#include "rtxvision.h"

#include "MainWindow.hpp"
#include "Checkerboard.hpp"


namespace rtx {

  class GUI {

    public:
      GUI(int&, char**, Camera*);
      virtual ~GUI();

      bool isPlaying() const;

      Glib::RefPtr<Gtk::Application> getGtkApplication() const;
      MainWindow* getWindow() const;

      Camera* getCamera() const;
      Frame* getFrame();

      Glib::RefPtr<Gdk::Pixbuf> getImage() const;

      Checkerboard* getCheckerboard() const;

      void setPlaying(const bool& = true);

      int run();
      bool updateFrame();

    private:
      int argc;
      char **argv;

      bool playing;

      Glib::RefPtr<Gtk::Application> gtkApplication;
      MainWindow *window;

      Camera *camera;
      Frame frame;
      Frame rgbFrame;

      Glib::RefPtr<Gdk::Pixbuf> image;

      Checkerboard *checkerboard;

      void initialiseImage();

  };

}

#endif // RTX_APPLICATIONS_CALIBRATION_PERSPECTIVE_GUI_H
