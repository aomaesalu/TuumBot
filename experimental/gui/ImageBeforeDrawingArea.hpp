/**
 * @file ImageDrawingArea.hpp
 * Experimental GUI color calibration application "before" image drawing area.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef GUI_IMAGE_BEFORE_DRAWING_AREA_H
#define GUI_IMAGE_BEFORE_DRAWING_AREA_H

#include <vector>
#include <gdkmm/pixbuf.h>

#include "ImageDrawingArea.hpp"


class ImageBeforeDrawingArea: public ImageDrawingArea {

  public:
    ImageBeforeDrawingArea();
    virtual ~ImageBeforeDrawingArea();

    bool maskEmpty() const;

  protected:
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>&);
    virtual bool on_button_press_event(GdkEventButton*);
    virtual bool on_button_release_event(GdkEventButton*);
    virtual bool on_scroll_event(GdkEventScroll*);

  private:
    Glib::RefPtr<Gdk::Pixbuf> image;

    unsigned int brushSize;
    std::vector<std::vector<bool>> mask;

    void initialiseBrush();
    void initialiseMask();
    
    bool drawImage(const Cairo::RefPtr<Cairo::Context>&);
    bool applyMask();

};

#endif // GUI_IMAGE_BEFORE_DRAWING_AREA_H
