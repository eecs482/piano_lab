/*
 * mutex.h -- interface to the mutex class
 */

#pragma once

class mutex {
public:
    mutex();
    ~mutex();

    void lock();
    void unlock();

    /*
     * Disable the copy constructor and copy assignment operator.
     */
    mutex(const mutex&) = delete;
    mutex& operator=(const mutex&) = delete;

    /*
     * Move constructor and move assignment operator.
     */
    mutex(mutex&&);
    mutex& operator=(mutex&&);
};
