/**
 *  @file GUI.hpp
 *  Team football application GUI interface class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 18 November 2015
 */

#ifndef RTX_APPLICATIONS_TEAMFOOTBALL_GUI_H
#define RTX_APPLICATIONS_TEAMFOOTBALL_GUI_H

#include <vector>
#include <string>

#include "rtxhal.hpp"

#include "MainWindow.hpp"


namespace rtx {

  class GUI {

    public:
      GUI(int&, char**, Camera*);
      virtual ~GUI();

      bool isColored(const unsigned int&, const unsigned int&) const;
      bool isColored(const unsigned int&, const unsigned int&, const unsigned int&, const unsigned int&) const;

      Glib::RefPtr<Gtk::Application> getGtkApplication() const;
      MainWindow* getWindow() const;

      Camera* getCamera() const;
      Frame* getFrame();

      Glib::RefPtr<Gdk::Pixbuf> getImage() const;

      int run();
      bool updateFrame();

    private:
      int argc;
      char **argv;

      Glib::RefPtr<Gtk::Application> gtkApplication;
      MainWindow *window;

      Camera *camera;
      Frame frame;
      Frame rgbFrame;

      Glib::RefPtr<Gdk::Pixbuf> image;

      void initialiseImage();

  };

}

#endif // RTX_APPLICATIONS_TEAMFOOTBALL_GUI_H
