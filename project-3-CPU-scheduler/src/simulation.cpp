#include "simulation.h"
#include "types/event.h"
#include <cassert>
#include <fstream>
#include <iostream>

using namespace std;


void Simulation::run(const string& filename) {
  read_file(filename);

  // While their are still events to process, invoke the corresponding methods
  // to handle them.
  while (!events.empty()) {
    const Event* event = events.top();
    events.pop();

    // Invoke the appropriate method on the scheduler for the given event type.
    switch (event->type) {
    case Event::THREAD_ARRIVED:
      handle_thread_arrived(event);
      break;

    case Event::THREAD_DISPATCH_COMPLETED:
      handle_thread_dispatch_completed(event);
      break;

    case Event::PROCESS_DISPATCH_COMPLETED:
      handle_process_dispatch_completed(event);
      break;

    case Event::CPU_BURST_COMPLETED:
      handle_cpu_burst_completed(event);
      break;

    case Event::IO_BURST_COMPLETED:
      handle_io_burst_completed(event);
      break;

    case Event::THREAD_COMPLETED:
      handle_thread_completed(event);
      break;

    case Event::THREAD_PREEMPTED:
      handle_thread_preempted(event);
      break;

    case Event::DISPATCHER_INVOKED:
      handle_dispatcher_invoked(event);
      break;
    }

    // If this event triggered s state change, print it out
    if(event->thread && event->thread->current_state != event->thread->previous_state){
      // print humand_readable message indicating that the thread associated with given event
      logger.print_state_transition(event,event->thread->previous_state,event->thread->current_state);
    }
    
    // Keep track of the total elapsed time
    stats.total_time = event->time;

    // Free the event's memory.
    delete event;
  }
  // Print process detials if needed
  for(auto entry: processes){
    logger.print_process_details(entry.second);
  }

  // All done! Calculate and print the state.
  logger.print_statistics(calculate_statistics());
}


//==============================================================================
// Event-handling methods
//==============================================================================


void Simulation::handle_thread_arrived(const Event* event) {
  // TODO: handle this event properly (feel free to modify code structure, tho)
  // ideas: 
  // cout << "At time " << event->time << ":" << endl;
  // cout << "\t" << "THREAD_ARRIVED" << endl;
  // Check whether current_state is new
  assert(event->thread->current_state == Thread::State::NEW);
  // Enqueue (the event and thread)
  scheduler->enqueue(event,event->thread);

  /* Set Ready */
  // set previous_state to New
  event->thread->previous_state = Thread::State::NEW;
  // Set current_state to Ready
  event->thread->current_state = Thread::State::READY;

  // Set arrival time
  // event->thread->arrival_time = event->time;
  
  // Check whether current_state is in Ready
  assert(event->thread->current_state == Thread::State::READY);
  // Processor Idle (no current thread)
  if(active_thread == nullptr){
    //cout << active_thread << endl;
    // true go to Dispatcher invoked event 
    Event* e = new Event(Event::Type::DISPATCHER_INVOKED, event->time, event->thread);   // use pointer to create a new event
    events.push(e);
  }
  // else{
  //   // No, check priority
      // if high priority goes to handle_thread_preempted 

  // }
  
}


