/**
 *  @file Vision.cpp
 *  Computer vision class using YUYV.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.2
 *  @date 3 December 2015
 */

#include "Vision.hpp"

#include <iostream> // TODO: Remove
#include <algorithm>
#include <set>

#include "Perspective.hpp"

#include "mathematicalConstants.hpp"
#include "entityConstants.hpp"


namespace rtx {

  namespace Vision {

    Samples flatSamples;
    Samples meshSamples;
    Samples radialSamples;

    BlobSet blobs;
    BlobSet blobsBuffer;

    LineSet lines;
    LineSet linesBuffer;

    CornerSet corners;
    CornerSet cornersBuffer;

    bool editingBlobs = false;
    bool editingLines = false;
    bool editingCorners = false;

    /*void emptyVector(std::vector<Feature*> &vector) {
      for (std::vector<Feature*>::iterator i = vector.begin(); i != vector.end();
           ++i) {
        delete *i;
      }
      vector.clear();
    }*/

    void setup() {
      initialiseFlatSamples();
      initialiseMeshSamples();
      initialiseRadialSamples();

      printf("\033[1;32m");
      printf("[Vision::setup()]Ready.");
      printf("\033[0m\n");
    }

    void initialiseFlatSamples() {
      for (unsigned int y = 0; y < CAMERA_HEIGHT; ++y) {
        std::vector<std::pair<unsigned int, unsigned int>> pointsInRow;
        for (unsigned int x = 0; x < CAMERA_WIDTH; ++x) {
          pointsInRow.push_back(std::pair<unsigned int, unsigned int>(x, y));
        }
        flatSamples.push_back(pointsInRow);
      }
    }

    void initialiseMeshSamples() {
      double step = 20; // TODO: Calibrate separate steps for horisontal and vertical coordinates
      std::set<std::pair<unsigned int, unsigned int>> seenPoints;
      for (double y = 0; y < FIELD_LENGTH; y += step) {
        std::vector<std::pair<unsigned int, unsigned int>> pointsInRow;
        for (double x = -FIELD_LENGTH; x <= FIELD_LENGTH; x += step) {
          std::pair<unsigned int, unsigned int> virtualPoint = Perspective::realToVirtual(x, y);
          if (virtualPoint.first < CAMERA_WIDTH && virtualPoint.second < CAMERA_HEIGHT) {
            if (seenPoints.find(virtualPoint) == seenPoints.end()) {
              pointsInRow.push_back(virtualPoint);
              seenPoints.insert(virtualPoint);
            }
          }
        }
        if (!pointsInRow.empty()) {
          meshSamples.push_back(pointsInRow);
        }
      }
    }

    void initialiseRadialSamples() {
      double step = 20;
      double count = 200; //FIELD_LENGTH * PI / step; // TODO: Calibrate radial count
      std::set<std::pair<unsigned int, unsigned int>> seenPoints;
      for (double angle = -PI / 2; angle <= PI / 2; angle += PI / count) { // TODO: Calibrate field of view
        std::vector<std::pair<unsigned int, unsigned int>> pointsInRay;
        for (double distance = 0; distance <= FIELD_LENGTH; distance += step) {
          double realHorisontal = distance * sin(angle);
          double realVertical = distance * cos(angle);
          std::pair<unsigned int, unsigned int> virtualPoint = Perspective::realToVirtual(realHorisontal, realVertical);
          if (virtualPoint.first < CAMERA_WIDTH && virtualPoint.second < CAMERA_HEIGHT) {
            if (seenPoints.find(virtualPoint) == seenPoints.end()) {
              pointsInRay.push_back(virtualPoint);
              seenPoints.insert(virtualPoint);
            }
          }
        }
        if (!pointsInRay.empty()) {
          radialSamples.push_back(pointsInRay);
        }
      }

      // DEBUG:
      /*unsigned int elementCount = 0, rayCount = 0;
      for (Samples::iterator ray = radialSamples.begin(); ray != radialSamples.end(); ++ray) {
        for (SampleRay::iterator sample = ray->begin(); sample != ray->end(); ++sample) {
          std::cout << "(" << sample->first << ", " << sample->second << ")" << " ";
          elementCount++;
        }
        rayCount++;
        std::cout << std::endl << std::endl;
      }
      std::cout << std::endl << std::endl;
      std::cout << "Points: " << elementCount << std::endl;
      std::cout << "Actual rays: " << rayCount << std::endl;*/
    }

