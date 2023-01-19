#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <map>
#include <cassert>
#include "thread.h"

using std::cout;
using std::endl;

/* given helper functions and variables */
enum Note { na = 0, d0 = 1, re = 2, mi = 3, fa = 4, sol = 5, la = 6, ti = 7};
std::vector<std::string> notes_str = {"empty", "do", "re", "mi", "fa", "sol", "la", "ti"};

void play(Note i){
    assert(i != na);
    cout << notes_str[i] << endl;
}

/* Add global variables and helper functions here */
Note currentNote = na;
mutex noteMutex;
cv pianoCV;

/* Overload extraction operator so that we can read directly into a Note variable */
std::ifstream& operator>>(std::ifstream& stream, Note& note){
    int tmpNote;
    stream >> tmpNote;
    note = static_cast<Note>(tmpNote);
    return stream;
}

void conductor(void* arg){
    std::ifstream infile("input.txt");
    Note nextNote;
    while(infile >> nextNote){
        noteMutex.lock();

        while(currentNote != na){
            pianoCV.wait(noteMutex);
        }

        currentNote = nextNote;
        pianoCV.broadcast();
        noteMutex.unlock();
    }
}

void pianoKey(void* note){
    Note i = static_cast<Note>((reinterpret_cast<intptr_t>(note)));

    noteMutex.lock();
    while (true) {
        while(currentNote != i){
            pianoCV.wait(noteMutex);
        }

        play(i);
        currentNote = na;
        pianoCV.broadcast();
    }
    noteMutex.unlock();
}

void manageThreads(void* arg){
    for(intptr_t i = 1; i<=7; ++i){
        thread pianoKeyThread(reinterpret_cast<thread_startfunc_t>(pianoKey), reinterpret_cast<void*>(i));
    }
    thread conductorThread(reinterpret_cast<thread_startfunc_t>(conductor), nullptr);
}

int main(int argc, char **argv)
{
    cpu::boot(reinterpret_cast<thread_startfunc_t>(manageThreads), nullptr, 0);
}
