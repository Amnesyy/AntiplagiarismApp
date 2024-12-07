#include "Reader.hpp"
#include <stdexcept>
#include <cstring>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static void handle_error(const char* msg) {
    perror(msg); 
    exit(EXIT_FAILURE);
}

void Reader::openFile(const char* name)
{
    std::cout<<"Opening file: " << name <<"\n";
    fileDescriptor = open(name, O_RDWR | O_CREAT);
    if (fileDescriptor == -1)
        handle_error("open");

    struct stat sb;
    if (fstat(fileDescriptor, &sb) == -1)
        handle_error("fstat");

    content_length = sb.st_size;
    if(!content_length)
    {
        content_length = 9840580;
        if(posix_fallocate(fileDescriptor, 0 , content_length) == -1)
            handle_error("fallocate");
    }
    addr = static_cast<char*>(mmap(NULL, content_length, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0u));
    if (addr == MAP_FAILED)
        handle_error("mmap");

}

char* Reader::get()
{
    return addr;
}

void Reader::put(const char* text, size_t size)
{
    if (ftruncate(fileDescriptor, size) == -1) 
        handle_error("truncate");

    addr = static_cast<char*>(mremap(addr, content_length, size, MREMAP_MAYMOVE));
    if(addr == MAP_FAILED)
        handle_error("mremap");

    memcpy(addr, text, size);

    if(msync(addr, size, MS_SYNC) == -1)
        handle_error("msync");

    content_length = size;
}

void Reader::closeFile()
{
    std::cout<<"Closing file \n";
    if(munmap(addr, content_length) == -1)
        handle_error("munmap");
    close(fileDescriptor);
}