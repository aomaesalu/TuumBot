/**
 * @file MainWindow.hpp
 * Experimental GUI application main window.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef GUI_MAIN_WINDOW_H
#define GUI_MAIN_WINDOW_H

#include <gtkmm.h>


class MainWindow: public Gtk::Window {

  public:
    MainWindow();
    virtual ~MainWindow();

  protected:
    Gtk::Grid grid;

  private:
    void setProperties();

};

#endif // GUI_MAIN_WINDOW_H
