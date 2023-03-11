/******************************************************************************
*
* XG Graphical Compile Software
*
* License:      MIT
* Note:         Backend of the graphical Structure;
* Author:       Daniel Hagen infos.g.infos AT gmail DOT com
* Date:         17/03/2022
* History:      17/03/2022: v0.1:    - complete CodeMap works
* Structure:    [SECTION] Class Pre-Declaration
                [SECTION] class enums/enums
                [SECTION] Static functions
                [SECTION] Structs
                [SECTION] Classes
* Description:  Code Tree remains of Nodes. Nodes represent Code snaps. Therefore 
                they can be of different kinds like classes, functions etc. A Node has
                also References, which connect the Nodes. The Drawstyle is a struct to define
                how the Codemap appears. The FileReader reading all source code from
                a defined Folder (u pick a folder throuhg FilePicker.h) and transmits
                it to the Parser.
* TODO:         - Path finding of References
                - nicer look of Codemap
                - moving Codeblocks in adminmode
                - simulating 
******************************************************************************/

#ifndef BACKEND
#define BACKEND

#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <iostream>
#include <functional>
#include <filesystem>
#include <deque>
#include <tuple>
#include <thread>
#include <mutex>

//#include <clang-c/Index.h>
#include "program.hpp"

//include for doxyapp
#include "dir.h"
#include "doxygen.h"
#include "outputgen.h"
#include "parserintf.h"
#include "classdef.h"
#include "namespacedef.h"
#include "membername.h"
#include "filedef.h"
#include "util.h"
#include "classlist.h"
#include "config.h"
#include "filename.h"
#include "version.h"
#include "tree.hpp"


namespace gc
{
    
    class FileReader
    {
        private:
            std::filesystem::path path;
            std::vector<std::string> extensions;
            std::vector<std::filesystem::path> file_data;
        public:
            void fillAllFileData(std::filesystem::path p);
            void addFileExtension(std::string e);
            std::vector<std::string> getFileExtensions(std::string e);
            void printFileExtensions();
            void printAllFileData();
            std::vector<std::filesystem::path>*  getData();
            void reset();
            FileReader()
            {
                extensions.push_back(".cpp");
                extensions.push_back(".hpp");
                extensions.push_back(".h");
                extensions.push_back(".c");
                extensions.push_back(".cxx");
                extensions.push_back(".cc");
            }
    };

    class CppInterpreter
    {
        private:
            std::vector<std::filesystem::path>*  p_file_data;
            inline static CodeMap* map = nullptr;
            inline static std::string compiler_args = "";
            std::vector<std::tuple<const Definition*, Node*>> p_doxygen_to_code_tree;

            void addClass(const ClassDef* c);
            void addNamespace(const NamespaceDef* n);
            void addMember(const MemberDef* m);
            void addMemberList(const MemberList* m);
            void addMemberGroup(const MemberGroup* m);
            int alreadyDone(const Definition* d);

        public:
            void transmit_Code(std::vector<std::filesystem::path>*  file_data);
            void run();
            void setCompilerArguments(std::string s);
            std::string getCompilerArguments();
            void clearCompilerArguments();
            

        CppInterpreter(CodeMap* c)
        {
            map = c;
        }
    };
}
#endif /*BACKEND*/