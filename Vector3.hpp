#ifndef VECTOR3_4620
#define VECTOR3_4620
///Above is to help see if vector3 has already been referenced and defined.

#include<iostream>
#include<cmath>

struct Vector3
{
  ///Struct above allows it to all be public, otherwise is same as class.
  float x, y, z;

  Vector3& operator+=(const Vector3& rhs);

  Vector3& operator-=(const Vector3& rhs);

  Vector3& operator*=(const float rhs);

  float dot(const Vector3& rhs);

  Vector3 cross(const Vector3& rhs);

  float length();

  Vector3 normalize();
};

Vector3 operator+(Vector3 lhs, const Vector3 rhs);

Vector3 operator-(Vector3 lhs, const Vector3 rhs);

Vector3 operator*(Vector3 lhs, const float rhs);

std::ostream& operator<<(std::ostream& os, const Vector3& v);

#endif