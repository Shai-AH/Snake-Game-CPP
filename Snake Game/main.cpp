#include <iostream>
#include <raylib.h>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <raymath.h>

using namespace std;

Color green = {173, 204, 96, 255};
Color dark_green = {43, 51, 24, 255};

float lastUpdatetime = 0;

bool checkNoOverlap(Vector2 element, deque <Vector2> body){
    for(int i = 1; i < body.size(); i++){
        if(Vector2Equals(element, body[i])){
            return true;
        }
    }
    return false;
}

bool eventTrigger(float interval){
    float currentTime = (float)GetTime();
    if(currentTime - lastUpdatetime >= interval){
        lastUpdatetime = currentTime;
        return true;
    }
    return false;
}

float randNum(int maxV){
    float num = rand() % maxV;
    return num;
}

int cellSize = 30;
int cellCount = 20;

int offSet = 60;

class Snake{
public:
    deque<Vector2> body = {{6, 9}, {5, 9}, {4, 9}};
    Vector2 direction = {1, 0};
    bool directionX = true;
    bool directionY = false;
    bool running = true;
    int score = 0;

    void switchDir(){ //FOR RESTRICTING MOVEMENT OF SNAKE IN SAME AXIS
        if(directionX){
            directionY = true;
            directionX = false;
        }
        else{
            directionX = true;
            directionY = false;
        }
    }

    void Draw(){
        for(unsigned int i = 0; i < body.size(); i++){
            float x = body[i].x;
            float  y = body[i].y;
            Rectangle segment = {x * cellSize,offSet + y * cellSize, cellSize, cellSize};
            DrawRectangleRounded(segment, 0.5, 6, dark_green);
        }
    }
    void Update(bool eat = false){
        if(running){
            if(eat){
                score++;
                body.push_front(body[0] + direction); //IF SNAKE EATS DON'T POP THE BACK THIS TIME FOR
                //ONCE
            }
            else{
                body.pop_back();
                body.push_front(body[0] + direction); //this automatically makes new head everytime
                //and removes tail thats makes
                //illusion that snakes is actually moving!
            }
        }
    }
    void reset(){
        body = {{6, 9}, {5, 9}, {4, 9}};
        direction = {1, 0};
        score = 0;
    }
};

class Food{
public:
    Vector2 position = {randNum(20), randNum(20)};
    Texture2D texture;
    Food(){
        Image image = LoadImage("apple.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
    }

    ~Food(){
        UnloadTexture(texture);
    }

    void Draw(){
        DrawTexture(texture, position.x * cellSize, offSet + position.y * cellSize, WHITE);
    }
};

class Game{
public:
    Snake s1;
    Food f1;

    void Draw(){
        s1.Draw();
        f1.Draw();
    }

    void Update(){
        s1.Update();
        checkCollisionWfood();
        checkCollisionWwalls();
        checkCollisionWbody();
    }

    void checkCollisionWfood(){
        if(Vector2Equals(s1.body[0], f1.position)){
            Vector2 testPosition = {randNum(20), randNum(20)};
            while(checkNoOverlap(testPosition, s1.body)){
                testPosition = {randNum(20), randNum(20)};
            }
            f1.position = testPosition;
            s1.Update(true);
        }
    }

    void checkCollisionWwalls(){
        if(s1.body[0].x == cellCount || s1.body[0].x == -1){
            GameOver();
        }
        if(s1.body[0].y == cellCount || s1.body[0].y == -1){
            GameOver();
        }
    }

    void checkCollisionWbody(){
        if(checkNoOverlap(s1.body[0], s1.body)){
            GameOver();
        }
    }

    void GameOver(){
        s1.reset();
        Vector2 testPosition = {randNum(20), randNum(20)};
        while(checkNoOverlap(testPosition, s1.body)){
            testPosition = {randNum(20), randNum(20)};
        }
        f1.position = testPosition;
        s1.running = false;
    }
};

int main () {
    srand(time(0));
    InitWindow(cellSize*cellCount, offSet + cellSize*cellCount, "Retro Snake");
    SetTargetFPS(60);
    Game g1;

    while(!WindowShouldClose()){

        if(eventTrigger(0.15)){
            g1.Update();
        }

        if(g1.s1.directionX && IsKeyPressed(KEY_UP)){
            g1.s1.switchDir();
            g1.s1.direction = {0, -1};
            g1.s1.running = true;

        }
        if(g1.s1.directionX && IsKeyPressed(KEY_DOWN)){
            g1.s1.switchDir();
            g1.s1.direction = {0, 1};
            g1.s1.running = true;
        }
        if(g1.s1.directionY && IsKeyPressed(KEY_LEFT)){
            g1.s1.switchDir();
            g1.s1.direction = {-1, 0};
            g1.s1.running = true;
        }
        if(g1.s1.directionY && IsKeyPressed(KEY_RIGHT)){
            g1.s1.switchDir();
            g1.s1.direction = {1, 0};
            g1.s1.running = true;
        }


        BeginDrawing();
        ClearBackground(green);
        DrawRectangle(0,0, GetScreenWidth(), offSet, BLACK);
        DrawText("Retro Snake", 5, 5, 40, green);
        DrawText(TextFormat("%i", g1.s1.score), GetScreenWidth() - 40, 5, 40, green);
        g1.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
