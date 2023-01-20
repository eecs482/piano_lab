#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "thread.h"
#include <cassert>
#include <map>

using std::cout;
using std::endl;

/* given helper functions and variables */
enum Note { na = 0, d0 = 1, re = 2, mi = 3, fa = 4, sol = 5, la = 6, ti = 7 };
std::vector<std::string> notes_str = {"empty", "do", "re", "mi", "fa", "sol", "la", "ti"};

void play(Note i) {
    assert(i != na);
    cout << notes_str[i] << endl;
}

/* Add global variables and helper functions here */

void conductor(void *arg) {
    std::ifstream infile("input.txt");
    /* implement here */
}

void pianoKey(void *note) {
    Note i = static_cast<Note>((reinterpret_cast<intptr_t>(note)));
    /* implement here */
}

void manageThreads(void *arg) {
    for (intptr_t i = 1; i <= 7; ++i) {
        thread pianoKeyThread(reinterpret_cast<thread_startfunc_t>(pianoKey), reinterpret_cast<void *>(i));
    }
    thread conductorThread(reinterpret_cast<thread_startfunc_t>(conductor), nullptr);
}

int main(int argc, char **argv) {
    cpu::boot(reinterpret_cast<thread_startfunc_t>(manageThreads), nullptr, 0);
}