void Simulation::handle_thread_dispatch_completed(const Event* event) {
  // TODO: handle this event properly (feel free to modify code structure, tho)
  // cout << "At time " << event->time << ":" << endl;
  // cout << "\t" << "THREAD_DISPATCH_COMPLETED" << endl;
  // check whether current state is ready
  assert(event->thread->current_state = Thread::State::READY);
  
  // Change previous thread to active_thread
  prev_thread = active_thread;
  active_thread = NULL;

  // Set last thread == current thread, which means set to READY state.
  event->thread->previous_state = Thread::State::READY;

  // Set current state to Running state
  event->thread->current_state = Thread::State::RUNNING;
  
  if((signed)event->thread->start_time == -1){
    // Set up start_time;
    event->thread->start_time = event->time;
  }

  // Set up state_change_time
  event->thread->state_change_time = event->time;

  // Check current state whether is RUNNIng
  assert(event->thread->current_state == Thread::State::RUNNING);

  // get new active_thread;
  active_thread = event->thread;
  
  // get bursts
  Burst* thread_bursts = event->thread->bursts.front();
  // Compare Time slice with burst time
  if(event->scheduling_decision->time_slice < (unsigned int)thread_bursts->length){
    // if Time slice < burst time, goto handle_thread_preempted event
    Event* e = new Event(Event::Type::THREAD_PREEMPTED, (event->time + event->scheduling_decision->time_slice), event->thread);   // use pointer to create a new event
    events.push(e);
  }else{
      Event* e = new Event(Event::Type::CPU_BURST_COMPLETED, (event->time+thread_bursts->length), event->thread);   // use pointer to create a new event
      events.push(e);
  }

}


void Simulation::handle_process_dispatch_completed(const Event* event) {
  // same to handle_thread_dispatch_completed(event);
  handle_thread_dispatch_completed(event);
}


void Simulation::handle_cpu_burst_completed(const Event* event) {
  // TODO: handle this event properly (feel free to modify code structure, tho)
  // cout << "At time " << event->time << ":" << endl;
  // cout << "\t" << "CPU_BURST_COMPLETED" << endl;
  // Check current_state
  assert(event->thread->current_state == Thread::State::RUNNING);

  // pop burst
  event->thread->bursts.pop();

  // // change active thread and pre thread
  // prev_thread = active_thread;
  // active_thread = nullptr;

  // Check whether is the Last CPU burst
  if(event->thread->bursts.empty()){
     // if yes, goto handle_thread_completed
     Event* e = new Event(Event::Type::THREAD_COMPLETED, event->time, event->thread);   // use pointer to create a new event
     events.push(e);
  }else{
      // if no, goto handle_io_burst_completed and set current state to blocked
      event->thread->current_state = Thread::State::BLOCKED;
      // previous to running
      event->thread->previous_state = Thread::State::RUNNING;

      event->thread->service_time += event->time - event->thread->state_change_time;
      event->thread->state_change_time = event->time;
      active_thread = nullptr;
      // get front of burst queue
      Burst* burst = event->thread->bursts.front();

      Event* e_1 = new Event(Event::Type::IO_BURST_COMPLETED, event->time+burst->length, event->thread);   // use pointer to create a new event
      events.push(e_1);
      Event* e_2 = new Event(Event::Type::DISPATCHER_INVOKED, event->time, event->thread);   // use pointer to create a new event
      events.push(e_2);

    }
//     // Change the current state to READY
//     // event->thread->current_state = Thread::State::READY;
//     event->thread->service_time += event->time - event->thread->state_change_time;
//     event->thread->state_change_time = event->time;
//     Event* e_2 = new Event(Event::Type::DISPATCHER_INVOKED, event->time, event->thread);   // use pointer to create a new event
//     events.push(e_2);
}


void Simulation::handle_io_burst_completed(const Event* event) {
  // TODO: handle this event properly (feel free to modify code structure, tho)
  // cout << "At time " << event->time << ":" << endl;
  // cout << "\t" << "IO_BURST_COMPLETED" << endl;
  assert(event->thread->current_state = Thread::State::BLOCKED);

  // io_time
  event->thread->io_time += event->time - event->thread->state_change_time;

  // Set perivous state to current_state
  event->thread->previous_state = event->thread->current_state;
  // Set current state to Ready 
  event->thread->current_state = Thread::State::READY;

  // state change time
  event->thread->state_change_time = event->time;

  // Enqueue
  scheduler->enqueue(event,event->thread);

  // check type of burst
  Burst* burst = event->thread->bursts.front();
  assert(burst->type == Burst::IO);

  // Pop burst
  event->thread->bursts.pop();

  /// Processor Idle (no current thread)
  if(active_thread == nullptr){
    //cout << active_thread << endl;
    // true go to Dispatcher invoked event 
    Event* e = new Event(Event::Type::DISPATCHER_INVOKED, event->time, event->thread);   // use pointer to create a new event
    events.push(e);
  }
  // else{
  //   // No, check priority
      // if high priority goes to handle_thread_preempted 

  // }

}


