/**
 * @file ImageDrawingArea.hpp
 * Experimental GUI color calibration application "after" image drawing area.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef GUI_IMAGE_AFTER_DRAWING_AREA_H
#define GUI_IMAGE_AFTER_DRAWING_AREA_H

#include <vector>
#include <set>
#include <map>

#include <gtkmm.h>
#include <gdkmm/pixbuf.h>

#include "ImageDrawingArea.hpp"


class ImageAfterDrawingArea: public ImageDrawingArea {

  public:
    ImageAfterDrawingArea(MainWindow*, Gtk::Scale*);
    virtual ~ImageAfterDrawingArea();

    void calculateFilterBuffer(const std::set<unsigned int>&, const std::set<unsigned int>&);
    void addBufferToFilter();

  protected:
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>&);
    virtual bool on_scroll_event(GdkEventScroll*);

  private:
    Glib::RefPtr<Gdk::Pixbuf> image;
    Glib::RefPtr<Gdk::Pixbuf> filteredImage;

    Gtk::Scale *deltaScale;

    std::map<unsigned int, std::map<unsigned int, std::map<unsigned int, bool>>> filter;
    std::map<unsigned int, std::map<unsigned int, std::map<unsigned int, bool>>> filterAdditionBuffer;
    std::map<unsigned int, std::map<unsigned int, std::map<unsigned int, bool>>> filterRemovalBuffer;

    std::map<unsigned int, std::map<unsigned int, std::map<unsigned int, bool>>> resettedFilter;

    void initialiseProperties();
    void initialiseImage();
    void initialiseDeltaScale(Gtk::Scale*);
    void initialiseFilters();

    void initialiseFilterMap(std::map<unsigned int, std::map<unsigned int, std::map<unsigned int, bool>>>&);
    void initialiseFilter();
    void initialiseFilterBuffers();
    void initialiseFilterAdditionBuffer();
    void initialiseFilterRemovalBuffer();
    void initialiseResettedFilter();

    void resetFilterMap(std::map<unsigned int, std::map<unsigned int, std::map<unsigned int, bool>>>&);
    void resetFilter();
    void resetFilterBuffers();
    void resetFilterAdditionBuffer();
    void resetFilterRemovalBuffer();

    bool applyFilter();
    bool drawImage(const Cairo::RefPtr<Cairo::Context>&);

};

#endif // GUI_IMAGE_AFTER_DRAWING_AREA_H
