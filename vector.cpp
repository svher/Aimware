#include "includes.h"

Vec3 TransformVec(Vec3 src, Vec3 angle, float distance) {
    Vec3 newPos{};
    // yaw
    newPos.x = src.x + cosf(TO_RAD(angle.y)) * distance;
    newPos.y = src.y + sinf(TO_RAD(angle.y)) * distance;
    // pitch
    newPos.z = src.z + tanf(TO_RAD(angle.x)) * distance;
    return newPos;
}

Vec3 Vec3::operator+(const Vec3 &other) const {
    return Vec3{x + other.x, y + other.y, z + other.z};
}

Vec3 Vec3::operator-(const Vec3 &other) const {
    return Vec3{x - other.x, y - other.y, z - other.z};
}

Vec3 Vec3::operator*(float operand) const {
    return Vec3{x * operand, y * operand, z * operand};
}

float Vec3::LengthSqr() const {
    return x*x + y*y + z*z;
}

float Vec3::Norm2D() const {
    return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

bool Vec3::isZero() const {
    return x == 0.f && y == 0.f && x == 0.f;
}

void Vec3::Clear() {
    x = y = z = 0;
}

Vec3 Vec3::CalcAngles(const Vec3 &other) const {
    Vec3 deltaVec = other - *this;

    auto pitch = -asinf(deltaVec.z / deltaVec.Norm2D()) * (180 / PI);
    auto yaw = atan2f(deltaVec.y, deltaVec.x) * (180 / PI);

    return Vec3{ pitch, yaw, 0 };
}

Vec3Aligned &Vec3Aligned::operator=(const Vec3 &other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    return *this;
}
