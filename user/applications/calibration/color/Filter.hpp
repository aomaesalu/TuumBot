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

#include "Mask.hpp"


namespace rtx {

  typedef std::set<unsigned int> FilterValueSet;
  typedef std::vector<ModeFilter> FilterList;

  class Filter {

    public:
      Filter();
      virtual ~Filter();

      FilterList getValues() const;
      FilterValueSet getValues(const unsigned int&) const;

      FilterList getAdditionBuffers() const;
      FilterValueSet getAdditionBuffer(const unsigned int&) const;

      FilterList getRemovalBuffers() const;
      FilterValueSet getRemovalBuffer(const unsigned int&) const;

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
