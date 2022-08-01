#include "Math.h"



// vec3
std::ostream& operator<<(std::ostream &out,const Vec3 &v)
{
    return out<<v[0]<<' '<<v[1]<<' '<<v[2];
}

Vec3 operator+(const Vec3 &l,const Vec3 &r){
    return Vec3(l[0]+r[0],l[1]+r[1],l[2]+r[2]);
}


Vec3 operator-(const Vec3 &l,const Vec3 &r){
    return Vec3(l[0]-r[0],l[1]-r[1],l[2]-r[2]);
}

Vec3 operator*(const Vec3 &l,const Vec3 &r){
    return Vec3(l[0]*r[0],l[1]*r[1],l[2]*r[2]);
}

Vec3 operator*(double t,const Vec3 &r){
    return Vec3(t*r[0],t*r[1],t*r[2]);
}

Vec3 operator*(const Vec3 &l,double t){
    return Vec3(t*l[0],t*l[1],t*l[2]);
}

Vec3 operator/(const Vec3 &l,double t){
    return 1/t * l;
}

double dot(const Vec3 &l,const Vec3 &r){
    return l[0]*r[0] + l[1]*r[1] + l[2]*r[2];
}

Vec3 cross(const Vec3 &l,const Vec3 &r){
    return Vec3(l[1] * r[2] - l[2] * r[1],
                 l[2] * r[0] - l[0] * r[2],
                 l[0] * r[1] - l[1] * r[0]);
}

Vec3 unit_vector(Vec3 v){
    return v / v.length();
}

// color

std::vector<unsigned char> toRGBAColor(const Vec3 &pixelColor,unsigned char alpha){
    return std::vector<unsigned char>{  static_cast<unsigned char>(255.999 * pixelColor[0]),
                                        static_cast<unsigned char>(255.999 * pixelColor[1]),
                                        static_cast<unsigned char>(255.999 * pixelColor[2]),
                                        alpha};
}