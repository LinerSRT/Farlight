#include <string>
#include <cstdint>
#include <cmath>
#include <d3d9.h>
#include <DirectXMath.h>
#include "../utils/driver.h"
#include "../utils/strings.h"

template<class TEnum>
class TEnumAsByte {
public:
    inline TEnumAsByte() {
    }

    inline TEnumAsByte(TEnum _value)
            : value(static_cast<uint8_t>(_value)) {
    }

    explicit inline TEnumAsByte(int32_t _value)
            : value(static_cast<uint8_t>(_value)) {
    }

    explicit inline TEnumAsByte(uint8_t _value)
            : value(_value) {
    }

    inline explicit operator TEnum() const {
        return (TEnum) value;
    }

    inline TEnum getValue() const {
        return (TEnum) value;
    }

private:
    uint8_t value;
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

    [[nodiscard]] std::string toString(const uintptr_t gameBase) const {
        auto chunk = (UINT) ((int) (index) >> 16);
        auto name = (USHORT) index;
        auto poolChunk = read<UINT64>(gameBase + 0x6E61180 + ((chunk + 2) * 8));
        auto entryOffset = poolChunk + (ULONG) (2 * name);
        auto nameEntry = read<INT16>(entryOffset);
        auto nameLength = nameEntry >> 6;
        char buff[1028];
        if ((DWORD) nameLength && nameLength > 0) {
            read_array(entryOffset + 2, buff, nameLength);
            buff[nameLength] = '\0';
            return std::string(buff);
        } else return "";
    }
};


struct FVector {
    float x;
    float y;
    float z;

    inline FVector()
            : x(0), y(0), z(0) {}

    inline FVector(float x, float y, float z)
            : x(x),
              y(y),
              z(z) {}

    static FVector forward(const FVector &angles) {
        float sp, sy, cp, cy;
        float angle;
        angle = (float) (angles.x * (M_PI / 180.0f));
        sy = sinf(angle);
        cy = cosf(angle);
        angle = (float) (-angles.x * (M_PI / 180.0f));
        sp = sinf(angle);
        cp = cosf(angle);
        return {cp * cy, cp * sy, -sp};
    }

    [[nodiscard]] inline float dot(FVector v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    [[nodiscard]] inline float distance(FVector v) const {
        float xV = this->x - v.x;
        float yV = this->y - v.y;
        float zV = this->z - v.z;
        return sqrtf((xV * xV) + (yV * yV) + (zV * zV));// * 0.0135f;
    }

    [[nodiscard]] inline float distanceIgnoreZ(FVector v) const {
        float xV = this->x - v.x;
        float yV = this->y - v.y;
        return sqrtf((xV * xV) + (yV * yV));
    }

    [[nodiscard]] inline float distanceMeters(FVector v) const {
        return distance(v) * 0.01f;
    }


    [[nodiscard]] inline float distance2D(FVector v) const {
        float xV = this->x - v.x;
        float yV = this->y - v.y;
        return sqrtf((xV * xV) + (yV * yV));
    }

    [[nodiscard]] inline float angle(FVector v) const {
        double xDistance = x - v.x;
        double yDistance = y - v.y;
        auto angle = (float) (atan(yDistance / xDistance) * (180.0 / M_PI));
        float rotation = xDistance > 0 ? angle - 180 : angle;
        if (abs(angle) == 90)
            return -angle;
        return rotation;
    }

    [[nodiscard]] FVector direction(float angle, float length) const {
        double theta = angle * (M_PI / 180);
        auto dx = (float) (x + cos(theta) * length);
        auto dy = (float) (y + sin(theta) * length);
        return {dx, dy, 0};
    }

    FVector operator+(FVector v) const {
        return {x + v.x, y + v.y, z + v.z};
    }

    FVector operator-(FVector v) const {
        return {x - v.x, y - v.y, z - v.z};
    }

    FVector operator*(float number) const {
        return {x * number, y * number, z * number};
    }

    [[nodiscard]] std::string debug() const {
        return "{x=" + std::to_string(static_cast<int>(x)) + ", y=" + std::to_string(static_cast<int>(y)) + ", z=" + std::to_string(static_cast<int>(z)) + "}";
    }
};

struct FRotator {
    float x;
    float y;
    float z;

    [[nodiscard]] std::string debug() const {
        return "{x=" + std::to_string(static_cast<int>(x)) + ", y=" + std::to_string(static_cast<int>(y)) + ", z=" + std::to_string(static_cast<int>(z)) + "}";
    }
};

struct FQuat {
    float x;
    float y;
    float z;
    float w;
};


struct FTransform {
    FQuat rotation;
    FVector translation;
    char pad[4];
    FVector scale;
    char pad1[4];

