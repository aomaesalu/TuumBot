/**
 *  @file MainWindow.cpp
 *  Color calibration application main window.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 14 November 2015
 */

#include "MainWindow.hpp"

#include "Application.hpp"
#include "cameraConstants.hpp"

#include <iostream> // TODO: Remove
#include <fstream>


namespace rtx {

  MainWindow::MainWindow(Application *application):
    maskingArea(application),
    previewArea(application)
  {
    // Attach application information
    this->application = application;
    std::cout << "A1" << std::endl;

    // Set GUI main properties
    setProperties();
    std::cout << "A2" << std::endl;

    // Construct the GUI
    construct();
    std::cout << "A3" << std::endl;

    // Update video frame
    // TODO: updateFrame();
    maskingArea.redraw();
    std::cout << "A4" << std::endl;
    previewArea.queue_draw();
    std::cout << "A5" << std::endl;

    // Show GUI contents
    show_all_children();
    std::cout << "A6" << std::endl;
  }

  MainWindow::~MainWindow() {
    // Nothing to do here
  }

  void MainWindow::setProperties() {
    // Set window properties
    set_title("Color calibration program");
    // Set the window to be of static size
    set_resizable(false);
    // Set window border width
    set_border_width(5);
  }

  void MainWindow::construct() {
    std::cout << "B1" << std::endl;
    constructGrid();
    std::cout << "B2" << std::endl;
    constructDrawingButtonsBox();
    std::cout << "B3" << std::endl;
    constructGeneralButtonsBox();
    std::cout << "B4" << std::endl;
    constructMaskingAreaFrame();
    std::cout << "B5" << std::endl;
    constructPreviewAreaFrame();
    std::cout << "B6" << std::endl;
    constructMaskingAreaOptionsBox();
    std::cout << "B7" << std::endl;
    constructPreviewAreaOptionsBox();
    std::cout << "B8" << std::endl;
  }

  void MainWindow::constructGrid() {
    grid.set_row_spacing(10);
    grid.set_column_spacing(5);
    add(grid);
  }

  void MainWindow::constructDrawingButtonsBox() {
    std::cout << "C1" << std::endl;
    constructModeChooseComboBox(drawingButtonsBox);
    std::cout << "C2" << std::endl;
    constructBrushSizeScale(drawingButtonsBox);
    std::cout << "C3" << std::endl;
    constructDeltaChooseScale(drawingButtonsBox);
    std::cout << "C4" << std::endl;
    constructPlayButton(drawingButtonsBox);
    std::cout << "C5" << std::endl;
    constructStopButton(drawingButtonsBox);
    std::cout << "C6" << std::endl;
    drawingButtonsBox.set_spacing(10);
    std::cout << "C7" << std::endl;
    grid.attach(drawingButtonsBox, 0, 0, 1, 1);
    std::cout << "C8" << std::endl;
  }

  void MainWindow::constructGeneralButtonsBox() {
    constructFileChooseComboBox(generalButtonsBox);
    constructSaveButton(generalButtonsBox);
    constructExitButton(generalButtonsBox);
    generalButtonsBox.set_spacing(10);
    grid.attach(generalButtonsBox, 1, 0, 1, 1);
  }

