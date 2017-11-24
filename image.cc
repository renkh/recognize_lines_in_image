
// Class for representing a 2D gray-scale image,
// with support for reading/writing pgm images.
// To be used in Computer Vision class.

#include "image.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>

using namespace std;

namespace ComputerVisionProjects {

Image::Image(const Image &an_image){
  AllocateSpaceAndSetSize(an_image.num_rows(), an_image.num_columns());
  SetNumberGrayLevels(an_image.num_gray_levels());

  for (size_t i = 0; i < num_rows(); ++i)
    for (size_t j = 0; j < num_columns(); ++j){
      SetPixel(i,j, an_image.GetPixel(i,j));
    }
}

Image::~Image(){
  DeallocateSpace();
}

void
Image::AllocateSpaceAndSetSize(size_t num_rows, size_t num_columns) {
  if (pixels_ != nullptr) DeallocateSpace();
  pixels_ = new int*[num_rows];
  for (size_t i = 0; i < num_rows; ++i)
    pixels_[i] = new int[num_columns];

  num_rows_ = num_rows;
  num_columns_ = num_columns;
}

void
Image::DeallocateSpace() {
  for (size_t i = 0; i < num_rows_; i++)
    delete pixels_[i];
  delete pixels_;
  pixels_ = nullptr;
  num_rows_ = 0;
  num_columns_ = 0;
}

bool ReadImage(const string &filename, Image *an_image) {
  if (an_image == nullptr) abort();
  FILE *input = fopen(filename.c_str(),"rb");
  if (input == 0) {
    cout << "ReadImage: Cannot open file" << endl;
    return false;
  }

  // Check for the right "magic number".
  char line[1024];
  if (fread(line, 1, 3, input) != 3 || strncmp(line,"P5\n",3)) {
    fclose(input);
    cout << "ReadImage: Expected .pgm file" << endl;
    return false;
  }

  // Skip comments.
  do
    fgets(line, sizeof line, input);
  while(*line == '#');

  // Read the width and height.
  int num_columns,num_rows;
  sscanf(line,"%d %d\n", &num_columns, &num_rows);
  an_image->AllocateSpaceAndSetSize(num_rows, num_columns);


  // Read # of gray levels.
  fgets(line, sizeof line, input);
  int levels;
  sscanf(line,"%d\n", &levels);
  an_image->SetNumberGrayLevels(levels);

  // read pixel row by row.
  for (int i = 0; i < num_rows; ++i) {
    for (int j = 0;j < num_columns; ++j) {
      const int byte=fgetc(input);
      if (byte == EOF) {
        fclose(input);
        cout << "ReadImage: short file" << endl;
        return false;
      }
      an_image->SetPixel(i, j, byte);
    }
  }

  fclose(input);
  return true;
}

bool WriteImage(const string &filename, const Image &an_image) {
  FILE *output = fopen(filename.c_str(), "w");
  if (output == 0) {
    cout << "WriteImage: cannot open file" << endl;
    return false;
  }
  const int num_rows = an_image.num_rows();
  const int num_columns = an_image.num_columns();
  const int colors = an_image.num_gray_levels();

  // Write the header.
  fprintf(output, "P5\n"); // Magic number.
  fprintf(output, "#\n");  // Empty comment.
  fprintf(output, "%d %d\n%03d\n", num_columns, num_rows, colors);

  for (int i = 0; i < num_rows; ++i) {
    for (int j = 0; j < num_columns; ++j) {
      const int byte = an_image.GetPixel(i , j);
      if (fputc(byte,output) == EOF) {
	    fclose(output);
            cout << "WriteImage: could not write" << endl;
	    return false;
      }
    }
  }

  fclose(output);
  return true;
}

// Implements the Bresenham's incremental midpoint algorithm;
// (adapted from J.D.Foley, A. van Dam, S.K.Feiner, J.F.Hughes
// "Computer Graphics. Principles and practice",
// 2nd ed., 1990, section 3.2.2);
void
DrawLine(int x0, int y0, int x1, int y1, int color,
  Image *an_image) {
  if (an_image == nullptr) abort();

  #ifdef SWAP
  #undef SWAP
  #endif
  #define SWAP(a,b) {a^=b; b^=a; a^=b;}

  const int DIR_X = 0;
  const int DIR_Y = 1;

  // Increments: East, North-East, South, South-East, North.
  int incrE, incrNE, incrS, incrSE, incrN;
  int d;         /* the D */
  int x,y;       /* running coordinates */
  int mpCase;    /* midpoint algorithm's case */
  int done;      /* set to 1 when done */

  int xmin = x0;
  int xmax = x1;
  int ymin = y0;
  int ymax = y1;

  int dx = xmax - xmin;
  int dy = ymax - ymin;
  int dir;

  if (dx * dx > dy * dy) {  // Horizontal scan.
    dir=DIR_X;
    if (xmax < xmin) {
      SWAP(xmin, xmax);
      SWAP(ymin , ymax);
    }
    dx = xmax - xmin;
    dy = ymax - ymin;

    if (dy >= 0) {
      mpCase = 1;
      d = 2 * dy - dx;
    } else {
      mpCase = 2;
      d = 2 * dy + dx;
    }

    incrNE = 2 * (dy - dx);
    incrE = 2 * dy;
    incrSE = 2 * (dy + dx);
  } else {// vertical scan.
    dir = DIR_Y;
    if (ymax < ymin) {
      SWAP(xmin, xmax);
      SWAP(ymin, ymax);
    }
    dx = xmax - xmin;
    dy = ymax-ymin;

    if (dx >=0 ) {
      mpCase = 1;
      d = 2 * dx - dy;
    } else {
      mpCase = 2;
      d = 2 * dx + dy;
    }

    incrNE = 2 * (dx - dy);
    incrE = 2 * dx;
    incrSE = 2 * (dx + dy);
  }

  /// Start the scan.
  x = xmin;
  y = ymin;
  done = 0;

  while (!done) {
    an_image->SetPixel(x,y,color);

    // Move to the next point.
    switch(dir) {
      case DIR_X:
      if (x < xmax) {
       switch(mpCase) {
         case 1:
         if (d <= 0) {
          d += incrE;
          x++;
        } else {
          d += incrNE;
          x++;
          y++;
        }
        break;

        case 2:
        if (d <= 0) {
          d += incrSE;
          x++;
          y--;
        } else {
          d += incrE;
          x++;
        }
        break;
      }
    } else {
     done=1;
   }
   break;

   case DIR_Y:
   if (y < ymax) {
    switch(mpCase) {
     case 1:
     if (d <= 0) {
       d += incrE;
       y++;
     } else {
       d += incrNE;
       y++;
       x++;
     }
     break;

     case 2:
     if (d <= 0) {
      d += incrSE;
      y++;
      x--;
    } else {
      d += incrE;
      y++;
    }
    break;
	  } // mpCase
        } // y < ymin
        else {
         done=1;
       }
       break;
     }
   }
 }

/**
 * ConvertToBinary( ) sets image pixels to 0 if its value is below threshold
 * and 1 if its value is above threshold
 *
 * @param {int} threshold_value: the threshold value
 * @param {Image} an_image: input image
 */
void ConvertToBinary(const int threshold_value, Image *an_image) {
  if (an_image == nullptr) abort();
  int pixel;
  int row = an_image->GetNumberOfRows();
  int column = an_image->GetNumberOfColumns();
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < column; ++j) {
      pixel = an_image->GetPixel(i,j);
      if (pixel <= threshold_value)
        an_image->SetPixel(i,j,0);
      else
        an_image->SetPixel(i,j,1);
    }
  }
  an_image->SetNumberGrayLevels(1);
}

