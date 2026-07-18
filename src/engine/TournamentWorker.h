#pragma once
#include "core/Config.h"
#include "core/Graph.h"
#include "core/Metrics.h"
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

class TournamentWorker {
public:
  TournamentWorker() = default;
  ~TournamentWorker();

  void startTournament(const std::vector<Graph> &fields,
                       const std::vector<ACOConfig> &competitors,
                       float time_limit_seconds);

  bool isRunning() const { return m_is_running; }
  float getProgress() const { return m_progress.load(); }
  std::vector<RoundResult> getResults();

private:
  std::thread m_worker_thread;
  std::atomic<bool> m_is_running{false};
  std::atomic<float> m_progress{0.0f};

  std::mutex m_result_mutex;
  std::vector<RoundResult> m_results;

  void execute(std::vector<Graph> fields, std::vector<ACOConfig> competitors,
               float time_limit_seconds);
};