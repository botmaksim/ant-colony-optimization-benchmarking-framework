#include "TournamentManager.h"
#include "ACOSolver.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

TournamentManager::~TournamentManager() {
  if (m_worker_thread.joinable()) {
    m_worker_thread.join();
  }
}

void TournamentManager::start(const std::vector<Graph> &fields,
                              const std::vector<ACOConfig> &configs,
                              float time_limit) {
  if (m_is_running)
    return;

  m_is_running = true;
  m_progress = 0.0f;

  {
    std::lock_guard<std::mutex> lock(m_result_mutex);
    m_report = TournamentReport{};

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    m_report.date = ss.str();
    m_report.time_limit_seconds = time_limit;
  }

  m_worker_thread = std::thread(&TournamentManager::workerLoop, this, fields,
                                configs, time_limit);
}

void TournamentManager::workerLoop(std::vector<Graph> fields,
                                   std::vector<ACOConfig> configs,
                                   float time_limit) {
  float total_tasks = fields.size() * configs.size();
  float completed_tasks = 0;

  std::vector<RoundResult> rounds;

  for (size_t f = 0; f < fields.size(); ++f) {
    fields[f].computeDistanceMatrix();
    RoundResult round;
    round.field_id = static_cast<int>(f);
    round.graph = fields[f];

    for (size_t c = 0; c < configs.size(); ++c) {
      ACOSolver solver(fields[f], configs[c]);

      auto start = std::chrono::steady_clock::now();
      double limit_ms = time_limit * 1000.0;

      while (true) {
        solver.step();
        auto now = std::chrono::steady_clock::now();
        double elapsed =
            std::chrono::duration<double, std::milli>(now - start).count();
        if (elapsed >= limit_ms)
          break;
      }

      auto metrics = solver.getMetrics();
      metrics.best_tour = solver.getGlobalBestTour();
      round.competitors_metrics.push_back(metrics);
      completed_tasks++;
      m_progress.store(completed_tasks / total_tasks);
    }
    rounds.push_back(round);
  }

  {
    std::lock_guard<std::mutex> lock(m_result_mutex);
    m_report.rounds = rounds;
  }

  m_is_running = false;
  m_worker_thread.detach();
}

TournamentReport TournamentManager::getReport() {
  std::lock_guard<std::mutex> lock(m_result_mutex);
  return m_report;
}
