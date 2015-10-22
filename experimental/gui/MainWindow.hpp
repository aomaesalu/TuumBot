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

    // Frames and boxes for the layout
    Gtk::Box drawingButtonsBox;
    Gtk::Box generalButtonsBox;
    Gtk::Frame imageBeforeFrame;
    Gtk::Frame imageAfterFrame;
    Gtk::Box imageBeforeOptionsBox;
    Gtk::Box imageAfterOptionsBox;

    // Drawing buttons
    Gtk::ComboBox entityChooseButton;
    Gtk::ComboBox brushSizeButton;

    // General buttons
    Gtk::ComboBox fileChooseButton;
    Gtk::Button saveButton;
    Gtk::Button exitButton;

    // Options buttons
    Gtk::CheckButton displayMaskBeforeButton;
    Gtk::CheckButton displayMaskAfterButton;

  private:
    void setProperties();

    void construct();
    void constructGrid();

    void constructDrawingButtonsBox();
    void constructGeneralButtonsBox();
    void constructImageBeforeFrame();
    void constructImageAfterFrame();
    void constructImageBeforeOptionsBox();
    void constructImageAfterOptionsBox();

    void constructEntityChooseButton(Gtk::Container&);
    void constructBrushSizeButton(Gtk::Container&);
    void constructFileChooseButton(Gtk::Container&);
    void constructSaveButton(Gtk::Container&);
    void constructExitButton(Gtk::Container&);

};

#endif // GUI_MAIN_WINDOW_H
