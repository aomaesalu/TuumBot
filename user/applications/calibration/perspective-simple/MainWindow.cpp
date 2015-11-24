/**
 *  @file MainWindow.cpp
 *  Perspective calibration application GUI main window.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 24 November 2015
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
     constructScalesBox();
   }

   void MainWindow::constructGrid() {
     grid.set_row_spacing(10);
     grid.set_column_spacing(5);
     add(grid);
   }

   void MainWindow::constructImageAreaFrame() {
     imageArea.add_events(Gdk::BUTTON_PRESS_MASK);
     imageAreaFrame.add(imageArea);
     imageAreaFrame.set_label("Image");
     imageAreaFrame.set_size_request(CAMERA_WIDTH, CAMERA_HEIGHT);
     imageAreaFrame.set_border_width(0);
     grid.attach(imageAreaFrame, 0, 0, 1, 1);
   }

   void MainWindow::constructScalesBox() {
     constructScale(scalesBox, AScale, ALabel, "A");
     constructScale(scalesBox, BScale, BLabel, "B");
     constructScale(scalesBox, CScale, CLabel, "C");
     scalesBox.set_spacing(10);
     grid.attach(scalesBox, 1, 0, 1, 1);
   }

   void MainWindow::constructScale(Gtk::Container &parentContainer, Gtk::Scale &scale, Gtk::Label &label, const std::string &name) {
     label.set_text(name + ":");
     parentContainer.add(label);
     scale.set_adjustment(Gtk::Adjustment::create(1.0, 0.0, 10.0, 1.0, 1.0));
     scale.set_digits(4);
     scale.set_size_request(100);
     scale.signal_value_changed().connect(sigc::mem_fun(*this, &MainWindow::on_scale_value_changed));
     parentContainer.add(scale);
   }

   ImageArea* MainWindow::getImageArea() {
     return &imageArea;
   }

   Gtk::Scale* MainWindow::getAScale() {
     return &AScale;
   }

   Gtk::Scale* MainWindow::getBScale() {
     return &BScale;
   }

   Gtk::Scale* MainWindow::getCScale() {
     return &CScale;
   }

   void MainWindow::on_scale_value_changed() {
     gui->getCheckerboard()->setA(AScale.get_value());
     gui->getCheckerboard()->setB(BScale.get_value());
     gui->getCheckerboard()->setC(CScale.get_value());
     imageArea.queue_draw();
   }

 }
