#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int orig[9][9];
int test[9][9];
void reset_board();
void print_board(int board[9][9]);
bool check_squares();
bool check_cols();
bool check_rows();
bool board_check();
bool crosshatch();
bool rowFill();
bool colFill();
bool bruteForce();
bool row_avail(int i, int r);
bool col_avail(int i, int c);
bool sqr_avail(int i, int row, int col);

/*-------------------------------------------------------------------------------------------------------------
|          WELCOME TO MY CODE! THIS ENTIRE THING WAS CODED BY ME, NICHOLAS BOMBARDIERI!                       |
|                                                                                                             |
|  THIS CODE TAKES IN A USER INPUT OF ANY SUDOKU PUZZLE (AS LONG AS IT IS NOT WRONG BY DEFAULT) AND SOLVES IT |
|  THE SOLVING METHOD I USED TRIES TO SOLVE AS MANY PLACES AS POSSIBLE USING CROSSHATCHING, AND AFTER THAT,   |
|  USES A BACKTRACKING METHOD TO FILL IN ALL THE EMPTY SPACES UNTIL IT FINDS A SOLUTION.                      |
|                                                                                                             |
|  I WAS FIGURING OUT HOW TO DO THE SOLVING WHILE CODING MY crosshatch() FUNCTION, SO THAT WHOLE THING IS A   |
|  MESS, BUT ACTUALLY USED FUNCTIONS IN MY bruteForce() FUNCTION SO IT LOOKS MUCH CLEANER (I ALSO KNEW WHAT   |
|  I WAS DOING THERE, AND THAT HELPED TOO.                                                                    |
|                                                                                                             |
|  THIS COMBINATION OF METHODS ALLOWS THE CODE TO HANDLE SIMPLE PUZZLES QUICKLY, AS WELL AS (USUALLY)         |
|  UNSOLVABLE PUZZLES USING BACKTRACKING. (I GOT THE IDEA TO USE BACKTRACKING FROM LOGAN YUE, (A FRIEND)      |
|                                                                                                             |
|  I ALSO EXPERIMENTED WITH UNICODE FOR MY OUTPUT, WHICH MAKES THE OUTPUT LOOK VERY PRETTY, BUT MAKES MY      |
|  print_board() FUNCTION NOT SO PRETTY!                                                                      |
|                                                                                                             |
|  THANKS FOR CHECKING OUT MY CODE! HOPE IT HELPS.                                                            |
-------------------------------------------------------------------------------------------------------------*/

//EXPECTED USER INPUT: ./programName filename.txt

int main(int argc, char *argv[]){
  //initializes the values of board to 0
  for (int i = 0; i < 9; i++)
    for(int j = 0; j < 9; j++)
      orig[i][j] = 0;
  FILE *infile;
  infile = fopen(argv[1], "r");
  
  int num = 0;
  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++){
      num = getc(infile) - 48;
      if (num == -38)
	num = getc(infile) - 48;
      orig[i][j] = num;
      getc(infile);
    }
  fclose(infile);
  
  /*-------------------------------------------------------------------------------------------------------------
  |                                            HERE BEGINS THE SEARCH FOR ANSWERS                               |
  -------------------------------------------------------------------------------------------------------------*/
  bool found = false;
  bool crossTime = true;
  reset_board();
  while (!found){
    //reset_board();
    if (crossTime)
      crossTime = crosshatch();
    else
      bruteForce();
    found = board_check();
  }
  
  /*-------------------------------------------------------------------------------------------------------------
  |                                           ANSWER IS FOUND. BLESSED BE THE LORD.                             |
  -------------------------------------------------------------------------------------------------------------*/
  
  printf("ORIGINAL: VVV \n");
  print_board(orig);
  printf("SOLUTION: VVV \n");
  print_board(test);
  printf("\n");

  return 0;

}


//----------------------------------------------MAIN IS DONE. HERE ARE FUNCIONS----------------------------------

void reset_board(){ //RESETS THE BOARD VALUES TO INTIAL VALUES IN ORIG
  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++){
      test[i][j] = orig[i][j];
    }
  return;
}

