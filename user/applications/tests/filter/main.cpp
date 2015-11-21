/**
 *  @file main.cpp
 *  Color filter testing application.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 20 November 2015
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::string readFilterFromFile(const std::string &fileName) {
  std::string filter = "";
  std::ifstream inputFile(fileName);
  std::stringstream buffer;
  buffer << inputFile.rdbuf();
  filter = buffer.str();
  inputFile.close();
  return filter;
}

int main(int argc, char* argv[]) {
  // Check for parameters
  if (argc != 2) {
    std::cout << "The program accepts the color filter file as a command line parameter." << std::endl;
    return 1;
  }

  // Read filter
  std::string filter = readFilterFromFile(argv[1]);

  // Analyse Filter
  if (filter.size() != 256 * 256 * 256) {
    std::cout << "The filter is not the correct size." << std::endl;
    std::cout << "The expected size is " << 256 * 256 * 256 << " bytes but the actual size is " << filter.size() << " bytes." << std::endl;
  } else {
    std::cout << "The filter file is the correct size." << std::endl;
  }

  return 0;
}
