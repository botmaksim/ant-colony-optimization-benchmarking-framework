#include "FieldGenerator.h"
#include <algorithm>
#include <random>

Graph FieldGenerator::generateUniform(int size, float width, float height) {
  Graph g;
  std::mt19937 gen(std::random_device{}());
  std::uniform_real_distribution<float> distX(50.0f, width - 50.0f);
  std::uniform_real_distribution<float> distY(50.0f, height - 50.0f);

  for (int i = 0; i < size; ++i) {
    g.addNode(distX(gen), distY(gen));
  }
  return g;
}

Graph FieldGenerator::generateClustered(int size, int clusters, float width,
                                        float height) {
  Graph g;
  std::mt19937 gen(std::random_device{}());
  std::uniform_real_distribution<float> distCenterX(100.0f, width - 100.0f);
  std::uniform_real_distribution<float> distCenterY(100.0f, height - 100.0f);

  std::vector<std::pair<float, float>> centers;
  for (int i = 0; i < clusters; ++i) {
    centers.push_back({distCenterX(gen), distCenterY(gen)});
  }

  std::uniform_int_distribution<int> clusterSelect(0, clusters - 1);
  std::normal_distribution<float> offsetDist(0.0f, 40.0f);

  for (int i = 0; i < size; ++i) {
    int c = clusterSelect(gen);
    float cx = centers[c].first;
    float cy = centers[c].second;

    float x = cx + offsetDist(gen);
    float y = cy + offsetDist(gen);

    x = std::clamp(x, 50.0f, width - 50.0f);
    y = std::clamp(y, 50.0f, height - 50.0f);

    g.addNode(x, y);
  }
  return g;
}
