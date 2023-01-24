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
a sudoku (hard mode):
```bash
.\SudokuProblem.exe

1 0 0  | 0 0 7  | 0 9 0 
0 3 0  | 0 2 0  | 0 0 8
0 0 9  | 6 0 0  | 5 0 0
-------+--------+------
0 0 5  | 3 0 0  | 9 0 0
0 1 0  | 0 8 0  | 0 0 2 
6 0 0  | 0 0 4  | 0 0 0
-------+--------+------
3 0 0  | 0 0 0  | 0 1 0
0 4 0  | 0 0 0  | 0 0 7
0 0 7  | 0 0 0  | 3 0 0

percentage: 100%
1 6 2  | 8 5 7  | 4 9 3
5 3 4  | 1 2 9  | 6 7 8
7 8 9  | 6 4 3  | 5 2 1
-------+--------+------
4 7 5  | 3 1 2  | 9 8 6
9 1 3  | 5 8 6  | 7 4 2 
6 2 8  | 7 9 4  | 1 3 5
-------+--------+------
3 5 6  | 4 7 8  | 2 1 9 
2 4 1  | 9 3 5  | 8 6 7
8 9 7  | 2 6 1  | 3 5 4
```