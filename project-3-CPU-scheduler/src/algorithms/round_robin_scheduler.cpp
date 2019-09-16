#include "round_robin_scheduler.h"
#include <boost/format.hpp>   //include formate function

using namespace std;


SchedulingDecision* RoundRobinScheduler::get_next_thread(const Event* event) {
  // TODO: implement me
  // Create a SchedulingDecision pointer 
  SchedulingDecision* decision = new SchedulingDecision();
  
  // Next thread
  Thread* next;
  
  // Check RR queue size
  if(RR.empty()){
    return nullptr;
  }

  next = RR.front();
  decision->thread = next;
  boost::format message("Selected from %d threads; will run at most 3 time_slice");

  message
          % RR.size();
  decision->explanation = message.str();
  decision->time_slice = time_slice;
  
  // Delete the front of RR Ready queue.
  RR.pop();
  return decision;
}


void RoundRobinScheduler::enqueue(const Event* event, Thread* thread) {
  // TODO: implement me
  RR.push(thread);
}


bool RoundRobinScheduler::should_preempt_on_arrival(const Event* event) const {
  // TODO: implement me
  return false;
}


size_t RoundRobinScheduler::size() const {
  // TODO: implement me
  return RR.size();
}
