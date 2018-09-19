
#pragma once

#include <fstream>
#include <string>
#include <stdexcept>
#include <iostream>


namespace shader{


class ShaderReader
{
public:

    char * read(const char * file_name)
    {
        std::ifstream ifs(file_name);

        if (!ifs)
            throw std::runtime_error("Failed to open file!");

        // get length of file:
        ifs.seekg(0, ifs.end);
        int length = ifs.tellg();
        ifs.seekg(0, ifs.beg);

        char * buffer = new char[length + 1];

        // read data as a block:
        ifs.read (buffer, length);

        //if(!ifs)
        //    throw std::runtime_error("File was not completely read!");

        ifs.close();

        buffer[length] = '\0';

        return buffer;
    }

};


}//namespace shader
