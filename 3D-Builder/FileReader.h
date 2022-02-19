#ifndef _MAIN_H_
#define _MAIN_H_
#include "hpp/material.hpp"
#include "hpp/model.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/writer.h"
#include <algorithm>

using namespace rapidjson;
using namespace std;
class FileReader
{
private:
    string _M_filedir;
    Ivy233::camera _M_camera;
    vector<Material*> _M_materials;
    vector<Ivy_Model> _M_models;
    int nx, ny, ns;

public:
    FileReader() : nx(400), ny(200), ns(10) {}
    FileReader(string dir) { readFile(dir); }
    // camera
    void setCameraLookFrom(float x, float y, float z)
    {
        _M_camera._M_lookfrom.x = x;
        _M_camera._M_lookfrom.y = y;
        _M_camera._M_lookfrom.z = z;
    }
    void getCameraLookFrom(float& x, float& y, float& z) const
    {
        x = _M_camera._M_lookfrom.x;
        y = _M_camera._M_lookfrom.y;
        z = _M_camera._M_lookfrom.z;
    }

    void setCameraLookAt(float x, float y, float z)
    {
        _M_camera._M_lookat.x = x;
        _M_camera._M_lookat.y = y;
        _M_camera._M_lookat.z = z;
    }
    void getCameraLookAt(float& x, float& y, float& z) const
    {
        x = _M_camera._M_lookat.x;
        y = _M_camera._M_lookat.y;
        z = _M_camera._M_lookat.z;
    }

    void setCameraVup(float x, float y, float z)
    {
        _M_camera._M_vup.x = x;
        _M_camera._M_vup.y = y;
        _M_camera._M_vup.z = z;
    }
    void getCameraVup(float& x, float& y, float& z) const
    {
        x = _M_camera._M_vup.x;
        y = _M_camera._M_vup.y;
        z = _M_camera._M_vup.z;
    }

    void setCameraVfov(float x) { _M_camera._M_vfov = x; }
    void getCameraVfov(float& x) const { x = _M_camera._M_vfov; }

    void setCameraAspect(float x) { _M_camera._M_aspect = x; }
    void getCameraAspect(float& x) const { x = _M_camera._M_aspect; }

    void setCameraAperture(float x) { _M_camera._M_aperture = x; }
    void getCameraAperture(float& x) const { x = _M_camera._M_aperture; }

    void setCameraFocusDist(float x) { _M_camera._M_focus_dist = x; }
    void getCameraFocusDist(float& x) const { x = _M_camera._M_focus_dist; }

    void setModelList(const vector<Ivy_Model>& obj_list) { _M_models = obj_list; }
    void getModelList(vector<Ivy_Model>& obj_list) const { obj_list = _M_models; }
    Ivy_Model& getModelByIndex(const size_t& x) { return _M_models[x]; }
    const Ivy_Model getModelByIndex(const size_t& x) const { return _M_models[x]; }
    void setModel(const size_t& x, const Ivy_Model& v) { _M_models[x] = v; }
    void setModel(const Ivy_Model& v) { _M_models.push_back(v); }
    void setModelMaterial(const vector<Material*>& clr_list) { _M_materials = clr_list; }
    void getModelMaterial(vector<Material*>& clr_list) const { clr_list = _M_materials; }
    Material* getMaterialByIndex(const size_t& x) { return _M_materials[x]; }
    const Material* const getMaterialByIndex(const size_t& x) const { return _M_materials[x]; }
    // nx, ny, ns
    void setRenderParam(int nx, int ny, int ns)
    {
        this->nx = nx;
        this->ny = ny;
        this->ns = ns;
    }
    void getRenderParam(int& nx, int& ny, int& ns)
    {
        nx = this->nx;
        ny = this->ny;
        ns = this->ns;
    }

