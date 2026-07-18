#include "Serializer.h"
#include "nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;

NLOHMANN_JSON_SERIALIZE_ENUM(SelectionStrategy, {
    {SelectionStrategy::RouletteWheel, "RouletteWheel"},
    {SelectionStrategy::PseudoRandom, "PseudoRandom"},
    {SelectionStrategy::RandomWalk, "RandomWalk"}
})

NLOHMANN_JSON_SERIALIZE_ENUM(LocalSearchStrategy, {
    {LocalSearchStrategy::None, "None"},
    {LocalSearchStrategy::TwoOpt, "TwoOpt"},
    {LocalSearchStrategy::TwoPointFiveOpt, "TwoPointFiveOpt"}
})

NLOHMANN_JSON_SERIALIZE_ENUM(PheromoneUpdateStrategy, {
    {PheromoneUpdateStrategy::AllAnts, "AllAnts"},
    {PheromoneUpdateStrategy::Elitist, "Elitist"},
    {PheromoneUpdateStrategy::RankBased, "RankBased"},
    {PheromoneUpdateStrategy::BestOnly, "BestOnly"}
})

NLOHMANN_JSON_SERIALIZE_ENUM(PheromoneLimits, {
    {PheromoneLimits::None, "None"},
    {PheromoneLimits::MaxMin, "MaxMin"}
})

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ACOConfig, name, ant_count, alpha, beta, rho, xi, q0, elitist_weight, rank_count, tau_min, tau_max, selection_strategy, local_search, update_strategy, limits, use_local_evaporation, candidate_list_size)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Node, x, y, id)

void to_json(json& j, const Graph& g) {
    j = json{{"nodes", g.getNodes()}};
}
void from_json(const json& j, Graph& g) {
    g.clear();
    for (auto& n : j.at("nodes")) {
        g.addNode(n.at("x").get<float>(), n.at("y").get<float>());
    }
}

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ConvergencePoint, time_offset_ms, iteration, best_distance)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SolverMetrics, config_name, final_best_distance, total_execution_time_ms, time_to_best_ms, iterations_completed, history, best_tour)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RoundResult, field_id, graph, competitors_metrics)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TournamentReport, date, time_limit_seconds, rounds)

bool Serializer::saveConfig(const ACOConfig &config, const std::string &filename) {
    std::ofstream out(filename);
    if (!out) return false;
    json j = config;
    out << j.dump(4);
    return true;
}

bool Serializer::loadConfig(ACOConfig &config, const std::string &filename) {
    std::ifstream in(filename);
    if (!in) return false;
    json j;
    in >> j;
    config = j.get<ACOConfig>();
    return true;
}

bool Serializer::saveConfigs(const std::vector<ACOConfig> &configs, const std::string &filename) {
    std::ofstream out(filename);
    if (!out) return false;
    json j = configs;
    out << j.dump(4);
    return true;
}

bool Serializer::loadConfigs(std::vector<ACOConfig> &configs, const std::string &filename) {
    std::ifstream in(filename);
    if (!in) return false;
    json j;
    in >> j;
    configs = j.get<std::vector<ACOConfig>>();
    return true;
}

bool Serializer::saveGraph(const Graph &graph, const std::string &filename) {
    std::ofstream out(filename);
    if (!out) return false;
    json j = graph;
    out << j.dump(4);
    return true;
}

bool Serializer::loadGraph(Graph &graph, const std::string &filename) {
    std::ifstream in(filename);
    if (!in) return false;
    json j;
    in >> j;
    graph = j.get<Graph>();
    return true;
}

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TournamentTask, fields, configs)

bool Serializer::saveTournamentTask(const TournamentTask &task, const std::string &filename) {
  json j = task;
  std::ofstream o(filename);
  if (!o.is_open()) return false;
  o << j.dump(4);
  return true;
}

bool Serializer::loadTournamentTask(TournamentTask &task, const std::string &filename) {
  std::ifstream i(filename);
  if (!i.is_open()) return false;
  json j;
  i >> j;
  task = j.get<TournamentTask>();
  return true;
}

bool Serializer::saveTournamentReport(const TournamentReport &report, const std::string &filename) {
    std::ofstream out(filename);
    if (!out) return false;
    json j = report;
    out << j.dump(4);
    return true;
}

bool Serializer::loadTournamentReport(TournamentReport &report, const std::string &filename) {
    std::ifstream in(filename);
    if (!in) return false;
    json j;
    in >> j;
    report = j.get<TournamentReport>();
    return true;
}
