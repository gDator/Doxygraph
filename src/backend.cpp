#include "backend.hpp"
#include <typeinfo>

namespace gc
{

void FileReader::fillAllFileData(std::filesystem::path p)
{
    if(std::filesystem::is_directory(p))
    {
        for(auto f : std::filesystem::directory_iterator(p))
        {
            // std::cout << f.path().string() << std::endl;
            if(f.is_directory())
            {
                fillAllFileData(f.path());
            }
            if(f.is_regular_file())
            {
                std::string file_extension = f.path().extension().string();
                for(auto e : extensions)
                {
                    if(file_extension.compare(e) == 0)
                    {
                        file_data.push_back(f.path());
                    }
                }
            }
        }

    }
    else
    {
        if(std::filesystem::is_regular_file(p))
        {
            std::string file_extension = p.extension().string();
            for(auto e : extensions)
            {
                if(file_extension.compare(e) == 0)
                {
                    file_data.push_back(p);
                }
            }
        }
        
    }

}

void FileReader::addFileExtension(std::string e)
{
    extensions.push_back(e);
}

void FileReader::printFileExtensions()
{
    for(auto e : extensions)
        std::cout << e << std::endl;
}

void FileReader::printAllFileData()
{
    for(auto f : file_data)
    {
        std::cout << f.string();
        std::cout << std::endl;
    }
}

std::vector<std::filesystem::path>*  FileReader::getData()
{
    return &file_data;
}

std::vector<std::string> FileReader::getFileExtensions(std::string e)
{
    return extensions;
}

void FileReader::reset()
{
    file_data.clear();
}

void CppInterpreter::transmit_Code(std::vector<std::filesystem::path>*  file_data)
{
    cleanUpDoxygen();
    p_file_data = file_data;
    map->throwMessage("Successfully transmitted Data");
    for(auto& p : *p_file_data)
        map->throwMessage("File added: " + p.string());
    std::thread t1(&CppInterpreter::run, this);
    if(t1.joinable())
    {
        t1.detach();
    }
}

void CppInterpreter::run()
{
    //include standard verification
    char** argv;
    //see main function doxygen
    
    initDoxygen(); 
    checkConfiguration();
    adjustConfiguration();
    std::filesystem::create_directory(std::filesystem::current_path().string() + "/tmp");
    std::filesystem::create_directory(std::filesystem::current_path().string() + "/tmp/dxy");
    Config_updateString(OUTPUT_DIRECTORY, "./tmp/dxy");
  // disable html output
    Config_updateBool(GENERATE_HTML,true);
    Config_updateBool(GENERATE_LATEX,false);
    // be quiet
    Config_updateBool(QUIET,false);
    // turn off warnings
    Config_updateBool(WARNINGS,true);
    Config_updateBool(WARN_IF_UNDOCUMENTED,false);
    Config_updateBool(WARN_IF_DOC_ERROR,false);
    // Extract as much as possible
    Config_updateBool(EXTRACT_ALL,true);
    Config_updateBool(EXTRACT_STATIC,true);
    Config_updateBool(EXTRACT_PRIVATE,true);
    Config_updateBool(EXTRACT_PRIV_VIRTUAL,true);
    Config_updateBool(EXTRACT_LOCAL_METHODS,true);
    Config_updateBool(EXTRACT_LOCAL_METHODS,true);
    Config_updateBool(HAVE_DOT, true);
    Config_updateBool(CALLER_GRAPH,true);

    //Used for creating References
    Config_updateBool(REFERENCED_BY_RELATION, true);
    Config_updateBool(REFERENCES_RELATION, true);
    Config_updateBool(REFERENCES_LINK_SOURCE, true);
    // Extract source browse information, needed
    // to make doxygen gather the cross reference info
    Config_updateBool(SOURCE_BROWSER,true);
    // Config_updateList(FILE_PATTERNS, list);
    // In case of a directory take all files on directory and its subdirectories
    Config_updateBool(RECURSIVE,true);
    std::vector<std::string> input_list;
    for(auto f : *p_file_data)
    {
        input_list.push_back(f.string());
    }
    Config_updateList(INPUT,input_list);
    
    
    parseInput();
    // generateOutput();
   


    cmsg("Making Tree structue...");
    double progress = 0;
    double max = Doxygen::namespaceLinkedMap->size() + Doxygen::classLinkedMap->size() + Doxygen::memberNameLinkedMap->size() + Doxygen::functionNameLinkedMap->size();
    for(auto i = Doxygen::namespaceLinkedMap->begin(); i < Doxygen::namespaceLinkedMap->end(); i++)
    {
        addNamespace((*i).get());
        map->setProgress(progress/max);
        progress++;
    }

    //everthing else which is in no namespace!
    for(auto i = Doxygen::classLinkedMap->begin(); i < Doxygen::classLinkedMap->end(); i++)
    {
        addClass((*i).get());
        map->setProgress(progress/max);
        progress++;
    }

    //everthing else which is in no namespace or class
    for(auto& i: *Doxygen::memberNameLinkedMap)
    {
        for(auto& mem : *i)
        {
            addMember(mem.get());
            map->setProgress(progress/max);
            progress++;
        }
    }

    //everthing else which is in no namespace or class
    for(auto& i : *Doxygen::functionNameLinkedMap)
    {
        for(auto& mem : *i)
        {
            addMember(mem.get());
            map->setProgress(progress/max);
            progress++;
        }
    }
    
    // for(auto& def : *Doxygen::symbolMap)
    // {
    //     for(auto& d : done)
    //     {
    //         if(d == def.second)
    //         {
    //             std::cout << "Already exists" << std::endl;
    //         }
    //         continue;
    //     }
    //     switch(def.second->definitionType())
    //     {
    //         done.push_back(def.second);
    //         // case Definition::DefType::TypeClass: addClass((ClassDef*)def.second); break;
    //         case Definition::DefType::TypeNamespace: addNamespace((NamespaceDef*)def.second); break;
    //         // case Definition::DefType::TypeMember: addMember((MemberDef*)def.second); break;
    //         // case Definition::DefType::TypeGroup: addMemberGroup((MemberGroup*)def.second); break;
    //     }
    //     map->setProgress(progress/max);
    //     progress++;
    // }
    // }
    cmsg("Code map has " + std::to_string(p_doxygen_to_code_tree.size()) + " Nodes");
    map->setProgress(1); 
    
    
    // //TODO: go throuh all members and check for refernces
    //NOTE: only a test
    // for(auto& [doxy_node, node_from] : p_doxygen_to_code_tree)
    // {
    //     int to = rand() % p_doxygen_to_code_tree.size();
    //     std::cout << "add Reference to: "<<to<< std::endl;
    //     node_from->addReference(std::get<1>(p_doxygen_to_code_tree[to]), ReferenceType::CALL);
    //     amount_references++;
    // }
    cmsg("Finding References...");

    for (const auto &fn : *Doxygen::inputNameLinkedMap)
    {
        for (const auto &fd : *fn)
        {
            StringVector filesInSameTu;
            fd->getAllIncludeFilesRecursively(filesInSameTu);
            fd->parseSource(nullptr);
        }
    }

    cmsg("Finding Inheritence");
    int amount_inheritance = 0;
    for(auto i = Doxygen::classLinkedMap->begin(); i < Doxygen::classLinkedMap->end(); i++)
    {
        //for each class
        int index = alreadyDone((*i).get());
        if(index == -1 || index >= p_doxygen_to_code_tree.size())
        {
            std::cout << "No such Node exists" << std::endl;
            break;
        }
        Node* node_from = std::get<1>(p_doxygen_to_code_tree[index]);

        //add all referneces to subclasses
        for(auto& sub_class : (*i).get()->subClasses())
        {
            index = alreadyDone(sub_class.classDef);
            if(index == -1 || index >= p_doxygen_to_code_tree.size())
            {
                std::cout << "No such Node exists" << std::endl;
                continue;
            }
            Node* node_to = std::get<1>(p_doxygen_to_code_tree[index]);            
            if(node_to == nullptr)
                continue;
            node_from->addReference(node_to, ReferenceType::INHERITANCE);
            amount_inheritance++;
        }
        // map->setProgress(progress/max);
        // progress++;
    }
    cmsg("Code map has " + std::to_string(amount_inheritance) + " Inheritances");


    cmsg("Generating References...");
    int amount_call_references = 0;
    int amount_type_references = 0; 
    for(auto& [doxy_node, node_from] : p_doxygen_to_code_tree)
    {
        //for each node
        auto references = doxy_node->getReferencesMembers();
        // std::cout << references.size() << " References from Node : " << node_from->getName() << std::endl;
        for(auto& ref : references)
        {
            int index = alreadyDone(ref);
            if(index == -1 || index >= p_doxygen_to_code_tree.size())
                continue;

            Node* node_to = std::get<1>(p_doxygen_to_code_tree[index]);            
            if(node_to == nullptr)
                continue;
            if(node_from->getType() == NodeType::FUNCTION)
            {
                node_from->addReference(node_to, ReferenceType::CALL);
                amount_call_references++;
            }
            else
            {
                node_from->addReference(node_to, ReferenceType::DATATYPE);
                amount_type_references++;
            }
        }
    }

    for(auto& i: *Doxygen::memberNameLinkedMap)
    {
        for(auto& mem : *i)
        {
            int index = alreadyDone(mem.get());
            if(index == -1 || index >= p_doxygen_to_code_tree.size())
            {
                std::cout << "No such Node exists" << std::endl;
                continue;
            }
            Node* node_from = std::get<1>(p_doxygen_to_code_tree[index]);
            if(mem->category() == nullptr)
                continue;
            index = alreadyDone(mem->category());
            if(index == -1 || index >= p_doxygen_to_code_tree.size())
            {
                std::cout << "No such Node exists" << std::endl;
                continue;
            }
            Node* node_to = std::get<1>(p_doxygen_to_code_tree[index]);
            node_from->addReference(node_to, ReferenceType::DATATYPE);
            amount_type_references++;
        }
    }


    cmsg("Code map has " + std::to_string(amount_call_references) + " Call References");
    cmsg("Code map has " + std::to_string(amount_type_references) + " Type References");
    cmsg("Drawing Tree...");
    map->sortAndUpdate();
    cmsg("Finished...");
    // clean up after us
    Dir().rmdir("/tmp");
    // cleanUpDoxygen();

}

void CppInterpreter::setCompilerArguments(std::string s)
{
    compiler_args += s;
}

std::string CppInterpreter::getCompilerArguments()
{
    return compiler_args;
}

void CppInterpreter::clearCompilerArguments()
{
    compiler_args = "";
}

void  CppInterpreter::addClass(const ClassDef* c)
{
    bool exist = alreadyDone(c) == -1 ? false : true;
    if(exist)
    {
        // std::cout << "Already exists: " << c->localName() << std::endl;
        return;
    }
    
    if(!exist)
    {
        NodeType t; 
        switch(c->compoundType())
        {
            case ClassDef::CompoundType::Struct: t = NodeType::STRUCT; break;
            case ClassDef::CompoundType::Class: t = NodeType::CLASS; break;
            case ClassDef::CompoundType::Union: t = NodeType::UNION; break;
            default: t = NodeType::CLASS; break;
        }
        map->addNode(c->displayName(FALSE).str(), t,true);
        map->getCurrentNode()->addInformation("File", c->getDefFileName().str());
        map->getCurrentNode()->addInformation("Line", std::to_string(c->getDefLine()));
        map->getCurrentNode()->addInformation("Doc", c->documentation().str());
        // p_doxygen_to_code_tree.push_back(std::make_tuple(c, map->getLastAddedNode()));
    }
    
    for(auto& nestedc : c->getClasses())
    {
        addClass(nestedc);
    }
    for(auto& memgrp : c->getMemberGroups())
    {
        addMemberGroup(memgrp.get());
    }
    for(auto& mem : c->getMemberLists())
    {
        addMemberList(mem.get());
    }

    if(!exist)
        map->goUp();
}

void CppInterpreter::addNamespace(const NamespaceDef* n)
{
    bool exist = alreadyDone(n) == -1 ? false : true;
    if(exist)
    {
        // std::cout << "Already exists: " << n->localName() << std::endl;
        return;
    }
    
    if(!exist)
    {
        map->addNode(n->localName().str(), NodeType::NAMESPACE, true);
        map->getCurrentNode()->addInformation("File", n->getDefFileName().str());
        map->getCurrentNode()->addInformation("Line", std::to_string(n->getDefLine()));
        map->getCurrentNode()->addInformation("Doc", n->documentation().str());
        p_doxygen_to_code_tree.push_back(std::make_tuple(n, map->getLastAddedNode()));
    }

    
    for(auto& nsp : n->getNamespaces())
    {
        addNamespace(nsp);
    }
    for(auto& cl : n->getClasses())
    {
        addClass(cl);
    }
    for(auto& mem : n->getMemberGroups())
    {
        addMemberGroup(mem.get());
    }
    for(auto& intf : n->getInterfaces())
    {
        
    }
    for(auto& cm : n->getExceptions())
    {
        //nothing
    }
    for(auto& cm : n->getConcepts())
    {
        // nothing
    }
    for(auto& s : n->getStructs())
    {
        addClass(s);
    }
    for(auto& mem : n->getMemberLists())
    {
        addMemberList(mem.get());
    }

    if(!exist)
        map->goUp();
    
}

void CppInterpreter::addMember(const MemberDef* m)
{
    bool exist = alreadyDone(m) == -1 ? false : true;
    if(exist)
    {
        // std::cout << "Already exists: " << m->localName() << std::endl;
        return;
    }
        
    //look at doxygen/src/memberdef.h:107
    // int count = 0;
    // for(auto& member: m->getMemberLists())
    //     count += member->size()      
        
    if(!exist)
    {
        NodeType t = NodeType::UNKNOWN;
        if(m->isVariable())
            t = NodeType::VARIABLE;
        if(m->isRelated())
            t = NodeType::POINTER;
        if(m->isProperty())
            t = NodeType::VARIABLE;
        if(m->isEnumerate())
            t = NodeType::ENUM;
        if(m->isTypedef())
            t = NodeType::TYPEDEF;
        if(m->isFunction())
        {
            t = NodeType::FUNCTION;
        }
        map->addNode(m->localName().str(), t, true);
        map->getCurrentNode()->addInformation("Type", m->memberTypeName().str());
        map->getCurrentNode()->addInformation("File", m->getDefFileName().str());
        map->getCurrentNode()->addInformation("Line", std::to_string(m->getDefLine()));
        map->getCurrentNode()->addInformation("Doc", m->documentation().str());
        map->getCurrentNode()->addInformation("Datatype", m->getDeclType().str());
        if(t == NodeType::FUNCTION)
        map->getCurrentNode()->addInformation("Args", m->argsString().str());
        p_doxygen_to_code_tree.push_back(std::make_tuple(m, map->getLastAddedNode()));
        map->goUp();
    }    
}

void CppInterpreter::addMemberList(const MemberList* m)
{
    for(auto i = m->begin(); i < m->end(); i++)
        addMember(*i);
}
void CppInterpreter::addMemberGroup(const MemberGroup* m)
{
    addMemberList(&m->members());
}

int CppInterpreter::alreadyDone(const Definition* d)
{
    int index = 0;
    for(auto& [def, node] : p_doxygen_to_code_tree)
    {
        if(def == d)
        {
            return index;
        }
        index++;
    }
    return -1;
}

}


