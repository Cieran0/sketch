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

int shift_down() {

    if(IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
        return -1;
    }

    return 1;
}

int main() {

    Color draw_color = {.r = 0, .g = 0, .b = 0, .a = 0xff};

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1920,1080,"Sketch");
    SetTargetFPS(120);

    RenderTexture2D layer_0 = LoadRenderTexture(1920,1080);
    BeginTextureMode(layer_0);
    ClearBackground(WHITE);
    EndTextureMode();

    unsigned char thickness = 20;

    Vector2 lastMousePos = { -1, -1 };

    int screenWidth = 1920;
    int screenHeight = 1080;

    while (!WindowShouldClose())
    {
        if(IsWindowResized()) {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();

            RenderTexture2D before_resize = layer_0;
            layer_0 = LoadRenderTexture(screenWidth,screenHeight);

            BeginTextureMode(layer_0);
            ClearBackground(WHITE);
            DrawTextureRec(before_resize.texture, (Rectangle) { 0, 0, (float)before_resize.texture.width, (float)-before_resize.texture.height }, (Vector2) { 0, 0 }, WHITE);
            EndTextureMode();

            UnloadRenderTexture(before_resize);
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
                    DrawCircleV(intermediatePos, thickness, draw_color);
                }
            }
            DrawCircleV(mousePos, thickness, draw_color);
            EndTextureMode();
            lastMousePos = mousePos;
        } else {
            lastMousePos = (Vector2){ -1, -1};
        }

        if(IsKeyPressed(KEY_C)) {
            BeginTextureMode(layer_0);
            ClearBackground(WHITE);
            EndTextureMode();
        }


        if(IsKeyDown(KEY_R)) {
            draw_color.r += shift_down();
        }
        if(IsKeyDown(KEY_G)) {
            draw_color.g += shift_down();
        }
        if(IsKeyDown(KEY_B)) {
            draw_color.b += shift_down();
        }


        float movement = GetMouseWheelMove();

        thickness += (int)(movement*2);


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

