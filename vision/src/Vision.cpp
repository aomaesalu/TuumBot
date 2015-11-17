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


namespace rtx {

  namespace Vision {

    BlobSet blobs;
    LineSet lines;
    CornerSet corners;

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

    void blobDetection(const Frame &frame, const std::string &filter, const std::vector<unsigned int> &modeList) {
      blobs.clear();

      std::vector<std::vector<std::vector<bool>>> visited(8, std::vector<std::vector<bool>>(CAMERA_WIDTH, std::vector<bool>(CAMERA_HEIGHT, false)));

      unsigned char *pixels = frame.data;
      unsigned int channels = 3;
      unsigned int stride = frame.width * channels;

      for (unsigned int i = 0; i < CAMERA_WIDTH; i += 5) {
        for (unsigned int j = 0; j < CAMERA_HEIGHT; j += 5) {

          for (std::vector<unsigned int>::const_iterator mode = modeList.begin(); mode != modeList.end(); ++mode) {
            if (!visited[*mode][i][j]) {

              std::vector<std::pair<unsigned int, unsigned int>> blobPoints;
              std::vector<std::pair<unsigned int, unsigned int>> stack;
              stack.push_back(std::pair<unsigned int, unsigned int>(i, j));
              while (!stack.empty()) {
                std::pair<unsigned int, unsigned int> point = stack.back();
                stack.pop_back();

                visited[*mode][point.first][point.second] = true;

                unsigned char *pixel = pixels + point.first * channels + point.second * stride;

                if (isColored(frame, filter, pixel[0], pixel[1], pixel[2], *mode)) {
                  blobPoints.push_back(point);
                  if (point.first - 1 < CAMERA_WIDTH - 1) {
                    std::pair<unsigned int, unsigned int> newPoint(point.first - 1, point.second);
                    if (!visited[*mode][point.first - 1][point.second]) {
                      stack.push_back(newPoint);
                      visited[*mode][point.first - 1][point.second] = true;
                    }
                  }
                  if (point.first + 1 < CAMERA_WIDTH - 1) {
                    std::pair<unsigned int, unsigned int> newPoint(point.first + 1, point.second);
                    if (!visited[*mode][point.first + 1][point.second]) {
                      stack.push_back(newPoint);
                      visited[*mode][point.first + 1][point.second] = true;
                    }
                  }
                  if (point.second - 1 < CAMERA_HEIGHT - 1) {
                    std::pair<unsigned int, unsigned int> newPoint(point.first, point.second - 1);
                    if (!visited[*mode][point.first][point.second - 1]) {
                      stack.push_back(newPoint);
                      visited[*mode][point.first][point.second - 1] = true;
                    }
                  }
                  if (point.second + 1 < CAMERA_HEIGHT - 1) {
                    std::pair<unsigned int, unsigned int> newPoint(point.first, point.second + 1);
                    if (!visited[*mode][point.first][point.second + 1]) {
                      stack.push_back(newPoint);
                      visited[*mode][point.first][point.second + 1] = true;
                    }
                  }
                }

              }

              if (!blobPoints.empty()) {
                blobs.push_back(new Blob(blobPoints, intToColor(*mode)));
                blobPoints.clear();
              }

            }
          }

        }
      }
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
