/**
 *  @file Vision.cpp
 *  Computer vision class using YUYV.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.2
 *  @date 26 November 2015
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
    void joinBlobsInBuffer() { // TODO: Refactor to remove duplicate code
      std::set<unsigned int> toBeRemoved;

      // Join blobs
      for (unsigned int i = 0; i < blobsBuffer.size(); ++i) {

        if (std::find(toBeRemoved.begin(), toBeRemoved.end(), i) != toBeRemoved.end())
          continue;

        if (blobsBuffer[i]->getNumberOfPoints() < 10) {
          toBeRemoved.insert(i);
          continue;
        }

        for (unsigned int j = 0; j < blobsBuffer.size(); ++j) {

          if (i == j)
            continue;

          if (std::find(toBeRemoved.begin(), toBeRemoved.end(), j) != toBeRemoved.end())
            continue;

          if (blobsBuffer[j]->getNumberOfPoints() < 6) {
            toBeRemoved.insert(j);
            continue;
          }

          if (blobsBuffer[i]->isSameColor(*blobsBuffer[j])) {

            if (blobsBuffer[i]->getColor() == ROBOT_YELLOW_BLUE || blobsBuffer[i]->getColor() == ROBOT_BLUE_YELLOW) {

              if (blobsBuffer[i]->isClose(*blobsBuffer[j], 0.5)) { // Checks overlapping, too // TODO: Calibrate closeness indicator
                blobsBuffer[i]->join(*blobsBuffer[j]);
                toBeRemoved.insert(j);
              }

            } else {

              if (blobsBuffer[i]->isClose(*blobsBuffer[j], 0.25)) { // Checks overlapping, too // TODO: Calibrate closeness indicator
                blobsBuffer[i]->join(*blobsBuffer[j]);
                toBeRemoved.insert(j);
              }

            }

          } else {

            if (blobsBuffer[i]->getColor() == ROBOT_YELLOW_BLUE || blobsBuffer[i]->getColor() == ROBOT_BLUE_YELLOW) {
              if ((blobsBuffer[j]->isBlue() || blobsBuffer[j]->isYellow()) && blobsBuffer[i]->isClose(*blobsBuffer[j], 0.5) && blobsBuffer[j]->getHeight() / (double) blobsBuffer[i]->getHeight() < 2) { // Checks overlapping, too // TODO: Calibrate closeness indicator, calibrate height relation indicator
                // (yellow-blue, blue) OR (yellow-blue, yellow) OR (blue-yellow, blue) OR (blue-yellow, yellow)
                blobsBuffer[i]->join(*blobsBuffer[j]);
                toBeRemoved.insert(j);
              }

            } else if (blobsBuffer[i]->isBlue() || blobsBuffer[i]->isYellow()) {

              if (blobsBuffer[j]->getColor() == ROBOT_YELLOW_BLUE || blobsBuffer[j]->getColor() == ROBOT_BLUE_YELLOW) {
                // (blue, yellow-blue) OR (yellow, yellow-blue) OR (blue, blue-yellow) OR (yellow, blue-yellow)
                if (blobsBuffer[i]->isClose(*blobsBuffer[j], 0.5) && blobsBuffer[i]->getHeight() / (double) blobsBuffer[j]->getHeight() < 2) { // Checks overlapping, too // TODO: Calibrate closeness indicator, calibrate height relation indicator
                  blobsBuffer[j]->join(*blobsBuffer[i]);
                  toBeRemoved.insert(i);
                }

              } else if (blobsBuffer[j]->isBlue()) {
                // (yellow, blue)
                if (blobsBuffer[i]->isClose(*blobsBuffer[j], 0.5) && blobsBuffer[i]->getHeight() / (double) blobsBuffer[j]->getHeight() < 3 && blobsBuffer[j]->getHeight() / (double) blobsBuffer[i]->getHeight() < 3) { // Checks overlapping, too // TODO: Calibrate closeness indicator, calibrate height relation indicator
                  blobsBuffer[i]->join(*blobsBuffer[j]);
                  toBeRemoved.insert(j);

                  if (blobsBuffer[i]->isAbove(*blobsBuffer[j])) {
                    blobsBuffer[i]->setColor(ROBOT_YELLOW_BLUE);
                  } else {
                    blobsBuffer[i]->setColor(ROBOT_BLUE_YELLOW);
                  }

                }

              } else if (blobsBuffer[j]->isYellow()) {
                // (blue, yellow)
                if (blobsBuffer[i]->isClose(*blobsBuffer[j], 0.5) && blobsBuffer[i]->getHeight() / (double) blobsBuffer[j]->getHeight() < 3 && blobsBuffer[j]->getHeight() / (double) blobsBuffer[i]->getHeight() < 3) { // Checks overlapping, too // TODO: Calibrate closeness indicator, calibrate height relation indicator
                  blobsBuffer[i]->join(*blobsBuffer[j]);
                  toBeRemoved.insert(j);

                  if (blobsBuffer[i]->isAbove(*blobsBuffer[j])) {
                    blobsBuffer[i]->setColor(ROBOT_BLUE_YELLOW);
                  } else {
                    blobsBuffer[i]->setColor(ROBOT_YELLOW_BLUE);
                  }

                }

              }

            }

          }

        }

      }

      unsigned int removed = 0; // TODO: Refactor

      // Remove unnecessary blobs from the buffer
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

      translateBlobsBuffer();
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
