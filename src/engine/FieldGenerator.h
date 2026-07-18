#pragma once
#include "core/Graph.h"

class FieldGenerator {
public:
  static Graph generateUniform(int size, float width, float height);
  static Graph generateClustered(int size, int clusters, float width,
                                 float height);
};