    void process(const Frame &frame, const std::string &filter) {
      blobDetection(frame, filter, {0, 1, 2}, meshSamples);
      lineDetection(frame, filter, radialSamples);
      cornerDetection(frame, filter, meshSamples);
    }

    void processCheckerboard(const Frame &frame, const std::string &filter) {
      // TODO: Include other kind of samples (full board?)
      blobDetection(frame, filter, {6, 7});
      lineDetection(frame, filter);
      cornerDetection(frame, filter);
    }

    bool isColored(const Frame &frame, const std::string &filter, const unsigned int &pixel, const unsigned int &mode) {
      if (filter.size() > pixel) {
        //std::cout << (7 - mode) << " " << pixel << " " << filter[pixel] << " " << (filter[pixel] >> (7 - mode)) << " " << ((filter[pixel] >> (7 - mode)) & 0x1) << std::endl;
        return (filter[pixel] >> (7 - mode)) & 0x1;
      } else {
        //std::cout << "Filter is empty" << std::endl;
        return false;
      }
    }

    bool isColored(const Frame &frame, const std::string &filter, const unsigned int &x, const unsigned int &y, const unsigned int &z, const unsigned int &mode) {
      //std::cout << x << " " << y << " " << z << " " << (x << 16 + y << 8 + z) << std::endl;
      return isColored(frame, filter, (x << 16) + (y << 8) + z, mode);
    }

    BlobSet getBlobs() {
      BlobSet returnSet = blobs;
      while (editingBlobs) {
        returnSet = blobs;
      }
      return returnSet;
    }

    LineSet getLines() {
      LineSet returnSet = lines;
      while (editingLines) {
        returnSet = lines;
      }
      return returnSet;
    }

    CornerSet getCorners() {
      CornerSet returnSet = corners;
      while (editingCorners) {
        returnSet = corners;
      }
      return returnSet;
    }

    void translateBlobsBuffer() {
      editingBlobs = true;

      blobs.clear();
      blobs = blobsBuffer;
      blobsBuffer.clear();

      editingBlobs = false;
    }

    void translateLinesBuffer() {
      editingLines = true;

      lines.clear();
      lines = linesBuffer;
      linesBuffer.clear();

      editingLines = false;
    }

    void translateCornersBuffer() {
      editingCorners = true;

      corners.clear();
      corners = cornersBuffer;
      cornersBuffer.clear();

      editingCorners = false;
    }

