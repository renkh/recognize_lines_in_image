/******************************************************************************
 * Title          : h4.cc
 * Author         : Renat Khalikov
 * Created on     : October 31, 2017
 * Description    : finds lines in the image from its Hough Transform space,
 *                  using a given threshold, and draw the detected lines on
 *                  a copy of the original scene image
 * Purpose        :
 * Usage          : ./h4 hough_simple_1.pgm output_hough_voting_array.txt 175 hough_simple_h4_output.pgm
 * Build with     : make all
 */
#include "image.h"
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;

int main(int argc, char **argv){
  if (argc!=5) {
    printf("Usage: %s {input original gray-level image} {input Hough-voting-array} {input Hough threshold value} {output gray-level line image}\n", argv[0]);
    return 0;
  }
  const string input_gray_level_image(argv[1]);
  const string input_hough_voting_array(argv[2]);
  const string input_threshold_value(argv[3]);
  const string output_gray_level_line_image(argv[4]);

  Image an_image;
  if (!ReadImage(input_gray_level_image, &an_image)) {
    cout <<"Can't open file " << input_gray_level_image << endl;
    return 0;
  }
  int threshold_value = stoi(input_threshold_value);  // convert string to int
  DrawDetectedLines(input_hough_voting_array, threshold_value, &an_image);

  if (!WriteImage(output_gray_level_line_image, an_image)){
    cout << "Can't write to file " << output_gray_level_line_image << endl;
    return 0;
  }
}
