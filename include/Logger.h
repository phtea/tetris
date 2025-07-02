#pragma once

#if DEBUG == 1
#define LOG(s, ...)                                                                                \
    {                                                                                              \
        fprintf(stdout, "[LOG] %s at %s:%d: ", __FILE_NAME__, __func__, __LINE__);                 \
        fprintf(stdout, s, ##__VA_ARGS__);                                                         \
        fprintf(stdout, "\n");                                                                     \
    }
#else
#define LOG(s, ...)                                                                                \
    {}
#endif // DEBUG
