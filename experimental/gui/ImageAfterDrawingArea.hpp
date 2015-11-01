/**
 * @file ImageDrawingArea.hpp
 * Experimental GUI color calibration application "after" image drawing area.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef GUI_IMAGE_AFTER_DRAWING_AREA_H
#define GUI_IMAGE_AFTER_DRAWING_AREA_H

#include <map>
#include <vector>

#include <gtkmm.h>
#include <gdkmm/pixbuf.h>

#include "ImageDrawingArea.hpp"


class ImageAfterDrawingArea: public ImageDrawingArea {

  public:
    ImageAfterDrawingArea(MainWindow*, Gtk::Scale*);
    virtual ~ImageAfterDrawingArea();

    void calculateFilterBuffer(const std::vector<std::vector<bool>&);
    void addBufferToFilter();

  protected:
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>&);
    virtual bool on_scroll_event(GdkEventScroll*);

  private:
    Glib::RefPtr<Gdk::Pixbuf> image;
    Glib::RefPtr<Gdk::Pixbuf> filteredImage;

    Gtk::Scale *deltaScale;

    std::map<unsigned int, std::map<unsigned int, std::map<unsigned int, bool>>> filter;
    std::map<unsigned int, std::map<unsigned int, std::map<unsigned int, bool>>> filterBuffer;

    void initialiseProperties();
    void initialiseImage();
    void initialiseDeltaScale(Gtk::Scale*);
    void initialiseFilters();
    void initialiseFilterMap(std::map<unsigned int, std::map<unsigned int, std::map<unsigned int, bool>>>&);
    void initialiseFilter();
    void initialiseFilterBuffer();

    void resetFilterMap(std::map<unsigned int, std::map<unsigned int, std::map<unsigned int, bool>>>&);
    void resetFilter();
    void resetFilterBuffer();

    bool applyFilter();
    bool drawImage(const Cairo::RefPtr<Cairo::Context>&);

};

#endif // GUI_IMAGE_AFTER_DRAWING_AREA_H
