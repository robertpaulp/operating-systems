// SPDX-License-Identifier: BSD-3-Clause

module race_condition_mutex_coarse;

import core.sync.mutex;

enum NUM_ITER = 10_000_000;

__gshared int var;
shared Mutex lock;

void incrementVar()
{
    // TODO: wrap the whole `for` statement in the critical section and measure
    // the running times.
    lock.lock();
    for (size_t i = 0; i < NUM_ITER; i++)
    {
        var++;
    }
    lock.unlock();
}

void decrementVar()
{
    // TODO: wrap the whole `for` statement in the critical section and measure
    // the running times.
    lock.lock();
    for (size_t i = 0; i < NUM_ITER; i++)
    {
        var--;
    }
    lock.unlock();
}

void main()
{
    import core.thread.threadbase : thread_joinAll;
    import std.concurrency : spawn;
    import std.stdio : writeln;

    lock = new shared Mutex();

    spawn(&incrementVar);
    spawn(&decrementVar);

    thread_joinAll();

    writeln("var = ", var);
}
