#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "cpu.h"
#include "thread.h"
#include "mutex.h"
#include "cv.h"
#include <cassert>
#include <map>

using std::cout;
using std::endl;

/* given helper functions and variables */
enum class Note : uintptr_t { Na = 0, Do = 1, Re = 2, Mi = 3, Fa = 4, So = 5, La = 6, Ti = 7 };
std::vector<std::string> notes_str = {"empty", "do", "re", "mi", "fa", "sol", "la", "ti"};

/* Overload extraction operator so that we can read directly into a Note variable */
std::ifstream &operator>>(std::ifstream &stream, Note &note) {
    int tmpNote;
    stream >> tmpNote;
    note = static_cast<Note>(tmpNote);
    return stream;
}

void play(Note i) {
    assert(i != Note::Na);
    cout << notes_str[static_cast<uintptr_t>(i)] << endl;
}

/* Add global variables and helper functions here */
Note currentNote = Note::Na;
mutex noteMutex;
cv pianoCv;
cv conductorCv;
std::map<Note, cv> pianoCVs;

void conductor(void *arg) {
    std::ifstream infile("input.txt");
    Note nextNote;
    while(infile >> nextNote){
        noteMutex.lock();

        while(currentNote != Note::Na){
            conductorCv.wait(noteMutex);
        }

        currentNote = nextNote;
        pianoCVs[nextNote].signal();
        noteMutex.unlock();
    }
}

void pianoKey(void *note) {
    auto* p = static_cast<uintptr_t*>(note);
    auto i = static_cast<Note>(*p);
    delete p;

    noteMutex.lock();
    while (true) {
        while(currentNote != i){
            pianoCVs[i].wait(noteMutex);
        }

        play(i);
        currentNote = Note::Na;
        conductorCv.signal();
    }
    noteMutex.unlock();
}

void manageThreads(void *arg) {
    for (uintptr_t i = 1; i <= 7; ++i) {
        thread pianoKeyThread(pianoKey, new uintptr_t {i});
    }
    thread conductorThread(conductor, nullptr);
}

int main(int argc, char **argv) {
    cpu::boot(manageThreads, nullptr, 0);
}
