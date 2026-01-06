#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINE_SIZE 1024

double pullMaterialsFromSheet(const char *filename, int start_row, int column){


  FILE *file = fopen(filename, "r");
  if (!file){
    perror("failed to open file");
    return 1;
  }

  char line[LINE_SIZE];
  int current_row = 0;
  double sum = 0.0;

  while(fgets(line, sizeof(line), file)){
    if (current_row >= start_row){
      char *token;
      int current_col = 0;

      token = strtok(line, ",");

      while (token){
        if (current_col == column){
          sum += atof(token);
          break;
        }
        token = strtok(NULL, ",");
        current_col++;
      }
    }
    current_row++;
  }
  fclose(file);

  printf("Sum = %.2f\n", sum);
  return sum;

}

int main(){
  printf("wow\n");
}
