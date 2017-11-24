/******************************************************************************
 * Title          : h1.cc
 * Author         : Renat Khalikov
 * Created on     : October 31, 2017
 * Description    : thresholds a gray-level image at a certain threshold value
 * Purpose        :
 * Usage          : ./h2 hough_simple_1_h1_output.pgm 150 hough_simple_1_h2_output.pgm
 * Build with     : make all
 */
#include "image.h"
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;

int main(int argc, char **argv){
  if (argc!=4) {
    printf("Usage: %s {input gray–level image} {input gray–level threshold} {output binary image}\n", argv[0]);
    return 0;
  }
  const string input_file(argv[1]);
  const string value(argv[2]);
  const string output_file(argv[3]);

  Image an_image;
  if (!ReadImage(input_file, &an_image)) {
    cout <<"Can't open file " << input_file << endl;
    return 0;
  }
  int threshold_value = stoi(value);  // convert string to int
  ConvertToBinary(threshold_value, &an_image);

  if (!WriteImage(output_file, an_image)){
    cout << "Can't write to file " << output_file << endl;
    return 0;
  }
}
