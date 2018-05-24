// ========================================================= MAZE STUFF =============================================================
#define MAZE_LEN        16

#define FRONT_WALL_DETECTED       ((frontREC1 > 250) && (frontREC2 > 250))
#define LEFT_WALL_DETECTED        (leftREC > 250)
#define RIGHT_WALL_DETECTED       (rightREC > 250)      

enum DIRECTION {
  NORTH, SOUTH, EAST, WEST
};
DIRECTION curDirection = NORTH;


// Maze Cell Information
short distances[MAZE_LEN][MAZE_LEN];
bool northWalls[MAZE_LEN][MAZE_LEN] = {0};
bool southWalls[MAZE_LEN][MAZE_LEN] = {0};
bool eastWalls[MAZE_LEN][MAZE_LEN] = {0};
bool westWalls[MAZE_LEN][MAZE_LEN] = {0};


// current coordinates
short curX = 0;
short curY = 0;
short curDist = MAZE_LEN - 2;


void initializeMaze() {
  // Set Manhattan Distances
  for (unsigned short y = 0; y < MAZE_LEN / 2; y++) {
    for (unsigned short x = 0; x < MAZE_LEN / 2; x++) {
      distances[y][x] =
        distances[y][MAZE_LEN - x - 1] =
        distances[MAZE_LEN - y - 1][x] =
        distances[MAZE_LEN - y - 1][MAZE_LEN - x - 1] =
        MAZE_LEN - 2 - y - x;
    }
  }

  // Set Maze Boundary Walls
  for (int i = 0; i < MAZE_LEN; i++) {
    southWalls[i][0] = 1;
    northWalls[i][MAZE_LEN - 1] = 1;
    westWalls[0][i] = 0;
    eastWalls[MAZE_LEN - 1][i] = 1;
  }
  // Set default side starting wall
  eastWalls[0][0] = 1;
}

void mapWalls() {
  if(curDirection == NORTH) {
    if (FRONT_WALL_DETECTED) {
      northWalls[curX][curY] = 1;
      if (curY < MAZE_LEN - 1)
        southWalls[curX][curY + 1] = 1;
    }
    if (LEFT_WALL_DETECTED) {
      westWalls[curX][curY] = 1;
      if (curX > 0)
        eastWalls[curX - 1][curY] = 1; 
    }

    if (RIGHT_WALL_DETECTED) {
      eastWalls[curX][curY] = 1;
      if (curX < MAZE_LEN - 1)
        westWalls[curX + 1][curY] = 1;
    }
  }

  if(curDirection == SOUTH) {
    if (FRONT_WALL_DETECTED) {
      southWalls[curX][curY] = 1;
      if (curY > 0)
        northWalls[curX][curY - 1] = 1;
    }
    if (LEFT_WALL_DETECTED) {
      eastWalls[curX][curY] = 1;
      if (curX < MAZE_LEN - 1)
        westWalls[curX + 1][curY] = 1; 
    }

    if (RIGHT_WALL_DETECTED) {
      westWalls[curX][curY] = 1;
      if (curX > 0)
        eastWalls[curX - 1][curY] = 1;
    }
  }
  
  if(curDirection == EAST) {
    if (FRONT_WALL_DETECTED) {
      eastWalls[curX][curY] = 1;
      if (curX < MAZE_LEN - 1)
        westWalls[curX+1][curY] = 1;
    }
    if (LEFT_WALL_DETECTED) {
      northWalls[curX][curY] = 1;
      if (curY < MAZE_LEN - 1)
        southWalls[curX][curY + 1] = 1; 
    }

    if (RIGHT_WALL_DETECTED) {
      southWalls[curX][curY] = 1;
      if (curY > 0)
        northWalls[curX - 1][curY] = 1;
    }
  }

  if(curDirection == WEST) {
    if (FRONT_WALL_DETECTED) {
      westWalls[curX][curY] = 1;
      if (curX > 0)
        eastWalls[curX-1][curY] = 1;
    }
    if (LEFT_WALL_DETECTED) {
      southWalls[curX][curY] = 1;
      if (curY > 0)
        northWalls[curX][curY - 1] = 1; 
    }

    if (RIGHT_WALL_DETECTED) {
      northWalls[curX][curY] = 1;
      if (curY < MAZE_LEN - 1)
        southWalls[curX][curY + 1] = 1;
    }
  }
}
bool floodFillFlag = false; // turn true only when you need to run floodfill