void Simulation::handle_thread_completed(const Event* event) {
  // TODO: handle this event properly (feel free to modify code structure, tho)
  // cout << "At time " << event->time << ":" << endl;
  // cout << "\t" << "THREAD_COMPLETED" << endl;
  assert(event->thread->current_state == Thread::State::RUNNING);

  // service time
  event->thread->service_time += event->time - event->thread->state_change_time;

  // Set current_state to Exit
  event->thread->current_state = Thread::State::EXIT;
  // Set perivous_state to Running 
  event->thread->previous_state = Thread::State::RUNNING;

  // Update end_time
  event->thread->end_time = event->time;

  active_thread = NULL;

  Event* e = new Event(Event::Type::DISPATCHER_INVOKED, event->time, NULL);
  events.push(e);

  // cout << "event: THREAD_COMPLETED" << endl;
}


void Simulation::handle_thread_preempted(const Event* event) {
  // TODO: handle this event properly (feel free to modify code structure, tho)
  // cout << "At time " << event->time << ":" << endl;
  // cout << "\t" << "THREAD_PREEMPTED" << endl;
  // Check current_state
  assert(event->thread->current_state == Thread::State::RUNNING);
  active_thread = NULL;
  // update service_time
  event->thread->service_time += event->time - event->thread->state_change_time;

  // Set pervious_state to current_state
  event->thread->previous_state = event->thread->current_state;

  // Set current thread to ready
  event->thread->current_state = Thread::State::READY;
  
  // Decrease CPU burst;
  Burst* thread_bursts = event->thread->bursts.front();
  // check burst type
  assert(thread_bursts->type == Burst::CPU);
  event->thread->bursts.front()->length -= 3;

  // priority_level increase
  event->thread->priority ++;

  // Enqueue event
  scheduler->enqueue(event,event->thread);

  // Go to handle_dispatcher_invoked event
  Event* e = new Event(Event::Type::DISPATCHER_INVOKED, event->time, NULL);   // use pointer to create a new event
  events.push(e);

  // // cout << "event: THREAD_PREEMPTED" << endl;
}


void Simulation::handle_dispatcher_invoked(const Event* event) {
  // TODO: handle this event properly (feel free to modify code structure, tho)
  // cout << "At time " << event->time << ":" << endl;
  // cout << "\t" << "DISPATCHER_INVOKED" << endl;

  // //assert(event->thread->current_state == Thread::State::READY);
  // assert(!active_thread);
  SchedulingDecision* schedulingdecision = scheduler->get_next_thread(event);
  Thread* next = nullptr;

  if(schedulingdecision != nullptr){
    next = schedulingdecision->thread;
  }

  // If made a decision
  if(next != nullptr){
    Event* e_tem = new Event(Event::Type::DISPATCHER_INVOKED, event->time, next);   // use pointer to create a new event
    logger.print_verbose(e_tem,e_tem->thread,schedulingdecision->explanation);

    if(prev_thread && next->process == prev_thread->process){
      // if thread goto handle_thread_dispatch_completed
      Event* e = new Event(Event::Type::THREAD_DISPATCH_COMPLETED, event->time+thread_switch_overhead, next, schedulingdecision);   // use pointer to create a new event
      e->thread->state_change_time = event->time;
      events.push(e);
      // Update dispatch_time in stats
      stats.dispatch_time += thread_switch_overhead;
    }
    else{
        // if process goto handle_process_dispatch_completed
        Event* e = new Event(Event::Type::PROCESS_DISPATCH_COMPLETED, event->time+process_switch_overhead, next, schedulingdecision);   // use pointer to create a new event
        e->thread->state_change_time = event->time;
        events.push(e);
        // Update dispatch_time in stats
        stats.dispatch_time += process_switch_overhead;
    }
    active_thread = schedulingdecision->thread;
  }

}


