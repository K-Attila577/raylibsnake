#include "raylib.h"
#include "resource_dir.h"
#include <vector>	
#include <random>
#include <string>

struct Point
{
    int x;
    int y;
};
int screenwidth = 800;
int screenheight = 800;
float gridsize = screenwidth /20.0f;
std::vector<Point> snakeBody = {{10,9},{11,9},{12,9}};
std::vector<Point> foodCoord;
KeyboardKey lastKey = KEY_NULL;
bool shouldResetKey = false;
int framesToWaitAfterReset = 5;
bool isGameOver =  false;
double lastTime;
int score = 0;
std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 19);
int foodX;
int foodY;
bool isfoodready = false;
bool grow = false;
bool timer(float difference)
{
    double currentTime = GetTime();
    if(currentTime - lastTime >= difference)
    {
        lastTime = currentTime;
        return true;
    }
    return false;
}
void drawGrid();void food();void snake();void collision();void gameover();void movement();
int randomNum() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dist(1, 20);
    return dist(gen);
}
int main ()
{
	SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    SetTargetFPS(60);
	InitWindow(screenwidth, screenheight, "Hello Raylib");
	SearchAndSetResourceDir("resources");
	while (!WindowShouldClose())		
	{
		BeginDrawing();
		ClearBackground(RED);
		drawGrid();
        food();
        snake();
        
        if(!isGameOver)
        {
            movement();
        }
        
        DrawText(std::to_string(score).c_str(),0,0,50,RED);
        
        if(isGameOver)
        {
            gameover();
        }
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
void drawGrid()
{
    const float borderThickness = 0.5f; 
    for(int i = 0; i < gridsize; i++)
    {
        for(int j = 0; j < gridsize; j++)
        {
            Color cellColor = (i + j) % 2 == 0 ? LIME : DARKGREEN;
            DrawRectangle(
                i * gridsize + borderThickness, 
                j * gridsize + borderThickness, 
                gridsize - 2*borderThickness, 
                gridsize - 2*borderThickness, 
                cellColor
            );
        }
    }
}
void food()
{
    

    while(!isfoodready)
    {
        foodX = distrib(gen);
        foodY = distrib(gen);
        for(int i = 0; i<snakeBody.size();i++)
        {
            if(foodX == snakeBody[i].x && foodY == snakeBody[i].y){isfoodready=false;}
            else{isfoodready=true;}

        }
    }
    DrawRectangle(foodX*gridsize,foodY*gridsize,gridsize,gridsize,RED);
    

}
void snake()
{
    for(int i = 0; i < snakeBody.size(); i++)
    {
        DrawRectangle(snakeBody[i].x * gridsize, snakeBody[i].y * gridsize, gridsize, gridsize, BLACK);
    }
}
void collision()
{
        for(int i = 1; i < snakeBody.size();i++)
        {
            if(snakeBody[0].x == snakeBody[i].x && snakeBody[0].y == snakeBody[i].y){isGameOver=true;}
            
    
        }
        if(snakeBody[0].x == foodX && snakeBody[0].y == foodY)
        {
            grow=true;
        }
        
        

        
}

void gameover()
{
    int textSize = MeasureText("Game Over", 50);
    DrawText("Game Over", (screenwidth - textSize) / 2, 400, 50, RED);
    int anykey = GetKeyPressed();
    if (anykey != 0)
    {
        snakeBody = {{10, 9}, {11, 9}, {12, 9}};
        lastKey = KEY_NULL;
        shouldResetKey = true;
        framesToWaitAfterReset = 5;
        isGameOver = false;
    }
    score = 0;
    isfoodready=false;
}

void movement()
{
        if (shouldResetKey)
    {
        if (framesToWaitAfterReset > 0)
        {
            framesToWaitAfterReset--;
            return; 
        }
        lastKey = KEY_NULL;
        shouldResetKey = false;
    }
   
    if (IsKeyDown(KEY_RIGHT) && lastKey != KEY_LEFT) lastKey = KEY_RIGHT;
    if (IsKeyDown(KEY_LEFT) && lastKey != KEY_RIGHT) lastKey = KEY_LEFT;
    if (IsKeyDown(KEY_DOWN) && lastKey != KEY_UP) lastKey = KEY_DOWN;
    if (IsKeyDown(KEY_UP) && lastKey != KEY_DOWN) lastKey = KEY_UP;
    if ( timer(0.13f))
    {
        
        Point oldHead = snakeBody[0];
        switch (lastKey)
        {
            case KEY_RIGHT:
                if (snakeBody[0].x >= 20) isGameOver = true;
                else snakeBody[0].x++;
                break;
            case KEY_LEFT:
                if (snakeBody[0].x <= 0) isGameOver = true;
                else snakeBody[0].x--;
                break;
            case KEY_DOWN:
                if (snakeBody[0].y >= 20) isGameOver = true;
                else snakeBody[0].y++;
                break;
            case KEY_UP:
                if (snakeBody[0].y <= 0) isGameOver = true;
                else snakeBody[0].y--;
                break;
            default:
                return; 
        }
        for (int i = 1; i < snakeBody.size(); i++)
        {
            Point oldPos = snakeBody[i];
            snakeBody[i] = oldHead;
            oldHead = oldPos;
            if(grow)
            {
                snakeBody.push_back(oldHead);
                grow= false;
                isfoodready= false;
                score++;
            }
        }
        collision();
    }
}