/**
 *  @file Mask.cpp
 *  Color calibration application mask class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 14 November, 2015
 */


namespace rtx {

  Mask::Mask() {
    // TODO
  }

  Mask::~Mask() {
    // TODO
  }

  MaskList Mask::getAdditionValues() const {
    return additionValues;
  }

  MaskValueSet Mask::getAdditionValues(const unsigned int &mode) const {
    return additionValues[mode];
  }

  MaskList Mask::getRemovalValues() const {
    return removalValues;
  }

  MaskValueSet Mask::getRemovalValues(const unsigned int &mode) const {
    return removalValues[mode];
  }

  MaskMapList Mask::getAdditionMaps() const {
    return additionMaps;
  }

  MaskValueMap Mask::getAdditionMap(const unsigned int &mode) const {
    return additionMaps[mode];
  }

  MaskMapList Mask::getRemovalMaps() const {
    return removalMaps;
  }

  MaskMapList Mask::getRemovalMap(const unsigned int &mode) const {
    return removalMaps[mode];
  }

  bool Mask::isEmpty() const {
    // TODO
  }

  bool Mask::isEmpty(const unsigned int &mode) const {
    // TODO
  }

  bool Mask::isAdditionEmpty(const unsigned int &mode) const {
    // TODO
  }

  bool Mask::isRemovalEmpty(const unsigned int &mode) const {
    // TODO
  }

  void add(const unsigned int &x, const unsigned int &y) {
    change(x, y, true);
  }

  void remove(const unsigned int &x, const unsigned int &y) {
    change(x, y, false);
  }

  void change(const unsigned int &x, const unsigned int &y, const bool &value) {
    // TODO
  }

}
