#include "stdio.h"
#include "stdlib.h"
#include "raylib.h"
#include "math.h"

const int MAX_THICKNESS = 50;
const int MIN_THICKNESS = 1;

float Vector2Distance(Vector2 v1, Vector2 v2) {
    return sqrtf((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y));
}

Vector2 Vector2Subtract(Vector2 v1, Vector2 v2) {
    return (Vector2){ v1.x - v2.x, v1.y - v2.y };
}

Vector2 Vector2Scale(Vector2 v, float scale) {
    return (Vector2){ v.x * scale, v.y * scale };
}

Vector2 Vector2Add(Vector2 v1, Vector2 v2) {
    return (Vector2){ v1.x + v2.x, v1.y + v2.y };
}

Vector2 Vector2Normalize(Vector2 v) {
    float length = sqrtf(v.x * v.x + v.y * v.y);
    if (length != 0) {
        return (Vector2){ v.x / length, v.y / length };
    }
    return (Vector2){ 0.0f, 0.0f };
}

int main() {

    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1920,1080,"Sketch");
    SetTargetFPS(120);

    RenderTexture2D layer_0 = LoadRenderTexture(1920,1080);
    BeginTextureMode(layer_0);
    ClearBackground(WHITE);
    EndTextureMode();

    int thickness = 20;

    Vector2 lastMousePos = { -1, -1 };

    int screenWidth = 1920;
    int screenHeight = 1080;

    while (!WindowShouldClose())
    {
        if(IsWindowResized()) {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();
            UnloadRenderTexture(layer_0);
            layer_0 = LoadRenderTexture(screenWidth,screenHeight);
            
            BeginTextureMode(layer_0);
            ClearBackground(WHITE);
            EndTextureMode();

            printf("Screen width: %i, Screen height: %i",screenWidth, screenHeight);
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            BeginTextureMode(layer_0);
            Vector2 mousePos = GetMousePosition();
            if (lastMousePos.x >= 0 && lastMousePos.y >= 0) {

            
                // Interpolate and draw circles between the last and current mouse positions
                float distance = Vector2Distance(mousePos, lastMousePos);
                Vector2 direction = Vector2Subtract(mousePos, lastMousePos);
                Vector2 normDirection = Vector2Scale(Vector2Normalize(direction), 1.0f);

                for (float i = 0; i < distance; i += 1.0f) {
                    Vector2 intermediatePos = Vector2Add(lastMousePos, Vector2Scale(normDirection, i));
                    DrawCircleV(intermediatePos, thickness, BLACK);
                }
            }
            DrawCircleV(mousePos, thickness, BLACK);
            EndTextureMode();
            lastMousePos = mousePos;
        } else {
            lastMousePos = (Vector2){ -1, -1};
        }

        thickness += GetMouseWheelMove()*2;
        if(thickness < MIN_THICKNESS) {
            thickness = MIN_THICKNESS;
        } else if (thickness >= MAX_THICKNESS) {
            thickness = MAX_THICKNESS;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTextureRec(layer_0.texture, (Rectangle) { 0, 0, (float)layer_0.texture.width, (float)-layer_0.texture.height }, (Vector2) { 0, 0 }, WHITE);

        DrawCircleLines(GetMouseX(), GetMouseY(), thickness+1, BLACK);

        EndDrawing();
    }
    
}

