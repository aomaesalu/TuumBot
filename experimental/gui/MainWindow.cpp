/**
 * @file MainWindow.cpp
 * Experimental GUI application main window.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "MainWindow.hpp"

#include <iostream>


MainWindow::MainWindow():
  imageBeforeArea(&brushSizeScale),
  imageAfterArea(&deltaChooseScale)
{
  setProperties();
  construct();
  show_all_children();
}

MainWindow::~MainWindow() {
  // Nothing to do here
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
  constructEntityChooseComboBox(drawingButtonsBox);
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
  displayMaskBeforeButton.set_label("Display mask on \"before\" image");
  displayMaskBeforeButton.set_active();
  displayMaskBeforeButton.set_can_focus(false);
  imageBeforeOptionsBox.add(displayMaskBeforeButton);
  grid.attach(imageBeforeOptionsBox, 0, 2, 1, 1);
}

void MainWindow::constructImageAfterOptionsBox() {
  displayMaskAfterButton.set_label("Display mask on \"after\" image");
  displayMaskAfterButton.set_active();
  displayMaskAfterButton.set_can_focus(false);
  imageAfterOptionsBox.add(displayMaskAfterButton);
  grid.attach(imageAfterOptionsBox, 1, 2, 1, 1);
}

void MainWindow::constructEntityChooseComboBox(Gtk::Container &parentContainer) {
  entityChooseLabel.set_text("Entity:");
  parentContainer.add(entityChooseLabel);
  entityChooseComboBox.append("Ball");
  entityChooseComboBox.append("Blue goal");
  entityChooseComboBox.append("Yellow goal");
  entityChooseComboBox.append("Field");
  entityChooseComboBox.append("White line");
  entityChooseComboBox.append("Black line");
  entityChooseComboBox.set_active(0);
  parentContainer.add(entityChooseComboBox);
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
  parentContainer.add(playButton);
}

void MainWindow::constructStopButton(Gtk::Container &parentContainer) {
  stopButton.set_label("Stop");
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
  parentContainer.add(saveButton);
}

void MainWindow::constructExitButton(Gtk::Container &parentContainer) {
  exitButton.set_label("Exit");
  parentContainer.add(exitButton);
}
