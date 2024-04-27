#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define GEAR_SYMBOL '*'

// What is the sum of all of the gear ratios in your engine schematic?

struct Gear {
  int row, col, ratio;
  bool isComplete; // if has multiplied 2 part numbers already
  bool isOverParted; // if exceeded 2 adjacent parts
};

// Including invalid gears: those with only 1 part and those with too many parts. 
int numGears = 0;
int gearArraySize = 0;
struct Gear *gears = NULL;

// Inspired by https://stackoverflow.com/questions/48936647/read-a-matrix-from-a-file
char** readMatrix(int *rows, int *cols) {
  *rows = 0;
  *cols = 0;
  const char *filename = "input.txt";
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "could not open %s: %s\n", filename, strerror(errno));
    return NULL;
  }

  char **matrix = NULL;
  char line[1024];

  char *ret;
  while((ret = fgets(line, sizeof line, fp))) {
    // calculate cols with first line 
    if (*cols == 0) {
      *cols = strlen(line);
      if (line[*cols - 1] == '\n') (*cols)--;
    }

    matrix = realloc(matrix, (*rows + 1) * sizeof *matrix);
    if (matrix == NULL) {
      fprintf(stderr, "could not reallocate matrix: %s\n", strerror(errno));
      fclose(fp);
      return NULL;
    }

    matrix[*rows] = calloc(*cols, sizeof matrix[*rows][0]);
    if(matrix[*rows] == NULL) {
      fprintf(stderr, "could not allocate line: %s\n", strerror(errno));
      fclose(fp);
      free(matrix);
      return NULL;
    }

    for (int i = 0; i < *cols; i++) {
      matrix[*rows][i] = line[i];
    }

    (*rows)++;
  }

  fclose(fp);
  return matrix;
}

// For debugging.
void printMatrix(char **matrix, int rows, int cols) {
  printf("%d rows | %d cols\n", rows, cols);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++)
    {
      printf("%c", matrix[i][j]);
    }
    printf("\n");
  }
  printf("Finished print\n");
}

void attributeNumToSingleGear(int num, int gearRow, int gearCol) {
  // find gear
  for (int i = 0; i < numGears; i++) {
    if (gears[i].row == gearRow && gears[i].col == gearCol) {
      if (gears[i].isComplete) gears[i].isOverParted = true; // assuming that already over-parted gears have (isComplete == full)
      else { // attribute second part
        gears[i].ratio *= num;
        gears[i].isComplete = true;
      }
      return;
    }
  }

  // create gear if not found
  if (numGears + 1 > gearArraySize) {
    gearArraySize += 100;
    gears = realloc(gears, gearArraySize * sizeof(struct Gear));
  }
  gears[numGears] = (struct Gear) { .row = gearRow, .col = gearCol, .ratio = num, .isComplete = false, .isOverParted = false }; // create with first part
  numGears += 1;
}

// Looks for gears adjacent to the number, and processes the number in each.
// No longer need to check for all symbols, as we only care for gears ('*').
// The global gears array is allocated here. Assumes it starts as NULL.
// Indirect output: the gears array, initialized with all processed gears.
void attributeNumToGears(int num, int row, int startInd, int endInd, char **matrix, int rows, int cols) {
  int topBottomStart = startInd > 0 ? startInd - 1 : startInd;
  int topBottomGear = endInd < cols - 1 ? endInd + 1 : endInd;
  // check top values
  if (row > 0) {
    for (int i = topBottomStart; i < topBottomGear; i++) {
      if (matrix[row - 1][i] == GEAR_SYMBOL) attributeNumToSingleGear(num, row - 1, i);
    }
  }

  // check bottom values
  if (row < rows - 1) {
    for (int i = topBottomStart; i < topBottomGear; i++) {
      if (matrix[row + 1][i] == GEAR_SYMBOL) attributeNumToSingleGear(num, row + 1, i);
    }
  }

  // check side values
  if (startInd > 0 && matrix[row][startInd - 1] == GEAR_SYMBOL) attributeNumToSingleGear(num, row, startInd - 1);
  if (endInd < cols - 1 && matrix[row][endInd] == GEAR_SYMBOL) attributeNumToSingleGear(num, row, endInd);
}

int calculateSum(char **matrix, int rows, int cols) {
  // initialize gears array
  for (int i = 0; i < rows; i++) {
    int j = 0;
    while (j < cols) {
      char c = matrix[i][j];
      // iterate until finding a digit
      if (c < '0' || c > '9') {
        j++;
        continue;
      }

      int num, offset;
      sscanf(matrix[i] + j, "%d%n", &num, &offset);
      attributeNumToGears(num, i, j, j + offset, matrix, rows, cols);
      j += offset;
    }
  }

  // iterate gears
  int sum = 0;
  for (int i = 0; i < numGears; i++) {
    if (!gears[i].isComplete || gears[i].isOverParted) continue;
    sum += gears[i].ratio;
  }
  return sum;
}

int main() {
  int rows, cols;
  char **matrix = readMatrix(&rows, &cols);
  if (matrix == NULL) {
    fprintf(stderr, "could not read matrix\n");
    return 1;
  }

  // Verify read.
  // printMatrix(matrix, rows, cols);

  int sum = calculateSum(matrix, rows, cols);
  printf("Sum: %d\n", sum);

  free(matrix);
  free(gears);

  if (sum != 80403602) {
    printf("Wrong solution\n");
    return 1;
  }

  return 0;
}
