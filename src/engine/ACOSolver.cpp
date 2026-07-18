#include "ACOSolver.h"
#include <algorithm>
#include <cmath>
#include <random>

ACOSolver::ACOSolver(const Graph &graph, const ACOConfig &config)
    : m_graph(graph), m_config(config) {
  init();
}

void ACOSolver::init() {
  int n = m_graph.size();
  float initial_tau = 1.0f / (n * 100.0f); // Грубая оценка базового ребра
  m_pheromones.assign(n, std::vector<float>(n, initial_tau));

  buildCandidateLists();

  m_ants.clear();
  for (int i = 0; i < m_config.ant_count; ++i) {
    m_ants.emplace_back(i % n, n);
  }

  m_metrics.config_name = m_config.name;
  m_start_time = std::chrono::steady_clock::now();
}

void ACOSolver::buildCandidateLists() {
  if (m_config.candidate_list_size <= 0)
    return;
  int n = m_graph.size();
  m_candidate_lists.assign(n, std::vector<int>());

  for (int i = 0; i < n; ++i) {
    std::vector<std::pair<int, float>> neighbors;
    for (int j = 0; j < n; ++j) {
      if (i != j)
        neighbors.push_back({j, m_graph.getDistance(i, j)});
    }
    std::sort(neighbors.begin(), neighbors.end(),
              [](const auto &a, const auto &b) { return a.second < b.second; });

    int size = std::min(m_config.candidate_list_size,
                        static_cast<int>(neighbors.size()));
    for (int k = 0; k < size; ++k) {
      m_candidate_lists[i].push_back(neighbors[k].first);
    }
  }
}

int ACOSolver::selectNextNode(const Ant &ant) {
  int curr = ant.getTour().back();
  int n = m_graph.size();
  
  bool use_candidates = !m_candidate_lists.empty() && m_config.candidate_list_size > 0;
  const std::vector<int>& candidates = use_candidates ? m_candidate_lists[curr] : std::vector<int>();

  static std::mt19937 gen(std::random_device{}());
  std::uniform_real_distribution<float> dis(0.0f, 1.0f);

  if (m_config.selection_strategy == SelectionStrategy::PseudoRandom) {
    if (dis(gen) < m_config.q0) {
      int best_node = -1;
      float max_val = -1.0f;
      
      auto check_node = [&](int next) {
        if (!ant.isVisited(next)) {
          float eta = 1.0f / m_graph.getDistance(curr, next);
          float val = std::pow(m_pheromones[curr][next], m_config.alpha) *
                      std::pow(eta, m_config.beta);
          if (val > max_val) {
            max_val = val;
            best_node = next;
          }
        }
      };

      if (use_candidates) {
        for (int next : candidates) check_node(next);
      }
      
      // Fallback if all candidates are visited, or no candidates
      if (best_node == -1) {
        for (int next = 0; next < n; ++next) check_node(next);
      }
      
      if (best_node != -1)
        return best_node;
    }
  } else if (m_config.selection_strategy == SelectionStrategy::RandomWalk) {
    if (dis(gen) < 0.05f) { // 5% chance to go random
      std::vector<int> available;
      for (int i = 0; i < n; ++i) if (!ant.isVisited(i)) available.push_back(i);
      if (!available.empty()) {
        std::uniform_int_distribution<int> rand_dist(0, available.size() - 1);
        return available[rand_dist(gen)];
      }
    }
  }

  // Классическая рулетка
  std::vector<float> probs(n, 0.0f);
  float sum = 0.0f;
  
  auto calc_prob = [&](int next) {
    if (!ant.isVisited(next)) {
      float eta = 1.0f / m_graph.getDistance(curr, next);
      probs[next] = std::pow(m_pheromones[curr][next], m_config.alpha) *
                    std::pow(eta, m_config.beta);
      sum += probs[next];
    }
  };

  if (use_candidates) {
    for (int next : candidates) calc_prob(next);
  }
  
  // Fallback to all nodes if candidate list yields no options
  if (sum == 0.0f) {
    for (int next = 0; next < n; ++next) calc_prob(next);
  }

  if (sum == 0.0f) {
    for (int i = 0; i < n; ++i)
      if (!ant.isVisited(i))
        return i;
  }

  float target = dis(gen) * sum;
  float current_sum = 0.0f;
  for (int i = 0; i < n; ++i) {
    if (probs[i] > 0.0f) {
      current_sum += probs[i];
      if (current_sum >= target)
        return i;
    }
  }
  return 0;
}

void ACOSolver::constructSolutions() {
  int n = m_graph.size();
  for (auto &ant : m_ants) {
    ant.reset(rand() % n);
    for (int step = 1; step < n; ++step) {
      int curr = ant.getTour().back();
      int next = selectNextNode(ant);
      
      if (m_config.use_local_evaporation) {
        float tau0 = 1.0f / (n * 100.0f);
        m_pheromones[curr][next] = (1.0f - m_config.xi) * m_pheromones[curr][next] + m_config.xi * tau0;
        m_pheromones[next][curr] = m_pheromones[curr][next];
      }
      
      ant.visitNode(next);
    }
    ant.calculateTourLength(m_graph);
  }
}

