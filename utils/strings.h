#include <string>

using namespace std;

namespace strings{
    bool isEqual(const string& s1, const char* check) {
        string s2(check);
        return (s1 == s2);
    }
    bool isStartWith(const string& str, const char* check) {
        return (str.rfind(check, 0) == 0);
    }

    bool isContain(const string& str, const string& check) {
        size_t found = str.find(check);
        return (found != string::npos);
    }
}
