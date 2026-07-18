#pragma once
#include "core/Config.h"
#include "core/Graph.h"
#include "core/Metrics.h"
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

class TournamentManager {
public:
  TournamentManager() = default;
  ~TournamentManager();

  void start(const std::vector<Graph> &fields,
             const std::vector<ACOConfig> &configs, float time_limit);

  bool isRunning() const { return m_is_running; }
  float getProgress() const { return m_progress.load(); }
  TournamentReport getReport();

private:
  std::thread m_worker_thread;
  std::atomic<bool> m_is_running{false};
  std::atomic<float> m_progress{0.0f};

  std::mutex m_result_mutex;
  TournamentReport m_report;

  void workerLoop(std::vector<Graph> fields, std::vector<ACOConfig> configs,
                  float time_limit);
};
