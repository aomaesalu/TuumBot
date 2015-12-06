/**
 *  @file ImageDrawingArea.cpp
 *  Perspective calibration application image drawing area.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 3 December 2015
 */

#include "ImageDrawingArea.hpp"

#include "MainWindow.hpp"
#include "cameraConstants.hpp"

#include <cairomm/context.h>
#include <gdkmm/general.h>
#include <gdkmm/pixbuf.h>
#include <glibmm/refptr.h>
#include <glibmm/fileutils.h>

#include <iostream> // TODO: Remove
#include <cstdlib>
#include <algorithm>


namespace rtx {

  // TODO: Move elsewhere
  struct verticalResultsSortPredicate {
    bool operator()(const std::pair<std::pair<std::pair<double, double>, std::pair<double, double>>, double> &left, const std::pair<std::pair<std::pair<double, double>, std::pair<double, double>>, double> &right) {
        return left.second < right.second;
    }
  };

  // TODO: Move elsewhere
  struct horisontalResultsSortPredicate {
    bool operator()(const std::pair<std::pair<double, double>, double> &left, const std::pair<std::pair<double, double>, double> &right) {
        return left.second < right.second;
    }
  };

  void partitionList(std::vector<double> &list, const unsigned int &numberOfDivisions) {
    unsigned int size = list.size();
    for (unsigned int j = 0; j < size; j += 2) {
      double difference = (list[j + 1] - list[j]) / numberOfDivisions;
      for (unsigned m = 0; m < numberOfDivisions; ++m) {
        list.push_back(list[j] + m * difference);
        list.push_back(list[j] + (m + 1) * difference); // TODO: Optimise
      }
    }
    list.erase(list.begin(), list.begin() + size);
  }

  ImageDrawingArea::ImageDrawingArea(MainWindow *mainWindow):
    mainWindow(mainWindow)
  {
    initialiseProperties();
    initialiseImage();
    initialiseConstants();
    initialiseBlobRegression();
  }

  ImageDrawingArea::~ImageDrawingArea() {
    // Nothing to do here
  }

  bool ImageDrawingArea::isPlaying() const {
    return mainWindow->isPlaying();
  }

  bool ImageDrawingArea::isCalculating() const {
    return mainWindow->isCalculating();
  }

  double ImageDrawingArea::getA() const {
    return bestA;
  }

  double ImageDrawingArea::getB() const {
    return bestB;
  }

  double ImageDrawingArea::getC() const {
    return bestC;
  }

  void ImageDrawingArea::updateFrame(Frame *frame, Frame *rgbFrame) {
    this->frame = frame;
    this->rgbFrame = rgbFrame;
    image = Gdk::Pixbuf::create_from_data((const guint8*) rgbFrame->data, Gdk::COLORSPACE_RGB, false, 8, (int) rgbFrame->width, (int) rgbFrame->height, (int) rgbFrame->width * 3);
    queue_draw();
  }

