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

    Vec3& operator=(const Vec3& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    float LengthSqr() const {
        return x*x + y*y + z*z;
    }

    void Clear() {
        x = y = z = 0;
    }

    Vec3 CalcAngles(const Vec3& other) const {
        Vec3 deltaVec = other - *this;

        auto pitch = -asinf(deltaVec.z / deltaVec.Norm2D()) * (180 / PI);
        auto yaw = atan2f(deltaVec.y, deltaVec.x) * (180 / PI);

        return Vec3{ pitch, yaw, 0 };
    }
};

class Vec3Aligned : public Vec3 {
public:
    // IMPORTANT: don't make this a virtual function
    Vec3Aligned& operator=(const Vec3& other) {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        return *this;
    }
    float w;
};

class Vec4 {
public:
    float x, y, z, w;
};
