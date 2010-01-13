#ifndef TASK_EXECUTE_MANAGER_HPP__
#define TASK_EXECUTE_MANAGER_HPP__

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <queue>

#if 0
/* worker thread */
class task_executer : boost::noncopyable {
	task_executer(task_manager::task_type const& t) : t_(t) {};

	void operator()() {
		t_();
	};

private:
	task_manager::task_type t_;
};
#endif // 0

/**
 * @class task_manager
 * class responsible for handling queue of tasks to execute and running defined tasks in configured thread(s).
 */
class task_manager : boost::noncopyable {
public:
	typedef boost::function<void (void)>	task_type;
	typedef std::queue<task_type>		task_queue;
public:
	task_manager(int max_thread_count);
	~task_manager();

public:
	void push(const task_type& t);
	/* take tasks from queue and assign it to worker thread */
	void operator()();

	void stop();					//!< set that task_manager should stop
private:
	const int		max_thread_count_;	//!< max number of threads which can be created
	int			thread_count_;		//!< actual number of threads created
	task_queue		tq;			//!< queue of tasks to be executed
	mutable boost::mutex	tq_mutex;		//!< mutex for protecting access to task_queue
	boost::condition	tq_condition;		//!< condition variable so threads can wait till new tasks arrive
	bool			can_stop;		//!< flag telling that task_manager should finish itself. After setting this no new tasks can be added to queue and tasks which are in queue will be dropped.
};

#endif // include guard
