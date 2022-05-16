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

    float Norm2D() const {
        return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    }

    bool isZero() const {
        return x == 0.f && y == 0.f && x == 0.f;
    };

    Vec3 operator-(const Vec3& other) const {
        return Vec3{x - other.x, y - other.y, z - other.z};
    }

    Vec3 operator+(const Vec3& other) const {
        return Vec3{x + other.x, y + other.y, z + other.z};
    }

    Vec3 operator*(float operand) const {
        return Vec3{x * operand, y * operand, z * operand};
    }

    Vec3 CalcAngles(const Vec3& other) const {
        Vec3 deltaVec = other - *this;

        auto pitch = -asinf(deltaVec.z / deltaVec.Norm2D()) * (180 / PI);
        auto yaw = atan2f(deltaVec.y, deltaVec.x) * (180 / PI);

        return Vec3{ pitch, yaw, 0 };
    }
};

class Vec4 {
public:
    float x, y, z, w;
};
