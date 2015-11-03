/**
 * @file ImageDrawingArea.hpp
 * Color calibration application "after" image drawing area.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef GUI_IMAGE_AFTER_DRAWING_AREA_H
#define GUI_IMAGE_AFTER_DRAWING_AREA_H

#include <vector>
#include <set>

#include <gtkmm.h>
#include <gdkmm/pixbuf.h>

#include "ImageDrawingArea.hpp"


class ImageAfterDrawingArea: public ImageDrawingArea {

  public:
    ImageAfterDrawingArea(MainWindow*, Gtk::Scale*);
    virtual ~ImageAfterDrawingArea();

    void calculateFilterBuffer(const std::vector<std::set<unsigned int>>&, const std::vector<std::set<unsigned int>>&);
    void addBufferToFilter();

    std::string getOutput() const;

  protected:
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>&);
    virtual bool on_scroll_event(GdkEventScroll*);

  private:
    Glib::RefPtr<Gdk::Pixbuf> image;
    Glib::RefPtr<Gdk::Pixbuf> filteredImage;

    Gtk::Scale *deltaScale;

    std::vector<std::set<unsigned int>> filter;
    std::vector<std::set<unsigned int>> filterAdditionBufferList;
    std::vector<std::set<unsigned int>> filterRemovalBufferList;

    void initialiseProperties();
    void initialiseImage();
    void initialiseDeltaScale(Gtk::Scale*);
    void initialiseFilters();

    void resetFilter();
    void resetFilterBuffers();

    bool applyFilter();
    bool drawImage(const Cairo::RefPtr<Cairo::Context>&);

};

#endif // GUI_IMAGE_AFTER_DRAWING_AREA_H
