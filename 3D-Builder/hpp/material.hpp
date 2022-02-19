#ifndef _MATERIAL_HPP_
#define _MATERIAL_HPP_
#include "vec3.hpp"
enum material_t
{
    _E_error_type,
    _E_lamberian,
    _E_metal,
    _E_dieletric
};
class Material
{
    material_t _M_type;
    float _M_color[3], _M_alpha, _M_reflectivity;

public:
    Material(const material_t& _type = _E_error_type) : _M_type(_type)
    {
        _M_alpha = 1;
        _M_reflectivity = 0;
    }
    material_t getType() const { return _M_type; }

    void setColor(const float& r, const float& g, const float& b) { _M_color[0] = r, _M_color[1] = g, _M_color[2] = b; }
    void getColor(float& r, float& g, float& b) const { r = _M_color[0], g = _M_color[1], b = _M_color[2]; }

    void setalpha(const float& alpha) { _M_alpha = alpha; }
    float getalpha() { return _M_alpha; }

    void setreflect(const float& reflectivity) { _M_reflectivity = reflectivity; }
    float getreflect() { return _M_reflectivity; }
    void judge_type()
    {
        if (fabs(_M_alpha - 1) > 1e-5)
            _M_type = _E_dieletric;
        else if (_M_reflectivity)
            _M_type = _E_metal;
        else
            _M_type = _E_lamberian;
    }
};

// class metal : public Material
// {
//     float _M_color[3];
//     float _M_albedo;
//     float _M_fuzz;

// public:
//     metal() : Material(_E_metal) {}
//     void setColor(const int &r, const int &g, const int &b) { _M_color[0] = r, _M_color[1] = g, _M_color[2] = b; }
//     void getColor(float &r, float &g, float &b) const { r = _M_color[0], g = _M_color[1], b = _M_color[2]; }

//     void setalbedo(const float &a) { _M_albedo = a; }
//     void getalbedo(float &a) const { a = _M_albedo; }

//     void setfuzz(const float &_M_fuzz) { this->_M_fuzz = _M_fuzz; }
//     float getfuzz() const { return _M_fuzz; }
//     void setref(const float &ref){};
//     float getref() const { return -1; };
// };

// class lamberian : public Material
// {
//     float _M_color[3];
//     float _M_albedo;

// public:
//     lamberian() : Material(_E_lamberian) {}
//     void setColor(const int &r, const int &g, const int &b) { _M_color[0] = r, _M_color[1] = g, _M_color[2] = b; }
//     void getColor(float &r, float &g, float &b) const { r = _M_color[0], g = _M_color[1], b = _M_color[2]; }

//     void setalbedo(const float &a) { _M_albedo = a; }
//     void getalbedo(float &a) const { a = _M_albedo; }
//     void setfuzz(const float &_M_fuzz) {}
//     float getfuzz() const { return -1; }
//     void setref(const float &ref){};
//     float getref() const { return -1; };
// };
// class dieletric : public Material
// {
//     float _M_ref;

// public:
//     dieletric() : Material(_E_dieletric) {}

//     void setColor(const float &r, const float &g, const float &b) {}
//     void getColor(float &r, float &g, float &b) const { r = 1, g = 1, b = 1; }
//     void setalbedo(const float &a) {}
//     void getalbedo(float &a) const {}
//     void setfuzz(const float &_M_fuzz) {}
//     float getfuzz() const { return -1; }
//     void setref(const float &ref) { _M_ref = ref; }
//     float getref() const { return _M_ref; }
// };
#endif