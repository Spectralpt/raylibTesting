#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "raylib.h"

int main(void)
{
  FILE *texturesFile;
  char line[50];
  int numLines = 3;

  texturesFile =  fopen("sprites/textures.txt", "r");
  if (texturesFile == NULL) {
    printf("Get Fucked");
    exit(1);
  }

  char** filenames = (char**)malloc(numLines * sizeof(char*));
  Texture2D *textures = (Texture2D*)malloc(numLines * sizeof(Texture2D));
  
  for(int i = 0; i < numLines; i++){
  fgets(line, 50, texturesFile);
  line[strcspn(line, "\r\n")] = 0; // Remove newline characters
  printf("%s\n", line);
  textures[i] = LoadTexture(line); // Copy the filename into the array
  }

  fclose(texturesFile);

  // Free the filenames array
  for(int i = 0; i < numLines; i++){
    free(filenames[i]);
  }
  free(filenames);

  return 0;
}


