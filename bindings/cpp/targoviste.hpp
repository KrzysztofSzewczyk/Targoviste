
/*
MIT License

Copyright (c) 2018 Krzysztof Szewczyk

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

CPP bindings have no documentation. Please check original version.
Under construction.
*/

#ifndef __TARGOVISTE_HPP_
#define __TARGOVISTE_HPP_

namespace targoviste {
    class file {
        private:
            __targoviste_inner::targoviste_file innerFile; //Original header should be included, but functions hidden.
        public:
            file();
            char * getContents();
            void setContents(char * c); //Possibly use std::string?
            char * getName();
            void setName(char * c);
            long getSize();
            int fromArchive(char * filename);
            file * list(char * path, int * error, int * amount);
    }
    
    class archive {
        private:
            __targoviste_inner::targoviste_archive innerArchive; //Original header should be included, but functions hidden.
        public:
            archive();
            archive(char * filename); //For file reading
            ~archive();
            int getAmount();
            int writeArchive(char * file);
            void add(targoviste_file src);
    }
}

namespace __targoviste_inner {
    #include <targoviste.h>
}

#endif
