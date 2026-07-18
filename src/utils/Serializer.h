#pragma once
#include "core/Config.h"
#include "core/Graph.h"
#include "core/Metrics.h"
#include <string>
#include <vector>

class Serializer {
public:
  static bool saveConfig(const ACOConfig &config, const std::string &filename);
  static bool loadConfig(ACOConfig &config, const std::string &filename);

  static bool saveConfigs(const std::vector<ACOConfig> &configs, const std::string &filename);
  static bool loadConfigs(std::vector<ACOConfig> &configs, const std::string &filename);

  static bool saveGraph(const Graph &graph, const std::string &filename);
  static bool loadGraph(Graph &graph, const std::string &filename);

  static bool saveTournamentTask(const TournamentTask &task, const std::string &filename);
  static bool loadTournamentTask(TournamentTask &task, const std::string &filename);

  static bool saveTournamentReport(const TournamentReport &report, const std::string &filename);
  static bool loadTournamentReport(TournamentReport &report, const std::string &filename);
};