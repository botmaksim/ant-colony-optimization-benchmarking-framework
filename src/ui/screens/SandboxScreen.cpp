#include "SandboxScreen.h"
#include "raylib.h"
#include "../components/Button.h"
#include "utils/Serializer.h"

void SandboxScreen::update(AppContext& ctx, float dt) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse = GetMousePosition();
        if (mouse.x < 1000) {
            int clicked_node = -1;
            const auto& nodes = ctx.sandbox_graph.getNodes();
            for (size_t i = 0; i < nodes.size(); ++i) {
                float dx = mouse.x - nodes[i].x;
                float dy = mouse.y - nodes[i].y;
                if (dx * dx + dy * dy <= 100) { // radius 10
                    clicked_node = (int)i;
                    break;
                }
            }
            
            if (clicked_node != -1) {
                if (ctx.user_path.empty() || ctx.user_path.back() != clicked_node) {
                    ctx.user_path.push_back(clicked_node);
                    ctx.user_path_distance = 0.0f;
                    for(size_t i = 0; i < ctx.user_path.size() - 1; ++i) {
                        ctx.user_path_distance += ctx.sandbox_graph.getDistance(ctx.user_path[i], ctx.user_path[i+1]);
                    }
                }
            } else {
                ctx.sandbox_graph.addNode(mouse.x, mouse.y);
                ctx.sandbox_graph.computeDistanceMatrix();
                if (ctx.sandbox_solver && !ctx.sandbox_configs.empty()) {
                    ctx.sandbox_solver = std::make_unique<ACOSolver>(ctx.sandbox_graph, ctx.sandbox_configs[ctx.sandbox_config_idx]);
                }
                ctx.user_path.clear();
                ctx.user_path_distance = 0.0f;
            }
        }
    }

    if (ctx.is_animating && ctx.sandbox_graph.size() >= 3) {
        if (!ctx.sandbox_solver && !ctx.sandbox_configs.empty()) {
            ctx.sandbox_graph.computeDistanceMatrix();
            ctx.sandbox_solver = std::make_unique<ACOSolver>(ctx.sandbox_graph, ctx.sandbox_configs[ctx.sandbox_config_idx]);
        }
        ctx.animation_timer += dt;
        if (ctx.animation_timer >= ctx.animation_speed) {
            if (ctx.sandbox_solver) ctx.sandbox_solver->step();
            ctx.animation_timer = 0.0f;
        }
    }
}

