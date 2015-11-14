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

#include "Mask.hpp"
#include "Filter.hpp"


namespace rtx {

  class Application {

    public:
      Application(int, char*, Camera*, Camera*);
      virtual ~Application();

      Glib::RefPtr<Gtk::Application> getGtkApplication() const;
      MainWindow* getWindow() const;

      Camera* getFrontCamera() const;
      Camera* getBackCamera() const;

      std::vector<std::string> getModes() const;
      unsigned int getMode() const;

      bool isPlaying() const;
      bool isMasking() const;

      void setPlaying(const bool& = true);
      void setMasking(const bool& = true);

      void readFilterFromFile(const std::string&);
      void saveFilterToFile(const std::string&);

      int run();

    private:
      Glib::RefPtr<Gtk::Application> gtkApplication;
      MainWindow *window;

      Camera *frontCamera;
      Camera *backCamera;

      bool playing;
      bool masking;

      std::vector<std::string> modes;
      unsigned int mode;

      Mask mask;
      Filter filter;

  };

}

#endif // RTX_APPLICATIONS_CALIBRATION_COLOR_APPLICATION_H
