#include "algorithms/priority_scheduler.h"
#include "types/process.h"
#include <string>
#include <boost/format.hpp>   //include formate function
using namespace std;


SchedulingDecision* PriorityScheduler::get_next_thread(const Event* event) {
  // TODO: implement me
  // Create a SchedulingDecision pointer 
  SchedulingDecision* decision = new SchedulingDecision();
  
  // Next thread
  Thread* next;

  for(int i = 0; i < 4; i++){
    if(PS[i].size() != 0){
      next = PS[i].front();
      decision->thread = next;
      boost::format message("Selected from queue %d [S:%d I:%d N:%d B:%d ]");
    		message
          		% i
          		% PS[0].size()
          		% PS[1].size()
          		% PS[2].size()
          		% PS[3].size();
    		decision->explanation = message.str();
    		PS[i].pop();
        return decision;
    }
  }
  return nullptr;
}


void PriorityScheduler::enqueue(const Event* event, Thread* thread) {
  // TODO: implement me
  Process* proc = thread->process;
	Process::Type type = proc->type;
  if(type == Process::Type::SYSTEM){
    PS[0].push(thread);
  }
  else if(type == Process::Type::INTERACTIVE){
    PS[1].push(thread);
  }
	else if(type == Process::Type::NORMAL){
    PS[2].push(thread);
  }
  else if(type == Process::Type::BATCH){
    PS[3].push(thread);
  }
}


bool PriorityScheduler::should_preempt_on_arrival(const Event* event) const {
  // TODO: implement me
  return false;
}


size_t PriorityScheduler::size() const {
  // TODO: implement me
  return PS.size();
}