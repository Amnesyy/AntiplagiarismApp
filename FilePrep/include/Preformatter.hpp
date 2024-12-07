#pragma once
#ifndef READER_H
#define READER_H
#include "Reader.hpp"
#endif

class Preformatter
{
public:
    void preformatFile(std::string);
private:
    void clearWhitespaces();
    void clearInterpunction();
    void clearCase();

    char* addr;
    Reader reader;
    std::string text;
};