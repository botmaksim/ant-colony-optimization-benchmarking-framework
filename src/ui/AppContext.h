#pragma once
#include "core/Graph.h"
#include "core/Config.h"
#include "engine/ACOSolver.h"
#include "engine/TournamentManager.h"
#include <vector>
#include <memory>

enum class ScreenState { MainMenu, Sandbox, TournamentMode, Viewer };

struct AppContext {
    ScreenState state = ScreenState::MainMenu;

    // Sandbox Data
    Graph sandbox_graph;
    std::vector<ACOConfig> sandbox_configs;
    int sandbox_config_idx = 0;
    std::unique_ptr<ACOSolver> sandbox_solver;
    bool is_animating = false;
    float animation_speed = 0.1f;
    float animation_timer = 0.0f;
    std::vector<int> user_path;
    float user_path_distance = 0.0f;

    // Tournament Data
    TournamentManager tournament_manager;
    std::vector<Graph> tournament_fields;
    std::vector<ACOConfig> competitors;
    TournamentReport cached_report;
    float tournament_time_limit = 2.0f;
    int gen_nodes = 50;
    int gen_clusters = 4;
};
