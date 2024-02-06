#include <string>
#include <cstdint>
#include <random>

using namespace std;

namespace strings {
    bool isEqual(const string &s1, const char *check){
        string s2(check);
        return (s1 == s2);
    }

    bool isStartWith(const string &str, const char *check) {
        return (str.rfind(check, 0) == 0);
    }

    bool isContain(const string &str, const string &check) {
        size_t found = str.find(check);
        return (found != string::npos);
    }

    string hexToString(uint64_t hexValue) {
        std::ostringstream stream;
        stream << std::hex << hexValue;
        return "0x" + stream.str();
    }

    string randomString(string::size_type length) {
        static auto &charset = "0123456789"
                               "abcdefghijklmnopqrstuvwxyz"
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        thread_local static std::mt19937 rg{std::random_device{}()};
        thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(charset) - 2);
        std::string s;
        s.reserve(length);
        while (length--)
            s += charset[pick(rg)];
        return s;
    }
}
