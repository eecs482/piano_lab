#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <map>
#include <cassert>
#include "disk.h"
#include "thread.h"

using std::cout;
using std::endl;

// helpers
enum Note { na = 0, d0 = 1, re = 2, mi = 3, fa = 4, sol = 5, la = 6, ti = 7};
std::string notes_str[] = {"empty", "do", "re", "mi", "fa", "sol", "la", "ti"};

void play(Note i){
    assert(i != na);
    cout << notes_str[i] << endl;
}


/* Add global variables and helper functions here */
Note currentNote = na;
mutex noteMutex;
cv pianoCv;
cv conductorCv;

void conductor(){
    std::ifstream infile("input.txt");
    int tmpNote;
    while(infile >> tmpNote){
        noteMutex.lock();

        while(currentNote != na){
            conductorCv.wait(noteMutex);
        }

        currentNote = (Note)tmpNote;
        pianoCv.broadcast();
        noteMutex.unlock();
    }
}

void pianoKey(Note i){
    noteMutex.lock();

    while(currentNote != i){
        pianoCv.wait(noteMutex);
    }

    play(i);
    currentNote = na;
    conductorCv.signal();

    noteMutex.unlock();
}

/* more given helper functions for thread setup*/
void pianoKeyWrapper(void* note){
    Note i = (Note)(intptr_t) note;
    while(true){
        pianoKey(i);
    }
}

void manageThreads(){
    for(intptr_t i = 1; i<=7; ++i){
        thread((thread_startfunc_t)pianoKeyWrapper, (void*) i);
    }
    thread((thread_startfunc_t)conductor, (void*) 0);
}

int main(int argc, char **argv)
{
    cpu::boot((thread_startfunc_t) manageThreads, (void *) 0, 0);
}