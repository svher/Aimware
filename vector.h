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
};

class Vec4 {
public:
    float x, y, z, w;
};
