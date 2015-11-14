/**
 *  @file MainWindow.hpp
 *  Color calibration application main window.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 14 November 2015
 */

#ifndef RTX_APPLICATIONS_CALIBRATION_COLOR_MAIN_WINDOW_H
#define RTX_APPLICATIONS_CALIBRATION_COLOR_MAIN_WINDOW_H

#include <vector>
#include <set>
#include <string>

#include <gtkmm.h>
#include <gtkmm/window.h>

#include "MaskingArea.hpp"
#include "PreviewArea.hpp"


namespace rtx {

  class Application;

  class MainWindow: public Gtk::Window {

    public:
      MainWindow(Application*);
      virtual ~MainWindow();

      MaskingArea* getMaskingArea();
      PreviewArea* getPreviewArea();

      Gtk::Scale* getBrushScale();
      Gtk::Scale* getDeltaScale();

      void setPlaying(const bool& = true);

    protected:
      Gtk::Grid grid;

      // Frames and boxes for the layout
      Gtk::Box drawingButtonsBox;
      Gtk::Box generalButtonsBox;
      Gtk::Frame maskingAreaFrame;
      Gtk::Frame previewAreaFrame;
      Gtk::Box maskingAreaOptionsBox;
      Gtk::Box previewAreaOptionsBox;

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
      MaskingArea maskingArea;
      PreviewArea previewArea;

    private:
      Application *application;

      void setProperties();

      void construct();
      void constructGrid();

      void constructDrawingButtonsBox();
      void constructGeneralButtonsBox();
      void constructMaskingAreaFrame();
      void constructPreviewAreaFrame();
      void constructMaskingAreaOptionsBox();
      void constructPreviewAreaOptionsBox();

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
      void on_modeChooseComboBox_changed();
      void on_saveButton_clicked();

  };

}

#endif // RTX_APPLICATIONS_CALIBRATION_COLOR_MAIN_WINDOW_H
