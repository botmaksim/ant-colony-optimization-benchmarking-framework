#pragma once
#include "Ant.h"
#include "core/Config.h"
#include "core/Graph.h"
#include "core/Metrics.h"
#include <chrono>

class ACOSolver {
public:
  ACOSolver(const Graph &graph, const ACOConfig &config);

  void init();
  void step();

  const std::vector<int> &getGlobalBestTour() const {
    return m_global_best_tour;
  }
  float getGlobalBestDistance() const { return m_global_best_distance; }
  const std::vector<std::vector<float>> &getPheromones() const {
    return m_pheromones;
  }
  const SolverMetrics &getMetrics() const { return m_metrics; }
  int getIterationCount() const { return m_current_iteration; }

private:
  const Graph &m_graph;
  ACOConfig m_config;

  std::vector<std::vector<float>> m_pheromones;
  std::vector<std::vector<int>> m_candidate_lists;
  std::vector<Ant> m_ants;

  std::vector<int> m_global_best_tour;
  float m_global_best_distance = 1e9f;

  SolverMetrics m_metrics;
  int m_current_iteration = 0;
  std::chrono::steady_clock::time_point m_start_time;

  int selectNextNode(const Ant &ant);
  void constructSolutions();
  void apply2Opt(Ant &ant);
  void updatePheromones();
  void buildCandidateLists();
};