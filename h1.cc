/******************************************************************************
 * Title          : h1.cc
 * Author         : Renat Khalikov
 * Created on     : October 31, 2017
 * Description    : locates edges in a gray-level image and generates an "edge"
 *                  image where the intensity at each point is proportional to
 *                  edge magnitude
 * Purpose        : 
 * Usage          : ./h1 hough_simple_1.pgm hough_simple_1_h1_output.pgm
 * Build with     : make all
 */
#include "image.h"
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;

int main(int argc, char **argv){  
  if (argc!=3) {
    printf("Usage: %s {input gray-level image} {output gray-level edge image}\n", argv[0]);
    return 0;
  }
  const string input_file(argv[1]);
  const string output_file(argv[2]);

  Image an_image;
  if (!ReadImage(input_file, &an_image)) {
    cout <<"Can't open file " << input_file << endl;
    return 0;
  }

  LocateEdges(&an_image);
  
  if (!WriteImage(output_file, an_image)){
    cout << "Can't write to file " << output_file << endl;
    return 0;
  }
}
