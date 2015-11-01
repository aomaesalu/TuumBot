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
#include <gtkmm.h>
#include <gdkmm/pixbuf.h>

#include "ImageDrawingArea.hpp"


class ImageBeforeDrawingArea: public ImageDrawingArea {

  public:
    ImageBeforeDrawingArea(MainWindow*, Gtk::Scale*);
    virtual ~ImageBeforeDrawingArea();

    bool isMasking() const;
    bool isMaskEmpty() const;

    void setPlaying(const bool& = true);
    void setMasking(const bool& = true);

  protected:
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>&);
    virtual bool on_button_press_event(GdkEventButton*);
    virtual bool on_button_release_event(GdkEventButton*);
    virtual bool on_motion_notify_event(GdkEventMotion*);
    virtual bool on_scroll_event(GdkEventScroll*);

  private:
    Glib::RefPtr<Gdk::Pixbuf> image;
    Glib::RefPtr<Gdk::Pixbuf> maskedImage;
    Glib::RefPtr<Gdk::Pixbuf> brushedImage;

    Gtk::Scale *brushScale;
    std::vector<std::vector<bool>> mask;

    unsigned int maskMinX;
    unsigned int maskMinY;
    unsigned int maskMaxX;
    unsigned int maskMaxY;

    unsigned int brushX;
    unsigned int brushY;

    bool addingMode;
    bool erasingMode;

    void initialiseProperties();
    void initialiseImage();
    void initialiseBrush(Gtk::Scale*);
    void initialiseMask();
    void initialiseMaskBoundaries();
    void initialiseDrawingModes();

    bool drawImage(const Cairo::RefPtr<Cairo::Context>&);
    bool applyMask();
    bool locateBrush(const unsigned int&, const unsigned int&);
    bool applyBrush();

    void addToMask(const unsigned int&, const unsigned int&);
    void eraseFromMask(const unsigned int&, const unsigned int&);
    void changeValueInMask(const unsigned int&, const unsigned int&, const bool&);

};

#endif // GUI_IMAGE_BEFORE_DRAWING_AREA_H
