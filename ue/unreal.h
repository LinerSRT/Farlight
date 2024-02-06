#include <cstdint>
#include <string>
#include "../utils/driver.h"
#include "../cheat/offsets.h"

enum EClassCastFlags : uint64_t {
    None = 0x0000000000000000,
    Field = 0x0000000000000001,
    Int8Property = 0x0000000000000002,
    Enum = 0x0000000000000004,
    Struct = 0x0000000000000008,
    ScriptStruct = 0x0000000000000010,
    Class = 0x0000000000000020,
    ByteProperty = 0x0000000000000040,
    IntProperty = 0x0000000000000080,
    FloatProperty = 0x0000000000000100,
    UInt64Property = 0x0000000000000200,
    ClassProperty = 0x0000000000000400,
    UInt32Property = 0x0000000000000800,
    InterfaceProperty = 0x0000000000001000,
    NameProperty = 0x0000000000002000,
    StrProperty = 0x0000000000004000,
    Property = 0x0000000000008000,
    ObjectProperty = 0x0000000000010000,
    BoolProperty = 0x0000000000020000,
    UInt16Property = 0x0000000000040000,
    Function = 0x0000000000080000,
    StructProperty = 0x0000000000100000,
    ArrayProperty = 0x0000000000200000,
    Int64Property = 0x0000000000400000,
    DelegateProperty = 0x0000000000800000,
    NumericProperty = 0x0000000001000000,
    MulticastDelegateProperty = 0x0000000002000000,
    ObjectPropertyBase = 0x0000000004000000,
    WeakObjectProperty = 0x0000000008000000,
    LazyObjectProperty = 0x0000000010000000,
    SoftObjectProperty = 0x0000000020000000,
    TextProperty = 0x0000000040000000,
    Int16Property = 0x0000000080000000,
    DoubleProperty = 0x0000000100000000,
    SoftClassProperty = 0x0000000200000000,
    Package = 0x0000000400000000,
    Level = 0x0000000800000000,
    Actor = 0x0000001000000000,
    PlayerController = 0x0000002000000000,
    Pawn = 0x0000004000000000,
    SceneComponent = 0x0000008000000000,
    PrimitiveComponent = 0x0000010000000000,
    SkinnedMeshComponent = 0x0000020000000000,
    SkeletalMeshComponent = 0x0000040000000000,
    Blueprint = 0x0000080000000000,
    DelegateFunction = 0x0000100000000000,
    StaticMeshComponent = 0x0000200000000000,
    MapProperty = 0x0000400000000000,
    SetProperty = 0x0000800000000000,
    EnumProperty = 0x0001000000000000,
};


template<class T>
class TArray {
public:
    [[nodiscard]] int length() const {
        return dataCount;
    }

    [[nodiscard]] bool isValid() const {
        if (dataCount > dataMax)
            return false;
        if (!memoryData)
            return false;
        return true;
    }

    [[nodiscard]] uint64_t getAddress() const {
        return memoryData;
    }

    T getValue(int position) {
        return read<T>(memoryData + position * 0x8);
    }

    T getObject(int position) {
        return T(getValuePointer(position));
    }


    DWORD_PTR getValuePointer(int position) {
        return read<DWORD_PTR>(memoryData + position * 0x8);
    }


protected:
    uint64_t memoryData;
    uint32_t dataCount;
    uint32_t dataMax;
};

struct FString : private TArray<wchar_t> {

    [[nodiscard]] std::string toString() const {
        auto *buffer = new wchar_t[dataCount];
        read_array(memoryData, buffer, dataCount);
        std::string s = wchar_to_UTF8(buffer);
        delete[] buffer;
        return s;
    }

