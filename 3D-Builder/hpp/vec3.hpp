#pragma once
#ifndef _VEC3_HPP_
#define _VEC3_HPP_
#include <cmath>
#include <direct.h>
#include <io.h>
#include <iostream>
#include <string>
namespace Ivy233
{
    template <class T>
    struct vec3
    {
        T x, y, z;
        vec3() : x(0), y(0), z(0) {}
        vec3(const T& _x, const T& _y, const T& _z) : x(_x), y(_y), z(_z) {}
        void print() const
        {
            std::cout << "vec3: ( x = " << x << ", y = " << y << ", z = " << z << ")" << std::endl;
        }
        vec3 operator-(const vec3& b)
        {
            return vec3(x - b.x, y - b.y, z - b.z);
        }
        vec3 operator*(const vec3& b)
        {
            return vec3(y * b.z - z * b.y, -(x * b.z - z * b.x), x * b.y - y * b.x);
        }
        void normalize()
        {
            float tmp = sqrt(x * x + y * y + z * z);
            x /= tmp, y /= tmp, z /= tmp;
        }
    };
    struct camera
    {
        vec3<float> _M_lookfrom, _M_lookat, _M_vup;
        double _M_vfov, _M_aspect, _M_aperture, _M_focus_dist;
        camera() {}
        camera(const vec3<float>& __lookfrom, const vec3<float>& __lookat, const vec3<float>& __vup,
            const double& __vfov, const double& __aspect, const double& __aperture, const double& __focus_dist)
            : _M_lookfrom(__lookfrom), _M_lookat(__lookat), _M_vup(__vup), _M_vfov(__vfov), _M_aspect(__aspect), _M_aperture(__aperture), _M_focus_dist(__focus_dist)
        {
#define _M_debug_cam
#ifdef _M_debug_cam
            std::cout << "camera = camera( " << std::endl;
            std::cout << "\tlookfrom = ";
            _M_lookfrom.print();
            std::cout << "\tlookup = ";
            _M_lookat.print();
            std::cout << "\tvup = ";
            _M_vup.print();
            std::cout << "\tvfov = " << _M_vfov << ", aspect = " << _M_aspect << ", aperture = " << _M_aperture << ", focus_dist = " << _M_focus_dist << std::endl;
            std::cout << ")" << std::endl;
#endif
        }
    };
    // void smkdir(std::string dir)
    // {
    //     int i, len;
    //     len = dir.size();
    //     for (i = 0; i < len; i++)
    //     {
    //         if (dir[i] == '/' || dir[i] == '\\') //遇到文件夹符号
    //         {
    //             dir[i] = '\0'; //修改以方便访问
    //             if (access(dir.c_str(), 0) != 0)
    //                 mkdir(dir.c_str());
    //             dir[i] = '\\'; //改回去
    //         }
    //     }
    //     if (len > 0 && access(dir.c_str(), 0) != 0)
    //         mkdir(dir.c_str());
    // }
}; // namespace Ivy233
#endif