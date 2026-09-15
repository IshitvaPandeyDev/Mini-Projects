# CPU Scheduling Simulator

# Need - 
Modern operating systems rely on CPU scheduling to efficiently manage process execution and system resources.Modern operating systems rely on CPU scheduling to efficiently manage process execution and system resources. As systems grow more complex, understanding how different scheduling algorithms perform under varying workloads becomes critical for OS designers and developers. This simulator provides a hands-on environment to visualize and compare four fundamental scheduling algorithms — FCFS, SJF, Priority, and Round Robin — without needing a real OS environment. It bridges the gap between theoretical concepts taught in classrooms and practical implementation, making it especially valuable for students and educators. By generating processes automatically and running simulations interactively, users can observe how metrics like waiting time, turnaround time, and CPU utilization shift across algorithms. The built-in comparison mode further helps users make data-driven decisions about algorithm efficiency, reinforcing core OS concepts through direct experimentation rather than passive study.

## Files
| File | Member | Responsibility |
|------|--------|----------------|
| process.h | Member 1 | Process, ProcessManager, RandomProcessGenerator |
| scheduler.h | Member 2 | Scheduler (abstract), FCFS, SJF, PriorityScheduler, RoundRobin |
| engine.h | Member 3 | SimulationEngine, CPUTracker, AlgorithmComparator |
| ui.h | Member 4 | TerminalUI, GanttChart, StatisticsView |
| main.cpp | — | Entry point only |

## Compile & Run
```
g++ main.cpp -o run
./run          (Linux/Mac)
.\run.exe      (Windows PowerShell)
```

## Features
- FCFS, SJF (non-preemptive), Priority (non-preemptive), Round Robin
- Auto-generated processes (manual count or random count)
- Gantt chart display
- Per-process: completion time, turnaround time, waiting time
- CPU utilization % and idle time
- Algorithm comparison mode with rankings
- Save/load simulation to file (simulation.txt)

## OOP Concepts Used
- **Abstraction**: Scheduler is a pure abstract base class with execute() as a pure virtual function
- **Inheritance**: FCFS, SJF, PriorityScheduler, RoundRobin all inherit from Scheduler
- **Polymorphism**: SimulationEngine::run(Scheduler*) calls the correct algorithm at runtime
- **Encapsulation**: Process fields are private, accessed via getters/setters
- **Exception Handling**: Empty process list and invalid quantum throw and are caught in UI
- **Constructors/Destructors**: Defined in Process and all scheduler classes

## Menu Options
1. FCFS
2. SJF (Non-Preemptive)
3. Priority Scheduling (Non-Preemptive)
4. Round Robin (prompts for time quantum)
5. Compare All Algorithms
6. Save Last Simulation
7. Load Simulation
8. Exit

## Process Generation Formula
- PID: i+1
- Arrival Time: i % 4
- Burst Time: (i*3) % 7 + 1
- Priority: (i%5) + 1

## Team Members :
    =>Vasu Mittal
    =>Ishitva Pandey
    =>Aayush Gupta
    =>Khushi Jain