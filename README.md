# sudoku-solver
A program which solves a text file representing an unsolved sudoku puzzle.

This program is run by calling the program name and including the name of the sudoku puzzle text file in the command line.
The puzzle file has the format :
      
      0 0 0 0 0 0 0 1 0 
      0 0 0 0 0 0 0 0 0 
      0 0 7 0 0 0 0 0 0 
      0 0 0 0 0 0 0 0 1 
      0 0 0 0 2 0 0 0 0 
      0 0 0 0 0 0 2 0 0 
      0 0 3 0 0 5 0 0 0 
      0 0 0 0 0 0 0 0 0 
      0 4 0 0 0 0 0 9 0 
    
Where any '0' entry represents an empty square, and any non-zero integer entry represents a solved square. 

The proram solves the puzzle by first using a cross-hatching method to find when squares can have only one solution, and when this fails to find any more guarenteed solutions, then it uses backtracking to find a solution.
