/**
 * @file Vision.cpp
 * Computer vision class using YUYV.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 *  @date 17 November 2015
 */

#include "Vision.hpp"

#include <iostream> // TODO: Remove
#include <algorithm>
#include <set>


namespace rtx {

  namespace Vision {

    BlobSet blobs;
    BlobSet blobsBuffer;
    LineSet lines;
    CornerSet corners;

    bool editingBlobs = false;

    /*void emptyVector(std::vector<Feature*> &vector) {
      for (std::vector<Feature*>::iterator i = vector.begin(); i != vector.end();
           ++i) {
        delete *i;
      }
      vector.clear();
    }*/

    void setup() {
      printf("\033[1;32m");
      printf("[Vision::setup()]Ready.");
      printf("\033[0m\n");
    }

    void process(const Frame &frame, const std::string &filter) {
      blobDetection(frame, filter, {0, 1, 2});
      lineDetection(frame, filter);
      cornerDetection(frame, filter);
    }

    void processCheckerboard(const Frame &frame, const std::string &filter) {
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

    void translateBlobsBuffer() {
      editingBlobs = true;

      blobs.clear();
      blobs = blobsBuffer;
      blobsBuffer.clear();

      editingBlobs = false;
    }

    // Joins same-colored blobs if their box areas overlap
    void joinBlobsInBuffer() {
      std::set<unsigned int> toBeRemoved;

      // Join blobs
      for (unsigned int i = 0; i < blobsBuffer.size(); ++i) {
        if (std::find(toBeRemoved.begin(), toBeRemoved.end(), i) == toBeRemoved.end()) {
          for (unsigned int j = i + 1; j < blobsBuffer.size(); ++j) {
            if (std::find(toBeRemoved.begin(), toBeRemoved.end(), j) == toBeRemoved.end()) {
              if (blobsBuffer[i]->overlaps(*blobsBuffer[j])) {
                blobsBuffer[i]->join(*blobsBuffer[j]);
                toBeRemoved.insert(j);
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
      blobsBuffer.clear();

      std::vector<std::vector<std::vector<bool>>> visited(8, std::vector<std::vector<bool>>(CAMERA_WIDTH, std::vector<bool>(CAMERA_HEIGHT, false))); // TODO: Optimise

      unsigned char *pixels = frame.data;
      unsigned int channels = 3;
      unsigned int stride = frame.width * channels;

      for (std::vector<unsigned int>::const_iterator mode = modeList.begin(); mode != modeList.end(); ++mode) {
        for (unsigned int i = 0; i < CAMERA_WIDTH; i += 5) {
          for (unsigned int j = 0; j < CAMERA_HEIGHT; j += 5) {

            if (!visited[*mode][i][j]) {

              std::vector<std::pair<unsigned int, unsigned int>> blobPoints;
              std::vector<std::pair<unsigned int, unsigned int>> stack;
              stack.push_back(std::pair<unsigned int, unsigned int>(i, j));
              visited[*mode][i][j] = true;

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

    void blobDetection(const Frame &frame, const std::string &filter, const std::vector<unsigned int> &modeList, const std::vector<Point2D> &samples) {
      // TODO
    }

    void lineDetection(const Frame &frame, const std::string &filter) {
      // TODO
    }

    void lineDetection(const Frame &frame, const std::string &filter, const std::vector<Point2D> &samples) {
      // TODO
    }

    void cornerDetection(const Frame &frame, const std::string &filter) {
      // TODO
    }

    void cornerDetection(const Frame &frame, const std::string &filter, const std::vector<Point2D> &samples) {
      // TODO
    }

  };

};