void print_board(int board[9][9]){ //PRINTS A BOARD INCLUDED AS AN ARGUMENT
  //THIS LOOKS LIKE A MESS BECAUSE I USED UNICODE TO MAKE THE OUTPUT LOOK PRETTY :D
  printf("\u2554");
  for (int i = 0; i < 9; i++){
    if (i != 0)
      printf("\u2560");
    for (int n = 0; n < 35; n++){
      if (n%4 != 3)
	printf("\u2550");
      else{
	if (i != 0)
	  printf("\u256C");
	else
	  printf("\u2566");
      }
    }
    if (i == 0)
      printf("\u2557");
    else if (i == 9)
      printf("\u255A");
    else
      printf("\u2563");
    printf("\n");
    
    for (int j = 0; j < 9; j++){
      printf("\u2551 %d ", board[i][j]);
    }
    printf("\u2551 \n");
  }
  printf("\u255A");
  for (int n = 0; n < 35; n++){
    if (n%4 != 3)
      printf("\u2550");
    else
      printf("\u2569");
  }
  printf("\u255D \n \n");
}

/*--------------------------------------------------------------------------------------------------------------\
|                                        CROSSHATCH CHECK                                                       |
|           THIS CHECKS EVERY SINGLE MISSING NUMBER IN A 3X3 IF IT CAN ONLY FIT IN ONE SPOT                     |
\--------------------------------------------------------------------------------------------------------------*/

bool crosshatch(){
  int missing[9];//WHERE A ZERO ENTRY IN MISSING[k] MEANS THAT THIS SQUARE IS MISSING INT K
  int open[3][3];
  int number, spotcount;
  int poss_i[3][3];
  bool did_it = false;
  for (int row = 0; row < 9; row += 3){
    for (int col = 0; col < 9; col += 3){
      //THIS IS THE LOOP FOR GOING THROUGH EACH 3x3.
      for (int k = 0; k < 9; k++)
	missing[k] = 0; //SETS ALL MISSING VALUES TO NONE MISSING
      
      for (int k = 0; k < 3; k++)
	for (int l = 0; l < 3; l++)
	  open[k][l] = 0;//SETS SPACES TO NOT OPEN

      for (int r = 0; r < 3; r++){
	for (int c = 0; c < 3; c++){
	  if (test[row+r][col+c] != 0)
	    missing[test[row+r][col+c] - 1]++;//FINDS ALL MISSING VALUES
	  else
	    open[r][c]++; //FINDS ALL OPEN SPACES
	}
      }

      /* THIS NEXT PORTION CHECKS CROSSHATCH FOR EACH MISSING NUMBER. IF THERE IS ONLY ONE SPACE THAT THAT NUMBER
	 CAN FIT INTO, THEN SETS THAT VALUE AS THAT NUMBER. */
      for (int i = 0; i < 9; i++){
	if (missing[i] == 0){
	  number = i+1;
	  spotcount = 0;
	  for(int r = 0; r < 3; r++){
	    for(int c = 0; c < 3; c++){
	      if (open[r][c] == 1){
		poss_i[r][c] = 1;
	      }
	      else
		poss_i[r][c] = 0;
	    }
	  }

	  for (int r = 0; r < 3; r++){
	    for (int c = 0; c < 3; c++){
	      if (poss_i[r][c] == 1){
		for (int m = 0; m < 9; m++){
		  if (number == test[m][c+col] || number == test[r+row][m]){
		    poss_i[r][c] = 0;
		  }
		}
	      }
	    }
	  }
	  
	  for(int r = 0; r < 3; r++){
	    for(int c = 0; c < 3; c++){
	      if (poss_i[r][c] == 1){
		spotcount++;
		if (rowFill(row+r) || colFill(col+c)){
		  test[row+r][col+c] = number;
		  open[r][c] = 0;
		  spotcount = 2;
		  did_it = true;
		}
	      }
	    }
	  }
	  
	  if (spotcount == 1){
	    for (int r = 0; r < 3; r++){
	      for (int c = 0; c < 3; c++){
		if (poss_i[r][c] == 1){
		  test[row+r][col+c] = number;
		  open[r][c] = 0;
		  did_it = true;
		}
	      }
	    }
	  }
	}
      }
    }
  }
  return did_it;
}
//---------------------------------------CROSSHATCH CHECK DONE--------------------------------------------------

bool rowFill(int row){ //CHECKS IF THERE IS ONLY ONE SPACE MISSING A NUMBER IN A LINE. FILLS IT.
  int missing[9];
  for (int m; m < 9; m++){
    missing[m] = 0;
    if (test[row][m] != 0)
      missing[test[row][m] - 1] = 1;
  }
  
  int needcount = 0;
  for (int m = 0; m < 9; m++){
    if (missing[m] == 0)
      needcount++;
  }
  if (needcount == 1)
    return true;
  return false;
}


