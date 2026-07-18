# Ant Colony Optimization Benchmarking Framework

## Overview
A high-performance C++ benchmarking framework for Ant Colony Optimization (ACO) algorithms applied to the Traveling Salesperson Problem (TSP). This tool is designed to provide a fair, rigorous environment for evaluating different ACO variations, offering extreme flexibility through composable algorithmic blocks and comprehensive telemetry collection.

## Features
- **Highly Modular Architecture**: Compose custom ACO variants by mixing and matching distinct phases:
  - **Selection Strategies**: Roulette Wheel, Pseudo-Random (ACS style), and Random Walk.
  - **Local Search**: 2-Opt and 2.5-Opt for post-construction path refinement.
  - **Pheromone Updates**: All Ants (AS), Elitist (EAS), Rank-Based (ASrank), and Best-Only (ACS/MMAS).
  - **Pheromone Limits**: Unbounded or Max-Min (MMAS) boundaries to prevent premature stagnation.
  - **Evaporation Mechanics**: Standard global evaporation and per-step local evaporation (ACS).
- **Procedural Field Generation**: Generate uniformly distributed points or complex clustered distributions to test algorithm robustness against local optima.
- **Fair Tournament Execution**: Time-boxed execution per algorithm, ensuring true algorithmic efficiency is measured rather than just implementation speed.
- **JSON Telemetry**: Detailed serialization of tournament tasks and reports, including convergence histories and final optimal tours, built for integration with external analysis tools.

## Building and Running

### Prerequisites
- C++20 compliant compiler
- CMake 3.16 or higher
- [Raylib](https://github.com/raysan5/raylib)
- [nlohmann/json](https://github.com/nlohmann/json)

### Build Instructions
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Usage
Run the built executable to access the interactive menu:
1. **Interactive Sandbox**: Visualize how a specific ACO configuration solves a TSP instance in real-time, step-by-step.
2. **Tournament Runner**: Run a benchmark comparing multiple configurations across vСоздай подробный профессиональный readme без глупых картинок в папке antarious generated or imported graphs. The results are saved to `tournament_report.json`.

## Configuration Parameters (`ACOConfig`)

The engine's behavior is entirely driven by the `ACOConfig` structure, allowing you to create countless hybrid algorithms:

- `ant_count`: Number of ants dispatched per iteration.
- `alpha`: Weight of the pheromone trail ($\alpha$).
- `beta`: Weight of the heuristic information (inverse distance, $\beta$).
- `rho`: Global evaporation rate ($\rho$).
- `xi`: Local evaporation rate (used when `use_local_evaporation` is enabled).
- `q0`: Probability of making a greedy choice in Pseudo-Random selection.
- `elitist_weight`: Additional pheromone weight applied to the global best tour in the Elitist update strategy.
- `rank_count`: Number of top ants that deposit pheromones in the Rank-Based update strategy.
- `tau_min` / `tau_max`: Pheromone boundaries used when Max-Min limits are enabled.
- `candidate_list_size`: Restricts the next-node choices to the $N$ nearest neighbors to optimize large graphs (0 for a full dense graph).

## Project Structure

- `src/core/`: Fundamental data structures, metrics, and configuration definitions.
- `src/engine/`: The core ACO mathematical logic (`ACOSolver`), local search implementations, field generators, and tournament orchestration.
- `src/utils/`: Serialization layer for converting between C++ memory structures and JSON representations.
- `src/ui/`: Raylib-based interface for the sandbox visualizer and tournament runner.

## Analysis and Visualization
While this C++ framework acts as the computation engine, it is designed to work in tandem with a web-based React frontend. The engine exports `tournament_report.json` files that can be loaded into the React application to render detailed interactive graphs, convergence charts, and side-by-side metric comparisons.
