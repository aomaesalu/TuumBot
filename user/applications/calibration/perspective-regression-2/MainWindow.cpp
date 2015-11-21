/**
 *  @file MainWindow.cpp
 *  Perspective calibration application main window.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 20 November 2015
 */

#include "MainWindow.hpp"

#include "cameraConstants.hpp"

#include <iostream> // TODO: Remove
#include <fstream>


namespace rtx {

  MainWindow::MainWindow(Camera *camera):
    camera(camera),
    imageArea(this),
    playing(true),
    calculating(false),
    filter("")
  {
    setProperties();
    readFilterFromFile("../data/colors/1.txt");
    construct();
    show_all_children();
    updateFrame();
  }

  MainWindow::~MainWindow() {
    // Nothing to do here
  }

  bool MainWindow::isPlaying() const {
    return playing;
  }

  bool MainWindow::isCalculating() const {
    return calculating;
  }

  bool MainWindow::isColored(const unsigned int &pixel, const unsigned int &mode) const {
    if (filter.size() > pixel) {
      return (filter[pixel] >> (7 - mode)) & 0x1;
    } else {
      return false;
    }
  }

  bool MainWindow::isColored(const unsigned int &x, const unsigned int &y, const unsigned int &z, const unsigned int &mode) const {
    return isColored((x << 16) + (y << 8) + z, mode);
  }

  void MainWindow::setPlaying(const bool &value) {
    playButton.set_sensitive(!value);
    stopButton.set_sensitive(value);
    if (value)
      imageArea.resetBlobRegression();
    playing = value;
    setCalculating(!value);
  }

  void MainWindow::setCalculating(const bool &value) {
    if (value)
      imageArea.resetConstants();
    calculating = value;
  }

  bool MainWindow::updateFrame() {
    if (playing) {
      frame = camera->getFrame();
      rgbFrame = toRGB(frame);
    }
    imageArea.updateFrame(&frame, &rgbFrame);
    return true;
  }

  void MainWindow::setProperties() {
    // Set window properties
    set_title("Perspective calibration program");
    // Set not resizable
    set_resizable(false);
    // Set window border width
    set_border_width(5);
  }

  void MainWindow::construct() {
    constructGrid();
    constructGeneralButtonsBox();
    constructImageFrame();
  }

  void MainWindow::constructGrid() {
    grid.set_row_spacing(10);
    grid.set_column_spacing(5);
    add(grid);
  }

  void MainWindow::constructGeneralButtonsBox() {
    constructPlayButton(generalButtonsBox);
    constructStopButton(generalButtonsBox);
    constructFileChooseComboBox(generalButtonsBox);
    constructSaveButton(generalButtonsBox);
    constructExitButton(generalButtonsBox);
    generalButtonsBox.set_spacing(10);
    grid.attach(generalButtonsBox, 0, 0, 1, 1);
  }

  void MainWindow::constructImageFrame() {
    imageArea.add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
    imageFrame.add(imageArea);
    imageFrame.set_label("Image");
    imageFrame.set_size_request(CAMERA_WIDTH, CAMERA_HEIGHT);
    imageFrame.set_border_width(0);
    grid.attach(imageFrame, 0, 1, 1, 1);
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

  void MainWindow::readFilterFromFile(const std::string &fileName) {
    std::ifstream inputFile(fileName);
    inputFile >> filter;
    inputFile.close();
  }

  void MainWindow::saveConstantsToFile(const std::string &fileName) {
    std::ofstream outputFile(fileName);
    outputFile << imageArea.getB() << " " << imageArea.getC();
    outputFile.close();
  }

  void MainWindow::on_playButton_clicked() {
    setPlaying();
  }

  void MainWindow::on_stopButton_clicked() {
    setPlaying(false);
  }

  void MainWindow::on_saveButton_clicked() {
    Gtk::FileChooserDialog dialog("Please choose where to save the calibrated perspective file", Gtk::FILE_CHOOSER_ACTION_SAVE);
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
      saveConstantsToFile(dialog.get_filename());
    } else if (result == Gtk::RESPONSE_CANCEL) {
      std::cout << "Cancel clicked" << std::endl;
    } else {
      std::cout << "Unexpected button clicked." << std::endl;
    }
  }

  // TODO: Spacebar key event to play or stop video and to start calculations

};
