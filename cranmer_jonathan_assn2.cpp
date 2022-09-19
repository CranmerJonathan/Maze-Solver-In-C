/* 
Name: Jonathan Cranmer  
Date: June 10th, 2022
Description: This code creates a maze for a user and uses a recursive wavepath algorithm to find the shortest path from a starting position to an end position. This code identifies diagonal moves as equal in length to vertical or horizontal moves. 
Usage: The command line of this code is <exe>. To use the code, simply follow the prompts :).
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <limits>
using std::cout;
using std::cin;
using std::endl;
using std::setw;
using std::streamsize;
using std::max;
using std::numeric_limits;

int** createIntArray(int, int);
int** fillIntArray(int**, int, int);
void PrintEnvironment(int**, int, int);
int** randOnes(int**, int, int, int);
int** check(int**, int, int, int, int, int);
char** pathFinder(char** ,int**, int, int, int, int);
char** createCharArray(int, int);
char** fillCharArray(char**, int**, int , int);
void printCharArray(char**, int, int);
int correctDataTypeInt();
char correctDataTypeChar();
int getWidth();
int getHeight();
int getWallsPercent();
int getGoalX(int);
int getGoalY(int);
int getStartX(int);
int getStartY(int);

int main(int argc, char** argv)
{
  //Ties rand() function to a random time
  srand(time(NULL));
  
  //create maze and solve if console command is valid
  if (argc == 1)
    { 
      //Initializing all of the needed variables
      int width = 0, height = 0, walls, **intMazeArray, goalX = -1, goalY = -1, startX = 0, startY = 0;
      char **charMazeArray;
      
      //getting user defined width and height
      cout << "Welcome to Jonathan Cranmer's CSE240 WaveFront Pather" << endl << endl;

      //Get user height, width, and percentage of walls
      width = getWidth(); 
      height = getHeight();
      walls = getWallsPercent();
      
      //Create array
      intMazeArray = createIntArray(width, height);
      intMazeArray = fillIntArray(intMazeArray, width, height);
      intMazeArray = randOnes(intMazeArray, width, height, walls);
	  
      //Prints array
      PrintEnvironment(intMazeArray, width, height);

      //Ends code if maze is unsolvable
      if(walls == 100)
	{
	  cout <<"The maze is unsolvable";
	  return 0;
	}

      goalX = getGoalX(width);
      goalY = getGoalY(height);
      
      //Check to make sure goal position isn't on impassible terrain
      while (intMazeArray[goalY][goalX] == -1)
        {
          cout << "The Goal Position is on an impassible terrain" << endl;
	  goalX = getGoalX(width);
	  goalY = getGoalY(height);
        }


      //Creates Paths around Goal Position
      intMazeArray[goalY][goalX] = 1;
      intMazeArray = check(intMazeArray, width, height, goalX, goalY, 2);
      PrintEnvironment(intMazeArray, width, height);

      //Creates character version of maze array
      charMazeArray = createCharArray(width, height);
      charMazeArray = fillCharArray(charMazeArray, intMazeArray, width, height);
      charMazeArray[goalY][goalX] = '$';
      printCharArray(charMazeArray, width, height);
      
      //Prompt user with Start position
      startX = getStartX(width);
      startY = getStartY(height); 

      //Check if Start position is on Goal Position
      while(intMazeArray[startY][startX] == 1)
        {
          cout << "The Start Position is on the Goal Position. ";
	  startX = getStartX(width);
	  startY = getStartY(height);
        }

      //check to see if start position is on impassible terrain
      while(intMazeArray[startY][startX] == -1)
	{
	  cout << "The Start Position is on an impassible terrain square. ";
	
	  startX = getStartX(width);
	  startY = getStartY(height);

	  //Check if Start position is on Goal Position
	  while(intMazeArray[startY][startX] == 1)
	    {
	      cout << "The Start Position is on the Goal Position. ";
	      startX = getStartX(width);
	      startY = getStartY(height);
	    }
	}


      //Find the Path from Start Position to Goal Position
      charMazeArray = pathFinder(charMazeArray, intMazeArray, width, height, startX, startY);
      printCharArray(charMazeArray, width, height);

      //closing arrays
      int y;
      for(y = 0; y< height; y++)
	{
	  delete[] intMazeArray[y];
	}
      delete[] intMazeArray;

      for(y = 0; y < height; y++)
	{
	  delete[] charMazeArray[y];
	}
      delete[] charMazeArray;

      return 0;
    }

  //incorrect console call.
  else 
	 {
	   cout << "Error -- usage: <exe>" << endl;
	   return 0;
	 }
}

//Creates an int array
int** createIntArray(int width, int height)
{
  //Dynamically allocates array
  int** array;
  array = new int* [height];
  for (int i = 0; i < height; i++)
    {
      array [i] = new int[width];
    }

  return array;
}

//Fill array with 0's
int** fillIntArray(int** array,int width, int height)
{
  //variables
  int x,y;

  //filling array with 0's
  for (y = 0; y < height; y++)
    {
      for (x = 0; x < width; x++)
	{
	  array[y][x] = 0;
	}
    }

  return array;
}

//Prints out an int array/environment
void PrintEnvironment(int** array, int width, int height)
{
  //vairables
  int x, y;

  //printing out the array
    for (y = 0; y < height; y++)
    {
      for (x = 0; x < width; x++)
	{
	  cout << setw(3) << array[y][x];
	}
      cout << endl;
    }
}

//Takes in the percent of impassible terrain and fills the array with said terrain
int** randOnes(int** array, int width, int height, int percent)
{
  //variables
  int numPercent, total, x, y, i;
  
  //Total amounts of squares
  total = width * height;

  //Number of impassible terrain using total number of squares and percentage of impassible terrain
  numPercent = (percent * total) / 100;

  //Places the impassible terrain randomly
  for (i = 0; i < numPercent; i++)
   {
     y = rand() % height;
     x = rand() % width;
     if (array[y][x] != -1)
       {
  	 array[y][x] = -1;
       }
     else 
       {
	 numPercent++;
       }
   }

  //Prints out terrain
  cout << numPercent << endl;

  return array;
}

//Checks the neighborsquares and fills the array with values using recursion.
//These values depend on how far away they are from the goal value.
int** check(int** array, int width, int height, 
	    int checkW, int checkH, int pathValue)
{ 
  //checks squares where Width doesn't change
  if(checkH + 1 < height)
    {
      if (array[checkH + 1][checkW] == 0 || array[checkH + 1][checkW] > pathValue)
	{
	  array[checkH + 1][checkW] = pathValue;
	  array = check(array, width, height, checkW, checkH + 1, pathValue + 1);
	}
    }
  if(checkH - 1 >= 0)
    {
      if (array[checkH - 1][checkW] == 0 || array[checkH - 1][checkW] > pathValue)
        {
	  array[checkH - 1][checkW] = pathValue;
	  array = check(array, width, height, checkW, checkH - 1, pathValue + 1);
	}
    }

  //checks squares where Width + 1
  if(checkW + 1 < width)
    {
      if (array[checkH][checkW + 1] == 0 || array[checkH][checkW + 1] > pathValue)
	{
	  array[checkH][checkW + 1] = pathValue;
	  array = check(array, width, height, checkW + 1, checkH, pathValue + 1);
	}
      if(checkH + 1 < height)
	{
	  if (array[checkH + 1][checkW + 1] == 0 || array[checkH + 1][checkW + 1] > pathValue)
	    {
	      array[checkH + 1][checkW + 1] = pathValue;
	      array = check(array, width, height, checkW + 1, checkH + 1, pathValue + 1);
	    }
	}   
      if(checkH - 1 >= 0)
	{
	  if (array[checkH - 1][checkW + 1] == 0 || array[checkH - 1][checkW + 1] > pathValue)
	    {
	      array[checkH - 1][checkW + 1] = pathValue;
	      array = check(array, width, height, checkW + 1, checkH - 1, pathValue + 1);
	    }
	}
    }

  //checks squares where Width - 1
  if(checkW - 1 >= 0)
    {
      if (array[checkH][checkW - 1] == 0 || array[checkH][checkW - 1] > pathValue)
        {
	  array[checkH][checkW - 1] = pathValue;
	  array = check(array, width, height, checkW - 1, checkH, pathValue + 1);
	}
      if(checkH + 1 < height)
        {
          if (array[checkH + 1][checkW - 1] == 0 || array[checkH + 1][checkW - 1] > pathValue)
            {
	      array[checkH + 1][checkW - 1] = pathValue;
	      array = check(array, width, height, checkW - 1, checkH + 1, pathValue + 1);
	    }
        }
      if(checkH - 1 >= 0)
        {
          if (array[checkH - 1][checkW - 1] == 0 || array[checkH - 1][checkW - 1] > pathValue)
            {
	      array[checkH - 1][checkW - 1] = pathValue;
	      array = check(array, width, height, checkW - 1, checkH - 1, pathValue + 1);
	    }
        }
    }

  return array;
}

//Finds the fastes route from start to finish and creates a path of * to the destination using recursion
char** pathFinder(char** charArray, int** intArray, 
		  int width, int height, int checkW, int checkH)
{
  //fills Starting position with @
  if(charArray[checkH][checkW] == ' ')
    {
      charArray[checkH][checkW] = '@';
    }

  //Checks to makes sure within bounds and checks the squares with width+1 and height+1
  if(checkW + 1 < width)
    {
      if(checkH + 1 < height)
	{
	  if(intArray[checkH + 1][checkW + 1] == 1)
	    {
	      charArray[checkH + 1][checkW + 1] = '$';
	      return charArray;
	    }
	  if(intArray[checkH + 1][checkW + 1] < intArray[checkH][checkW] && intArray[checkH + 1][checkW + 1] != -1)
	    {
	      charArray[checkH + 1][checkW + 1] = '*';
	      charArray = pathFinder(charArray, intArray, width, height, checkW + 1, checkH + 1);
	      return charArray;
	    }
	}
    }

  //Checks to makes sure within bounds and checks the squares with width+1 and height-1     
  if(checkW + 1 < width)
    {
      if(checkH - 1 >= 0)
        {
	  if(intArray[checkH - 1][checkW + 1] == 1)
            {
	      charArray[checkH - 1][checkW + 1] = '$';
              return charArray;
            }
          if(intArray[checkH - 1][checkW + 1] < intArray[checkH][checkW] && intArray[checkH - 1][checkW + 1] != -1)
            {
	      charArray[checkH - 1][checkW + 1] = '*';
              charArray = pathFinder(charArray, intArray, width, height, checkW + 1, checkH - 1);
	      return charArray;
	    }
        }
    }

  //Checks to makes sure within bounds and checks the squares with width-1 and height+1     
  if(checkW - 1 >= 0)
    {
      if(checkH + 1 < height)
        {
	  if(intArray[checkH + 1][checkW - 1] == 1)
            {
	      charArray[checkH + 1][checkW - 1] = '$';
              return charArray;
            }
          if(intArray[checkH + 1][checkW - 1] < intArray[checkH][checkW] && intArray[checkH + 1][checkW - 1] != -1)
            {
	      charArray[checkH + 1][checkW - 1] = '*';
              charArray = pathFinder(charArray, intArray, width, height, checkW - 1, checkH + 1);
	      return charArray;
	    }
        }
    }

  //Checks to makes sure within bounds and checks the squares with width-1 and height-1     
  if(checkW - 1 >= 0)
    {
      if(checkH - 1 >= 0)
        {
	  if(intArray[checkH - 1][checkW - 1] == 1)
            {
	      charArray[checkH - 1][checkW - 1] = '$';
              return charArray;
            }
          if(intArray[checkH - 1][checkW - 1] < intArray[checkH][checkW] && intArray[checkH - 1][checkW - 1] != -1)
            {
	      charArray[checkH - 1][checkW - 1] = '*';
              charArray = pathFinder(charArray, intArray, width, height, checkW - 1, checkH - 1);
	      return charArray;
	      }
        }
    }

  //Checks to makes sure within bounds and checks the squares with width+1     
  if(checkW + 1 < width)
    {
      if(intArray[checkH][checkW + 1] == 1)
	{
	  charArray[checkH][checkW + 1] = '$';
	  return charArray;
	}
      if(intArray[checkH][checkW + 1] < intArray[checkH][checkW] && intArray[checkH][checkW + 1] != -1)
	{
	  charArray[checkH][checkW + 1] = '*';
	  charArray = pathFinder(charArray, intArray, width , height, checkW + 1, checkH);
	  return charArray;
	}
    }

  //Checks to makes sure within bounds and checks the squares with width-1     
  if(checkW - 1 >= 0)
    {
      if(intArray[checkH][checkW - 1] == 1)
	{
	  charArray[checkH][checkW - 1] = '$';
	  return charArray;
	}
      if(intArray[checkH][checkW - 1] < intArray[checkH][checkW] && intArray[checkH][checkW - 1] != -1)
	{
	  charArray[checkH][checkW - 1] = '*';
	  charArray = pathFinder(charArray, intArray, width, height, checkW - 1, checkH);
	  return charArray;
	}
    }

  //Checks to makes sure within bounds and checks the squares with height+1     
  if(checkH + 1 < height)
    {
      if(intArray[checkH + 1][checkW] == 1)
	{
	  charArray[checkH + 1][checkW] = '$';
	  return charArray;
	}
      if(intArray[checkH + 1][checkW] < intArray[checkH][checkW] && intArray[checkH + 1][checkW] != -1)
	{
	  charArray[checkH + 1][checkW] = '*';
	  charArray = pathFinder(charArray, intArray, width, height, checkW, checkH + 1);
	  return charArray;
	}
    }

  //Checks to makes sure within bounds and checks the squares with height-1     
  if(checkH - 1 >= 0)
    {
      if(intArray[checkH - 1][checkW] == 1)
	{
	  charArray[checkH - 1][checkW] = '$';
	  return charArray;
	}
      if(intArray[checkH - 1][checkW] < intArray[checkH][checkW] && intArray[checkH - 1][checkW] != -1)
	{
	  charArray[checkH - 1][checkW] = '*';
	  charArray = pathFinder(charArray, intArray, width, height, checkW, checkH - 1);
	  return charArray;
	}
    }
  return charArray;
}

//Creates a character array
char** createCharArray(int width, int height)
{
  //dynamically allocates a character array with size height*width
  char** array;
  array = new char* [height];
  int i;
  for (i = 0; i < height; i++)
    {
      array [i] = new char[width];
    }

  return array;
}

//Fill character array with spaces on passible terrain
//Fill character array with # on impassible terrain
char** fillCharArray(char** charArray ,int** intArray,int width, int height)
{
  //variables
  int x,y;

  //Loops throught to print spaces on passible terrain and # on impassible terrain
  for (y = 0; y < height; y++)
    {
      for (x = 0; x < width; x++)
        {
	  if(intArray[y][x] == -1)
	    {
	      charArray[y][x] = '#';
	    }
          else
	    {
	      charArray[y][x] = ' ';
	    }
        }
    }

  return charArray;
}

//Prints character array
void printCharArray(char** array, int width, int height)
{
  //varaibles
  int x,y;

  //Prints characterarray
  for(y = 0; y < height; y++)
    {
      for(x = 0; x < width; x++)
	{
	  cout << setw(3) << array[y][x];
	}
      cout << endl;
    }
}

//Checks to make sure cin stream only has correct data types
int correctDataTypeInt()
{
  int userInput = 0;

  while(!(cin >> userInput))
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Invalid input. Try again: ";
    }
    
  return userInput;
}

//Checks to make sure cin stream only has correct data types
char correctDataTypeChar()
{
  char userInput = 'a';

  while (!(cin >> userInput))
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Invalid input. Try again: ";
    }
 
  return userInput;
}

//Gets width from user
int getWidth()
{
  int width;

  cout << "Enter the width (at least 10): ";
  width = correctDataTypeInt();

  //If width is too big reprompt the user for a new width                   
  while(width > 1000)
    {
      cout << "The maze width is too big."
	   << " Please enter a new width";
      width = correctDataTypeInt();
    }

  while (width < 10)
    {
      cout << "Your width is below 10 or you inputed an invalid value"
	   << " please enter a new width" << endl
	   << " Enter the width (at least 10): ";
      width = correctDataTypeInt();
      while(width > 1000)
	{
	  cout << "The maze width is too big."
	       << " Please enter a new width";
	  width = correctDataTypeInt();
	}
    }
  
  return width;
}

int getHeight()
{
  int height;

  cout << "Enter the height (at least 10): ";
  height = correctDataTypeInt();

  //If height is too big reprompt  the user for a new height                                                                                                                                            
  while(height > 1000)
    {
      cout << "The maze hieght is too big."
	   << " Please enter a new height: ";
      height = correctDataTypeInt();
    }

  //If height is below 10 reprompt user until height is between 10-1000                                                                                                                                 
  while (height < 10)
    {
      cout << "Your width is below 10 or you inputed an invalid value"
	   << " please enter a new height" << endl
	   << "Enter the height (at least 10): ";
      height = correctDataTypeInt();
      while (height > 1000)
	{
	  cout << "The maze hieght is too big."
	       << " Please enter a new height: ";
	  height = correctDataTypeInt();
	}
    }

  return height;
}

//get walls percentage from user
int getWallsPercent()
{
  int walls;
  char wallsPrompt = 'n';

  //Get the percent of impassible terrain from the user                                                                                                                                                 
  cout << "Enter the percent of impassible terrain (0 - 100): ";
  walls = correctDataTypeInt();

  //If percent of impassible terrain is negative, reprompts the user                                                                                                                                    
  while(walls < 0 || walls > 100)
    {
      cout << "The value of impassible terrain must be between 0 - 100. "
	   << "enter a new percent of impassible terrain";
      walls = correctDataTypeInt();
    }

  //If percent of impassible terrain > 35, make sure the user is ok with that and if not reprompt them.                                                                                                 
  while (walls > 35 && wallsPrompt == 'n')
    {
      cout << "Having a value greater than 35% might create poor results, do you wish to continue? (y/n): ";
      wallsPrompt = correctDataTypeChar();
      while (wallsPrompt != 'y' && wallsPrompt != 'n')
	{
	  cout << "This input is in the incorrect format "
	       << "Having a value greater than 35% might create poor results, do you wish to continue? (y/n): ";
	  wallsPrompt = correctDataTypeChar();
	}
      if(wallsPrompt == 'n')
	{
	  cout << "Enter the new percent of impassible terrain (0 - 35): ";
	  walls = correctDataTypeInt();
	  while(walls < 0 || walls > 100)
	    {
	      cout << "The value of impassible terrain must be inbetween (0 - 100) "
		   << "Enter a new percent of impassible terrain";
	      walls = correctDataTypeInt();
	    }
	}
    }

  return walls;
}

int getGoalX(int width)
{
  int goalX;

  //Prompt User for Goal Position                                                                                                                                                                       
  cout << "Please enter the Goal Position X: (0 - " << width - 1 << ")";
  goalX = correctDataTypeInt();

  //Check to make sure that goal position is within the bounds of the array on the x position                                                                                                           
  while (goalX < 0 || goalX >= width)
    {
      cout << "The Goal Position X was outside the boundaries of the maze."
	   << "Please enter the Goal Position X: ";
      goalX = correctDataTypeInt();
    }

  return goalX;
}

int getGoalY(int height)
{
  int goalY;

  //Prompt User for Goal Position                                                                                                                                                                       
  cout << "Please enter the Goal Position Y: (0 - " << height - 1 << ")";
  goalY = correctDataTypeInt();


  //Check to make sure that goal position is within the bounds of the array on the y position                                                                                                           
  while (goalY < 0 || goalY >= height)
    {
      cout << "The Goal Positionb Y was outside the boundaries of the maze."
	   << "Please enter a new Goal Position Y: ";
      goalY = correctDataTypeInt();
    }
  

  return goalY;
}

int getStartX(int width)
{
  int startX;

  cout << "Please enter the Start Position X: ";
  startX = correctDataTypeInt();

  //check if on impassible terrain or goal position within x bounds             
  while (startX < 0 || startX >= width)
    {
      cout << "The Start Position X was outside the boundaries of the maze."
	   << " Please enter the Start Position X: ";
      startX = correctDataTypeInt();
    }

  return startX;
}

int getStartY(int height)
{
  int startY;

  cout << "Please enter the Start Position Y: ";
  startY = correctDataTypeInt();

  //check if on impassible terrain or goal position within y bounds         
  while (startY < 0 || startY >= height)
    {
      cout << "The Start Positionb Y was outside the boundaries of the maze. "
	   << "Please enter a new Start Position Y: ";
      startY = correctDataTypeInt();
    }


  return startY;
}
