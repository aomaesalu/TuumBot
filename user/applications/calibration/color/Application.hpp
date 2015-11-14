/**
 *  @file Application.hpp
 *  Color calibration application class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 14 November 2015
 */

#ifndef RTX_APPLICATIONS_CALIBRATION_COLOR_APPLICATION_H
#define RTX_APPLICATIONS_CALIBRATION_COLOR_APPLICATION_H

#include <vector>
#include <string>

#include "MainWindow.hpp"
#include "Mask.hpp"
#include "Filter.hpp"


namespace rtx {

  class Application {

    public:
      Application(int&, char**, Camera*);
      virtual ~Application();

      Glib::RefPtr<Gtk::Application> getGtkApplication() const;
      MainWindow* getWindow() const;

      Camera* getCamera() const;
      Frame* getFrame() const;

      Glib::RefPtr<Gdk::Pixbuf> getImage() const;

      bool isPlaying() const;
      bool isMasking() const;

      std::vector<std::string> getModes() const;
      unsigned int getMode() const;

      Mask* getMask() const;
      Filter* getFilter() const;

      void setPlaying(const bool& = true);
      void setMasking(const bool& = true);

      void setMode(const unsigned int&);

      void sendMasksToFilter();

      void readFilterFromFile(const std::string&);
      void saveFilterToFile(const std::string&);

      int run();
      bool updateFrame();

    private:
      Glib::RefPtr<Gtk::Application> gtkApplication;
      MainWindow *window;

      Camera *camera;
      Frame frame;
      Frame rgbFrame;

      Glib::RefPtr<Gdk::Pixbuf> image;

      bool playing;
      bool masking;

      std::vector<std::string> modes;
      unsigned int mode;

      Mask mask;
      Filter filter;

      void initialiseImage();

  };

}

#endif // RTX_APPLICATIONS_CALIBRATION_COLOR_APPLICATION_H
