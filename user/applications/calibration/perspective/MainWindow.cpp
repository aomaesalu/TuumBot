/**
 *  @file MainWindow.cpp
 *  Perspective calibration application GUI main window.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 18 November 2015
 */

 #include "MainWindow.hpp"

 #include "GUI.hpp"
 #include "cameraConstants.hpp"

 #include <iostream> // TODO: Remove


 namespace rtx {

   MainWindow::MainWindow(GUI *gui):
     imageArea(gui)
   {
     // Attach GUI interface information
     this->gui = gui;
   }

   MainWindow::~MainWindow() {
     // Nothing to do here
   }

   void MainWindow::initialise() {
     imageArea.initialise();

     // Set GUI main properties
     setProperties();

     // Construct the GUI
     construct();

     // Update video frame
     gui->updateFrame();

     // Show GUI contents
     show_all_children();
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
     constructGrid();
     constructImageAreaFrame();
   }

   void MainWindow::constructGrid() {
     grid.set_row_spacing(10);
     grid.set_column_spacing(5);
     add(grid);
   }

   void MainWindow::constructImageAreaFrame() {
     imageAreaFrame.add(imageArea);
     imageAreaFrame.set_label("Image");
     imageAreaFrame.set_size_request(CAMERA_WIDTH, CAMERA_HEIGHT);
     imageAreaFrame.set_border_width(0);
     grid.attach(imageAreaFrame, 1, 1, 1, 1);
   }

   ImageArea* MainWindow::getImageArea() {
     return &imageArea;
   }

 }
