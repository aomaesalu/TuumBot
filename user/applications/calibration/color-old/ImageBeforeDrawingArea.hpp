/**
 * @file ImageDrawingArea.hpp
 * Color calibration application "before" image drawing area.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef RTX_APPLICATIONS_CALIBRATION_COLOR_IMAGE_BEFORE_DRAWING_AREA_H
#define RTX_APPLICATIONS_CALIBRATION_COLOR_IMAGE_BEFORE_DRAWING_AREA_H

#include <vector>
#include <set>

#include <gtkmm.h>
#include <gdkmm/pixbuf.h>

#include "Camera.hpp"

#include "ImageDrawingArea.hpp"


namespace rtx {

  class ImageBeforeDrawingArea: public ImageDrawingArea {

    public:
      ImageBeforeDrawingArea(MainWindow*, Gtk::Scale*);
      virtual ~ImageBeforeDrawingArea();

      bool isMasking() const;
      bool areMasksEmpty() const;
      bool areMasksEmpty(const unsigned int&) const;
      bool areCurrentMasksEmpty() const;
      bool isAdditionMaskEmpty(const unsigned int&) const;
      bool isCurrentAdditionMaskEmpty() const;
      bool isRemovalMaskEmpty(const unsigned int&) const;
      bool isCurrentRemovalMaskEmpty() const;

      void setPlaying(const bool& = true);
      void setMasking(const bool& = true);

      void updateFrame(Frame*, Frame*);

      void redraw();

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

      Frame *frame;
      Frame *rgbFrame;

      Gtk::Scale *brushScale;
      std::vector<std::vector<std::vector<bool>>> additionMask;
      std::vector<std::vector<std::vector<bool>>> removalMask;
      std::vector<std::set<unsigned int>> additionMaskList;
      std::vector<std::set<unsigned int>> removalMaskList;

      unsigned int maskMinX;
      unsigned int maskMinY;
      unsigned int maskMaxX;
      unsigned int maskMaxY;

      unsigned int brushX;
      unsigned int brushY;

      bool addingMode;
      bool removingMode;

      void initialiseProperties();
      void initialiseImage();
      void initialiseBrush(Gtk::Scale*);
      void initialiseMasks();
      void initialiseMaskMatrices();
      void initialiseMaskLists();
      void initialiseDrawingModes();
      void initialiseMaskBoundaries();
      void maximiseMaskBoundaries();

      bool isMaskEmpty(const std::vector<std::vector<bool>>&) const;

      bool applyMask();
      bool locateBrush(const unsigned int&, const unsigned int&);
      bool applyBrush();
      bool drawImage(const Cairo::RefPtr<Cairo::Context>&);

      void addToMask(const unsigned int&, const unsigned int&);
      void removeFromMask(const unsigned int&, const unsigned int&);
      void changeValueInMask(const unsigned int&, const unsigned int&, const bool&);

      void sendMasksToFilter();

  };

};

#endif // RTX_APPLICATIONS_CALIBRATION_COLOR_IMAGE_BEFORE_DRAWING_AREA_H
