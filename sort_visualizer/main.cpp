#include<raylib.h>
#include<vector>
#include<cstdlib>


using namespace std;


const int WIDTH = 1200;
const int HEIGHT = 800;
const int SIZE = 100;


int main(){

  InitWindow(WIDTH, HEIGHT, "Sorting Visualizer");

  vector<int> arr(SIZE);

  for(int i = 0;i<SIZE;i++){
    arr[i] = rand()%600 + 50;
  }

  SetTargetFPS(60);
  int i = 0;
  int j = 0;

  while(!WindowShouldClose()){
    if(i < SIZE - 1){
      if(j < SIZE - i - 1){
        if(arr[i] > arr[j + 1])
          swap(arr[j], arr[j+1]);
        j++;
      }else{
        j = 0;
        i++;
      }
    }
    BeginDrawing();
    ClearBackground(BLACK);
    for(int k = 0; k < SIZE; k++){
      Color color = GREEN;
      if(k == j || k == j + 1)
        color = RED;
      DrawRectangle(
          k*12,
          HEIGHT-arr[k],
          10,
          arr[k],
          color
          );
    }
    DrawText("Bubble Sort Visualization", 10, 10, 20, WHITE);
    EndDrawing();
  }
  CloseWindow();
}
