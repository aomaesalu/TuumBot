/**
 *  @file Filter.cpp
 *  Color calibration application filter class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 14 November 2015
 */


namespace rtx {

  Filter::Filter() {
    // TODO
  }

  Filter::~Filter() {
    // TODO
  }

  FilterList Filter::getValues() const {
    return values;
  }

  ModeFilter Filter::getValues(const unsigned int &mode) const {
    return values[mode];
  }

  FilterList Filter::getAdditionBuffers() const {
    return additionBuffers;
  }

  ModeFilter Filter::getValues(const unsigned int &mode) const {
    return additionBuffers[mode];
  }

  FilterList Filter::getRemovalBuffers() const {
    return removalBuffers;
  }

  FilterList Filter::getRemovalBuffer(const unsigned int &mode) const {
    return removalBuffers[mode];
  }

  void Filter::addMasksToBuffers(const MaskList &additionMasks, const MaskList &removalMasks) {
    // TODO
  }

  void Filter::addBuffersToFilter() {
    // TODO
  }

  std::string Filter::getString() const {
    // TODO
  }

}
