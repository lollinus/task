#include "task_manager.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <iomanip>

struct print_self_task {
	print_self_task(int id) : id_(id) {};
	void operator()() const {
		// simulate longer execution time
		std::cout << ">>> BEGIN print_self_task: " << id_ << std::endl;
		sleep(1);
		std::cout << "<<< END print_self_task: " << id_ << std::endl;
	}
private:
	int id_;
};

int main(int argc, char **argv) {
	boost::shared_ptr<task_manager> tm(new task_manager(2));

	boost::thread tm_thread(boost::ref(*tm));
	for(int i=0; i<20; ++i) {
		boost::function<void (void)> f=print_self_task(i);
		tm->push(f);
	}
	for(int i=0; i<4; ++i) {
		sleep(3);
		std::cout << std::setfill('=') << std::setw(80) << "main function iteration: " << i << std::endl;
	}
	tm->stop();
	tm_thread.join();
	return 0;
}
