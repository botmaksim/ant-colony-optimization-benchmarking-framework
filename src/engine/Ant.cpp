#include "Ant.h"

Ant::Ant(int start_node, int graph_size) {
  m_visited.resize(graph_size, false);
  reset(start_node);
}

void Ant::visitNode(int node) {
  m_tour.push_back(node);
  m_visited[node] = true;
}

bool Ant::isVisited(int node) const { return m_visited[node]; }

void Ant::reset(int start_node) {
  m_tour.clear();
  std::fill(m_visited.begin(), m_visited.end(), false);
  visitNode(start_node);
  m_tour_length = 0.0f;
}

void Ant::calculateTourLength(const Graph &graph) {
  m_tour_length = 0.0f;
  for (size_t i = 0; i < m_tour.size() - 1; ++i) {
    m_tour_length += graph.getDistance(m_tour[i], m_tour[i + 1]);
  }
  m_tour_length += graph.getDistance(m_tour.back(), m_tour[0]);
}