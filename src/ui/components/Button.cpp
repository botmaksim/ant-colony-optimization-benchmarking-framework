#include "Button.h"
#include "raylib.h"

namespace UI {
    bool DrawButton(float x, float y, float width, float height, const char* text) {
        Rectangle bounds = {x, y, width, height};
        Vector2 mouse = GetMousePosition();
        bool hover = CheckCollisionPointRec(mouse, bounds);
        bool clicked = false;
        if (hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            clicked = true;
        }
        
        DrawRectangleRec(bounds, hover ? LIGHTGRAY : RAYWHITE);
        DrawRectangleLinesEx(bounds, 1, hover ? DARKGRAY : GRAY);
        
        int textWidth = MeasureText(text, 20);
        DrawText(text, bounds.x + (bounds.width - textWidth) / 2, bounds.y + bounds.height / 2 - 10, 20, hover ? BLACK : DARKGRAY);
        
        return clicked;
    }
}