void SandboxScreen::draw(AppContext& ctx) {
    const auto &nodes = ctx.sandbox_graph.getNodes();

    // Draw user path
    if (ctx.user_path.size() > 1) {
        for (size_t i = 1; i < ctx.user_path.size(); ++i) {
            DrawLineEx(Vector2{nodes[ctx.user_path[i-1]].x, nodes[ctx.user_path[i-1]].y},
                       Vector2{nodes[ctx.user_path[i]].x, nodes[ctx.user_path[i]].y}, 2.0f, ORANGE);
        }
    }

    if (ctx.sandbox_solver) {
        int n = ctx.sandbox_graph.size();
        const auto &pheromones = ctx.sandbox_solver->getPheromones();
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                float intensity = pheromones[i][j] * 10.0f;
                if (intensity > 1.0f)
                    intensity = 1.0f;
                DrawLineEx(Vector2{nodes[i].x, nodes[i].y},
                           Vector2{nodes[j].x, nodes[j].y}, 1.0f,
                           Fade(BLUE, intensity));
            }
        }

        const auto &tour = ctx.sandbox_solver->getGlobalBestTour();
        if (!tour.empty()) {
            for (size_t i = 0; i < tour.size() - 1; ++i) {
                DrawLineEx(Vector2{nodes[tour[i]].x, nodes[tour[i]].y},
                           Vector2{nodes[tour[i + 1]].x, nodes[tour[i + 1]].y}, 3.0f,
                           GREEN);
            }
            DrawLineEx(Vector2{nodes[tour.back()].x, nodes[tour.back()].y},
                       Vector2{nodes[tour[0]].x, nodes[tour[0]].y}, 3.0f, GREEN);
        }
    }

    for (const auto &node : nodes) {
        DrawCircle(static_cast<int>(node.x), static_cast<int>(node.y), 6, RED);
        DrawText(TextFormat("%d", node.id), static_cast<int>(node.x) + 8, static_cast<int>(node.y) - 8, 10, DARKGRAY);
    }

    DrawRectangle(1000, 0, 280, 720, LIGHTGRAY);
    DrawText("Sandbox Controls", 1020, 20, 20, BLACK);

    // Algorithm selection
    DrawText("Algorithm:", 1020, 60, 16, DARKGRAY);
    if (!ctx.sandbox_configs.empty()) {
        DrawText(ctx.sandbox_configs[ctx.sandbox_config_idx].name.c_str(), 1020, 80, 16, BLUE);
        
        if (UI::DrawButton(1020, 105, 120, 30, "Prev Algo")) {
            ctx.sandbox_config_idx = (ctx.sandbox_config_idx - 1 + ctx.sandbox_configs.size()) % ctx.sandbox_configs.size();
            if (ctx.sandbox_solver) ctx.sandbox_solver = std::make_unique<ACOSolver>(ctx.sandbox_graph, ctx.sandbox_configs[ctx.sandbox_config_idx]);
        }
        if (UI::DrawButton(1150, 105, 120, 30, "Next Algo")) {
            ctx.sandbox_config_idx = (ctx.sandbox_config_idx + 1) % ctx.sandbox_configs.size();
            if (ctx.sandbox_solver) ctx.sandbox_solver = std::make_unique<ACOSolver>(ctx.sandbox_graph, ctx.sandbox_configs[ctx.sandbox_config_idx]);
        }
    }

    if (UI::DrawButton(1020, 145, 250, 30, "Reload Configs")) {
        std::vector<ACOConfig> loaded;
        if (Serializer::loadConfigs(loaded, "saved_configs.json") && !loaded.empty()) {
            ctx.sandbox_configs = loaded;
            ctx.sandbox_config_idx = 0;
            if (ctx.sandbox_solver) ctx.sandbox_solver = std::make_unique<ACOSolver>(ctx.sandbox_graph, ctx.sandbox_configs[0]);
        }
    }

    if (ctx.is_animating) {
        if (UI::DrawButton(1020, 200, 120, 35, "Pause")) ctx.is_animating = false;
    } else {
        if (UI::DrawButton(1020, 200, 120, 35, "Play")) ctx.is_animating = true;
    }
    
    if (UI::DrawButton(1150, 200, 120, 35, "Step")) {
        if (!ctx.sandbox_solver && ctx.sandbox_graph.size() >= 3 && !ctx.sandbox_configs.empty()) {
            ctx.sandbox_graph.computeDistanceMatrix();
            ctx.sandbox_solver = std::make_unique<ACOSolver>(ctx.sandbox_graph, ctx.sandbox_configs[ctx.sandbox_config_idx]);
        }
        if (ctx.sandbox_solver) ctx.sandbox_solver->step();
    }

    // Info
    int iters = ctx.sandbox_solver ? ctx.sandbox_solver->getIterationCount() : 0;
    float best_d = ctx.sandbox_solver ? ctx.sandbox_solver->getGlobalBestDistance() : 0.0f;
    DrawText(TextFormat("Iters: %d", iters), 1020, 250, 16, BLACK);
    DrawText(TextFormat("Best Dist: %.1f", best_d), 1020, 270, 16, BLACK);
    
    if (ctx.user_path.size() > 1) {
        DrawText(TextFormat("User Path Dist: %.1f", ctx.user_path_distance), 1020, 300, 16, ORANGE);
    }
    if (UI::DrawButton(1020, 330, 250, 30, "Clear User Path")) {
        ctx.user_path.clear();
        ctx.user_path_distance = 0.0f;
    }

    if (UI::DrawButton(1020, 380, 250, 35, "Add Random Node")) {
        ctx.sandbox_graph.addNode(GetRandomValue(50, 950), GetRandomValue(50, 650));
        ctx.sandbox_graph.computeDistanceMatrix();
        if (ctx.sandbox_solver && !ctx.sandbox_configs.empty()) ctx.sandbox_solver = std::make_unique<ACOSolver>(ctx.sandbox_graph, ctx.sandbox_configs[ctx.sandbox_config_idx]);
    }

    if (UI::DrawButton(1020, 425, 250, 35, "Clear Canvas")) {
        ctx.sandbox_graph.clear();
        ctx.sandbox_solver.reset();
        ctx.user_path.clear();
        ctx.user_path_distance = 0.0f;
    }

    if (UI::DrawButton(1020, 650, 250, 35, "Back to Menu"))
        ctx.state = ScreenState::MainMenu;
}