    static std::string wchar_to_UTF8(const wchar_t *in) {
        std::string out;
        unsigned int codepoint = 0;
        for (in; *in != 0; ++in) {
            if (*in >= 0xd800 && *in <= 0xdbff)
                codepoint = ((*in - 0xd800) << 10) + 0x10000;
            else {
                if (*in >= 0xdc00 && *in <= 0xdfff)
                    codepoint |= *in - 0xdc00;
                else
                    codepoint = *in;
                if (codepoint <= 0x7f)
                    out.append(1, static_cast<char>(codepoint));
                else if (codepoint <= 0x7ff) {
                    out.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
                    out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
                } else if (codepoint <= 0xffff) {
                    out.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
                    out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
                    out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
                } else {
                    out.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
                    out.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
                    out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
                    out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
                }
                codepoint = 0;
            }
        }
        return out;
    }
};


struct FName {
    uint32_t index = 0;
    int32_t number = 0;

    inline bool operator==(const FName &other) const {
        return index == other.index && number == other.number;
    }

    inline bool operator!=(const FName &other) const {
        return index != other.index || number != other.number;
    }

    [[nodiscard]] std::string toString() const {
        auto chunk = (UINT) ((int) (index) >> 16);
        auto name = (USHORT) index;
        auto poolChunk = read<UINT64>(baseId + 0x6EA2840 + ((chunk + 2) * 8));
        auto entryOffset = poolChunk + (ULONG) (2 * name);
        auto nameEntry = read<INT16>(entryOffset);
        auto nameLength = nameEntry >> 6;
        char buff[1028];
        if ((DWORD) nameLength && nameLength > 0) {
            read_array(entryOffset + 2, buff, nameLength);
            buff[nameLength] = '\0';
            return {buff};
        } else return "";
    }
};


struct UObject {
    static int32_t getIndex(DWORD_PTR object) {
        return read<int32_t>(object + 0x000C);
    }

    static DWORD_PTR getClass(DWORD_PTR object) {
        return read<DWORD_PTR>(object + 0x0010);
    }

    static uint64_t getClassFlags(DWORD_PTR object){
        return read<uint64_t>(getClass(object) + 0x00D0);
    }

    static int32_t getNameID(DWORD_PTR object) {
        return read<int32_t>(object + 0x0018);
    }

    static DWORD_PTR getOuter(DWORD_PTR object) {
        return read<DWORD_PTR>(object + 0x0020);
    }

    static std::string getName(DWORD_PTR object) {
        auto id = getNameID(object);
        auto chunk = (UINT) ((int) (id) >> 16);
        auto name = (USHORT) id;
        auto poolChunk = read<UINT64>(baseId + offsets::gNames + ((chunk + 2) * 8));
        auto entryOffset = poolChunk + (ULONG) (2 * name);
        auto nameEntry = read<INT16>(entryOffset);
        auto nameLength = nameEntry >> 6;
        char buff[1028];
        if ((DWORD) nameLength && nameLength > 0) {
            read_array(entryOffset + 2, buff, nameLength);
            buff[nameLength] = '\0';
            return {buff};
        } else return "";
    }

    static std::string getClassName(DWORD_PTR object) {
        return getName(getClass(object));
    }

    static bool isValid(DWORD_PTR object) {
        return (object > 0 && getNameID(object) > 0 && getClass(object) > 0);
    }
};


struct UObjectArray{
    static int32_t getObjectCount(){
        return read<int32_t>((baseId + offsets::gObject) + 0x14);
    }

    static DWORD_PTR getObject(int32_t index) {
        auto gObjects = read<DWORD_PTR>(baseId + offsets::gObject);
        auto chunk = read<DWORD_PTR>(gObjects + 0x8 * (index / 0x10000));
        return read<DWORD_PTR>(chunk + 0x18 * (index % 0x10000));
    }

    static DWORD_PTR findObjectExact(const std::string& name){
        for (int i = 0; i < getObjectCount(); ++i) {
            auto object = getObject(i);
            if(!UObject::isValid(object))
                continue;
            if(UObject::getClassName(object) == name){
                return object;
            }
        }
        return 0;
    }
    static DWORD_PTR findObject(const std::string& name){
        for (int i = 0; i < getObjectCount(); ++i) {
            auto object = getObject(i);
            if(!UObject::isValid(object))
                continue;
            if(UObject::getClassName(object).find(name) != std::string::npos){
                return object;
            }
        }
        return 0;
    }
};