#include <string>
#include <fstream>
#include <iostream>

#ifndef LOGGER_H
#define LOGGER_H

class Logger {
    public:
        Logger() {}
        Logger(std::string filename) {
            open(filename); 
        }

        bool open(std::string filename) { 
            this->filename = filename; 
            this->file.open(this->filename.c_str()); 
            return file.good(); 
        }

        void close() { 
            file.close(); 
        }

        void write(std::string logs) { 
            this->file << logs << std::endl; 
            this->file.flush();
        }

    private:
        std::ofstream  file;
        std::string    filename;
};

static Logger logger;

#endif // LOGGER_H