/**
 *  @file Mask.cpp
 *  Color calibration application mask class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 14 November 2015
 */


namespace rtx {

  Mask::Mask(const unsigned int &numberOfModes, const unsigned int &frameWidth,
             const unsigned int &frameHeight) {
    // Set properties
    numberOfModes = numberOfModes;
    frameWidth = frameWidth;
    frameHeight = frameHeight;

    // Initialise data
    initialiseLists();
    initialiseMaps();
    initialiseBoundaries();
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

  void Mask::initialiseBoundaries() {
    minX = frameWidth;
    minY = frameHeight;
    maxX = 0;
    maxY = 0;
  }

  void Mask::reset() {
    resetLists();
    resetMaps();
    initialiseBoundaries();
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

  unsigned int Mask::getMinX() const {
    return minX;
  }

  unsigned int Mask::getMinY() const {
    return minY;
  }

  unsigned int Mask::getMaxX() const {
    return maxX;
  }

  unsigned int Mask::getMaxY() const {
    return maxY;
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

  void Mask::add(const unsigned int &x, const unsigned int &y,
                 const unsigned int &mode, const int &radius) {
    change(x, y, mode, radius, true);
  }

  void Mask::remove(const unsigned int &x, const unsigned int &y,
                    const unsigned int &mode, const int &radius) {
    change(x, y, mode, radius, false);
  }

  void Mask::change(const unsigned int &x, const unsigned int &y,
                    const unsigned int &mode, const int &radius,
                    const bool &value) {

    unsigned int radiusSquared = radius * radius;

    for (int i = -radius; i < radius; ++i) {
      for (int j = -radius; j < radius; ++j) {

        if (i * i + j * j <= radiusSquared) {

          unsigned int currentX = x + i;
          unsigned int currentY = y + j;

          // If the value overflows, it's already smaller than the maximal value
          // because of usage of unsigned integers, therefore, other checks are
          // not necessary.
          if (currentX < frameWidth && currentY < frameHeight) {

            // Add values to maps
            additionMaps[mode][currentX][currentY] = value;
            removalMaps[mode][currentX][currentY] = !value;

            // Add values to lists
            unsigned int linearCoordinate = currentY * frameWidth + currentX;
            if (value) {
              additionValues[mode].insert(linearCoordinate);
              removalValues[mode].erase(linearCoordinate);
            } else {
              removalValues[mode]insert(linearCoordinate);
              additionValues[mode].erase(linearCoordinate);
            }

            // Calculate new boundaries
            if (currentX < minX) {
              minX = currentX;
            }
            if (currentY < minY) {
              minY = currentY;
            }
            if (currentX > maxX) {
              maxX = currentX;
            }
            if (currentY > maxY) {
              maxY = currentY;
            }

          }

        }

      }
    }

  }

}
