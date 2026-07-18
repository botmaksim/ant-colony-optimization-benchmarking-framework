#include "MainMenuScreen.h"
#include "raylib.h"
#include "../components/Button.h"

void MainMenuScreen::update(AppContext& ctx, float dt) {
    // No specific update logic
}

void MainMenuScreen::draw(AppContext& ctx) {
    DrawText("ACO TSP BENCHMARKING TOOL", 400, 150, 30, DARKGRAY);
    if (UI::DrawButton(500, 300, 280, 40, "Interactive Sandbox"))
        ctx.state = ScreenState::Sandbox;
    if (UI::DrawButton(500, 360, 280, 40, "Tournament Runner"))
        ctx.state = ScreenState::TournamentMode;
}
