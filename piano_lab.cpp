#include <fstream>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#include <cassert>

#include "cv.h"
#include "cpu.h"
#include "mutex.h"
#include "thread.h"

using std::cout;
using std::endl;

/* given helper functions and variables */

enum class Note : uintptr_t { Na = 0, Do = 1, Re = 2, Mi = 3, Fa = 4, So = 5, La = 6, Ti = 7 };
std::vector<std::string> notes_str{"empty", "do", "re", "mi", "fa", "sol", "la", "ti"};

/* Overload extraction operator so that we can read directly into a Note variable */
std::ifstream &operator>>(std::ifstream &stream, Note &note) {
    std::underlying_type_t<Note> temp{};
    stream >> temp;
    note = static_cast<Note>(temp);
    return stream;
}

void play(Note note) {
    assert(note != Note::Na);
    cout << notes_str[static_cast<size_t>(note)] << endl;
}

/* Add global variables and helper functions here */

void conductor(void *arg) {
    std::ifstream infile("input.txt");
    /* implement here */
}

void pianoKey(void *arg) {
    auto note = static_cast<Note>(reinterpret_cast<uintptr_t>(arg));
    /* implement here */
}

void manageThreads(void *arg) {
    for (uintptr_t i = 1; i <= 7; ++i) {
        thread pianoKeyThread(pianoKey, reinterpret_cast<void *>(i));
    }
    thread conductorThread(conductor, nullptr);
}

int main(int argc, char **argv) {
    cpu::boot(manageThreads, nullptr, 0);
}
