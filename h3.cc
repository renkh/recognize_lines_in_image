/******************************************************************************
 * Title          : h3.cc
 * Author         : Renat Khalikov
 * Created on     : October 31, 2017
 * Description    : generates an image of the Hough Transform space of a given
 *                  binary edge image
 * Purpose        :
 * Usage          : ./h3 hough_simple_h2_output.pgm hough_simple_h3_output.pgm output_hough_voting_array.txt
 * Build with     : make all
 */
#include "image.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;
using namespace ComputerVisionProjects;

int main(int argc, char **argv){
  if (argc!=4) {
    printf("Usage: %s {input binary edge image} {output gray-level Hough image} {output Hough-voting-array}\n", argv[0]);
    return 0;
  }
  const string input_file(argv[1]);
  const string output_gray_level_hough_image(argv[2]);
  const string output_hough_voting_array(argv[3]);

  Image an_image;
  if (!ReadImage(input_file, &an_image)) {
    cout <<"Can't open file " << input_file << endl;
    return 0;
  }
  // create and fill dimentions of hough image
  int row = an_image.GetNumberOfRows();
  int column = an_image.GetNumberOfColumns();
  int _accu_h = sqrt((row*row) + (column*column));
  int _accu_w = 360;
  Image hough_image;
  hough_image.AllocateSpaceAndSetSize(_accu_h, _accu_w);
  hough_image.SetNumberGrayLevels(255);
  std::ofstream output_filename(output_hough_voting_array);
  if (output_filename.fail()) {
    cerr << "Could not open: {output database}\n";
    exit(1); // 1 indicates an error occurred
  }
  HoughTransform(&an_image, &hough_image, output_filename);

  if (!WriteImage(output_gray_level_hough_image, hough_image)){
    cout << "Can't write to file " << output_gray_level_hough_image << endl;
    return 0;
  }
}
