#pragma once
#include <csignal>

#if DEBUG == 1
#define LOG(s, ...)                                                                                \
    {                                                                                              \
        fprintf(stdout, "[LOG] %s at %s:%d: ", __FILE_NAME__, __func__, __LINE__);                 \
        fprintf(stdout, s, ##__VA_ARGS__);                                                         \
        fprintf(stdout, "\n");                                                                     \
    }
#define FATAL(s, ...)                                                                              \
    {                                                                                              \
        fprintf(stdout, "[FATAL] %s at %s:%d: ", __FILE_NAME__, __func__, __LINE__);               \
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
