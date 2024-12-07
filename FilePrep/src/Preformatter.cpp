#include "Preformatter.hpp"
#include <set>

void Preformatter::preformatFile(std::string fileName)
{
    text.reserve(3e3);
    reader.openFile(fileName.c_str()); //lokalizacja plikow zawsze wzgledem builda!!

    addr = reader.get();
    text = addr;

    std::cout<<"Preformatting file: "<<fileName<<"\n";
    clearWhitespaces();
    clearInterpunction();
    clearCase();

    reader.put(text.c_str(), text.length());
    reader.closeFile();
}

void Preformatter::clearWhitespaces()
{
    std::set<char> whiteSpaceCharacters{
        char(9), char(10), char(11), char(12), char(13), char(32), char(133), char(160), char(5760),
        char(8192), char(8193), char(8194), char(8195), char(8196), char(8197), char(8198), char(8199), 
        char(8200), char(8201), char(8202), char(8232), char(8233), char(8239), char(8287), char(12288)};

    std::erase_if(text,
    [&whiteSpaceCharacters](char ch)
    {
        return whiteSpaceCharacters.contains(ch);
    });
}

void Preformatter::clearInterpunction()
{
     std::set<char> interpunctionCharacters{
        '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
        ':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_', '`', '{', '|', '}', '~'};

    std::erase_if(text,
    [&interpunctionCharacters](char ch)
    {
        return interpunctionCharacters.contains(ch);
    });
}

void Preformatter::clearCase()
{
    std::transform(text.begin(), text.end(), text.begin(),
    [](char ch)
    {
        return std::tolower(ch);
    });
}