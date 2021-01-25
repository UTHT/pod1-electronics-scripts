#include <ExampleLibrary.h>

// Example library code

Example::Example(int* address, int defaultvalue) : Example((int*)malloc(sizeof(int)), 0){
    this->address = address;
    *address = defaultvalue;
}

int Example::read(){
    return *(this->address);
}

void Example::write(int value){
    *(this->address) = value;
}