    void readFile(const string& dir)
    {
        _M_filedir = dir;
        Document docu;
        {
            ifstream ifs(dir);
            IStreamWrapper isw(ifs);
            // json解析
            docu.ParseStream(isw);
        }
#ifdef _M_debug
        cout << (nx = docu["nx"].GetInt()) << endl;
        cout << (ny = docu["ny"].GetInt()) << endl;
        cout << (ns = docu["ns"].GetInt()) << endl;
#else
        nx = docu["nx"].GetInt();
        ny = docu["ny"].GetInt();
        ns = docu["ns"].GetInt();
#endif
        // camera类对象产生
        _M_camera = Ivy233::camera(
            Ivy233::vec3<float>(docu["camera"]["lookfrom"][0].GetDouble(),
                docu["camera"]["lookfrom"][1].GetDouble(),
                docu["camera"]["lookfrom"][2].GetDouble()),
            Ivy233::vec3<float>(docu["camera"]["lookat"][0].GetDouble(),
                docu["camera"]["lookat"][1].GetDouble(),
                docu["camera"]["lookat"][2].GetDouble()),
            Ivy233::vec3<float>(docu["camera"]["vup"][0].GetDouble(),
                docu["camera"]["vup"][1].GetDouble(),
                docu["camera"]["vup"][2].GetDouble()),
            docu["camera"]["vfov"].GetDouble(),
            docu["camera"]["aspect"].GetDouble(),
            docu["camera"]["aperture"].GetDouble(),
            docu["camera"]["focus_dist"].GetDouble());
        // 产生material类对象
#ifdef _M_debug
        int mat_cnt;
        cout << (mat_cnt = docu["materials"].Size()) << endl;
#else
        int mat_cnt = docu["materials"].Size();
#endif
        // 可能使用多次，所以需要清空
        _M_materials.clear();
        for (int i = 0; i < mat_cnt; i++)
        {
            // lamberian
            if (strcmp(docu["materials"][i]["type"].GetString(), "lamberian") == 0)
            {
                Material* tmp = new Material();
                if (docu["materials"][i].HasMember("albedo"))
                    tmp->setColor(docu["materials"][i]["albedo"][0].GetDouble(),
                        docu["materials"][i]["albedo"][1].GetDouble(),
                        docu["materials"][i]["albedo"][2].GetDouble());
                else
                    tmp->setColor(1, 1, 1);
                _M_materials.push_back(tmp);
            }
            // metal
            else if ((strcmp(docu["materials"][i]["type"].GetString(), "metal") == 0))
            {
                Material* tmp = new Material();
                if (docu["materials"][i].HasMember("albedo"))
                    tmp->setColor(docu["materials"][i]["albedo"][0].GetDouble(),
                        docu["materials"][i]["albedo"][1].GetDouble(),
                        docu["materials"][i]["albedo"][2].GetDouble());
                else
                    tmp->setColor(1, 1, 1);
                tmp->setreflect(docu["materials"][i].HasMember("fuzz") ? 1 - docu["materials"][i]["fuzz"].GetDouble() : 0.8);
                _M_materials.push_back(tmp);
            }
            // dieletric
            else if (((strcmp(docu["materials"][i]["type"].GetString(), "dieletric") == 0)))
            {
                Material* tmp = new Material();
                if (docu["materials"][i].HasMember("albedo"))
                    tmp->setColor(docu["materials"][i]["albedo"][0].GetDouble(),
                        docu["materials"][i]["albedo"][1].GetDouble(),
                        docu["materials"][i]["albedo"][2].GetDouble());
                else
                    tmp->setColor(1, 1, 1);
                tmp->setalpha(docu["materials"][i].HasMember("ref_idx") ? docu["materials"][i]["ref_idx"].GetDouble() : 0.4);
                _M_materials.push_back(tmp);
            }
        }
        _M_models.clear();
        int modelsize = docu["objfile"].Size();
        cout << "modelsize: " << modelsize << endl;
        for (int i = 0; i < modelsize; i++)
        {
            std::string filedir = docu["objfile"][i]["dir"].GetString();
            std::ifstream infile(filedir.c_str());
            if (infile.good())
            {
                std::cout << "SUCC: loading obj: " << filedir << std::endl;
                _M_models.push_back(Ivy_Model(
                    docu["objfile"][i]["dir"].GetString(),
                    docu["objfile"][i]["material"].GetInt() - 1));
            }
            else
                std::cout << "ERROR: loading obj:(" << filedir << ") file is not good" << std::endl;
        }
    }

