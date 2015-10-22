/**
 * @file MainWindow.cpp
 * Experimental GUI application main window.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "MainWindow.hpp"


MainWindow::MainWindow() {
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
  constructEntityChooseButton();
  constructBrushSizeButton();
  grid.attach(drawingButtonsBox, 0, 0, 1, 1);
}

void MainWindow::constructGeneralButtonsBox() {
  constructFileChooseButton();
  constructSaveButton();
  constructExitButton();
  grid.attach(generalButtonsBox, 1, 0, 1, 1);
}

void MainWindow::constructImageBeforeFrame() {
  imageBeforeFrame.set_label("Before");
  imageBeforeFrame.set_size_request(640, 480);
  grid.attach(imageBeforeFrame, 0, 1, 1, 1);
}

void MainWindow::constructImageAfterFrame() {
  imageAfterFrame.set_label("After");
  imageAfterFrame.set_size_request(640, 480);
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

void MainWindow::constructEntityChooseButton() {
  // TODO
  drawingButtonsBox.add(entityChooseButton);
}

void MainWindow::constructBrushSizeButton() {
  // TODO
  drawingButtonsBox.add(brushSizeButton);
}

void MainWindow::constructFileChooseButton() {
  // TODO
  generalButtonsBox.add(fileChooseButton);
}

void MainWindow::constructSaveButton() {
  saveButton.set_label("Save");
  generalButtonsBox.add(saveButton);
}

void MainWindow::constructExitButton() {
  exitButton.set_label("Exit");
  generalButtonsBox.add(exitButton);
}
