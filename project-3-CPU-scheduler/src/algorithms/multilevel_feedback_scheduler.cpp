#include "algorithms/multilevel_feedback_scheduler.h"
#include <boost/format.hpp>
#include <string>
#include <types/process.h>
//#include <iostream>
using namespace std;

/**
 * Mutilevel Queues:
 *  - Ready Queue is paritioned into separate queues
 *      - Foreground(interactive)
 *      - Background(batch)
 *  - Each queue has its own scheduling algorithm
 *      - Foreground: RR
 *      - Background: FCFS
 *  - Priority:
 *      1. system processes
 *      2. interative process
 *      3. batch process
 *      4. normal process
 *  - A multilevel feedback queue scheduler is defined by the following parameters
 *      - number of queues
 *      - scheduling algorithms for each queue
 *      - Mehod used to determine when to promote or demot a process
 *      - Method used to determine which queue a process will enter when that process needs service
 */

SchedulingDecision* MultilevelFeedbackScheduler::get_next_thread(
    const Event* event) {
  // TODO: implement me
  // Create a SchedulingDecision pointer 
  SchedulingDecision* decision = new SchedulingDecision();
  
  // Get Next thread
  Thread* next;
  	for (int i = 0; i < 19; ++i)
  	{
  		if (MF[i].size()!=0)
  		{
  			next = MF[i].front();
    		decision->thread = next;
    		boost::format message("Selected from %d (Queue %d) threads ");
    		message
          		% MF[i].size()
          		% (i+1);
    		decision->explanation = message.str();
        Process::Type type = decision->thread->process->type;
        // Base on different type, set different time slice
        if(type ==  Process::Type::SYSTEM){
          // RR for System
          decision->time_slice = 2;
        }
        else if(type == Process::Type::INTERACTIVE){
          // RR for Interactive
          decision->time_slice = 3;
        }
        // other FCFS
        // pop MF
    		MF[i].pop();
    		return decision;
  		}
  	}
  return nullptr;
}


void MultilevelFeedbackScheduler::enqueue(const Event* event, Thread* thread) {
  // TODO: implement me
  Process* proc = thread->process;
	Process::Type type = proc->type;
	int level = thread->priority;
	if (level >= 2){
		level = 2;
	}
  if(type == Process::Type::SYSTEM){
    MF[0+level*4].push(thread);
  }
  else if(type == Process::Type::INTERACTIVE){
    MF[9+level*4].push(thread);
  }
	else if(type == Process::Type::NORMAL || type == Process::Type::BATCH){
    MF[18].push(thread);
  }
}


bool MultilevelFeedbackScheduler::should_preempt_on_arrival(
    const Event* event) const {
  // // TODO: implement me
  // SchedulingDecision* decision = new SchedulingDecision();
  // // Get Next thread
  // Thread* next;
  // for (int i = 0; i < 12; ++i){
  // 	if (MF[i].size()!=0){
  // 			next = MF[i].front();
  //   		decision->thread = next;
  //       Process::Type type = decision->thread->process->type;
  //       if(type == Process::Type::SYSTEM){
  //         return false;
  //       }   
  //       else if(type == Process::Type::INTERACTIVE){
  //         return true;
  //       }
  //       else if(type == Process::Type::BATCH){
  //         return true;
  //       }
  //       else if(type == Process::Type::NORMAL){
  //         return true;
  //       }
  //   }
  // }
  return false;
}


size_t MultilevelFeedbackScheduler::size() const {
  // TODO: implement me
  return MF.size();
}
