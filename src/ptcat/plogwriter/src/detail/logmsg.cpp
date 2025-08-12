//
// Created by 27852 on 2025/7/10.
//

#include "detail/logmsg.h"
/*
 *提示：
*msg.filename = __FILE__;
    msg.line = __LINE__;
    msg.funcname = __func__;
 */

namespace ptcat {
    namespace plog {
        LogMsg::LogMsg(std::string msg) : msg_{msg} {}

        LogMsg::LogMsg(std::chrono::system_clock::time_point timestamp, std::string msg) : timestamp_{timestamp},  msg_{msg} {}

        LogMsg::LogMsg(LogLevel log_level, std::chrono::system_clock::time_point timestamp, std::string msg) : log_level_{log_level},
                                                                                                                    timestamp_{timestamp},
                                                                                                                        msg_{msg} {}
        LogMsg::LogMsg(std::string name, LogLevel log_level, std::chrono::system_clock::time_point timestamp, std::string msg) : name_{name},
                                                                                                                                    log_level_{log_level},
                                                                                                                                        timestamp_{timestamp},
                                                                                                                                            msg_{msg} {}
        LogMsg::LogMsg(SourceInfo source_info, std::string name, LogLevel log_level, std::chrono::system_clock::time_point timestamp, std::string msg)  : source_info_{source_info},
                                                                                                                                                                name_{name},
                                                                                                                                                                    log_level_{log_level},
                                                                                                                                                                        timestamp_{timestamp},
                                                                                                                                                                            msg_{msg} {}
    }
}