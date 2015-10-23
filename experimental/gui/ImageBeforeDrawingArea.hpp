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
    virtual bool on_motion_notify_event(GdkEventMotion*);
    virtual bool on_scroll_event(GdkEventScroll*);

  private:
    Glib::RefPtr<Gdk::Pixbuf> image;

    unsigned int brushSize;
    std::vector<std::vector<bool>> mask;

    bool addingMode;
    bool erasingMode;

    void initialiseProperties();
    void initialiseImage();
    void initialiseBrush();
    void initialiseMask();
    void initialiseDrawingModes();

    bool drawImage(const Cairo::RefPtr<Cairo::Context>&);
    bool drawBrush(const unsigned int&, const unsigned int&);
    bool applyMask();

    void addToMask(const unsigned int&, const unsigned int&);
    void eraseFromMask(const unsigned int&, const unsigned int&);
    void changeValueInMask(const unsigned int&, const unsigned int&, const bool&);

};

#endif // GUI_IMAGE_BEFORE_DRAWING_AREA_H
