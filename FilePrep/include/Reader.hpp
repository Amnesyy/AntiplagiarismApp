#include <string>
#include <iostream>

// for mmap:
class Reader
{
public:
    void openFile(const char*);
    char* get();
    void put(const char*, size_t size);
    void closeFile();
private:
    char* addr;
    size_t content_length{10};
    int fileDescriptor;
};