void ACOSolver::apply2Opt(Ant &ant) {
  if (m_config.local_search == LocalSearchStrategy::None)
    return;
  bool improved = true;
  auto tour = ant.getTour();
  int n = tour.size();

  while (improved) {
    improved = false;
    for (int i = 1; i < n - 2; ++i) {
      for (int j = i + 1; j < n; ++j) {
        if (j - i == 1)
          continue;
        int next_j = (j == n - 1) ? 0 : j + 1;
        float cur_dist = m_graph.getDistance(tour[i - 1], tour[i]) +
                         m_graph.getDistance(tour[j], tour[next_j]);
        float new_dist = m_graph.getDistance(tour[i - 1], tour[j]) +
                         m_graph.getDistance(tour[i], tour[next_j]);

        if (new_dist < cur_dist - 0.001f) {
          std::reverse(tour.begin() + i, tour.begin() + j + 1);
          improved = true;
        }
      }
    }
    
    // 2.5-opt (Node relocation)
    if (!improved && m_config.local_search == LocalSearchStrategy::TwoPointFiveOpt) {
      for (int i = 1; i < n && !improved; ++i) {
        for (int j = 1; j < n; ++j) {
           if (i == j || i == j + 1 || j == i + 1) continue;
           int prev_i = i - 1;
           int next_i = (i == n - 1) ? 0 : i + 1;
           int next_j = (j == n - 1) ? 0 : j + 1;
           
           if (next_i == j || next_j == i) continue; // overlapping edges
           
           float cur_dist = m_graph.getDistance(tour[prev_i], tour[i]) + 
                            m_graph.getDistance(tour[i], tour[next_i]) +
                            m_graph.getDistance(tour[j], tour[next_j]);
                            
           float new_dist = m_graph.getDistance(tour[prev_i], tour[next_i]) + 
                            m_graph.getDistance(tour[j], tour[i]) +
                            m_graph.getDistance(tour[i], tour[next_j]);
                            
           if (new_dist < cur_dist - 0.001f) {
             int node_i = tour[i];
             tour.erase(tour.begin() + i);
             
             // adjust j because we erased i before j
             int insert_pos = (j > i) ? j : j + 1;
             
             tour.insert(tour.begin() + insert_pos, node_i);
             improved = true;
             break;
           }
        }
      }
    }
  }
  ant.reset(tour[0]);
  for (size_t i = 1; i < tour.size(); ++i)
    ant.visitNode(tour[i]);
  ant.calculateTourLength(m_graph);
}

void ACOSolver::updatePheromones() {
  int n = m_graph.size();

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      m_pheromones[i][j] *= (1.0f - m_config.rho);
      if (m_config.limits == PheromoneLimits::MaxMin) {
        if (m_pheromones[i][j] < m_config.tau_min)
          m_pheromones[i][j] = m_config.tau_min;
      }
    }
  }

  auto add_pheromone = [&](const std::vector<int>& tour, float length, float weight) {
    float delta = weight / length;
    for (size_t i = 0; i < tour.size() - 1; ++i) {
      m_pheromones[tour[i]][tour[i + 1]] += delta;
      m_pheromones[tour[i + 1]][tour[i]] += delta;
    }
    m_pheromones[tour.back()][tour.front()] += delta;
    m_pheromones[tour.front()][tour.back()] += delta;
  };

  if (m_config.update_strategy == PheromoneUpdateStrategy::AllAnts) {
    for (const auto &ant : m_ants) {
      add_pheromone(ant.getTour(), ant.getTourLength(), 1.0f);
    }
  } else if (m_config.update_strategy == PheromoneUpdateStrategy::Elitist) {
    for (const auto &ant : m_ants) {
      add_pheromone(ant.getTour(), ant.getTourLength(), 1.0f);
    }
    if (!m_global_best_tour.empty()) {
      add_pheromone(m_global_best_tour, m_global_best_distance, static_cast<float>(m_config.elitist_weight));
    }
  } else if (m_config.update_strategy == PheromoneUpdateStrategy::RankBased) {
    std::vector<const Ant*> ranked;
    for (const auto &ant : m_ants) ranked.push_back(&ant);
    std::sort(ranked.begin(), ranked.end(), [](const Ant* a, const Ant* b) {
      return a->getTourLength() < b->getTourLength();
    });
    
    int w = m_config.rank_count;
    for (int i = 0; i < w && i < ranked.size(); ++i) {
      add_pheromone(ranked[i]->getTour(), ranked[i]->getTourLength(), static_cast<float>(w - i));
    }
    if (!m_global_best_tour.empty()) {
      add_pheromone(m_global_best_tour, m_global_best_distance, static_cast<float>(w));
    }
  } else if (m_config.update_strategy == PheromoneUpdateStrategy::BestOnly) {
    if (!m_global_best_tour.empty()) {
      add_pheromone(m_global_best_tour, m_global_best_distance, 1.0f);
    }
  }

  if (m_config.limits == PheromoneLimits::MaxMin) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        if (m_pheromones[i][j] > m_config.tau_max)
          m_pheromones[i][j] = m_config.tau_max;
      }
    }
  }
}

void ACOSolver::step() {
  constructSolutions();

  for (auto &ant : m_ants) {
    apply2Opt(ant);
  }

  bool found_new_best = false;
  for (const auto &ant : m_ants) {
    if (ant.getTourLength() < m_global_best_distance) {
      m_global_best_distance = ant.getTourLength();
      m_global_best_tour = ant.getTour();
      found_new_best = true;
    }
  }

  updatePheromones();
  m_current_iteration++;

  auto now = std::chrono::steady_clock::now();
  double elapsed_ms =
      std::chrono::duration<double, std::milli>(now - m_start_time).count();

  if (found_new_best) {
    m_metrics.time_to_best_ms = elapsed_ms;
    m_metrics.history.push_back(
        {elapsed_ms, m_current_iteration, m_global_best_distance});
  }

  m_metrics.final_best_distance = m_global_best_distance;
  m_metrics.iterations_completed = m_current_iteration;
  m_metrics.total_execution_time_ms = elapsed_ms;
}