  void MainWindow::constructMaskingAreaFrame() {
    maskingArea.add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::POINTER_MOTION_MASK | Gdk::SCROLL_MASK);
    maskingAreaFrame.add(maskingArea);
    maskingAreaFrame.set_label("Masking");
    maskingAreaFrame.set_size_request(CAMERA_WIDTH, CAMERA_HEIGHT);
    maskingAreaFrame.set_border_width(0);
    grid.attach(maskingAreaFrame, 0, 1, 1, 1);
  }

  void MainWindow::constructPreviewAreaFrame() {
    previewArea.add_events(Gdk::SCROLL_MASK);
    previewAreaFrame.add(previewArea);
    previewAreaFrame.set_label("Preview");
    previewAreaFrame.set_size_request(CAMERA_WIDTH, CAMERA_HEIGHT);
    previewAreaFrame.set_border_width(0);
    grid.attach(previewAreaFrame, 1, 1, 1, 1);
  }

  void MainWindow::constructMaskingAreaOptionsBox() {
    displayMaskBeforeButton.set_label("Display mask on \"masking\" image"); // TODO: Add scale to change brightness instead
    displayMaskBeforeButton.set_active();
    displayMaskBeforeButton.set_can_focus(false);
    maskingAreaOptionsBox.add(displayMaskBeforeButton);
    grid.attach(maskingAreaOptionsBox, 0, 2, 1, 1);
  }

  void MainWindow::constructPreviewAreaOptionsBox() {
    displayMaskAfterButton.set_label("Display filter on \"preview\" image"); // TODO: Add scale to change brightness instead
    displayMaskAfterButton.set_active();
    displayMaskAfterButton.set_can_focus(false);
    previewAreaOptionsBox.add(displayMaskAfterButton);
    grid.attach(previewAreaOptionsBox, 1, 2, 1, 1);
  }

  void MainWindow::constructModeChooseComboBox(Gtk::Container &parentContainer) {
    modeChooseLabel.set_text("Mode:");
    parentContainer.add(modeChooseLabel);
    for (std::vector<std::string>::const_iterator mode = application->getModes().begin(); mode != application->getModes().end(); ++mode) {
      modeChooseComboBox.append(*mode);
    }
    std::cout << "D6" << std::endl;
    modeChooseComboBox.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::on_modeChooseComboBox_changed));
    std::cout << "D7" << std::endl;
    modeChooseComboBox.set_active(0);
    std::cout << "D8" << std::endl;
    parentContainer.add(modeChooseComboBox);
    std::cout << "D9" << std::endl;
  }

  void MainWindow::constructBrushSizeScale(Gtk::Container &parentContainer) {
    brushSizeLabel.set_text("Brush size:");
    parentContainer.add(brushSizeLabel);
    brushSizeScale.set_adjustment(Gtk::Adjustment::create(30, 1, 75, 1, 5));
    brushSizeScale.set_digits(0);
    brushSizeScale.set_size_request(100);
    parentContainer.add(brushSizeScale);
  }

  void MainWindow::constructDeltaChooseScale(Gtk::Container &parentContainer) {
    deltaChooseLabel.set_text("Delta:");
    parentContainer.add(deltaChooseLabel);
    deltaChooseScale.set_adjustment(Gtk::Adjustment::create(0, 0, 10, 1, 1));
    deltaChooseScale.set_digits(0);
    deltaChooseScale.set_size_request(100);
    parentContainer.add(deltaChooseScale);
  }

  void MainWindow::constructPlayButton(Gtk::Container &parentContainer) {
    playButton.set_label("Play");
    playButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_playButton_clicked));
    playButton.set_sensitive(false);
    parentContainer.add(playButton);
  }

  void MainWindow::constructStopButton(Gtk::Container &parentContainer) {
    stopButton.set_label("Stop");
    stopButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_stopButton_clicked));
    parentContainer.add(stopButton);
  }

  void MainWindow::constructFileChooseComboBox(Gtk::Container &parentContainer) {
    fileChooseComboBox.append("New...");
    // TODO: Add other files
    fileChooseComboBox.set_active(0);
    fileChooseComboBox.set_hexpand();
    parentContainer.add(fileChooseComboBox);
  }

  void MainWindow::constructSaveButton(Gtk::Container &parentContainer) {
    saveButton.set_label("Save");
    saveButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_saveButton_clicked) );
    parentContainer.add(saveButton);
  }

  void MainWindow::constructExitButton(Gtk::Container &parentContainer) {
    exitButton.set_label("Exit");
    parentContainer.add(exitButton);
  }

  MaskingArea* MainWindow::getMaskingArea() {
    return &maskingArea;
  }

  PreviewArea* MainWindow::getPreviewArea() {
    return &previewArea;
  }

  Gtk::Scale* MainWindow::getBrushScale() {
    return &brushSizeScale;
  }

  Gtk::Scale* MainWindow::getDeltaScale() {
    return &deltaChooseScale;
  }

  void MainWindow::setPlaying(const bool &value) {
    playButton.set_sensitive(!value);
    stopButton.set_sensitive(value);
  }

  void MainWindow::on_playButton_clicked() {
    application->setPlaying();
  }

  void MainWindow::on_stopButton_clicked() {
    application->setPlaying(false);
  }

  void MainWindow::on_modeChooseComboBox_changed() {
    application->setMode(modeChooseComboBox.get_active_row_number());
    maskingArea.redraw();
    previewArea.queue_draw();
  }

  void MainWindow::on_saveButton_clicked() {
    Gtk::FileChooserDialog dialog("Please choose where to save the calibrated color file", Gtk::FILE_CHOOSER_ACTION_SAVE);
    dialog.set_transient_for(*this);
    // Add buttons
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Save", Gtk::RESPONSE_OK);
    // TODO
    // Show dialog and wait for response
    int result = dialog.run();
    // Handle response
    if (result == Gtk::RESPONSE_OK) {
      std::cout << "Save clicked" << std::endl;
      application->saveFilterToFile(dialog.get_filename());
    } else if (result == Gtk::RESPONSE_CANCEL) {
      std::cout << "Cancel clicked" << std::endl;
    } else {
      std::cout << "Unexpected button clicked." << std::endl;
    }
  }

  // TODO: Spacebar key event to play or stop video

};
