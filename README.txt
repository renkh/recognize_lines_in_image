Assignment
----------------
H4 is incomplete, abort trap 6 error for DrawLine() function
----------------
Bugs:
Abort trap 6 for h4
----------------
Compilation Instructions
----------------
Open a terminal.
Go to the directory containing the source code.
To compile type:
  make all
----------------
It is assumed that you are using a Linux machine and a g++ compiler.
----------------
To execute:
For h1
./h1 hough_simple_1.pgm hough_simple_1_h1_output.pgm

For h2
./h2 hough_simple_1_h1_output.pgm 150 hough_simple_1_h2_output.pgm

For h3
./h3 hough_simple_h2_output.pgm hough_simple_h3_output.pgm output_hough_voting_array.txt

For h4
./h4 hough_simple_1.pgm output_hough_voting_array.txt 175 hough_simple_h4_output.pgm
---------------
Note:
Threshold value for h2 is 150 (reduces noise)
Threshold value for h4 is 175