bool colFill(int col){ //CHECKS IF THERE IS ONLY ONE SPACE MISSING A NUMBER IN A LINE. FILLS IT.
  int missing[9];
  for (int m; m < 9; m++){
    missing[m] = 0;
    if (test[m][col] != 0)
      missing[test[m][col] - 1] = 1;
  }
  
  int needcount = 0;
  for (int m = 0; m < 9; m++){
    if (missing[m] == 0)
      needcount++;
  }
  if (needcount == 1)
    return true;
  return false;
}

/*-------------------------------------------------------------------------------------------------------------- \
|                                  BRUTEFORCE FINDER. CHECKS EVERY POSSIBLE ENTRY VALUE                          | 
\---------------------------------------------------------------------------------------------------------------*/

bool bruteForce(){//RECURSIVE FUNCTION. BACKTRACKS IF IT HITS A BRICK WALL. IF IT DOESNT, RETURNS FALSE.
  bool found;
  int i;
  bool bruteFail;
  for (int row = 0; row < 9; row++){
    for (int col = 0; col < 9; col++){
      if (test[row][col] == 0){//CHECKS EVERY SINGLE SLOT THAT IS EMPTY.
	found = false;
	i = 0;
	while (!found){
	  i++;
	  if (i > 9)
	    return true;
	  bruteFail = false;
	  if (row_avail(i, row) && col_avail(i, col) && sqr_avail(i, row, col)){
	      test[row][col] = i;
	      bruteFail = bruteForce(); //HERE IS WHERE THE FUNCTION CALLS ITSELF AGAIN.
	      if (!bruteFail)
		found = true;
	    }
	  if (bruteFail){
	    test[row][col] = 0;
	  }
	}
      }
    }
  }
  return false;
}

/*-------------------------------------------------------------------------------------------------------------- \
|                                  THESE ARE THE AVAILABILITY CHECK FUNCTIONS                                    | 
\---------------------------------------------------------------------------------------------------------------*/
bool row_avail(int i, int r){//CHECKS IF A ROW IS FREE OF INT i
  for (int m = 0; m < 9; m++){
    if (i == test[r][m])
      return false;
  }
  return true;
}

bool col_avail(int i, int c){//CHECKS IF A COLUMN IS FREE OF INT i
  for (int m = 0; m < 9; m++)
    if (i == test[m][c])
      return false;
  return true;
}

bool sqr_avail(int i, int row, int col){//CHECKS IF A 3X3 SQUARE IS FREE OF INT i
  row = row - (row%3);
  col = col - (col%3);
  for (int r = 0; r < 3; r++){
    for (int c = 0; c < 3; c++){
      if (i == test[row + r][col + c])
	return false;
    }
  }
  return true;
}

/*-------------------------------------------------------------------------------------------------------------- \
|                                  THESE ARE THE COMPLETENESS CHECK FUNCTIONS                                    | 
\---------------------------------------------------------------------------------------------------------------*/

bool board_check(){ //CALLS ALL OF THE CHECKS AND RETURNS FALSE IF ANY HAVE A VIOLATION
  bool good;
  good = check_squares();
  if(!good)
    return false;
  good = check_cols();
  if(!good)
    return false;
  good = check_rows();
  if(!good)
    return false;
  return true;
}

bool check_squares(){ //CHECKS ALL 3x3 SQUARES FOR DUPLICATES
  int count[9];
  for (int row = 0; row < 3; row++){
    for(int col = 0; col < 3; col++){
      for (int k = 0; k < 9; k++)
	count[k] = 0;
      for (int i = 0; i < 3; i++){
	for (int j = 0; j < 3; j++){
	  count[test[((3*row)+i)][((3*col)+j)] - 1]++;
	}
      }
      for (int k = 0; k < 9; k++)
	if (count[k] != 1)
	  return false;
    }
  }
  return true;
}

bool check_cols(){ //CHECKS EACH COLUMN FOR DUPLICATES
  int count[9];
  for (int row = 0; row < 9; row++){
    for (int k = 0; k < 9; k++)
      count[k] = 0;
    for(int col = 0; col < 9; col++){
      count[test[row][col] - 1]++;
    }
    for (int k = 0; k < 9; k++)
      if (count[k] != 1)
	return false;
  }
  return true;
}

bool check_rows(){ //CHECKS EACH ROW FOR DUPLICATES
  int count[9];
  for (int col = 0; col < 9; col++){
    for (int k = 0; k < 9; k++)
      count[k] = 0;
    for(int row = 0; row < 9; row++){
      count[test[row][col] - 1]++;
    }
    for (int k = 0; k < 9; k++)
      if (count[k] != 1)
	return false;
  }
  return true;
}
