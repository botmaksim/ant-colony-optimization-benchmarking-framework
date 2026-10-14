# ACO-Benchmark: Metaheuristic Optimization Framework for TSP

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![CMake 3.14+](https://img.shields.io/badge/CMake-3.14%2B-blue.svg)](https://cmake.org/)
[![Algorithms](https://img.shields.io/badge/Algorithms-Ant%20Colony%20Optimization-brightgreen.svg)](#)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A high-performance C++17 benchmarking framework for evaluating Ant Colony Optimization metaheuristics applied to the Traveling Salesperson Problem. The tool provides a rigorous environment for profiling Ant System, Ant Colony System, and Max-Min Ant System algorithms with automated statistical metric collection against standard TSPLIB benchmark instances.

---

## Table of Contents
1. [Supported Algorithms](#supported-algorithms)
2. [Benchmark Capabilities](#benchmark-capabilities)
3. [Prerequisites](#prerequisites)
4. [Build & Execution](#build--execution)
5. [Directory Structure](#directory-structure)
6. [License](#license)

---

## Supported Algorithms

* **Ant System**: Standard pheromone trail reinforcement and evaporation dynamics.
* **Ant Colony System**: Local pheromone updating with aggressive pseudo-random proportional action choice.
* **Max-Min Ant System**: Pheromone trail limits bounding min and max bounds to prevent premature convergence, coupled with best-so-far reinforcement.
* **Local Search Heuristics**: Integrated 2-opt trajectory optimization improving tour convergence rates.

---

## Benchmark Capabilities

* **TSPLIB Compatibility**: Native parser ingesting standard symmetric TSP coordinate matrix instances.
* **Statistical Metrics**: Automated calculation of best, worst, mean tour lengths, standard deviation, and convergence iteration profiles across repeated seed runs.
* **Extensible Architecture**: Decoupled algorithm interfaces allowing custom metaheuristic variants to be plugged in.

---

## Prerequisites

* Compiler: GCC 9+ or Clang 10+ with C++17 support
* Build System: CMake 3.14+

---

## Build & Execution

```bash
git clone git@github.com:botmaksim/ant-colony-optimization-benchmarking-framework.git
cd ant-colony-optimization-benchmarking-framework

cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)

./build/aco_benchmark
```

---

## Directory Structure

```text
ant-colony-optimization-benchmarking-framework/
├── CMakeLists.txt           # Build automation configuration
├── src/                     # Core metaheuristics, pheromone models, and solvers
└── third_party/             # TSPLIB instance loaders and matrix utilities
```

---

## License

This project is open-source software licensed under the [MIT License](LICENSE).
