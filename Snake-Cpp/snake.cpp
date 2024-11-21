#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>
using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

const int cellSize = 30;
const int cellCount = 25;
const int screen = cellSize * cellCount;

double lastUpdateTime = 0;

bool eventTriggered(double interval)
{
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Snake
{
public:
    deque<Vector2> body = {Vector2{12, 12}, Vector2{12, 13}, Vector2{12, 14}};
    Vector2 direction = {0, -1};

    void draw()
    {
        for (unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle rec = Rectangle{x * cellSize, y * cellSize, (float)cellSize - 1, (float)cellSize - 1};
            DrawRectangleRounded(rec, 0.5, 6, darkGreen);
        }
    }

    void update()
    {
        body.pop_back();
        body.push_front(body[0] + direction);
    }

    void updateSize()
    {
        body.push_back(body[body.size()]);
    }
};

class Food
{
public:
    Vector2 position;
    Texture2D texture;

    Food()
    {
        position = getRandomPos();
        texture = LoadTexture("texture/food.png");
    }

    ~Food()
    {
        UnloadTexture(texture);
    }

    void draw()
    {
        DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
    }

    Vector2 getRandomPos()
    {
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return {x, y};
    }
};

class Game
{
    public:
        Snake snake = Snake();
        Food food = Food();

        void Draw()
        {
            food.draw();
            snake.draw();
        }

        void Update()
        {
            if(eventTriggered(0.2))
            {
                snake.update();
            }

            // if(Vector2Equals(snake.body[0], food.position))
            if(snake.body[0] == food.position)
            {
                food.position = food.getRandomPos();
                snake.updateSize();
            }
        }
};

int main()
{

    InitWindow(screen, screen, "My first game in c++!");
    SetTargetFPS(60);

    Game game = Game();

    while (!WindowShouldClose())
    {
        BeginDrawing();

        game.Update();

        if(IsKeyPressed(KEY_UP) && (game.snake.direction.y != 1))
            game.snake.direction = {0, -1};
        if(IsKeyPressed(KEY_DOWN) && (game.snake.direction.y != -1))
            game.snake.direction = {0, 1};
        if(IsKeyPressed(KEY_LEFT) && (game.snake.direction.x != 1))
            game.snake.direction = {-1, 0};
        if(IsKeyPressed(KEY_RIGHT) && (game.snake.direction.x != -1))
            game.snake.direction = {1, 0};

        ClearBackground(green);
        game.Draw();

        EndDrawing();
    }

    CloseWindow();
}