    void writeFile(const string& dir)
    {

        Document docu;
        docu.SetObject()
            .AddMember("nx", nx, docu.GetAllocator())
            .AddMember("ny", ny, docu.GetAllocator())
            .AddMember("ns", ns, docu.GetAllocator());
        // camera
        {
            Value camera;
            camera.SetObject()
                .AddMember("vfov", _M_camera._M_vfov, docu.GetAllocator())
                .AddMember("aspect", _M_camera._M_aspect, docu.GetAllocator())
                .AddMember("aperture", _M_camera._M_aperture, docu.GetAllocator())
                .AddMember("focus_dist", _M_camera._M_focus_dist, docu.GetAllocator());
            Value lookfrom, lookat, vup;
            lookfrom.SetArray()
                .PushBack(_M_camera._M_lookfrom.x, docu.GetAllocator())
                .PushBack(_M_camera._M_lookfrom.y, docu.GetAllocator())
                .PushBack(_M_camera._M_lookfrom.z, docu.GetAllocator());
            lookat.SetArray()
                .PushBack(_M_camera._M_lookat.x, docu.GetAllocator())
                .PushBack(_M_camera._M_lookat.y, docu.GetAllocator())
                .PushBack(_M_camera._M_lookat.z, docu.GetAllocator());
            vup.SetArray()
                .PushBack(_M_camera._M_vup.x, docu.GetAllocator())
                .PushBack(_M_camera._M_vup.y, docu.GetAllocator())
                .PushBack(_M_camera._M_vup.z, docu.GetAllocator());
            camera.AddMember("lookfrom", lookfrom, docu.GetAllocator())
                .AddMember("lookat", lookat, docu.GetAllocator())
                .AddMember("vup", vup, docu.GetAllocator());
            docu.AddMember("camera", camera, docu.GetAllocator());
        }
        // materials
        {
            Value materials, material, tmp;
            float a[3];
            materials.SetArray();
            for (Material* x : _M_materials)
            {
                material.SetObject();
                x->judge_type();
                if (x->getType() == _E_dieletric)
                {
                    x->getColor(a[0], a[1], a[2]);
                    tmp.SetArray()
                        .PushBack(a[0], docu.GetAllocator())
                        .PushBack(a[1], docu.GetAllocator())
                        .PushBack(a[2], docu.GetAllocator());
                    material.AddMember("type", "dieletric", docu.GetAllocator())
                        .AddMember("ref_idx", x->getalpha(), docu.GetAllocator())
                        .AddMember("albedo", tmp, docu.GetAllocator());
                }
                else if (x->getType() == _E_metal)
                {
                    x->getColor(a[0], a[1], a[2]);
                    tmp.SetArray()
                        .PushBack(a[0], docu.GetAllocator())
                        .PushBack(a[1], docu.GetAllocator())
                        .PushBack(a[2], docu.GetAllocator());
                    material.AddMember("type", "metal", docu.GetAllocator())
                        .AddMember("fuzz", 1 - x->getreflect(), docu.GetAllocator())
                        .AddMember("albedo", tmp, docu.GetAllocator());
                }
                else if (x->getType() == _E_lamberian)
                {
                    x->getColor(a[0], a[1], a[2]);
                    tmp.SetArray()
                        .PushBack(a[0], docu.GetAllocator())
                        .PushBack(a[1], docu.GetAllocator())
                        .PushBack(a[2], docu.GetAllocator());
                    material.AddMember("type", "lamberian", docu.GetAllocator())
                        .AddMember("albedo", tmp, docu.GetAllocator());
                }
                materials.PushBack(material, docu.GetAllocator());
            }
            docu.AddMember("materials", materials, docu.GetAllocator());
        }

        // Models
        {
#define _M_debug_output
            Value objfiles, objfile;
            objfiles.SetArray();
            for (const Ivy_Model& x : _M_models)
            {
#ifdef _M_debug_output
                size_t len1 = std::max(dir.find_last_of('/') + 1, dir.find_last_of('\\') + 1);
                string filedir = x.getfiledir();
                size_t len2 = std::max(filedir.find_last_of('/') + 1, filedir.find_last_of('\\') + 1);
                // cout << filedir.substr(len) << " " << len << endl;
                filedir = dir.substr(0, len1) + filedir.substr(len2);
                x.output_file(filedir);
#else
                x.output_file(x.getfiledir());
#endif
                objfile.SetObject()
                    .AddMember("dir", Value(x.getfiledir().c_str(), x.getfiledir().length()), docu.GetAllocator())
                    .AddMember("material", x.getmat() + 1, docu.GetAllocator());
                objfiles.PushBack(objfile, docu.GetAllocator());
            }
            docu.AddMember("objfile", objfiles, docu.GetAllocator());
        }
        ofstream ofs(dir);
        OStreamWrapper osw(ofs);
        Writer<OStreamWrapper> writer(osw);
        docu.Accept(writer);
    }
};

#endif