/**
 * LocateEdges( ) locates edges using sobel derivatives. Sets the color of
 * image based on the gradient approximation of sobel derivatives.
 *
 * @param {Image} an_image: input image
 */
void LocateEdges(Image *an_image){
  // matrix dimensions
  int row = an_image->GetNumberOfRows();
  int column = an_image->GetNumberOfColumns();
  // sobel operator in x direction
  int sobel_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
  // sobel operator in y direction
  int sobel_y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
  int image_x[row][column];
  int image_y[row][column];
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < column; ++j) {
      // https://stackoverflow.com/questions/16385570/sobel-edge-detector-using-c-without-any-special-library-or-tool
      int pixel = 0;
      int x_convolution = 0;
      int y_convolution = 0;
      // x and y convolutions
      for (int x = -1; x <= 1; x++){
        for (int y = -1; y <= 1; y++){
          pixel = (i == 0 || i == row-1 || j == 0 || j == column-1) ? 0 : an_image->GetPixel(i+x, j+y);
          x_convolution = x_convolution + (pixel * sobel_x[1+x][1+y]);
          y_convolution = y_convolution + (pixel * sobel_y[1+x][1+y]);
        }
      }
      image_x[i][j] = x_convolution;
      image_y[i][j] = y_convolution;
    }
  }
  // gradient approximation
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < column; ++j) {
      double gradient_approximation = sqrt(image_x[i][j]*image_x[i][j] + image_y[i][j]*image_y[i][j]);
      an_image->SetPixel(i,j,gradient_approximation);
    }
  }
}

/**
 * HoughTransform( ) creates an accumulator array of the hough space and
 * outputs the accumulator array to an output image for visualization
 * @param an_image    [input binary image]
 * @param hough_image [output hough image used for visualization]
 * @param output_file [output accumulator array to txt file]
 */
