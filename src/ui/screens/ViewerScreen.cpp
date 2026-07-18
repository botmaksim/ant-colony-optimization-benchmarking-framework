#include "ViewerScreen.h"
#include "raylib.h"
#include "../components/Button.h"

void ViewerScreen::update(AppContext& ctx, float dt) {
    // No specific logic needed
}

void ViewerScreen::draw(AppContext& ctx) {
    DrawText("ANALYTICAL VIEWER", 50, 30, 24, DARKGRAY);

    int y_offset = 100;
    for (const auto &round : ctx.cached_report.rounds) {
        DrawText((std::string("Field #") + std::to_string(round.field_id)).c_str(),
                 50, y_offset, 18, BLACK);
        y_offset += 25;
        
        for (const auto &metric : round.competitors_metrics) {
            std::string res =
                metric.config_name +
                " -> Best Dist: " + std::to_string(metric.final_best_distance) +
                " | Time to best: " + std::to_string(metric.time_to_best_ms) + "ms";
            DrawText(res.c_str(), 80, y_offset, 16, GRAY);
            y_offset += 20;
        }
        y_offset += 15;
    }

    if (UI::DrawButton(50, 650, 180, 35, "Back to Menu"))
        ctx.state = ScreenState::MainMenu;
}
