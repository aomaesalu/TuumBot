/**
 * @file MainWindow.cpp
 * Experimental GUI application main window.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "MainWindow.hpp"

#include <iostream>
#include <fstream>


std::vector<std::string> modes = {"Ball", "Blue goal", "Yellow goal", "Field", "White line", "Black line", "Checkerboard white", "Checkerboard black"};

MainWindow::MainWindow():
  imageBeforeArea(this, &brushSizeScale),
  imageAfterArea(this, &deltaChooseScale),
  playing(true),
  masking(false)
{
  setProperties();
  construct();
  show_all_children();
}

MainWindow::~MainWindow() {
  // Nothing to do here
}

bool MainWindow::isPlaying() const {
  return playing;
}

bool MainWindow::isMasking() const {
  return masking;
}

std::vector<std::string> MainWindow::getModes() const {
  return modes;
}

unsigned int MainWindow::getMode() const {
  return mode;
}

void MainWindow::setPlaying(const bool &value) {
  playing = value;
  if (value) {
    imageBeforeArea.setMasking(false);
    imageAfterArea.addBufferToFilter();
  }
  playButton.set_sensitive(!value);
  stopButton.set_sensitive(value);
}

void MainWindow::setMasking(const bool &value) {
  masking = value;
}

void MainWindow::sendToFilter(const std::vector<std::set<unsigned int>> &additionMaskList, const std::vector<std::set<unsigned int>> &removalMaskList) {
  imageAfterArea.calculateFilterBuffer(additionMaskList, removalMaskList);
}

void MainWindow::setProperties() {
  // Set window properties
  set_title("Color calibration program");
  //set_size_request(300, 300);
  set_resizable(false);
  // Set window border width
  set_border_width(5);
}

void MainWindow::construct() {
  constructGrid();
  constructDrawingButtonsBox();
  constructGeneralButtonsBox();
  constructImageBeforeFrame();
  constructImageAfterFrame();
  constructImageBeforeOptionsBox();
  constructImageAfterOptionsBox();
}

void MainWindow::constructGrid() {
  grid.set_row_spacing(10);
  grid.set_column_spacing(5);
  add(grid);
}

void MainWindow::constructDrawingButtonsBox() {
  constructModeChooseComboBox(drawingButtonsBox);
  constructBrushSizeScale(drawingButtonsBox);
  constructDeltaChooseScale(drawingButtonsBox);
  constructPlayButton(drawingButtonsBox);
  constructStopButton(drawingButtonsBox);
  drawingButtonsBox.set_spacing(10);
  grid.attach(drawingButtonsBox, 0, 0, 1, 1);
}

void MainWindow::constructGeneralButtonsBox() {
  constructFileChooseComboBox(generalButtonsBox);
  constructSaveButton(generalButtonsBox);
  constructExitButton(generalButtonsBox);
  generalButtonsBox.set_spacing(10);
  grid.attach(generalButtonsBox, 1, 0, 1, 1);
}

void MainWindow::constructImageBeforeFrame() {
  imageBeforeArea.add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::POINTER_MOTION_MASK | Gdk::SCROLL_MASK);
  imageBeforeFrame.add(imageBeforeArea);
  imageBeforeFrame.set_label("Before");
  imageBeforeFrame.set_size_request(640, 480);
  imageBeforeFrame.set_border_width(0);
  grid.attach(imageBeforeFrame, 0, 1, 1, 1);
}

void MainWindow::constructImageAfterFrame() {
  imageAfterArea.add_events(Gdk::SCROLL_MASK);
  imageAfterFrame.add(imageAfterArea);
  imageAfterFrame.set_label("After");
  imageAfterFrame.set_size_request(640, 480);
  imageAfterFrame.set_border_width(0);
  grid.attach(imageAfterFrame, 1, 1, 1, 1);
}

void MainWindow::constructImageBeforeOptionsBox() {
  displayMaskBeforeButton.set_label("Display mask on \"before\" image"); // TODO: Add scale to change brightness instead
  displayMaskBeforeButton.set_active();
  displayMaskBeforeButton.set_can_focus(false);
  imageBeforeOptionsBox.add(displayMaskBeforeButton);
  grid.attach(imageBeforeOptionsBox, 0, 2, 1, 1);
}

void MainWindow::constructImageAfterOptionsBox() {
  displayMaskAfterButton.set_label("Display filter on \"after\" image"); // TODO: Add scale to change brightness instead
  displayMaskAfterButton.set_active();
  displayMaskAfterButton.set_can_focus(false);
  imageAfterOptionsBox.add(displayMaskAfterButton);
  grid.attach(imageAfterOptionsBox, 1, 2, 1, 1);
}

void MainWindow::constructModeChooseComboBox(Gtk::Container &parentContainer) {
  modeChooseLabel.set_text("Mode:");
  parentContainer.add(modeChooseLabel);
  for (std::vector<std::string>::iterator mode = modes.begin(); mode != modes.end(); ++mode) {
    modeChooseComboBox.append(*mode);
  }
  modeChooseComboBox.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::on_modeChooseComboBox_changed));
  modeChooseComboBox.set_active(0);
  parentContainer.add(modeChooseComboBox);
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

void MainWindow::saveFilterToFile(const std::string &fileName) {
  std::ofstream outputFile(fileName);
  outputFile << imageAfterArea.getOutput();
  outputFile.close();
}

void MainWindow::readFilterFromFile(const std::string &filename) {
  // TODO
}

void MainWindow::on_playButton_clicked() {
  setPlaying();
}

void MainWindow::on_stopButton_clicked() {
  setPlaying(false);
}

void MainWindow::on_modeChooseComboBox_changed() {
  mode = modeChooseComboBox.get_active_row_number();
  imageBeforeArea.redraw();
  imageAfterArea.queue_draw();
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
    saveFilterToFile(dialog.get_filename());
  } else if (result == Gtk::RESPONSE_CANCEL) {
    std::cout << "Cancel clicked" << std::endl;
  } else {
    std::cout << "Unexpected button clicked." << std::endl;
  }
}

// TODO: Spacebar key event to play or stop video
