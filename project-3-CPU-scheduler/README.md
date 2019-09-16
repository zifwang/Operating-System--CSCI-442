# Project 3 - CPU Sheduling Simulator

# Name: Zifan Wang

# File list:

    The CPU Scheduling Simulator project contains files to run.

    All codes are in src folder.

    algorithms folder:
        - fcfs_scheduler.{h,cpp}: Use the             first-come-first-service logic to simulate the CPU Scheduling. Inheritance of scheduler.h file. 
        - multilevel_feedback_scheduler.{h,cpp}: Use the multilevel-feedback logic to simulate the CPU Scheduling. Inheritance of scheduler.h file.
        - priority_scheduler.{h,cpp}: Use the priority design logic to simulate the CPU Scheduling. Inheritance of scheduler.h file.
        - round_robin_scheduler.{h,cpp}: Use the RR logic to simulate the CPU Scheduling. Inheritance of scheduler.h file.
        - scheduler.h is the parent class for all algorithms
    types folder:
        - burst.h: Define the type of burst: CPU or IO
        - event.h: Define events in the simulation after process comes into CPU.
        - process.h: Define the type of processes.
        - scheduling_decision.h: contains time_slice, thread, and explanation of processes.
        - system_stats.h: contains various of system statistics.
        - thread.{h,cpp}: represents a thread of execution, with one or more CPU + I/O bursts.
    util folder:
        - flags.{h,cpp}: Parses any provided flags, populating the given config and file path as appropriate.
        - logger.{h,cpp}: Help to print simulation.
    simulation.{h,cpp}: Use to handle the coming process in each event in the simulation.
    main.cpp: Entry point of CPU simulation.

# No interesting features in my programs.

# 15-20 hours to finish the project.

# Explain of multi-level feedback queue scheduling algorithm:
    1. System processes will have the highest priority. Then, the interactive processes. The last are batch and normal process. 
    2. The total service time of CPU and the response time of system threads are metrics I want to optimize.
    3. The system and interactive processes are use RR algorithm to execute. System processes use 2 time slices to be executed. Interactive processes use 3 time slices to be executed. However, thier prioirties decrease as they been 
    executed which make them move to lower level of queues. The batch and normal processes use FCFS algorithm.
    4. When system processes are detected, they are places in the top of the first 9 queues. When interactive processes are deteced, they are places in the next 9 queues. Their priorities decrease as they been executed. The batch and normal processes are placed in the last queues.
    5. Yes, starvation is possible in my algorithm, espically taken batch and normal threads into consideration. Because these two threads are in the last of queues.
    6. No. The algorithm will favor the system and interactive threads.