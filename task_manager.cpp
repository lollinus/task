#include "task_manager.hpp"

#include <iostream>

task_manager::task_manager(int max_thread_count)
	: max_thread_count_(max_thread_count_)
	, thread_count_(0)
	, tq()
	, tq_mutex()
	, tq_condition()
	, can_stop(false) {
}

task_manager::~task_manager() {
}

void task_manager::push(const task_type& t) {
	boost::mutex::scoped_lock guard(tq_mutex);
	tq.push(t);
	tq_condition.notify_one();
}


void task_manager::operator()() {
	for(;;) {
		task_type execute_task;
		{ // mutex lock scope
			boost::mutex::scoped_lock guard(tq_mutex);
			if(can_stop) {
				std::cout << "Stopping" << std::endl;
				break;
			}
			if(tq.empty()) {
				tq_condition.wait(guard);
			}
			if(tq.empty()) {
				continue;
			}
			execute_task = tq.front();
			tq.pop();
		} // end mutex lock scope
		execute_task();
	}
}


void task_manager::stop() {
	boost::mutex::scoped_lock guard(tq_mutex);
	can_stop = true;
	tq_condition.notify_one();
}




