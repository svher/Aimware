#pragma once

class Vec2 {
public:
    float x, y;
};

class Vec3 {
public:
    float x, y, z;

    Vec3() = default;
    Vec3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vec3 operator+(const Vec3& other) const;
    Vec3 operator-(const Vec3& other) const;
    Vec3 operator*(float operand) const;
    Vec3& operator=(const Vec3& other) = default;
    float LengthSqr() const;
    float Norm2D() const;
    bool isZero() const;;
    void Clear();
    Vec3 CalcAngles(const Vec3& other) const;
};

class Vec3Aligned : public Vec3 {
public:
    // IMPORTANT: don't make this a virtual function
    Vec3Aligned& operator=(const Vec3& other);
    float w;
};

class Vec4 {
public:
    float x, y, z, w;
};

Vec3 TransformVec(Vec3 src, Vec3 angle, float distance);