    // Joins same-colored blobs if their box areas are close or overlap; remove too small blobs at the same time
    void joinBlobsInBuffer() {
      std::set<unsigned int> toBeRemoved;

      unsigned int minimumNumberOfPoints = 6; // TODO: Add to constants

      // Join blobs (robot blobs first) // TODO: Refactor hacky solution
      for (unsigned int i = 0; i < blobsBuffer.size(); ++i) {

        // If the blob with the index i is already supposed to be removed, move to the next one
        if (std::find(toBeRemoved.begin(), toBeRemoved.end(), i) != toBeRemoved.end())
          continue;

        // If the blob with the index i doesn't have enough points in it, remove it and move to the next one // TODO: Should we do it after blob joining operations? Or maybe both, with different constants?
        if (blobsBuffer[i]->getNumberOfPoints() < minimumNumberOfPoints) {
          toBeRemoved.insert(i);
          continue;
        }

        for (unsigned int j = 0; j < blobsBuffer.size() && std::find(toBeRemoved.begin(), toBeRemoved.end(), i) == toBeRemoved.end(); ++j) {

          // If the blob indexes match, continue to the next one with j - we do not want to join a blob with itself and then delete itself
          if (i == j)
            continue;

          // If the blob with the index j is already supposed to be removed, move to the next one
          if (std::find(toBeRemoved.begin(), toBeRemoved.end(), j) != toBeRemoved.end())
            continue;

          // If the blob with the index j doesn't have enough points in it, remove it and move to the next one // TODO: Should we do it after blob joining operations? Or maybe both, with different constants?
          if (blobsBuffer[j]->getNumberOfPoints() < minimumNumberOfPoints) {
            toBeRemoved.insert(j);
            continue;
          }

          // If the blobs are close to each other (and of the same color type), join them
          if (blobsBuffer[i]->isClose(*blobsBuffer[j])) {

            // If the blobs are not of the same color, and the first blob is not a robot blob (but could be)
            if (!blobsBuffer[i]->isSameColor(*blobsBuffer[j]) && (blobsBuffer[i]->isBlue() || blobsBuffer[i]->isYellow())) {

              // If the second blob is not a robot blob
              if (blobsBuffer[j]->isBlue() || blobsBuffer[j]->isYellow()) {

                // It does not matter which blob we add to which one, as we change the color anyway
                blobsBuffer[i]->join(*blobsBuffer[j]);
                toBeRemoved.insert(j);

                // We now need to change the first blobs color type to the appropriate robot blob color type

                // We can assume that the blobs are not of the same color, but both of them are either yellow or blue; therefore, we need to join them based on which one is above the other

                // If the first blob is yellow and the second blob is blue
                if (blobsBuffer[i]->isYellow()) {

                  // As the first blob is yellow, if the first blob is above the other one, the robot is of the yellow-blue marking; otherwise, it is of the blue-yellow marking
                  if (blobsBuffer[i]->isAbove(*blobsBuffer[j])) {
                    blobsBuffer[i]->setColor(ROBOT_YELLOW_BLUE);
                  } else {
                    blobsBuffer[i]->setColor(ROBOT_BLUE_YELLOW);
                  }

                // If the first blob is blue and the second blob is yellow
                } else {

                  // As the first blob is yellow, if the first blob is above the other one, the robot is of the blue-yellow marking; otherwise, it is of the yellow-blue marking
                  if (blobsBuffer[i]->isAbove(*blobsBuffer[j])) {
                    blobsBuffer[i]->setColor(ROBOT_BLUE_YELLOW);
                  } else {
                    blobsBuffer[i]->setColor(ROBOT_YELLOW_BLUE);
                  }

                }

              // If the second blob is a robot blob
              } else {

                // We can assume that the second blob is a robot blob and the first one is either yellow or blue, therefore, we can simply join the first blob with the second one, and delete the first one.
                blobsBuffer[j]->join(*blobsBuffer[i]);
                toBeRemoved.insert(i);

              }

            // If the blobs are of the same color, or the first blob is a robot blob
            } else if (blobsBuffer[i]->isYellowBlue() || blobsBuffer[i]->isBlueYellow()) {

              // We can assume that the blobs are the exact same color, or that the first blob is already a robot blob and the second blob is either yellow or blue, therefore, we can simply join the second blob with the first one, and delete the other one.
              blobsBuffer[i]->join(*blobsBuffer[j]);
              toBeRemoved.insert(j);

            }

          }

        }

      }

      // Remove unnecessary blobs from the buffer // TODO: Refactor
      unsigned int removed = 0;
      for (std::set<unsigned int>::iterator i = toBeRemoved.begin(); i != toBeRemoved.end(); ++i) {
        blobsBuffer.erase(blobsBuffer.begin() + *i - removed);
        removed++;
      }
      toBeRemoved.clear();

      // Join blobs
      for (unsigned int i = 0; i < blobsBuffer.size(); ++i) {

        // If the blob with the index i is already supposed to be removed, move to the next one
        if (std::find(toBeRemoved.begin(), toBeRemoved.end(), i) != toBeRemoved.end())
          continue;

        // If the blob with the index i doesn't have enough points in it, remove it and move to the next one // TODO: Should we do it after blob joining operations? Or maybe both, with different constants?
        if (blobsBuffer[i]->getNumberOfPoints() < minimumNumberOfPoints) {
          toBeRemoved.insert(i);
          continue;
        }

        for (unsigned int j = 0; j < blobsBuffer.size() && std::find(toBeRemoved.begin(), toBeRemoved.end(), i) == toBeRemoved.end(); ++j) {

          // If the blob indexes match, continue to the next one with j - we do not want to join a blob with itself and then delete itself
          if (i == j)
            continue;

          // If the blob with the index j is already supposed to be removed, move to the next one
          if (std::find(toBeRemoved.begin(), toBeRemoved.end(), j) != toBeRemoved.end())
            continue;

          // If the blob with the index j doesn't have enough points in it, remove it and move to the next one // TODO: Should we do it after blob joining operations? Or maybe both, with different constants?
          if (blobsBuffer[j]->getNumberOfPoints() < minimumNumberOfPoints) {
            toBeRemoved.insert(j);
            continue;
          }

          // If the blobs are close to each other (and of the same color type), join them
          if (blobsBuffer[i]->isClose(*blobsBuffer[j])) {

            // If the blobs are not of the same color, and the first blob is not a robot blob (but could be)
            if (!blobsBuffer[i]->isSameColor(*blobsBuffer[j]) && (blobsBuffer[i]->isBlue() || blobsBuffer[i]->isYellow())) {

              // If the second blob is not a robot blob
              if (blobsBuffer[j]->isBlue() || blobsBuffer[j]->isYellow()) {

                // It does not matter which blob we add to which one, as we change the color anyway
                blobsBuffer[i]->join(*blobsBuffer[j]);
                toBeRemoved.insert(j);

                // We now need to change the first blobs color type to the appropriate robot blob color type

                // We can assume that the blobs are not of the same color, but both of them are either yellow or blue; therefore, we need to join them based on which one is above the other

                // If the first blob is yellow and the second blob is blue
                if (blobsBuffer[i]->isYellow()) {

                  // As the first blob is yellow, if the first blob is above the other one, the robot is of the yellow-blue marking; otherwise, it is of the blue-yellow marking
                  if (blobsBuffer[i]->isAbove(*blobsBuffer[j])) {
                    blobsBuffer[i]->setColor(ROBOT_YELLOW_BLUE);
                  } else {
                    blobsBuffer[i]->setColor(ROBOT_BLUE_YELLOW);
                  }

                // If the first blob is blue and the second blob is yellow
                } else {

                  // As the first blob is yellow, if the first blob is above the other one, the robot is of the blue-yellow marking; otherwise, it is of the yellow-blue marking
                  if (blobsBuffer[i]->isAbove(*blobsBuffer[j])) {
                    blobsBuffer[i]->setColor(ROBOT_BLUE_YELLOW);
                  } else {
                    blobsBuffer[i]->setColor(ROBOT_YELLOW_BLUE);
                  }

                }

              // If the second blob is a robot blob
              } else {

                // We can assume that the second blob is a robot blob and the first one is either yellow or blue, therefore, we can simply join the first blob with the second one, and delete the first one.
                blobsBuffer[j]->join(*blobsBuffer[i]);
                toBeRemoved.insert(i);

              }

            // If the blobs are of the same color, or the first blob is a robot blob
            } else {

              // We can assume that the blobs are the exact same color, or that the first blob is already a robot blob and the second blob is either yellow or blue, therefore, we can simply join the second blob with the first one, and delete the other one.
              blobsBuffer[i]->join(*blobsBuffer[j]);
              toBeRemoved.insert(j);

            }

          }

        }

      }

      // Remove unnecessary blobs from the buffer // TODO: Refactor
      removed = 0;
      for (std::set<unsigned int>::iterator i = toBeRemoved.begin(); i != toBeRemoved.end(); ++i) {
        blobsBuffer.erase(blobsBuffer.begin() + *i - removed);
        removed++;
      }
      toBeRemoved.clear();

    }

