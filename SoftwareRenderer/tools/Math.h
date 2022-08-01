#pragma once
#include <cmath>
#include <vector>
#include <assert.h>
#include <ostream>
class Vec3{
public:
    Vec3():array{0,0,0}{};
    Vec3(double x,double y,double z):array{x,y,z}{};

    double x() const {return array[0];};
    double y() const {return array[1];};
    double z() const {return array[2];};

    Vec3 operator-() const {return Vec3(-array[0],-array[1],-array[2]);};
    double operator[](int i) const {return array[i];};
    double& operator[](int i) {return array[i];};

    Vec3& operator+=(Vec3 vec3) {
        array[0] += vec3[0];
        array[1] += vec3[1];
        array[2] += vec3[2];
        return *this;
    };

    Vec3& operator*=(const double t){
        array[0] *= t;
        array[1] *= t;
        array[2] *= t;
        return *this;
    };

    Vec3& operator/=(const double t){
        *this *= 1/t;
        return *this;
    };

    double length() const{
        return sqrt(length_squared());
    }

    double length_squared() const{
        return array[0]*array[0] + array[1]*array[1] + array[2]*array[2];
    }



private:
    double array[3];
};

// vec3
std::ostream& operator<<(std::ostream &out,const Vec3 &v);
Vec3 operator+(const Vec3 &l,const Vec3 &r);
Vec3 operator-(const Vec3 &l,const Vec3 &r);
Vec3 operator*(const Vec3 &l,const Vec3 &r);
Vec3 operator*(double t,const Vec3 &r);
Vec3 operator*(const Vec3 &l,double t);
Vec3 operator/(const Vec3 &l,double t);
double dot(const Vec3 &l,const Vec3 &r);
Vec3 cross(const Vec3 &l,const Vec3 &r);
Vec3 unit_vector(Vec3 v);

// color

std::vector<unsigned char> toRGBAColor(const Vec3 &pixelColor, unsigned char alpha = 255);

using Point3 = Vec3;
using Color = Vec3;