  bool ImageDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cairo) {
    if (!applyCalculations())
      return false;

    if (!drawImage(cairo))
      return false;

    cairo->paint();

    return true;
  }

  void ImageDrawingArea::initialiseProperties() {
    set_size_request(CAMERA_WIDTH, CAMERA_HEIGHT);
  }

  void ImageDrawingArea::initialiseImage() {
    image = Gdk::Pixbuf::create_from_file("frame.ppm"); // TODO: Remove association with files
    //image = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, (int) image->get_width(), (int) image->get_height());

    // Show the whole image
    if (image)
      set_size_request(image->get_width(), image->get_height());
  }

  void ImageDrawingArea::initialiseConstants() {
    numberOfDivisions = 16;
    numberOfBestDivisions = 4;
    bestA = bestB = bestC = 0;
    lowerBound = -100000;
    upperBound = 100000;
    AList.clear();
    AList.push_back(lowerBound);
    AList.push_back(upperBound);
    partitionList(AList, numberOfDivisions);
    BList.clear();
    BList.push_back(lowerBound);
    BList.push_back(upperBound);
    partitionList(BList, numberOfDivisions);
    ABList.clear();
    for (std::vector<double>::iterator a = AList.begin(); a != AList.end(); a += 2) {
      for (std::vector<double>::iterator b = BList.begin(); b != BList.end(); b += 2) {
        ABList.push_back(std::pair<std::pair<double, double>, std::pair<double, double>>(std::pair<double, double>(*a, *(a + 1)), std::pair<double, double>(*b, *(b + 1))));
      }
    }
    AList.clear();
    BList.clear();
    CList.clear();
    CList.push_back(lowerBound);
    CList.push_back(upperBound);
    partitionList(CList, numberOfDivisions);
    maxError = 10;
    squareWidth = 25; // In millimeters; TODO: Move to constants file? Or ask from the user
    bestHorisontalMSE = 9999999;
    bestVerticalMSE = 9999999;
  }

  void ImageDrawingArea::resetConstants() {
    initialiseConstants();
  }

  void ImageDrawingArea::initialiseBlobRegression() {
    totalCount = 0;
  }

  void ImageDrawingArea::resetBlobRegression() {
    initialiseBlobRegression();
    blobCounts.clear();
  }

  void ImageDrawingArea::colorPixels(guint8 *pixels, const unsigned int &channels, const unsigned int &stride, guint8 *actualPixels, const unsigned int &actualChannels, const unsigned int &actualStride) {
    // Color pixels
    for (unsigned int i = 0; i < CAMERA_WIDTH; ++i) {
      for (unsigned int j = 0; j < CAMERA_HEIGHT; ++j) {
        guint8 *pixel = pixels + i * channels + j * stride;
        guint8 *actualPixel = actualPixels + i * actualChannels + j * actualStride;
        if (mainWindow->isColored(actualPixel[0], actualPixel[1], actualPixel[2], 6)) {
          if (mainWindow->isColored(actualPixel[0], actualPixel[1], actualPixel[2], 7)) {
            pixel[0] = 127;
            pixel[1] = 127;
            pixel[2] = 127;
          } else {
            pixel[0] = 235;
            pixel[1] = 235;
            pixel[2] = 235;
          }
        } else if (mainWindow->isColored(actualPixel[0], actualPixel[1], actualPixel[2], 7)) {
          pixel[0] = 0;
          pixel[1] = 0;
          pixel[2] = 0;
        }
      }
    }
  }

  void ImageDrawingArea::regressBlobs() {
    Vision::BlobSet blobs = Vision::blobs;
    while (Vision::editingBlobs) {
      blobs = Vision::blobs;
    }

    totalCount++;

    unsigned int maxDifference = 2;

    std::vector<Vision::Blob*> additionalBlobs;

    for (Vision::BlobSet::iterator blob = blobs.begin(); blob != blobs.end(); ++blob) {
      for (std::map<Vision::Blob*, unsigned int>::iterator blobOccurrence = blobCounts.begin(); blobOccurrence != blobCounts.end(); ++blobOccurrence) {
        unsigned int dx = abs(blobOccurrence->first->getPosition()->getX() - (*blob)->getPosition()->getX());
        unsigned int dy = abs(blobOccurrence->first->getPosition()->getY() - (*blob)->getPosition()->getY());
        if (dx * dx + dy * dy <= maxDifference * maxDifference) {
          blobOccurrence->second++;
          continue;
        }
      }
      additionalBlobs.push_back(*blob);
    }

    for (unsigned int i = 0; i < additionalBlobs.size(); ++i) {
      blobCounts.insert(std::pair<Vision::Blob*, unsigned int>(additionalBlobs[i], 1));
    }
  }

  void ImageDrawingArea::colorBlobs(guint8 *pixels, const unsigned int &channels, const unsigned int &stride) {
    for (std::map<Vision::Blob*, unsigned int>::iterator blobOccurrence = blobCounts.begin(); blobOccurrence != blobCounts.end(); ++blobOccurrence) {
      if (blobOccurrence->second < totalCount / 2) {
        continue;
      }
      Vision::Blob *blob = blobOccurrence->first;
      if (blob) {
        unsigned int x = blob->getCentroid()->getX();
        unsigned int y = blob->getCentroid()->getY();
        unsigned int minX = blob->getMinX();
        unsigned int maxX = blob->getMaxX();
        unsigned int minY = blob->getMinY();
        unsigned int maxY = blob->getMaxY();
        if (minX >= CAMERA_WIDTH || maxX >= CAMERA_WIDTH || minY >= CAMERA_HEIGHT || maxY >= CAMERA_HEIGHT) {
          continue;
        }
        Vision::Color color = blob->getColor();
        double density = blob->getDensity();
        unsigned int boxArea = blob->getBoxArea();

        if (boxArea > 20 * 20 && density <= 1.0 && boxArea <= CAMERA_WIDTH * CAMERA_HEIGHT) {
          unsigned int value = 0;
          if (color == CHECKERBOARD_WHITE) {
            value = 255;
          }
          for (unsigned int i = minX; i <= maxX; ++i) {
            guint8 *pixel = pixels + i * channels + minY * stride;
            for (unsigned int p = 0; p < 3; ++p) {
              pixel[p] = value;
            }
            pixel = pixels + i * channels + maxY * stride;
            for (unsigned int p = 0; p < 3; ++p) {
              pixel[p] = value;
            }
          }
          for (unsigned int j = minY; j <= maxY; ++j) {
            guint8 *pixel = pixels + minX * channels + j * stride;
            for (unsigned int p = 0; p < 3; ++p) {
              pixel[p] = value;
            }
            pixel = pixels + maxX * channels + j * stride;
            for (unsigned int p = 0; p < 3; ++p) {
              pixel[p] = value;
            }
          }

          unsigned int firstValue = 255, secondValue = 0;
          if (color == CHECKERBOARD_WHITE) {
            firstValue = 0;
            secondValue = 255;
          }
          for (int dx = -3; dx < 3; ++dx) {
            for (int dy = -3; dy < 3; ++dy) {
              if (x + dx < CAMERA_WIDTH && x + dx >= 0 && y + dy < CAMERA_HEIGHT && y + dy >= 0) {
                guint8 *pixel = pixels + (x + dx) * channels + (y + dy) * stride;
                pixel[0] = firstValue;
                pixel[1] = 0;
                pixel[2] = secondValue;
              }
            }
          }
        }

      }
    }
  }

  double getVerticalDistance(const unsigned int &verticalCoordinate, const double &A, const double &B) {
    return A + B / verticalCoordinate;
  }

  double getHorisontalDistance(const int &horisontalCoordinate, const unsigned int &verticalCoordinate, const double &C) {
    return C * (horisontalCoordinate - CAMERA_WIDTH / 2) / verticalCoordinate;
  }

  void ImageDrawingArea::regressConstants(guint8 *pixels, const unsigned int &channels, const unsigned int &stride) { // TODO: Optimise
    // Calculate points
    std::vector<std::pair<unsigned int, unsigned int>> verticalPoints, horisontalPoints;
    for (std::map<Vision::Blob*, unsigned int>::iterator blobOccurrence = blobCounts.begin(); blobOccurrence != blobCounts.end(); ++blobOccurrence) {
      if (blobOccurrence->second < totalCount / 2)
        continue;
      verticalPoints.push_back(std::pair<unsigned int, unsigned int>(blobOccurrence->first->getMinY(), blobOccurrence->first->getMaxY()));
      horisontalPoints.push_back(std::pair<unsigned int, unsigned int>(blobOccurrence->first->getMinX(), blobOccurrence->first->getMaxX()));
    }

    // Constant regression step initialisation
    double A = 0, B = 0, C = 0;
    double nextA = 0, nextB = 0, nextC = 0;
    double verticalMSECurrent = 0, horisontalMSECurrent = 0;
    double verticalMSENext = 0, horisontalMSENext = 0;

    // Bounds division by best bounds division
    std::sort(verticalResultsList.begin(), verticalResultsList.end(), verticalResultsSortPredicate());
    std::sort(horisontalResultsList.begin(), horisontalResultsList.end(), horisontalResultsSortPredicate());

    // Fill A and B constant combination list with the best bound values
    if (ABList.empty()) {
      for (unsigned int i = 0; i < numberOfBestDivisions; ++i) {
        if (i >= verticalResultsList.size())
          break;

        // Add A value with previous and next values
        AList.push_back(verticalResultsList[i].first.first.first);
        AList.push_back(verticalResultsList[i].first.first.second);

        partitionList(AList, numberOfDivisions);

        // Add B value with previous and next values
        BList.push_back(verticalResultsList[i].first.second.first);
        BList.push_back(verticalResultsList[i].first.second.second);

        partitionList(BList, numberOfDivisions);

        // Fill ABList with A and B value combinations
        for (std::vector<double>::iterator a = AList.begin(); a != AList.end(); a += 2 * numberOfDivisions) {
          for (std::vector<double>::iterator b = BList.begin(); b != BList.end(); b += 2 * numberOfDivisions) {
            for (unsigned int i = 0; i < 2 * numberOfDivisions; i += 2) {
              for (unsigned int j = 0; j < 2 * numberOfDivisions; j += 2) {
                ABList.push_back(std::pair<std::pair<double, double>, std::pair<double, double>>(std::pair<double, double>(*(a + i), *(a + i + 1)), std::pair<double, double>(*(b + j), *(b + j + 1))));
              }
            }
          }
        }

        // Empty A and B value lists
        AList.clear();
        BList.clear();
      }
      verticalResultsList.clear();
    }

    // Fill C constant list with the best bound values
    if (CList.empty()) {
      for (unsigned int i = 0; i < numberOfBestDivisions; ++i) {
        if (i >= horisontalResultsList.size())
          break;

        // Add C value with previous and next values
        CList.push_back(horisontalResultsList[i].first.first);
        CList.push_back(horisontalResultsList[i].first.second);
      }

      partitionList(CList, numberOfDivisions);
    }

    // 1. Establish a condition C when to end the regression algorithm
    // TODO: Currently it is enough for the user to decide when to end the algorithm; should consider automatic calibration.

    // 2. Generate new model M (constant A, B and C estimations)

    A = ABList.front().first.first;
    nextA = ABList.front().first.second;
    B = ABList.front().second.first;
    nextB = ABList.front().second.second;
    ABList.erase(ABList.begin());
    C = CList.front();
    CList.erase(CList.begin());
    nextC = CList.front();
    CList.erase(CList.begin());

    // 3. For every point, calculate the estimate and the error
    std::vector<double> verticalEstimatesCurrent, verticalEstimatesNext;
    std::vector<double> horisontalEstimatesCurrent, horisontalEstimatesNext;
    std::vector<double> verticalErrorsCurrent, verticalErrorsNext;
    std::vector<double> horisontalErrorsCurrent, horisontalErrorsNext;
    for (unsigned int j = 0; j < verticalPoints.size(); ++j) { // Vertical points and horisontal points have the same amount of points
      verticalEstimatesCurrent.push_back(getVerticalDistance(verticalPoints[j].first, A, B) - getVerticalDistance(verticalPoints[j].second, A, B));
      verticalEstimatesNext.push_back(getVerticalDistance(verticalPoints[j].first, nextA, nextB) - getVerticalDistance(verticalPoints[j].second, nextA, nextB));
      horisontalEstimatesCurrent.push_back(getHorisontalDistance(horisontalPoints[j].second, verticalPoints[j].second, C) - getHorisontalDistance(horisontalPoints[j].first, verticalPoints[j].second, C));
      horisontalEstimatesNext.push_back(getHorisontalDistance(horisontalPoints[j].second, verticalPoints[j].second, nextC) - getHorisontalDistance(horisontalPoints[j].first, verticalPoints[j].second, nextC));
      verticalErrorsCurrent.push_back(verticalEstimatesCurrent.back() - squareWidth);
      verticalErrorsNext.push_back(verticalEstimatesNext.back() - squareWidth);
      horisontalErrorsCurrent.push_back(horisontalEstimatesCurrent.back() - squareWidth);
      horisontalErrorsNext.push_back(horisontalEstimatesNext.back() - squareWidth);
    }

    // 4. Calculate MSEs
    verticalMSECurrent = 0;
    verticalMSENext = 0;
    horisontalMSECurrent = 0;
    horisontalMSENext = 0;
    for (unsigned int j = 0; j < verticalPoints.size(); ++j) { // Vertical points and horisontal points have the same amount of points
      verticalMSECurrent += verticalErrorsCurrent[j] * verticalErrorsCurrent[j];
      verticalMSENext += verticalErrorsNext[j] * verticalErrorsNext[j];
      horisontalMSECurrent += horisontalErrorsCurrent[j] * horisontalErrorsCurrent[j];
      horisontalMSENext += horisontalErrorsNext[j] * horisontalErrorsNext[j];
    }

    verticalMSECurrent /= verticalPoints.size();
    verticalMSENext /= verticalPoints.size();
    horisontalMSECurrent /= horisontalPoints.size();
    horisontalMSENext /= horisontalPoints.size();

    // Add MSEs to result lists
    verticalResultsList.push_back(std::pair<std::pair<std::pair<double, double>, std::pair<double, double>>, double>(std::pair<std::pair<double, double>, std::pair<double, double>>(std::pair<double, double>(A, nextA), std::pair<double, double>(B, nextB)), std::min(verticalMSECurrent, verticalMSENext)));
    horisontalResultsList.push_back(std::pair<std::pair<double, double>, double>(std::pair<double, double>(C, nextC), std::min(horisontalMSECurrent, horisontalMSENext)));

    // 6. Find model with minimal error
    if (std::min(verticalMSECurrent, verticalMSENext) < bestVerticalMSE) {
      bestA = (verticalMSECurrent < verticalMSENext) ? A : nextA;
      bestB = (verticalMSECurrent < verticalMSENext) ? B : nextB;
      bestVerticalMSE = std::min(verticalMSECurrent, verticalMSENext);
      std::cout << "Found a vertical function with MSE = " << bestVerticalMSE << "; A = " << A << ", B = " << B << std::endl;
      drawPerspective(pixels, channels, stride);
    }
    if (std::min(horisontalMSECurrent, horisontalMSENext) < bestHorisontalMSE) {
      bestC = (horisontalMSECurrent < horisontalMSENext) ? C : nextC;
      bestHorisontalMSE = std::min(horisontalMSECurrent, horisontalMSENext);
      std::cout << "Found a horisontal function with MSE = " << bestHorisontalMSE << "; C = " << C << std::endl;
      drawPerspective(pixels, channels, stride);
    }

    // 5. Check for condition C (and return to step 2 if necessary)
    // TODO: Currently it is enough for the user to decide when to end the algorithm; should consider automatic calibration.

  }

  bool ImageDrawingArea::applyCalculations() {
    filteredImage = image->copy(); // TODO: Copy only where is necessary (?)

    guint8 *pixels = filteredImage->get_pixels();
    unsigned int channels = filteredImage->get_n_channels();
    unsigned int stride = filteredImage->get_rowstride();

    guint8 *actualPixels = frame->data;
    unsigned int actualChannels = 3;
    unsigned int actualStride = frame->width * actualChannels;

    if (!isCalculating()) {
      colorPixels(pixels, channels, stride, actualPixels, actualChannels, actualStride);
      regressBlobs();
    }

    colorBlobs(pixels, channels, stride);

    if (isCalculating()) {
      regressConstants(pixels, channels, stride);
    }

    return true;
  }

  std::pair<unsigned int, unsigned int> ImageDrawingArea::realToPixel(const double &x, const double &y) {
    unsigned int verticalCoordinate = bestB / (y - bestA);
    unsigned int horisontalCoordinate = x * verticalCoordinate / bestC + CAMERA_WIDTH / 2;
    return std::pair<unsigned int, unsigned int>(horisontalCoordinate, verticalCoordinate);
  }

  bool ImageDrawingArea::drawPerspective(guint8 *pixels, const unsigned int &channels, const unsigned int &stride) {
    if (bestA != 0 && bestB != 0 && bestC != 0) {
      for (unsigned int i = -100000; i < 100000; i += 50) {
        for (unsigned int j = -100000; j < 100000; j += 50) {
          std::pair<unsigned int, unsigned int> coordinates = realToPixel(i, j);
          if (coordinates.first < CAMERA_WIDTH && coordinates.second < CAMERA_HEIGHT) {
            for (int k = -1; k <= 1; k++) {
              for (int m = -1; m <= 1; ++m) {
                if (coordinates.first + k < CAMERA_WIDTH && coordinates.second + m < CAMERA_HEIGHT) {
                  guint8 *pixel = pixels + (coordinates.first + k) * channels + (coordinates.second + m) * stride;
                  pixel[0] = 255;
                  pixel[1] = 0;
                  pixel[2] = 0;
                }
              }
            }
          }
        }
      }
    }
  }

  bool ImageDrawingArea::drawImage(const Cairo::RefPtr<Cairo::Context> &cairo) {
    if (!filteredImage)
      return false;

    Gdk::Cairo::set_source_pixbuf(cairo, filteredImage, 0, 0);

    return true;
  }

}
