#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

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

bool isSymbol(char c) {
  return c != '.' && (c < '0' || c > '9');
}

bool isAdjacent(int num, int row, int startInd, int endInd, char **matrix, int rows, int cols) {
  int topBottomStart = startInd > 0 ? startInd - 1 : startInd;
  int topBottomGear = endInd < cols - 1 ? endInd + 1 : endInd;
  // check top values
  if (row > 0) {
    for (int i = topBottomStart; i < topBottomGear; i++) {
      if (isSymbol(matrix[row - 1][i])) return true;
    }
  }

  // check bottom values
  if (row < rows - 1) {
    for (int i = topBottomStart; i < topBottomGear; i++) {
      if (isSymbol(matrix[row + 1][i])) return true;
    }
  }

  // check side values
  if (startInd > 0 && isSymbol(matrix[row][startInd - 1])) return true;
  if (endInd < cols - 1 && isSymbol(matrix[row][endInd])) return true;

  return false;
}

int calculateSum(char **matrix, int rows, int cols) {
  int sum = 0;

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
      if (isAdjacent(num, i, j, j + offset, matrix, rows, cols)) {
        sum += num;
      }
      j += offset;
    }
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

  if (sum != 528819) {
    printf("Wrong solution\n");
    return 1;
  }

  return 0;
}
