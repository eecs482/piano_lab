/*
 * cv.h -- interface to the CV class
 */

#pragma once

#include "mutex.h"

class cv {
public:
    cv();
    ~cv();

    void wait(mutex&);                  // wait on this condition variable
    void signal();                      // wake up one thread on this condition
                                        // variable
    void broadcast();                   // wake up all threads on this condition
                                        // variable

    /*
     * Disable the copy constructor and copy assignment operator.
     */
    cv(const cv&) = delete;
    cv& operator=(const cv&) = delete;

    /*
     * Move constructor and move assignment operator.
     */
    cv(cv&&);
    cv& operator=(cv&&);
};
