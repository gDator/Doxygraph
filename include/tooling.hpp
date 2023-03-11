#ifndef TOOLING_HPP
#define TOOLING_HPP

#include <sstream>
#include <string>
#include <vector>


static std::vector<std::string> stringToVector(std::string n, std::string delimiter)
{
    std::vector<std::string> result;
    // std::stringstream stringStream(n);
    // std::string line;
    // while(std::getline(stringStream, line)) 
    // {
        std::size_t prev = 0, pos;
        while ((pos = n.find_first_of(delimiter, prev)) < n.length())
        {
            if (pos > prev)
                result.push_back(n.substr(prev, pos-prev));
            prev = pos+1;
        }
        if (prev < n.length())
            result.push_back(n.substr(prev, std::string::npos));
    // }
    return result;
}
#endif /*TOOLING_HPP*/