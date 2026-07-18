#include "TournamentWorker.h"
#include "ACOSolver.h"

TournamentWorker::~TournamentWorker() {
  if (m_worker_thread.joinable()) {
    m_worker_thread.join();
  }
}

void TournamentWorker::startTournament(
    const std::vector<Graph> &fields, const std::vector<ACOConfig> &competitors,
    float time_limit_seconds) {
  if (m_is_running)
    return;

  m_is_running = true;
  m_progress = 0.0f;
  {
    std::lock_guard<std::mutex> lock(m_result_mutex);
    m_results.clear();
  }

  m_worker_thread = std::thread(&TournamentWorker::execute, this, fields,
                                competitors, time_limit_seconds);
}

void TournamentWorker::execute(std::vector<Graph> fields,
                               std::vector<ACOConfig> competitors,
                               float time_limit_seconds) {
  float total_tasks = fields.size() * competitors.size();
  float completed_tasks = 0;

  for (size_t f = 0; f < fields.size(); ++f) {
    fields[f].computeDistanceMatrix();
    RoundResult round;
    round.field_id = static_cast<int>(f);

    for (size_t c = 0; c < competitors.size(); ++c) {
      ACOSolver solver(fields[f], competitors[c]);

      auto start = std::chrono::steady_clock::now();
      double limit_ms = time_limit_seconds * 1000.0;

      while (true) {
        solver.step();
        auto now = std::chrono::steady_clock::now();
        double elapsed =
            std::chrono::duration<double, std::milli>(now - start).count();
        if (elapsed >= limit_ms)
          break;
      }

      round.competitors_metrics.push_back(solver.getMetrics());
      completed_tasks++;
      m_progress.store(completed_tasks / total_tasks);
    }

    std::lock_guard<std::mutex> lock(m_result_mutex);
    m_results.push_back(round);
  }

  m_is_running = false;
  m_worker_thread.detach();
}

std::vector<RoundResult> TournamentWorker::getResults() {
  std::lock_guard<std::mutex> lock(m_result_mutex);
  return m_results;
}