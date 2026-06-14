#include <raylib.h>
#include <cstdlib>

struct Ball
{
    Vector2 position;
    Vector2 velocity;
    float radius;
    Color color;
};

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "10 Bouncing Balls");
    SetTargetFPS(60);

    Ball balls[10];

    for(int i = 0; i < 10; i++)
    {
        balls[i].position = {
            (float)(50 + rand() % 700),
            (float)(50 + rand() % 500)
        };

        balls[i].velocity = {
            (float)(100 + rand() % 300),
            (float)(100 + rand() % 300)
        };

        balls[i].radius = 20;

        balls[i].color = RED;
    }

    while(!WindowShouldClose())
    {
        float dt = GetFrameTime();

        for(int i = 0; i < 10; i++)
        {
            balls[i].position.x += balls[i].velocity.x * dt;
            balls[i].position.y += balls[i].velocity.y * dt;

            if(balls[i].position.x + balls[i].radius >= screenWidth ||
               balls[i].position.x - balls[i].radius <= 0)
            {
                balls[i].velocity.x *= -1;
            }

            if(balls[i].position.y + balls[i].radius >= screenHeight ||
               balls[i].position.y - balls[i].radius <= 0)
            {
                balls[i].velocity.y *= -1;
            }
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        for(int i = 0; i < 10; i++)
        {
            DrawCircle(
                balls[i].position.x,
                balls[i].position.y,
                balls[i].radius,
                balls[i].color
            );
        }

        EndDrawing();
    }

    CloseWindow();
}