    [[nodiscard]] D3DMATRIX toMatrix() const {
        D3DMATRIX m;
        m._41 = translation.x;
        m._42 = translation.y;
        m._43 = translation.z;

        float x2 = rotation.x + rotation.x;
        float y2 = rotation.y + rotation.y;
        float z2 = rotation.z + rotation.z;

        float xx2 = rotation.x * x2;
        float yy2 = rotation.y * y2;
        float zz2 = rotation.z * z2;
        m._11 = (1.0f - (yy2 + zz2)) * scale.x;
        m._22 = (1.0f - (xx2 + zz2)) * scale.y;
        m._33 = (1.0f - (xx2 + yy2)) * scale.z;

        float yz2 = rotation.y * z2;
        float wx2 = rotation.w * x2;
        m._32 = (yz2 - wx2) * scale.z;
        m._23 = (yz2 + wx2) * scale.y;

        float xy2 = rotation.x * y2;
        float wz2 = rotation.w * z2;
        m._21 = (xy2 - wz2) * scale.y;
        m._12 = (xy2 + wz2) * scale.x;

        float xz2 = rotation.x * z2;
        float wy2 = rotation.w * y2;
        m._31 = (xz2 + wy2) * scale.z;
        m._13 = (xz2 - wy2) * scale.x;

        m._14 = 0.0f;
        m._24 = 0.0f;
        m._34 = 0.0f;
        m._44 = 1.0f;

        return m;
    }
};

D3DMATRIX matrix(FRotator rot, FVector origin) {
    float radPitch = (rot.x * float(M_PI) / 180.f);
    float radYaw = (rot.y * float(M_PI) / 180.f);
    float radRoll = (rot.z * float(M_PI) / 180.f);
    float SP = sinf(radPitch);
    float CP = cosf(radPitch);
    float SY = sinf(radYaw);
    float CY = cosf(radYaw);
    float SR = sinf(radRoll);
    float CR = cosf(radRoll);
    D3DMATRIX matrix;
    matrix.m[0][0] = CP * CY;
    matrix.m[0][1] = CP * SY;
    matrix.m[0][2] = SP;
    matrix.m[0][3] = 0.f;
    matrix.m[1][0] = SR * SP * CY - CR * SY;
    matrix.m[1][1] = SR * SP * SY + CR * CY;
    matrix.m[1][2] = -SR * CP;
    matrix.m[1][3] = 0.f;
    matrix.m[2][0] = -(CR * SP * CY + SR * SY);
    matrix.m[2][1] = CY * SR - CR * SP * SY;
    matrix.m[2][2] = CR * CP;
    matrix.m[2][3] = 0.f;
    matrix.m[3][0] = origin.x;
    matrix.m[3][1] = origin.y;
    matrix.m[3][2] = origin.z;
    matrix.m[3][3] = 1.f;
    return matrix;
}

D3DMATRIX matrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2) {
    D3DMATRIX pOut;
    pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
    pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
    pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
    pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
    pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
    pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
    pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
    pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
    pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
    pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
    pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
    pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
    pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
    pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
    pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
    pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;
    return pOut;
}



struct FFastArraySerializerItem {
    int32_t ReplicationID;                                              // 0x0000   (0x0004)
    int32_t ReplicationKey;                                             // 0x0004   (0x0004)
    int32_t MostRecentArrayReplicationKey;                              // 0x0008   (0x0004)
};

struct FSolarItemData : FFastArraySerializerItem {
    int32_t ItemID;                                                     // 0x000C   (0x0004)
    int64_t ThisID;                                                     // 0x0010   (0x0008)
    FString Name;                                                       // 0x0018   (0x0010)
    FString Icon;                                                       // 0x0028   (0x0010)
    FString Info;                                                       // 0x0038   (0x0010)
    int32_t count;                                                      // 0x0048   (0x0004)
    uint32_t ItemType;                                                  // 0x004C   (0x0004)
    int32_t Quality;                                                    // 0x0050   (0x0004)
    int32_t MaxPile;                                                    // 0x0054   (0x0004)
    bool IfReplace;                                                     // 0x0058   (0x0001)
    unsigned char UnknownData00_5[0x7];                                 // 0x0059   (0x0007)  MISSED
    FString AbilityBP;                                                  // 0x0060   (0x0010)
    FString ItemBP;                                                     // 0x0070   (0x0010)
    int32_t DropTime;                                                   // 0x0080   (0x0004)
    int32_t UnPickupTime;                                               // 0x0084   (0x0004)
    bool IsRecommend;                                                   // 0x0088   (0x0001)
    bool bDirty;                                                        // 0x0089   (0x0001)
    bool IsPickupLock;                                                  // 0x008A   (0x0001)
    unsigned char UnknownData01_5[0x5];                                 // 0x008B   (0x0005)  MISSED
    int64_t UserId;                                                     // 0x0090   (0x0008)
    int32_t Level;                                                      // 0x0098   (0x0004)
    int32_t Prize;                                                      // 0x009C   (0x0004)
    float CurValue;                                                     // 0x00A0   (0x0004)
    bool UseOnPickup;                                                   // 0x00A4   (0x0001)
    uint8_t AppearanceType;                                             // 0x00A5   (0x0001)
    bool bUpdateOverlap;                                                // 0x00A6   (0x0001)
    unsigned char UnknownData02_5[0x1];                                 // 0x00A7   (0x0001)  MISSED
    int32_t SourceType;                                                 // 0x00A8   (0x0004)
    unsigned char UnknownData03_5[0x4];                                 // 0x00AC   (0x0004)  MISSED
    FString OwnerPlayerID;                                              // 0x00B0   (0x0010)
    unsigned char UnknownData04_6[0x8];                                 // 0x00C0   (0x0008)  MISSED
};


struct FMinimalViewInfo {
    FVector Location;
    FRotator Rotation;
    float FOV;
    float DesiredFOV;
    float OrthoWidth;
    float OrthoNearClipPlane;
    float OrthoFarClipPlane;
    float AspectRatio;
};


struct FCameraCacheEntry {
    float timestamp;
    unsigned char UnknownData00_5[0xC];
    FMinimalViewInfo POV;
};





std::string nameFromId(int id) {
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


struct WeakPtr{
    static DWORD_PTR get(DWORD address) {
        auto gObjects = read<DWORD_PTR>(baseId + offsets::gObject);
        auto chunk = read<DWORD_PTR>(gObjects + 0x8 * (address / 0x10000));
        return read<DWORD_PTR>(chunk + 0x18 * (address % 0x10000));
    }
};