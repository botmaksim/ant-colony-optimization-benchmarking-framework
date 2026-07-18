#pragma once
#include <vector>

struct Node {
  float x;
  float y;
  int id;
};

class Graph {
public:
  Graph() = default;
  void addNode(float x, float y);
  void clear();

  int size() const;
  const std::vector<Node> &getNodes() const;
  float getDistance(int from, int to) const;

  void computeDistanceMatrix();

private:
  std::vector<Node> m_nodes;
  std::vector<std::vector<float>> m_matrix;
};