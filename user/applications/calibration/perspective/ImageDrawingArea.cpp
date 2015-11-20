/**
 *  @file ImageDrawingArea.cpp
 *  Perspective calibration application image drawing area.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 19 November 2015
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
    bool operator()(const std::pair<std::pair<std::pair<double, std::pair<double, double>>, std::pair<double, std::pair<double, double>>>, double> &left, const std::pair<std::pair<std::pair<double, std::pair<double, double>>, std::pair<double, std::pair<double, double>>>, double> &right) {
        return left.second < right.second;
    }
  };

  // TODO: Move elsewhere
  struct horisontalResultsSortPredicate {
    bool operator()(const std::pair<std::pair<double, std::pair<double, double>>, double> &left, const std::pair<std::pair<double, std::pair<double, double>>, double> &right) {
        return left.second < right.second;
    }
  };

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
    bestA = bestB = bestC = 0;
    lowerBound = -100000;
    upperBound = 100000;
    AList.push_back(lowerBound);
    AList.push_back(0);
    AList.push_back(upperBound);
    BList.push_back(lowerBound);
    BList.push_back(0);
    BList.push_back(upperBound);
    for (std::vector<double>::iterator a = AList.begin(); a != AList.end(); ++a) {
      for (std::vector<double>::iterator b = BList.begin(); b != BList.end(); ++b) {
        ABList.push_back(std::pair<*a, *b>);
      }
    }
    CList.push_back(lowerBound);
    CList.push_back(0);
    CList.push_back(upperBound);
    maxError = 10;
    squareWidth = 25; // In millimeters; TODO: Move to constants file? Or ask from the user
    numberOfDivisions = 8;
    numberOfBestDivisions = 4;
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

    std::vector<Blob*> additionalBlobs;

    for (Vision::BlobSet::iterator blob = blobs.begin(); blob != blobs.end(); ++blob) {
      for (std::map<Blob*, unsigned int>::iterator blobOccurrence = blobCounts.begin(); blobOccurrence != blobCounts.end(); ++blobOccurrence) {
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
      blobCounts.insert(std::pair<Blob*, unsigned int>(additionalBlobs[i], 1));
    }
  }

  void ImageDrawingArea::colorBlobs(guint8 *pixels, const unsigned int &channels, const unsigned int &stride) {
    for (std::map<Blob*, unsigned int>::iterator blobOccurrence = blobCounts.begin(); blobOccurrence != blobCounts.end(); ++blobOccurrence) {
      if (blobOccurrence->second < totalCount / 2) {
        continue;
      }
      Blob *blob = blobOccurrence->first;
      if (blob) {
        unsigned int x = blob->getPosition()->getX();
        unsigned int y = blob->getPosition()->getY();
        unsigned int minX = blob->getMinX();
        unsigned int maxX = blob->getMaxX();
        unsigned int minY = blob->getMinY();
        unsigned int maxY = blob->getMaxY();
        if (minX >= CAMERA_WIDTH || maxX >= CAMERA_WIDTH || minY >= CAMERA_HEIGHT || maxY >= CAMERA_HEIGHT) {
          continue;
        }
        Color color = blob->getColor();
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

  void ImageDrawingArea::regressConstants() { // TODO: Optimise
    // Calculate points
    std::vector<std::pair<unsigned int, unsigned int>> verticalPoints, horisontalPoints;
    for (std::map<Blob*, unsigned int>::iterator blobOccurrence = blobCounts.begin(); blobOccurrence != blobCounts.end(); ++blobOccurrence) {
      if (blobOccurrence->second < totalCount / 2)
        continue;
      verticalPoints.push_back(std::pair<unsigned int, unsigned int>(blobOccurrence->first->getMinY(), blobOccurrence->first->getMaxY()));
      horisontalPoints.push_back(std::pair<unsigned int, unsigned int>(blobOccurrence->first->getMinX(), blobOccurrence->first->getMaxX()));
    }

    // Constant regression step initialisation
    double A = 0, B = 0, C = 0;
    double verticalMSE = 0, horisontalMSE = 0;

    // Bounds division by best bounds division
    std::sort(verticalResultsList.begin(), verticalResultsList.end(), verticalResultsSortPredicate);
    std::sort(horisontalResultsList.begin(), horisontalResultsList.end(), horisontalResultsSortPredicate);

    // Fill A and B constant combination list with the best bound values
    if (ABList.empty()) {
      for (unsigned int i = 0; i < numberOfBestDivisions; ++i) {
        if (i >= verticalResultsList.size())
          break;

        // Add A value with previous and next values
        AList.push_back(verticalResultsList[i].first.first.second.first);
        AList.push_back(verticalResultsList[i].first.first.first);
        AList.push_back(verticalResultsList[i].first.first.second.second);

        // Partition A list areas
        unsigned int Asize = AList.size();
        for (unsigned int j = 0; j < Asize; j += 3) {
          for (unsigned int k = 0; k < 2; ++k) {
            double difference = (AList[j + k] - AList[j + k + 1]) / numberOfDivisions;
            for (unsigned m = 0; m < numberOfDivisions; ++m) {
              AList.push_back(AList[j + k] + m * difference);
            }
          }
          AList.push_back(AList[j + 2]);
        }
        AList.erase(AList.begin(), AList.begin() + Asize);

        // Add B value with previous and next values
        BList.push_back(verticalResultsList[i].first.second.second.first);
        BList.push_back(verticalResultsList[i].first.second.first);
        BList.push_back(verticalResultsList[i].first.second.second.second);

        // Partition B list areas
        unsigned int Bsize = BList.size();
        for (unsigned int j = 0; j < Bsize; j += 3) {
          for (unsigned int k = 0; k < 2; ++k) {
            double difference = (BList[j + k] - BList[j + k + 1]) / numberOfDivisions;
            for (unsigned m = 0; m < numberOfDivisions; ++m) {
              BList.push_back(BList[j + k] + m * difference);
            }
          }
          BList.push_back(BList[j + 2]);
        }
        BList.erase(BList.begin(), BList.begin() + Bsize);

        // Fill ABList with A and B value combinations
        for (std::vector<double>::iterator a = AList.begin(); a != AList.end(); a += 2 * numberOfDivisions + 1) {
          for (std::vector<double>::iterator b = BList.begin(); b != BList.end(); b += 2 * numberOfDivisions + 1) {
            for (unsigned int i = 0; i < 2 * numberOfDivisions + 1; ++i) {
              for (unsigned int j = 0; j < 2 * numberOfDivisions + 1, ++j) {
                ABList.push_back(std::pair<*(a + i), *(b + j)>);
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
        CList.push_back(horisontalResultsList[i].first.second.first);
        CList.push_back(horisontalResultsList[i].first.first);
        CList.push_back(horisontalResultsList[i].first.second.second);
      }
    }

    // Partition C list areas
    unsigned int Csize = CList.size();
    for (unsigned int j = 0; j < Csize; j += 3) {
      for (unsigned int k = 0; k < 2; ++k) {
        double difference = (CList[j + k] - CList[j + k + 1]) / numberOfDivisions;
        for (unsigned m = 0; m < numberOfDivisions; ++m) {
          CList.push_back(CList[j + k] + m * difference);
        }
      }
      CList.push_back(CList[j + 2]);
    }
    CList.erase(CList.begin(), CList.begin() + Csize);

    // 1. Establish a condition C when to end the regression algorithm
    // TODO: Currently it is enough for the user to decide when to end the algorithm; should consider automatic calibration.

    // 2. Generate new model M (constant A, B and C estimations)
    // TODO
    A = randDouble(lowerBound, upperBound);
    B = randDouble(lowerBound, upperBound);
    C = randDouble(lowerBound, upperBound);

    // 3. For every point, calculate the estimate and the error
    std::vector<double> verticalEstimates, horisontalEstimates;
    std::vector<double> verticalErrors, horisontalErrors;
    for (unsigned int j = 0; j < verticalPoints.size(); ++j) { // Vertical points and horisontal points have the same amount of points
      verticalEstimates.push_back(getVerticalDistance(verticalPoints[j].first, A, B) - getVerticalDistance(verticalPoints[j].second, A, B));
      horisontalEstimates.push_back(getHorisontalDistance(horisontalPoints[j].second, verticalPoints[j].second, C) - getHorisontalDistance(horisontalPoints[j].first, verticalPoints[j].second, C));
      verticalErrors.push_back(verticalEstimates.back() - squareWidth);
      horisontalErrors.push_back(horisontalEstimates.back() - squareWidth);
    }

    // 4. Calculate MSEs
    verticalMSE = 0;
    horisontalMSE = 0;
    for (unsigned int j = 0; j < verticalPoints.size(); ++j) { // Vertical points and horisontal points have the same amount of points
      verticalMSE += verticalErrors[j] * verticalErrors[j];
      horisontalMSE += horisontalErrors[j] * horisontalErrors[j];
    }
    verticalMSE /= verticalPoints.size();
    horisontalMSE /= horisontalPoints.size();

    // 6. Find model with minimal error
    if (verticalMSE < bestVerticalMSE) {
      bestA = A;
      bestB = B;
      bestVerticalMSE = verticalMSE;
      std::cout << "Found a vertical function with MSE = " << verticalMSE << "; A = " << A << ", B = " << B << std::endl;
    }
    if (horisontalMSE < bestHorisontalMSE) {
      bestC = C;
      bestHorisontalMSE = horisontalMSE;
      std::cout << "Found a horisontal function with MSE = " << horisontalMSE << "; C = " << C << std::endl;
    }

    // 5. Check for condition C (and return to step 2 if necessary)
    // TODO: Currently it is enough for the user to decide when to end the algorithm; should consider automatic calibration.

    // Debug output // TODO: Refactor
    /*if (bestVerticalMSE <= maxError * verticalPoints.size()) {
      std::cout << "The vertical function's MSE is low enough." << std::endl;
    }
    if (bestHorisontalMSE <= maxError * horisontalPoints.size()) {
      std::cout << "The horisontal function's MSE is low enough." << std::endl;
    }*/

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
      regressConstants();
    }

    return true;
  }

  bool ImageDrawingArea::drawImage(const Cairo::RefPtr<Cairo::Context> &cairo) {
    if (!filteredImage)
      return false;

    Gdk::Cairo::set_source_pixbuf(cairo, filteredImage, 0, 0);

    return true;
  }

}