    void filterBlobsInBufferBySize() {
      std::set<unsigned int> toBeRemoved;
      for (unsigned int i = 0; i < blobsBuffer.size(); ++i) {
        // If the current blob is a goal or ball blob
        if (blobsBuffer[i]->isBlue() || blobsBuffer[i]->isYellow() || blobsBuffer[i]->isOrange()) {
          // If the blob's height is smaller than half of the expected height at that position, remove the blob from the blob list.
          if (blobsBuffer[i]->getHeight() < 0.5 * getBlobExpectedVirtualSize(blobsBuffer[i]->getColor(), blobsBuffer[i]->getPosition()).second) {
            toBeRemoved.insert(i);
          }
        }
      }

      // Remove unnecessary blobs from the buffer // TODO: Refactor
      unsigned int removed = 0;
      for (std::set<unsigned int>::iterator i = toBeRemoved.begin(); i != toBeRemoved.end(); ++i) {
        blobsBuffer.erase(blobsBuffer.begin() + *i - removed);
        removed++;
      }
      toBeRemoved.clear();
    }

    void blobDetection(const Frame &frame, const std::string &filter, const std::vector<unsigned int> &modeList) {
      blobDetection(frame, filter, modeList, flatSamples);
    }

    void blobDetection(const Frame &frame, const std::string &filter, const std::vector<unsigned int> &modeList, const std::vector<std::vector<std::pair<unsigned int, unsigned int>>> &samples) {
      blobsBuffer.clear();

      std::vector<std::vector<std::vector<bool>>> visited(8, std::vector<std::vector<bool>>(CAMERA_WIDTH, std::vector<bool>(CAMERA_HEIGHT, false))); // TODO: Optimise

      unsigned char *pixels = frame.data;
      unsigned int channels = 3;
      unsigned int stride = frame.width * channels;

      for (std::vector<unsigned int>::const_iterator mode = modeList.begin(); mode != modeList.end(); ++mode) {

        for (std::vector<std::vector<std::pair<unsigned int, unsigned int>>>::const_iterator ray = samples.begin(); ray != samples.end(); ++ray) {
          for (std::vector<std::pair<unsigned int, unsigned int>>::const_iterator sample = ray->begin(); sample != ray->end(); ++sample) {

            if (!visited[*mode][sample->first][sample->second]) {

              std::vector<std::pair<unsigned int, unsigned int>> blobPoints;
              std::vector<std::pair<unsigned int, unsigned int>> stack;
              stack.push_back(std::pair<unsigned int, unsigned int>(sample->first, sample->second));
              visited[*mode][sample->first][sample->second] = true;

              while (!stack.empty()) {
                std::pair<unsigned int, unsigned int> point = stack.back();
                stack.pop_back();

                unsigned char *pixel = pixels + point.first * channels + point.second * stride;

                if (isColored(frame, filter, pixel[0], pixel[1], pixel[2], *mode)) {
                  blobPoints.push_back(point);
                  for (int step = -1; step <= 1; step += 2) {
                    if (point.first + step < CAMERA_WIDTH && point.first + step >= 0) {
                      std::pair<unsigned int, unsigned int> newPoint(point.first + step, point.second);
                      if (!visited[*mode][newPoint.first][newPoint.second]) {
                        stack.push_back(newPoint);
                        visited[*mode][newPoint.first][newPoint.second] = true;
                      }
                    }
                    if (point.second + step < CAMERA_HEIGHT && point.second + step >= 0) {
                      std::pair<unsigned int, unsigned int> newPoint(point.first, point.second + step);
                      if (!visited[*mode][newPoint.first][newPoint.second]) {
                        stack.push_back(newPoint);
                        visited[*mode][newPoint.first][newPoint.second] = true;
                      }
                    }
                  }
                }

              }

              if (!blobPoints.empty()) {
                blobsBuffer.push_back(new Blob(blobPoints, intToColor(*mode)));
              }

            }

          }
        }

      }

      joinBlobsInBuffer();

      filterBlobsInBufferBySize();

      translateBlobsBuffer();

      // DEBUG:
      /*for (BlobSet::iterator blob = blobs.begin(); blob != blobs.end(); ++blob) {
        std::pair<unsigned int, unsigned int> expectedVirtualSize = (*blob)->getExpectedVirtualSize();
        std::cout << "Blob: " << "(" << (*blob)->getCentroid()->getX() << ", " << (*blob)->getCentroid()->getY() << ") " << "(" << ((*blob)->getMaxX() - (*blob)->getMinX()) << ", " << ((*blob)->getMaxY() - (*blob)->getMinY()) << ") " << "(" << expectedVirtualSize.first << ", " << expectedVirtualSize.second << ") " << std::endl;
      }
      std::cout << std::endl << std::endl;*/
    }

    void lineDetection(const Frame &frame, const std::string &filter) {
      lineDetection(frame, filter, flatSamples);
    }

    void lineDetection(const Frame &frame, const std::string &filter, const std::vector<std::vector<std::pair<unsigned int, unsigned int>>>&) {
      // TODO

      translateLinesBuffer();
    }

    void cornerDetection(const Frame &frame, const std::string &filter) {
      cornerDetection(frame, filter, flatSamples);
    }

    void cornerDetection(const Frame &frame, const std::string &filter, const std::vector<std::vector<std::pair<unsigned int, unsigned int>>>&) {
      // TODO

      translateCornersBuffer();
    }

  };

}
