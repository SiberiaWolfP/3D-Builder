#ifndef _MODEL_HPP_
#define _MODEL_HPP_
#include "vec3.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
class Ivy_Model
{
    // 读入的时候初始化为
    std::string filedir;
    //vertex和colors元素数量一致
    std::vector<Ivy233::vec3<float>> vertices;
    std::vector<Ivy233::vec3<float>> normals;
    std::vector<int> indices;
    int _M_material;

    // 碰撞盒和视觉中心的计算，可以参考Model.cpp代码
    // 不做也行，我在Model.cpp另外算
    // 0最小点，1最大点
    Ivy233::vec3<float> AABB_Box[2];
    Ivy233::vec3<float> view_Center;
    //模型在X轴方向和Y轴方向碰撞盒大小的最大值
    float max_size;

public:
    // 关于结点
    std::vector<Ivy233::vec3<float>> getvertices() const { return vertices; }
    void setvertice(const std::vector<Ivy233::vec3<float>>& _vertices) { vertices = _vertices; }
    // 关于法向量
    std::vector<Ivy233::vec3<float>> getnormal() const { return normals; }
    void setnormal(const std::vector<Ivy233::vec3<float>>& _normals) { normals = _normals; }
    // 关于索引
    std::vector<int> getindice() const { return indices; }
    void setindice(const std::vector<int>& _indices) { indices = _indices; }
    // 关于材质
    const int& getmat() const { return _M_material; }
    void setmaterial(const int& _material) { _M_material = _material; }
    // 关于文件
    const std::string& getfiledir() const { return filedir; }
    // 整体赋值
    void set_AABB_Box(const Ivy233::vec3<float>& AABBBox1, const Ivy233::vec3<float>& AABBBox2) { AABB_Box[0] = AABBBox1, AABB_Box[1] = AABBBox2; }
    void setviewCenter(const Ivy233::vec3<float>& view_center) { view_Center = view_center; }
    void setmax_size(const float& _max_size) { max_size = _max_size; }

    void setfiledir(const std::string& _filedir) { filedir = _filedir; }

    Ivy_Model()
    {
    }

    Ivy_Model(const char* _filedir, const int& _M_mat)
        : filedir(_filedir), _M_material(_M_mat)
    {
        std::ifstream infile(filedir);
        std::string message, str;

        std::cout << "loading Model:" << std::endl;

        normals.clear();
        vertices.clear();
        indices.clear();
        if (!infile.good())
        {
            std::cout << "ERROR: loading obj:(" << filedir << ") file is not good" << std::endl;
            return;
        }
        std::istringstream sin;
        float a[3];
        int tmp[3];
        std::vector<bool> vertice_normal_cnt;
        while (!infile.eof())
        {
            getline(infile, message); //按顺序读取文件中的每行数据
            // std::cout << 1 << message << std::endl;
            if (message.size() == 0 || message[0] == '#')
                continue;
            if (message[0] == 'v')
            {
                sin = std::istringstream(message);
                sin >> str >> a[0] >> a[1] >> a[2];
                if (message[1] == 'n')
                    normals.push_back(Ivy233::vec3<float>(a[0], a[1], a[2])); //压入法线数组
                else if (message[1] == ' ')
                {
                    vertices.push_back(Ivy233::vec3<float>(a[0], a[1], a[2]));
                    vertice_normal_cnt.push_back(0);
                }
            }
            else if (message[0] == 'f')
            {
                sin = std::istringstream(message);
                sin >> str >> tmp[0] >> tmp[1] >> tmp[2];
                for (int i = 0; i < 3; i++)
                    if (vertice_normal_cnt[tmp[i] - 1] == 0)
                    {
                        indices.push_back(tmp[i] - 1);
                        vertice_normal_cnt[tmp[i] - 1] = 1;
                    }
                    else
                    {
                        indices.push_back(vertices.size());
                        vertice_normal_cnt.push_back(0);
                        vertices.push_back(vertices[tmp[i] - 1]);
                    }
            }
        }
        if (normals.size() == 0)
            calc_normal(); //计算法向
        infile.close();
#ifdef _M_debug
        std::cout << "counts_vertices: " << vertices.size() << std::endl;
        for (int i = 0; i < vertices.size(); i++)
        {
            std::cout << i << " ";
            vertices[i].print();
        }
        std::cout << "counts_normals: " << normals.size() << std::endl;
        for (int i = 0; i < normals.size(); i++)
        {
            std::cout << i << " ";
            normals[i].print();
        }
#endif
    }

    void calc_normal()
    {
        // 因为有可能被其他进行的调用
        normals.resize(vertices.size());
        for (int x = 0; x < indices.size(); x += 3)
        {
            int v1 = indices[x], v2 = indices[x + 1], v3 = indices[x + 2];
            normals[v3] = (vertices[v1] - vertices[v2]) * (vertices[v1] - vertices[v3]);
            normals[v3].normalize();
            normals[v1] = normals[v2] = normals[v3];
        }
    }
    void smkdir(std::string dir) const
    {
        int i, len;
        len = dir.size();
        for (i = 0; i < len; i++)
        {
            if (dir[i] == '/' || dir[i] == '\\') //遇到文件夹符号
            {
                dir[i] = '\0'; //修改以方便访问
                if (access(dir.c_str(), 0) != 0)
                    mkdir(dir.c_str());
                dir[i] = '\\'; //改回去
            }
        }
        if (len > 0 && access(dir.c_str(), 0) != 0)
            mkdir(dir.c_str());
    }
    // 如果写文件之前发现normals没有东西的话，建议自己手动调用calc_normal()
    void output_file(const std::string& filedir) const
    {
        size_t len = std::max(filedir.find_last_of('/') + 1, filedir.find_last_of('\\') + 1) - 1;
        // std::cout << len << std::endl;
        if (len != (size_t)-1)
            smkdir(filedir.substr(0, len)); //新建文件夹
        std::ofstream outputfile(filedir);
        for (const Ivy233::vec3<float>& x : vertices)
            outputfile << "v " << x.x << " " << x.y << " " << x.z << std::endl;
        for (const Ivy233::vec3<float>& x : normals)
            outputfile << "vn " << x.x << " " << x.y << " " << x.z << std::endl;
        for (int x = 0; x < indices.size(); x += 3)
            outputfile << "f " << indices[x] + 1 << " " << indices[x + 1] + 1 << " " << indices[x + 2] + 1 << std::endl;
    }
};
#endif