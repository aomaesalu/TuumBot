/**
 * @file Vision.cpp
 * Computer vision class using YUYV.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 *  @date 11. November 2015
 */

#include "Vision.hpp"


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
      blobDetection(frame, filter);
      lineDetection(frame, filter);
      cornerDetection(frame, filter);
    }

    bool isColored(const Frame &frame, const std::string &filter, const unsigned int &pixel, const unsigned int &mode) {
      if (filter.size() > pixel) {
        return (filter[pixel] >> (7 - mode)) & 0x1;
      } else {
        return false;
      }
    }

    bool isColored(const Frame &frame, const std::string &filter, const unsigned int &x, const unsigned int &y, const unsigned int &z, const unsigned int &mode) {
      return isColored(frame, filter, x * 256 * 256 + y * 256 + z, mode);
    }

    void blobDetection(const Frame &frame, const std::string &filter) {
      blobs.clear();

      std::vector<std::vector<std::vector<bool>>> visited(8, std::vector<std::vector<bool>>(CAMERA_WIDTH, std::vector<bool>(CAMERA_HEIGHT, false)));

      unsigned char *pixels = frame.data;
      unsigned int channels = 3;
      unsigned int stride = frame.width * channels;

      for (unsigned int i = 0; i < CAMERA_WIDTH; ++i) {
        for (unsigned int j = 0; j < CAMERA_HEIGHT; ++j) {

          for (unsigned int mode = 0; mode < 8; ++mode) {
            if (!visited[mode][i][j]) {

              std::vector<std::pair<unsigned int, unsigned int>> blobPoints;
              std::vector<std::pair<unsigned int, unsigned int>> stack;
              stack.push_back(std::pair<unsigned int, unsigned int>(i, j));
              while (!stack.empty()) {
                std::pair<unsigned int, unsigned int> point = stack.back();
                stack.pop_back();
                if (!visited[mode][point.first][point.second]) { //  Do we need to check it here? We check it again later...
                  visited[mode][point.first][point.second] = true;
                  blobPoints.push_back(point);

                  unsigned char *pixel = pixels + point.first * channels + point.second * stride;
                  if (isColored(frame, filter, pixel[0], pixel[1], pixel[2], mode)) {
                    if (point.first > 0) {
                      std::pair<unsigned int, unsigned int> newPoint(point.first - 1, point.second);
                      if (!visited[mode][point.first][point.second]) {
                        stack.push_back(newPoint);
                      }
                    }
                    if (point.first < CAMERA_WIDTH - 1) {
                      std::pair<unsigned int, unsigned int> newPoint(point.first + 1, point.second);
                      if (!visited[mode][point.first][point.second]) {
                        stack.push_back(newPoint);
                      }
                    }
                    if (point.second > 0) {
                      std::pair<unsigned int, unsigned int> newPoint(point.first, point.second - 1);
                      if (!visited[mode][point.first][point.second]) {
                        stack.push_back(newPoint);
                      }
                    }
                    if (point.second < CAMERA_HEIGHT - 1) {
                      std::pair<unsigned int, unsigned int> newPoint(point.first, point.second + 1);
                      if (!visited[mode][point.first][point.second]) {
                        stack.push_back(newPoint);
                      }
                    }
                  }

                }
              }

              blobs.push_back(new Blob(blobPoints, intToColor(mode)));

            }
          }

        }
      }
    }

    void blobDetection(const Frame &frame, const std::string &filter, const std::vector<Point2D> &samples) {
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