void floodFill() {
  if (curDist == 0)
    return;
}

// advance() follows manhattan distances, until it gets stuck
void advance() {
    while(curDist >= 0) {
      mydisplay.home();
      mydisplay.clear();
      mydisplay.print(curX);
      mydisplay.print(curY);
      mydisplay.print(curDist);
      delay(1000);
      if (!FRONT_WALL_DETECTED){
        if(curDirection == NORTH) {
          if (distances[curX][curY + 1] == curDist - 1) {
            curY++;
            forward(1);
            curDist = distances[curX][curY];
            mapWalls();
            continue;
          }
        }
        else if(curDirection == SOUTH) {
          if (distances[curX][curY - 1] == curDist - 1) {
            curY--;
            forward(1);
            curDist = distances[curX][curY];
            mapWalls();
            continue;
          }
        }
        else if (curDirection == EAST) {
          if (distances[curX + 1][curY] == curDist - 1) {
            curX++;
            forward(1);
            curDist = distances[curX][curY];
            mapWalls();
            continue;
          }
        }
        else if (curDirection == WEST) {
          if (distances[curX - 1][curY] == curDist -1) {
           curX--;
           forward(1);
           curDist = distances[curX][curY];
           mapWalls();
           continue;
          }
        }
      }
      if (!LEFT_WALL_DETECTED) {
        if(curDirection == NORTH) {
          if (distances[curX-1][curY] == curDist - 1) {
           curX--;
            left90();
            curDirection = WEST;
            forward(1);
            curDist = distances[curX][curY];
            mapWalls();
            continue;
          }
        }
        else if(curDirection == SOUTH) {
          if (distances[curX+1][curY] == curDist - 1) {
            curX++;
            left90();
            curDirection = EAST;
            forward(1);
            curDist = distances[curX][curY];
            mapWalls();
            continue;
          }
        }
        else if(curDirection == EAST) {
          if (distances[curX][curY+1] == curDist - 1) {
            curY++;
            left90();
            curDirection = NORTH;
            forward(1);
            curDist = distances[curX][curY];
            mapWalls();
            continue;
          }
        }
        else if(curDirection == WEST) {
          if (distances[curX][curY-1] == curDist - 1) {
            curY--;
            left90();
            curDirection = SOUTH;
            forward(1);
            curDist = distances[curX][curY];
            mapWalls();
            continue;
          }            
        }
      }
      if (!RIGHT_WALL_DETECTED) {
        if(curDirection == NORTH) {
          if (distances[curX+1][curY] == curDist - 1) {
            curX++;
            right90();
            curDirection = EAST;
            forward(1);
            curDist = distances[curX][curY];
            mapWalls();
            continue;
          }
        }
        else if(curDirection == SOUTH) {
          if (distances[curX-1][curY] == curDist - 1) {
            curX--;
            right90();
            curDirection = WEST;
            forward(1);
            curDist = distances[curX][curY];
            mapWalls();
            continue;
          }
        }
        else if(curDirection == EAST) {
          if (distances[curX][curY-1] == curDist - 1) {
            curY++;
            right90();
            curDirection = SOUTH;
            forward(1);
            curDist = distances[curX][curY];
            mapWalls();
            continue;
          }
        }
        else if(curDirection == WEST) {
          if (distances[curX][curY+1] == curDist - 1) {
            curY--;
            right90();
            curDirection = NORTH;
            forward(1);
            curDist = distances[curX][curY];
            mapWalls();
            continue;
          }             
        }
      }
      curDist = distances[curX][curY];
    }
}
// -----------------------------------------------------------------------------------------------------------------------------



