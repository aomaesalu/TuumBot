/**
 * @file MainWindow.hpp
 * Experimental GUI application main window.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef GUI_MAIN_WINDOW_H
#define GUI_MAIN_WINDOW_H

#include <set>

#include <gtkmm.h>
#include <gtkmm/window.h>

#include "ImageBeforeDrawingArea.hpp"
#include "ImageAfterDrawingArea.hpp"


class MainWindow: public Gtk::Window {

  public:
    MainWindow();
    virtual ~MainWindow();

    bool isPlaying() const;
    bool isMasking() const;

    void setPlaying(const bool& = true);
    void setMasking(const bool& = true);

    void sendToFilter(const std::set<unsigned int>&, const std::set<unsigned int>&);

  protected:
    Gtk::Grid grid;

    // Frames and boxes for the layout
    Gtk::Box drawingButtonsBox;
    Gtk::Box generalButtonsBox;
    Gtk::Frame imageBeforeFrame;
    Gtk::Frame imageAfterFrame;
    Gtk::Box imageBeforeOptionsBox;
    Gtk::Box imageAfterOptionsBox;

    // Drawing buttons and labels
    Gtk::Label modeChooseLabel;
    Gtk::ComboBoxText modeChooseComboBox;
    Gtk::Label brushSizeLabel;
    Gtk::Scale brushSizeScale;
    Gtk::Label deltaChooseLabel;
    Gtk::Scale deltaChooseScale;

    // Video control buttons
    Gtk::Button playButton;
    Gtk::Button stopButton;

    // General buttons
    Gtk::ComboBoxText fileChooseComboBox;
    Gtk::Button saveButton;
    Gtk::Button exitButton;

    // Options buttons
    Gtk::CheckButton displayMaskBeforeButton;
    Gtk::CheckButton displayMaskAfterButton;

    // Image drawing areas and contexts
    ImageBeforeDrawingArea imageBeforeArea;
    ImageAfterDrawingArea imageAfterArea;

    // Images
    Glib::RefPtr<Gdk::Pixbuf> imageBefore;
    Glib::RefPtr<Gdk::Pixbuf> imageAfter;

  private:
    bool playing;
    bool masking;

    void setProperties();

    void construct();
    void constructGrid();

    void constructDrawingButtonsBox();
    void constructGeneralButtonsBox();
    void constructImageBeforeFrame();
    void constructImageAfterFrame();
    void constructImageBeforeOptionsBox();
    void constructImageAfterOptionsBox();

    void constructModeChooseComboBox(Gtk::Container&);
    void constructBrushSizeScale(Gtk::Container&);
    void constructDeltaChooseScale(Gtk::Container&);

    void constructPlayButton(Gtk::Container&);
    void constructStopButton(Gtk::Container&);

    void constructFileChooseComboBox(Gtk::Container&);
    void constructSaveButton(Gtk::Container&);
    void constructExitButton(Gtk::Container&);

    void on_playButton_clicked();
    void on_stopButton_clicked();

};

#endif // GUI_MAIN_WINDOW_H
