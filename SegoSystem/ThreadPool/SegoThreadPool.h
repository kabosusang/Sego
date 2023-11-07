#include <thread>
#include <atomic>

class thread_pool
{
 std::atomic_bool done;
 thread_safe_queue<std::function<void()> > work_queue; // 1
 std::vector<std::thread> threads; // 2
 join_threads joiner; // 3
 void worker_thread()
 {
 while(!done) // 4
 {
 std::function<void()> task;
 if(work_queue.try_pop(task)) // 5
 {
 task(); // 6
 }
 else
 {
 std::this_thread::yield(); // 7
 }
 }
 }
public:
 thread_pool():
 done(false),joiner(threads)
 {
 unsigned const thread_count=std::thread::hardware_concurrency(); // 8
 try
 {
 for(unsigned i=0;i<thread_count;++i)
 {
 threads.push_back( 
 std::thread(&thread_pool::worker_thread,this)); // 9
 }
 }
 catch(...)
 {
 done=true; // 10
 throw;
 }
 }
 ~thread_pool()
 {
 done=true; // 11
 }
 template<typename FunctionType>
 void submit(FunctionType f)
 {
 work_queue.push(std::function<void()>(f)); // 12
 }
};
