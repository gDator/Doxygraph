#include "program.hpp"
#include "program_state_test.hpp"
#include <iostream>
// void initLog();
// using namespace boost;
// std::ostream& operator<< (std::ostream& strm, log::trivial::severity_level level)
// {
//     static const char* strings[] =
//     {
//         "normal",
//         "notification",
//         "warning",
//         "error",
//         "critical"
//     };

//     if (static_cast< std::size_t >(level) < sizeof(strings) / sizeof(*strings))
//         strm << strings[level];
//     else
//         strm << static_cast< int >(level);

//     return strm;
// }

int main()
{
    // initLog();  
    
    //DEB(lg, trace) << "A trace severity message";
    //DEB(lg, debug) << "A debug severity message";
    //DEB(lg, info) << "An informational severity message";
    //DEB(lg, warning) << "A warning severity message";
    //DEB(lg, error) << "An error severity message";
    //DEB(lg, fatal) << "A fatal severity message";
    xgl::program::Program program;
    program.init();

    program.pushState(new xgl::program::ProgramStateTest(&program));
    program.loop();
    
    
    return 0;
}




/*
    @name: initLog
    @func: initialize the BOOST-Logger
    @args: none
    @return: none
*/
// void initLog()
// {  
//         // Add attributes
//     log::add_common_attributes();

//     log::add_file_log
//     (
//         log::keywords::file_name = "bug_report%N.log",                                        /*< file name pattern >*/
//         log::keywords::rotation_size = 10 * 1024 * 1024,                                   /*< rotate files every 10 MiB... >*/
//         log::keywords::time_based_rotation = log::sinks::file::rotation_at_time_point(0, 0, 0), /*< ...or at midnight >*/
//         /*< log record format >*/
//         log::keywords::format =
//         (
//              log::expressions::stream
//                 << log::expressions::attr< unsigned int >("LineID")
//                 << log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", " [%Y-%m-%d %H:%M:%S]")
//                 << " <" << log::trivial::severity
//                 << ">: " << log::expressions::smessage
//         )                               
//     );

//     log::core::get()->set_filter
//     (
//         log::trivial::severity >= log::trivial::trace
//     );
// }
