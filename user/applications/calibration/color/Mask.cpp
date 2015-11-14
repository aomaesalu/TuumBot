/**
 *  @file Mask.cpp
 *  Color calibration application mask class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 14 November 2015
 */


namespace rtx {

  Mask::Mask(const unsigned int &numberOfModes, const unsigned int &frameWidth, const unsigned int &frameHeight) {
    // Set properties
    numberOfModes = numberOfModes;
    frameWidth = frameWidth;
    frameHeight = frameHeight;
    // Initialise data
    initialiseLists();
    initialiseMaps();
  }

  Mask::~Mask() {
    // Nothing to do here
  }

  void Mask::initialiseLists() {
    MaskValueSet emptySet;
    // TODO: Use set/vector initialisation constructors if possible
    for (unsigned int mode = 0; mode < numberOfModes; ++mode) {
      additionValues.push_back(emptySet);
      removalValues.push_back(emptySet);
    }
  }

  void Mask::initialiseMaps() {
    MaskMapList emptyMaps(numberOfModes, MaskValueMap(frameWidth, std::vector<bool>(frameHeight, false)));
    additionMaps = emptyMaps;
    removalMaps = additionMaps;
  }

  void Mask::reset() {
    resetLists();
    resetMaps();
  }

  void Mask::resetLists() {
    for (unsigned int mode = 0; mode < numberOfModes; ++mode) {
      additionValues[mode].clear();
      removalValues[mode].clear();
    }
  }

  void Mask::resetMaps() {
    for (unsigned int mode = 0; mode < numberOfModes; ++mode) {
      for (unsigned int x = 0; x < frameWidth; ++x) {
        for (unsigned int y = 0; y < frameHeight; ++y) {
          additionMaps[mode][x][y] = false;
          removalMaps[mode][x][y] = false;
        }
      }
    }
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
    for (unsigned int mode = 0; mode < additionValues.size(); ++mode) {
      if (!isAdditionEmpty(mode) || !isRemovalEmpty(mode)) {
        return false;
      }
    }
    return true;
  }

  bool Mask::isEmpty(const unsigned int &mode) const {
    return isAdditionEmpty(mode) && isRemovalEmpty(mode);
  }

  bool Mask::isAdditionEmpty(const unsigned int &mode) const {
    return additionValues[mode].empty();
  }

  bool Mask::isRemovalEmpty(const unsigned int &mode) const {
    return removalValues[mode].empty();
  }

  void Mask::add(const unsigned int &x, const unsigned int &y) {
    change(x, y, true);
  }

  void Mask::remove(const unsigned int &x, const unsigned int &y) {
    change(x, y, false);
  }

  void Mask::change(const unsigned int &x, const unsigned int &y, const bool &value) {
    // TODO
  }

}
