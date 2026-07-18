#pragma once
#include <string>
#include <vector>
#include "core/Graph.h"

enum class SelectionStrategy { RouletteWheel, PseudoRandom, RandomWalk };
enum class LocalSearchStrategy { None, TwoOpt, TwoPointFiveOpt };
enum class PheromoneUpdateStrategy { AllAnts, Elitist, RankBased, BestOnly };
enum class PheromoneLimits { None, MaxMin };

struct ACOConfig {
  std::string name = "Default";

  // Базовые константы
  int ant_count = 30;
  float alpha = 1.0f; // Вес феромона
  float beta = 2.0f;  // Вес расстояния (эвристики)
  float rho = 0.1f;   // Коэффициент испарения глобального
  float xi = 0.1f;    // Коэффициент испарения локального (для ACS)
  float q0 = 0.9f;    // Параметр жадного выбора для PseudoRandom

  int elitist_weight = 5; // Вес элитного муравья
  int rank_count = 5;     // Количество ранжируемых муравьев

  // Границы для MMAS
  float tau_min = 0.01f;
  float tau_max = 10.0f;

  // Переключатели стратегий
  SelectionStrategy selection_strategy = SelectionStrategy::RouletteWheel;
  LocalSearchStrategy local_search = LocalSearchStrategy::None;
  PheromoneUpdateStrategy update_strategy = PheromoneUpdateStrategy::AllAnts;
  PheromoneLimits limits = PheromoneLimits::None;

  bool use_local_evaporation = false; // Локальное испарение при проходе муравья

  // Оптимизации графа
  int candidate_list_size = 0; // 0 — полный граф
};

struct TournamentTask {
  std::vector<Graph> fields;
  std::vector<ACOConfig> configs;
};