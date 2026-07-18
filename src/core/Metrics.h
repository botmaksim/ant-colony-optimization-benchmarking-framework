#pragma once
#include <string>
#include <vector>
#include "Graph.h"

struct ConvergencePoint {
  double time_offset_ms;
  int iteration;
  float best_distance;
};

struct SolverMetrics {
  std::string config_name;
  float final_best_distance = 1e9f;
  double total_execution_time_ms = 0.0;
  double time_to_best_ms = 0.0;
  int iterations_completed = 0;

  std::vector<ConvergencePoint> history;
  std::vector<int> best_tour;
};

struct RoundResult {
  int field_id;
  Graph graph;
  std::vector<SolverMetrics> competitors_metrics;
};

struct TournamentReport {
  std::string date;
  float time_limit_seconds;
  std::vector<RoundResult> rounds;
};