//==============================================================================
// Utility methods
//==============================================================================


void Simulation::add_event(Event* event) {
  if (event != nullptr) {
    events.push(event);
  }
}


void Simulation::read_file(const string& filename) {
  ifstream file(filename.c_str());

  if (!file) {
    cerr << "Unable to open simulation file: " << filename << endl;
    exit(EXIT_FAILURE);
  }

  size_t num_processes;

  // Read the total number of processes, as well as the dispatch overheads.
  file >> num_processes >> thread_switch_overhead >> process_switch_overhead;

  // Read in each process.
  for (size_t p = 0; p < num_processes; p++) {
    Process* process = read_process(file);

    processes[process->pid] = process;
  }
}


Process* Simulation::read_process(istream& in) {
  int pid, type;
  size_t num_threads;

  // Read in the process ID, its type, and the number of threads.
  in >> pid >> type >> num_threads;

  // Create the process and register its existence in the processes map.
  Process* process = new Process(pid, (Process::Type) type);

  // Read in each thread in the process.
  for (size_t tid = 0; tid < num_threads; tid++) {
    process->threads.push_back(read_thread(in, tid, process));
  }

  return process;
}


Thread* Simulation::read_thread(istream& in, int tid, Process* process) {
  int arrival_time;
  size_t num_cpu_bursts;

  // Read in the thread's arrival time and its number of CPU bursts.
  in >> arrival_time >> num_cpu_bursts;

  Thread* thread = new Thread(arrival_time, tid, process);

  // Read in each burst in the thread.
  for (size_t n = 0, burst_length; n < num_cpu_bursts * 2 - 1; n++) {
    in >> burst_length;

    Burst::Type burst_type = (n % 2 == 0)
        ? Burst::CPU
        : Burst::IO;

    thread->bursts.push(new Burst(burst_type, burst_length));
  }

  // Add an arrival event for the thread.
  events.push(new Event(Event::THREAD_ARRIVED, thread->arrival_time, thread));

  return thread;
}


SystemStats Simulation::calculate_statistics() {
  // TODO: your code here (optional; feel free to modify code structure)

  // 
   
  for (int i = Process::SYSTEM; i<= Process::BATCH; i++){
    for (auto proc : processes)
    {
      if (proc.second->type == i)
      {
        for (auto j : proc.second->threads)
        {
          // update service time
          stats.service_time += j->service_time;
          // io_time update
          stats.io_time += j->io_time;
          // resp_time and turnaround_time update
          stats.avg_thread_response_times[i] += j->response_time();
          stats.avg_thread_turnaround_times[i] += j->turnaround_time();
          // Count thread types.
          stats.thread_counts[i] ++;
        }
      }
    }
  }


  // Calculate overall time.
  stats.total_cpu_time = stats.service_time + stats.dispatch_time;
  stats.total_idle_time = stats.total_time - stats.total_cpu_time;

  // Calculate overal utilization and efficiency.
  stats.cpu_utilization = 100.0 * stats.total_cpu_time / stats.total_time;
  stats.cpu_efficiency = 100.0 * stats.service_time / stats.total_time;

  // Calculate turnaround and response time
  for(int i = Process::SYSTEM; i <= Process::BATCH; i++){
    if(stats.thread_counts[i] == 0){
      continue;
    }
    stats.avg_thread_response_times[i] /= stats.thread_counts[i];
    stats.avg_thread_turnaround_times[i] /= stats.thread_counts[i];
  }
  return stats;
}




