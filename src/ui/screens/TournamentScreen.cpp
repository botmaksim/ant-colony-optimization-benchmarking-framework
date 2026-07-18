#include "TournamentScreen.h"
#include "raylib.h"
#include "../components/Button.h"
#include "utils/Serializer.h"
#include "engine/FieldGenerator.h"

void TournamentScreen::update(AppContext& ctx, float dt) {
    if (ctx.tournament_manager.isRunning()) {
        // Just wait for completion
    } else if (ctx.tournament_manager.getProgress() >= 1.0f &&
               ctx.cached_report.rounds.empty()) {
        ctx.cached_report = ctx.tournament_manager.getReport();
    }
}

void TournamentScreen::draw(AppContext& ctx) {
    DrawText("TOURNAMENT SETUP & EXECUTION", 50, 30, 24, DARKGRAY);

    if (ctx.tournament_manager.isRunning()) {
        float progress = ctx.tournament_manager.getProgress();
        DrawRectangle(50, 150, 400, 30, LIGHTGRAY);
        DrawRectangle(50, 150, static_cast<int>(400 * progress), 30, GREEN);
        DrawText(
            TextFormat("Progress: %d%%", int(progress * 100)),
            50, 200, 20, BLACK);
    } else {
        // Controls for setting up a tournament
        DrawText(TextFormat("Time Limit: %.1f s", ctx.tournament_time_limit), 90, 105, 20, BLACK);
        if (UI::DrawButton(50, 100, 30, 30, "-")) ctx.tournament_time_limit = std::max(0.5f, ctx.tournament_time_limit - 0.5f);
        if (UI::DrawButton(250, 100, 30, 30, "+")) ctx.tournament_time_limit += 0.5f;

        DrawText(TextFormat("Nodes: %d", ctx.gen_nodes), 90, 155, 20, BLACK);
        if (UI::DrawButton(50, 150, 30, 30, "-")) ctx.gen_nodes = std::max(10, ctx.gen_nodes - 10);
        if (UI::DrawButton(200, 150, 30, 30, "+")) ctx.gen_nodes += 10;

        DrawText(TextFormat("Clusters: %d", ctx.gen_clusters), 90, 205, 20, BLACK);
        if (UI::DrawButton(50, 200, 30, 30, "-")) ctx.gen_clusters = std::max(1, ctx.gen_clusters - 1);
        if (UI::DrawButton(220, 200, 30, 30, "+")) ctx.gen_clusters += 1;

        if (UI::DrawButton(50, 260, 220, 35, "+ Uniform Field")) {
            ctx.tournament_fields.push_back(FieldGenerator::generateUniform(ctx.gen_nodes, 1000, 700));
        }
        if (UI::DrawButton(290, 260, 220, 35, "+ Clustered Field")) {
            ctx.tournament_fields.push_back(FieldGenerator::generateClustered(ctx.gen_nodes, ctx.gen_clusters, 1000, 700));
        }
        if (UI::DrawButton(530, 260, 150, 35, "Clear Fields")) {
            ctx.tournament_fields.clear();
        }
        DrawText(TextFormat("Fields added: %d", (int)ctx.tournament_fields.size()), 700, 265, 20, DARKGRAY);

        if (UI::DrawButton(50, 310, 220, 35, "+ Classic AS")) {
            ACOConfig c;
            c.name = "Classic Ant System";
            c.local_search = LocalSearchStrategy::None;
            ctx.competitors.push_back(c);
        }
        if (UI::DrawButton(290, 310, 220, 35, "+ MMAS 2.5-Opt")) {
            ACOConfig c;
            c.name = "MMAS + 2.5-Opt";
            c.local_search = LocalSearchStrategy::TwoPointFiveOpt;
            c.limits = PheromoneLimits::MaxMin;
            c.selection_strategy = SelectionStrategy::PseudoRandom;
            ctx.competitors.push_back(c);
        }
        if (UI::DrawButton(530, 310, 150, 35, "Clear Configs")) {
            ctx.competitors.clear();
        }
        
        if (UI::DrawButton(700, 310, 150, 35, "Save Configs")) {
            if (!ctx.competitors.empty()) {
                Serializer::saveConfigs(ctx.competitors, "saved_configs.json");
            }
        }
        if (UI::DrawButton(870, 310, 150, 35, "Load Configs")) {
            std::vector<ACOConfig> loaded;
            if (Serializer::loadConfigs(loaded, "saved_configs.json")) {
                for (const auto& c : loaded) ctx.competitors.push_back(c);
            }
        }
        
        DrawText(TextFormat("Configs added: %d", (int)ctx.competitors.size()), 50, 355, 20, DARKGRAY);

        if (UI::DrawButton(50, 380, 250, 40, "START TOURNAMENT")) {
            if (!ctx.tournament_fields.empty() && !ctx.competitors.empty()) {
                ctx.cached_report = TournamentReport{};
                ctx.tournament_manager.start(ctx.tournament_fields, ctx.competitors, ctx.tournament_time_limit);
            }
        }
        if (UI::DrawButton(320, 380, 300, 40, "LOAD TASK & START")) {
            TournamentTask task;
            if (Serializer::loadTournamentTask(task, "tournament_task.json")) {
                ctx.tournament_fields = task.fields;
                ctx.competitors = task.configs;
                ctx.cached_report = TournamentReport{};
                ctx.tournament_manager.start(ctx.tournament_fields, ctx.competitors, ctx.tournament_time_limit);
            }
        }

        if (!ctx.cached_report.rounds.empty()) {
            DrawText("Tournament finished!", 50, 450, 20, GREEN);
            if (UI::DrawButton(50, 480, 180, 35, "Open Viewer")) {
                ctx.state = ScreenState::Viewer;
            }
            if (UI::DrawButton(250, 480, 200, 35, "Export to JSON")) {
                Serializer::saveTournamentReport(ctx.cached_report, "tournament_report.json");
            }
        }
    }

    if (UI::DrawButton(50, 650, 180, 35, "Back to Menu")) {
        ctx.state = ScreenState::MainMenu;
    }
}
