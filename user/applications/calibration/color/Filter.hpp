/**
 *  @file Filter.hpp
 *  Color calibration application filter class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 14 November 2015
 */

#ifndef RTX_APPLICATIONS_CALIBRATION_COLOR_FILTER_H
#define RTX_APPLICATIONS_CALIBRATION_COLOR_FILTER_H

#include <vector>
#include <set>


namespace rtx {

  typedef std::set<unsigned int> ModeFilter;
  typedef std::vector<ModeFilter> FilterList;

  class Filter {

    public:
      Filter();
      virtual ~Filter();

      FilterList getValues() const;
      ModeFilter getValues(const unsigned int&) const;

      FilterList getAdditionBuffers() const;
      ModeFilter getAdditionBuffer(const unsigned int&) const;

      FilterList getRemovalBuffers() const;
      ModeFilter getRemovalBuffer(const unsigned int&) const;

      void addMasksToBuffers(const MaskList&, const MaskList&);
      void addBuffersToFilter();

      std::string getString() const;

    private:
      FilterList values;
      FilterList additionBuffers;
      FilterList removalBuffers;

  };

}

#endif RTX_APPLICATIONS_CALIBRATION_COLOR_FILTER_H
