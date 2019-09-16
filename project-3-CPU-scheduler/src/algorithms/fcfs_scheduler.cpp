#include "fcfs_scheduler.h"
#include <boost/format.hpp>   //include formate function

using namespace std;


SchedulingDecision* FcfsScheduler::get_next_thread(const Event* event) {
  // TODO: implement me
  if(FCFS.empty()){
    return nullptr;
  }
  boost::format message("Selected from %lu threads; will run to completion of burst");
  // Create a SchedulingDecision pointer 
  SchedulingDecision* decision = new SchedulingDecision();
  // Set the thread of decision to the front of FCFS queue 
  decision->thread = FCFS.front();
  // pass message to explanation
  decision->explanation = (message%size()).str();
  // Delete the front of FCFS Ready queue.
  FCFS.pop();
  return decision;
}


void FcfsScheduler::enqueue(const Event* event, Thread* thread) {
  // TODO: implement me
  // Push thread into FCFS's ready queue.
  FCFS.push(thread);
}


bool FcfsScheduler::should_preempt_on_arrival(const Event* event) const {
  // TODO: implement me
  return false;
}


size_t FcfsScheduler::size() const {
  // TODO: implement me
  // Find the size of queue
  return FCFS.size();
}
