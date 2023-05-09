#include "raylib.h"

int main(void){
  
  Color color1, color2;
  bool sameColor = false;

  if (color1.r == color2.r && color1.g == color2.g && color1.b == color2.b && color1.a == color2.a) {
    return true;
  }
  return false;
}
