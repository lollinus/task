#ifndef TASK_EXECUTE_MANAGER_HPP__
#define TASK_EXECUTE_MANAGER_HPP__

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <queue>

/* worker thread */
class task_executer : boost::noncopyable {
	task_executer(task_manager::task_type const& t) : t_(t) {};

	void operator()() {
		t_();
	};

private:
	task_manager::task_type t_;
};

/**
 * @class task_manager
 * class responsible for handling queue of tasks to execute and running defined tasks in configured thread(s).
 */
class task_manager : boost::noncopyable {
public:
	typedef boost::function<void (void)>	task_type;
	typedef std::queue<task_type>		task_queue;
public:
	task_manager(int max_thread_count) : max_thread_count_(max_thread_count_), thread_count_(0), tq(), tq_mutex(), tq_condition(), can_stop(false) {};
	~task_manager() {};

public:
	void push(const task_type& t) {
		boost::mutex::scoped_lock guard(tq_mutex);
		tq.push(t);
		tq_condition.notify_one();
	}

	/* take tasks from queue and assign it to worker thread */
	void run() {
		for(;;) {
			task_type execute_task;
			{ // mutex lock scope
			boost::mutex::scoped_lock guard(tq_mutex);
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

private:
	const int		max_thread_count_;
	int			thread_count_;
	task_queue		tq;
	mutable boost::mutex	tq_mutex;
	boost::condition	tq_condition;
	bool			can_stop;
};

#endif // include guard
