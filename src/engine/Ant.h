#pragma once
#include "core/Graph.h"
#include <vector>

class Ant {
public:
  Ant(int start_node, int graph_size);
  void visitNode(int node);
  bool isVisited(int node) const;
  void reset(int start_node);

  const std::vector<int> &getTour() const { return m_tour; }
  float getTourLength() const { return m_tour_length; }
  void setTourLength(float length) { m_tour_length = length; }

  void calculateTourLength(const Graph &graph);

private:
  std::vector<int> m_tour;
  std::vector<bool> m_visited;
  float m_tour_length = 0.0f;
};