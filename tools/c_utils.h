//
// Created by ruppet on 2/4/2020.
//

#ifndef uc-sflying-loader-c-utils-h
#define uc-sflying-loader-c-utils-h


class c_utils {
public:
    void create_console();
    void log(const char* text, ...);
    const char* format(const char* text, ...);
    void generate_random(char* buffer, size_t size);
    std::vector<std::string> split(const std::string& str, const char* delim);
    unsigned long get_process_id(std::string name);
};


#endif //uc-sflying-loader-c-utils-h
