#include "App.h"
#include "raylib.h"
#include "screens/MainMenuScreen.h"
#include "screens/SandboxScreen.h"
#include "screens/TournamentScreen.h"
#include "screens/ViewerScreen.h"
#include "utils/Serializer.h"

App::App() {
    InitWindow(1280, 720, "Ant Colony Optimization Benchmarking Framework");
    SetTargetFPS(60);
    
    // Initialize screens
    m_screens[ScreenState::MainMenu] = std::make_unique<MainMenuScreen>();
    m_screens[ScreenState::Sandbox] = std::make_unique<SandboxScreen>();
    m_screens[ScreenState::TournamentMode] = std::make_unique<TournamentScreen>();
    m_screens[ScreenState::Viewer] = std::make_unique<ViewerScreen>();

    // Initial load
    if (!Serializer::loadConfigs(m_ctx.sandbox_configs, "saved_configs.json") || m_ctx.sandbox_configs.empty()) {
        ACOConfig c1;
        c1.name = "Classic Ant System";
        c1.local_search = LocalSearchStrategy::None;
        
        ACOConfig c2;
        c2.name = "MMAS + 2.5-Opt";
        c2.local_search = LocalSearchStrategy::TwoPointFiveOpt;
        c2.limits = PheromoneLimits::MaxMin;
        c2.selection_strategy = SelectionStrategy::PseudoRandom;
        
        m_ctx.sandbox_configs = {c1, c2};
    }
}

void App::run() {
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        update(dt);
        draw();
    }
    CloseWindow();
}

void App::update(float dt) {
    if (m_screens.count(m_ctx.state)) {
        m_screens[m_ctx.state]->update(m_ctx, dt);
    }
}

void App::draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    if (m_screens.count(m_ctx.state)) {
        m_screens[m_ctx.state]->draw(m_ctx);
    }
    
    EndDrawing();
}
