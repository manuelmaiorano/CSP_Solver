# CSP_Solver

Simple Constraint Satisfaction Problem Solver in C++. Can handle various classical CSP problems: the map coloring problem, the 8 queen problem and sudoku. Combines a search based approach with forward inference based on the famous AC3 algorithm.

## Usage

Typical Cmake workflow:
```bash
mkdir build
cd build
cmake ..\src
cmake --build .
```

Then try one of the examples: 
the map coloring problem from the book "AI: A Modern Approach"
```bash
cd examples
.\MapColoringProblem.exe
NSW : red
NT : red
Q : blue
SA : green
T : blue
V : blue
WA : blue
```
a sudoku (hard mode), here i test the parallel version and compare elapsed time in milliseconds (2 core machine):
```bash
.\SudokuProblem.exe
0 0 0  | 0 0 7  | 0 9 0 
0 0 0  | 0 2 0  | 0 0 8
0 0 9  | 6 0 0  | 5 0 0
-------+--------+------
0 0 5  | 3 0 0  | 9 0 0
0 1 0  | 0 8 0  | 0 0 2 
6 0 0  | 0 0 4  | 0 0 0
-------+--------+------
3 0 0  | 0 0 0  | 0 0 0 
0 4 0  | 0 0 0  | 0 0 0
0 0 7  | 0 0 0  | 3 0 0

percentage: 100%
8 3 6  | 5 4 7  | 2 9 1
7 5 4  | 1 2 9  | 6 3 8
1 2 9  | 6 3 8  | 5 4 7 
-------+--------+------
4 7 5  | 3 1 2  | 9 8 6
9 1 3  | 7 8 6  | 4 5 2 
6 8 2  | 9 5 4  | 1 7 3
-------+--------+------
3 6 8  | 4 9 1  | 7 2 5
5 4 1  | 2 7 3  | 8 6 9 
2 9 7  | 8 6 5  | 3 1 4

elapsed : 212
 doing parallel now
elapsed : 151
4 6 8  | 1 5 7  | 2 9 3
5 7 1  | 9 2 3  | 4 6 8
2 3 9  | 6 4 8  | 5 1 7
-------+--------+------
7 8 5  | 3 1 2  | 9 4 6 
9 1 4  | 5 8 6  | 7 3 2
6 2 3  | 7 9 4  | 1 8 5
-------+--------+------
3 9 2  | 8 7 1  | 6 5 4 
1 4 6  | 2 3 5  | 8 7 9
8 5 7  | 4 6 9  | 3 2 1 
```