#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include "settings.h"
#include "ring_buffer.h"

#include <cstdint>
#include <memory>
#include <vector>
#include <functional>
#include <thread>
#include <condition_variable>

namespace app {

//------------------------------------------------------------------------------
struct condition_var
{
    std::mutex mutex;
    std::condition_variable cv;
    void wait_for(const std::uint16_t);
    void notify_one();
};
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class thread_pool
{
public:
    struct worker;

private:
    using type = std::uint16_t;
    using run = std::function<void(settings *, ring_buffer *, condition_var *, std::function<bool()>)>;
    type size;
    std::vector<std::unique_ptr<worker>> pool;

public:
    thread_pool(const type);
    ~thread_pool();
    thread_pool(const thread_pool &) = delete;
    thread_pool(thread_pool &&) = delete;
    thread_pool &operator=(const thread_pool &) = delete;
    thread_pool &operator=(thread_pool &&) = delete;
    bool add(run fun, settings &sett, ring_buffer &rbuffer, condition_var &cv);
};
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class thread_pool::worker
{
    std::thread thread;
    run fun;
    bool event_stop{false};
    bool event_work{true};

public:
    worker(run, settings &, ring_buffer &, condition_var &);
    worker(const worker &) = delete;
    worker(worker &&) = delete;
    worker &operator=(const worker &) = delete;
    worker &operator=(worker &&) = delete;
    void start(settings *, ring_buffer *, condition_var *);
    void stop();
    void join();
    bool is_stop();
    bool is_work();
};
//------------------------------------------------------------------------------

} // namespace app
#endif // __THREAD_POOL_H__