void HoughTransform(Image *an_image, Image *hough_image, std::ostream &output_file){
  // matrix dimensions
  int row = an_image->GetNumberOfRows();
  int column = an_image->GetNumberOfColumns();

  // the maximum height depends on the image size
  int accu_row = sqrt((row*row) + (column*column));
  int accu_col = 360;

  // start with an accumulator array with all 0's
  // the width contain 360 bins
  std::vector<std::vector<int>> accumulator(accu_row, std::vector<int>(accu_col, 0));

  // compute r for every θ in the image
  for(int y = 0; y < row; y++){
    for(int x = 0; x < column; x++){
      if( an_image->GetPixel(y,x) != 0 ){
        for(int t=0;t<360;t++){
          // r = xcos(θ) + ysin(θ)
          double pi = atan(1)*4;
          double theta = (pi/360);
          double r = (x  * cos(t * theta)) + (y * sin(t * theta));
          if (r>=0)
            accumulator[r][t]++;
        }
      }
    }
  }
  // output header for file
  output_file << accu_row << " " << accu_col << endl;
  // output tbe accumulator array to output file
  // as well as draw the accumulator to output image
  for(int y = 0; y < accu_row; y++){
    for(int x = 0; x < accu_col; x++){
      hough_image->SetPixel(y, x, accumulator[y][x]);
      output_file << accumulator[y][x] << " ";
    }
  }
  output_file << endl;
}

/**
 * DrawDetectedLines( ) takes in hough voting array, recalculates points
 * in the image space from (r,theta) and draws the line segments on the image
 * @param input_hough_voting_array array containing the accumulator
 * @param threshold_value          threshold value for computing maxima
 * @param an_image                 image the lines will be drawn on
 */
void DrawDetectedLines(const string input_hough_voting_array, int threshold_value, Image *an_image){
  // matrix dimensions
  int row = an_image->GetNumberOfRows();
  int column = an_image->GetNumberOfColumns();

  ifstream accumulator_values(input_hough_voting_array);
  if (accumulator_values.fail()) {
    cerr << "Could not open: {input_hough_voting_array}\n";
    exit(1); // 1 indicates an error occurred
  }
  string line;
  getline(accumulator_values, line);

  // recreate the accumulator array from input file
  stringstream header(line);
  int accu_row, accu_col;
  header >> accu_row >> accu_col;
  std::vector<std::vector<int>> accumulator(accu_row, std::vector<int>(accu_col, 0));
  getline(accumulator_values, line);
  stringstream number_stream(line);
  for (int i = 0; i < accu_row; ++i){
    for (int j = 0; j < accu_col; ++j){
      int token;
      number_stream >> token;
      accumulator[i][j] = token;
    }
  }
  // lines array that will contain object edges
  std::vector< std::pair< std::pair<int, int>, std::pair<int, int> > > lines;
  int max = 0;
  // loop over the accumulator to check if the value of a bin
  // is on or above threshold_value
  for(int r=0;r<accu_row;r++){
    for(int t=0;t<accu_col;t++){
      if(accumulator[r][t] >= threshold_value){
        // check if this point is a local maxima
        max = accumulator[r][t];
        for(int ly=-4;ly<=4;ly++){
          for(int lx=-4;lx<=4;lx++){
            if( (ly+r>=0 && ly+r<accu_row) && (lx+t>=0 && lx+t<accu_col) ){
              if( accumulator[ly+r][lx+t] > max ){
                max = accumulator[ly+r][lx+t];
                ly = lx = 5;
              }
            }
          }
        }
        if(max > accumulator[r][t])
          continue;

        // (r, θ) coordinates are computed back to two points in image space
        int x1, y1, x2, y2;
        x1 = y1 = x2 = y2 = 0;
        double pi = atan(1)*4;
        double theta = (pi/360);
        if((t >= 45 && t <= 135) || (t >= 225 && t <= 315)){
          //y = (r - x cos(t)) / sin(t)
          x1 = 0;
          y1 = ( (r - (x1*cos(t*theta)) ) / sin(t * theta));
          x2 = column - 0;
          y2 = ( (r - (x2*cos(t * theta)) ) / sin(t * theta));
        }
        else{
          //x = (r - y sin(t)) / cos(t);
          y1 = 0;
          x1 = ((r - (y1*sin(t*theta)) ) / cos(t * theta));
          y2 = row - 0;
          x2 = ((r-(y2*sin(t*theta)) ) / cos(t * theta));
        }
        lines.push_back(std::pair< std::pair<int, int>, std::pair<int, int> >(std::pair<int, int>(x1,y1), std::pair<int, int>(x2,y2)));
      }
    }
  }
  // draw the computed lines to the input image
  std::vector< std::pair< std::pair<int, int>, std::pair<int, int> > >::iterator it;
  for(it=lines.begin();it!=lines.end();it++){
    if (abs(it->first.second) > column)
    {
      DrawLine(abs(it->first.first), column, abs(it->second.first), abs(it->second.second), 255, an_image);
    }
    else if (abs(it->second.second) > column)
    {
      DrawLine(abs(it->first.first), abs(it->first.second), abs(it->second.first), column, 255, an_image);
    }
    else if (abs(it->second.second) > column && abs(it->first.first) > column)
    {
      DrawLine(abs(it->first.first), column, abs(it->second.first), column, 255, an_image);
    }
    else
      DrawLine(abs(it->first.first), abs(it->first.second), abs(it->second.first), abs(it->second.second), 255, an_image);
  }
}
}  // namespace ComputerVisionProjects
