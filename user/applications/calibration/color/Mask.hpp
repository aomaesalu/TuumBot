/**
 *  @file Mask.hpp
 *  Color calibration application mask class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 14 November 2015
 */

#ifndef RTX_APPLICATIONS_CALIBRATION_COLOR_MASK_H
#define RTX_APPLICATIONS_CALIBRATION_COLOR_MASK_H

#include <vector>
#include <set>


namespace rtx {

  typedef std::set<unsigned int> MaskValueSet;
  typedef std::vector<MaskValueSet> MaskList;
  typedef std::vector<std::vector<bool>> MaskValueMap;
  typedef std::vector<MaskValueMap> MaskMapList;

  class Mask {

    public:
      Mask(const unsigned int&, const unsigned int&, const unsigned int&);
      virtual ~Mask();

      void reset();

      // TODO: Add boundaries maximisation and recalculation if necessary

      bool has(const unsigned int&, const unsigned int&, const unsigned int&) const;

      MaskList getAdditionValues() const;
      MaskValueSet getAdditionValues(const unsigned int&) const;

      MaskList getRemovalValues() const;
      MaskValueSet getRemovalValues(const unsigned int&) const;

      MaskMapList getAdditionMaps() const;
      MaskValueMap getAdditionMap(const unsigned int&) const;

      MaskMapList getRemovalMaps() const;
      MaskValueMap getRemovalMap(const unsigned int&) const;

      unsigned int getMinX() const;
      unsigned int getMinY() const;
      unsigned int getMaxX() const;
      unsigned int getMaxY() const;

      bool isEmpty() const;
      bool isEmpty(const unsigned int&) const;
      bool isAdditionEmpty(const unsigned int&) const;
      bool isRemovalEmpty(const unsigned int&) const;

      void add(const unsigned int&, const unsigned int&, const unsigned int&,
               const int&);
      void remove(const unsigned int&, const unsigned int&, const unsigned int&,
                  const int&);

    private:
      unsigned int numberOfModes;
      unsigned int frameWidth;
      unsigned int frameHeight;

      MaskList additionValues;
      MaskList removalValues;
      MaskMapList additionMaps;
      MaskMapList removalMaps;

      // TODO: Create boundaries construct, extend Boundable
      unsigned int minX;
      unsigned int minY;
      unsigned int maxX;
      unsigned int maxY;

      void initialiseLists(const unsigned int&);
      void initialiseMaps(const unsigned int&, const unsigned int&,
                          const unsigned int&);
      void initialiseBoundaries();

      void resetLists();
      void resetMaps();

      void change(const unsigned int&, const unsigned int&, const unsigned int&,
                  const int&, const bool&);

  };

}

#endif // RTX_APPLICATIONS_CALIBRATION_COLOR_MASK_H
