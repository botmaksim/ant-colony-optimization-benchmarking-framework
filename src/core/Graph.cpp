#include "Graph.h"
#include <cmath>

void Graph::addNode(float x, float y) {
  m_nodes.push_back({x, y, static_cast<int>(m_nodes.size())});
}

void Graph::clear() {
  m_nodes.clear();
  m_matrix.clear();
}

int Graph::size() const { return static_cast<int>(m_nodes.size()); }
const std::vector<Node> &Graph::getNodes() const { return m_nodes; }

float Graph::getDistance(int from, int to) const { return m_matrix[from][to]; }

void Graph::computeDistanceMatrix() {
  int n = size();
  m_matrix.assign(n, std::vector<float>(n, 0.0f));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (i == j)
        continue;
      float dx = m_nodes[i].x - m_nodes[j].x;
      float dy = m_nodes[i].y - m_nodes[j].y;
      m_matrix[i][j] = std::sqrt(dx * dx + dy * dy);
    }
  }
}