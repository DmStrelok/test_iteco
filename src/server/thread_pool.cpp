#include "thread_pool.h"

#include <cstring>

namespace app {

//------------------------------------------------------------------------------
void condition_var::wait_for(const std::uint16_t time_ms)
{
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait_for(lock, std::chrono::milliseconds(time_ms));
}
//------------------------------------------------------------------------------
void condition_var::notify_one()
{
    cv.notify_one();
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
thread_pool::worker::worker(run fun_, settings &sett, ring_buffer &rbuffer, condition_var &cv)
    : fun(fun_)
{
    thread = std::thread(&worker::start, this, &sett, &rbuffer, &cv);
}
//------------------------------------------------------------------------------
void thread_pool::worker::start(settings *sett, ring_buffer *rbuffer, condition_var *cv)
{
    fun(sett, rbuffer, cv, [this]()->bool {return this->is_stop();});
    event_work = false;
}
//------------------------------------------------------------------------------
void thread_pool::worker::stop()
{
    event_stop = true;
}
//------------------------------------------------------------------------------
void thread_pool::worker::join()
{
    if (thread.joinable()) thread.join();
}
//------------------------------------------------------------------------------
bool thread_pool::worker::is_stop()
{
    return event_stop;
}
//------------------------------------------------------------------------------
bool thread_pool::worker::is_work()
{
    return event_work;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
thread_pool::thread_pool(const type size_)
    : size(size_)
{}
//------------------------------------------------------------------------------
thread_pool::~thread_pool()
{
    for (type i{0}; i < pool.size(); i++) pool[i]->stop();
    for (type i{0}; i < pool.size(); i++) pool[i]->join();
}
//------------------------------------------------------------------------------
bool thread_pool::add(run fun, settings &sett, ring_buffer &rbuffer, condition_var &cv)
{
    for (type i{0}; i < pool.size(); i++)
        if (!pool[i]->is_work())
        {
            pool[i] = std::make_unique<worker>(fun, sett, rbuffer, cv);
            return true;
        }
    if (pool.size() >= size) return false;
    pool.emplace_back(std::make_unique<worker>(fun, sett, rbuffer, cv));
    return true;
}
//------------------------------------------------------------------------------

} // namespace app
