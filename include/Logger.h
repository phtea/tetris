#pragma once
#include <chrono>
#include <csignal>
#include <cstdio>
#include <ctime>
#include <iomanip>
#include <sstream>

inline std::string current_timestamp() {
    using namespace std::chrono;

    auto now = system_clock::now();
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    std::time_t t = system_clock::to_time_t(now);
    std::tm *tm = std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setw(3) << std::setfill('0') << ms.count();

    return oss.str();
}

#if DEBUG == 1
#define LOG(s, ...)                                                                                \
    {                                                                                              \
        fprintf(stdout, "[%s] [LOG] %s at %s:%d: ", current_timestamp().c_str(), __FILE_NAME__,    \
                __func__, __LINE__);                                                               \
        fprintf(stdout, s, ##__VA_ARGS__);                                                         \
        fprintf(stdout, "\n");                                                                     \
    }
#define FATAL(s, ...)                                                                              \
    {                                                                                              \
        fprintf(stdout, "[%s] [FATAL] %s at %s:%d: ", current_timestamp().c_str(), __FILE_NAME__,  \
                __func__, __LINE__);                                                               \
        fprintf(stdout, s, ##__VA_ARGS__);                                                         \
        fprintf(stdout, "\n");                                                                     \
        std::raise(SIGINT);                                                                        \
    }
#else
#define LOG(s, ...)                                                                                \
    {}
#define FATAL(s, ...)                                                                              \
    {}
#endif // DEBUG
