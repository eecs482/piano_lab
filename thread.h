/*
 * thread.h -- interface to the thread library
 *
 * This file should be included by the thread library and by application
 * programs that use the thread library.
 */
#pragma once

static constexpr unsigned int STACK_SIZE=262144;// size of each thread's stack

using thread_startfunc_t = void (*)(void*);

class thread {
public:
    thread(thread_startfunc_t func, void* arg); // create a new thread
    ~thread();

    void join();                                // wait for this thread to finish

    /*
     * Disable the copy constructor and copy assignment operator.
     */
    thread(const thread&) = delete;
    thread& operator=(const thread&) = delete;

    /*
     * Move constructor and move assignment operator.
     */
    thread(thread&&);
    thread& operator=(thread&&);
};
