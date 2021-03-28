#ifndef ExampleLibrary_H
#define ExampleLibrary_H

#include <stdlib.h>

// Example library header

class Example {
    public:
        Example(int* address, int defaultvalue)
    private:
        float read();
        void write(int value);
        int* address;
}

#endif