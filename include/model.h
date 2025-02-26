#ifndef MODEL_H
#define MODEL_H

#include <cmath>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "vec3.h"
#include "vec2.h"

// Model class for .obj files

template <typename T>
class Model
{
    public:
        Model(const std::string filename)
        {
            std::ifstream file;
            std::string line;

            file.open(filename, std::ios::in);
            if (file.is_open())
            {
                std::cout << "Loading model..." << std::endl;
                int nlines = 0;
                int currline = 0;
                float percentage = 0.0f;

                while(std::getline(file, line))
                    ++nlines;

                line.erase(); 
                file.clear();
                file.seekg(0);
                // std::cout << nlines << "\n";
                while (std::getline(file, line))
                {
                    if (line.starts_with("v "))
                        parse_vertex(line);

                    if (line.starts_with("f "))
                        parse_face(line);

                    if (line.starts_with("vt "))
                        parse_texture(line);

                    if (line.starts_with("vn "))
                        parse_normal(line);

                    percentage = (currline / (float) nlines);

                    std::cout << "[" << percentage * 100 << std::setprecision(2) <<  "%] \t\r" << std::flush;
                    ++currline;
                }
                std::cout << std::flush;
                std::cout << "[" << 100 << "%]\n";
                std::cout << "Loaded" << std::endl;
                file.close();
            }

        }

        int get_nvertexs() { return (int) verts.size(); }

        int get_ntex_coords() { return (int) tex_coords.size(); }

        int get_nfaces() { return (int) faces_verts.size(); }


        Vec3<T> vertex(const int idx) { return verts[idx]; }
        Vec2<T> texture_uv(int idx) { return tex_coords[idx]; }

        std::vector<int> face(int idx) { return faces_verts[idx]; }
        std::vector<int> face_texture(int idx) { return faces_tex[idx]; }


    private:
        std::vector<Vec3<T>>          verts;
        std::vector<Vec2<T>>          tex_coords;
        std::vector<Vec3<T>>          normals;
        std::vector<std::vector<int>> faces_verts;
        std::vector<std::vector<int>> faces_tex;
        std::vector<std::vector<int>> faces_normals;

        void parse_vertex(const std::string& line)
        {
            std::istringstream iss(line);
            char v;
            T n1, n2, n3;
            iss >> v >> n1 >> n2 >> n3;
            Vec3<T> vertex(n1, n2, n3);
            verts.push_back(vertex);
        }

        void parse_texture(const std::string& line)
        {
            std::istringstream iss(line);
            std::string vt;
            T n1, n2;
            iss >> vt >> n1 >> n2;
            tex_coords.push_back(Vec2<T>(n1, n2));
        }

        
        void parse_normal(const std::string& line)
        {
            std::istringstream iss(line);
            std::string vn;
            T n1, n2, n3;
            iss >> vn >> n1 >> n2 >> n3;
            normals.push_back(Vec3<T>(n1, n2, n3));
        }

        void parse_face(const std::string& line)
        {
            std::istringstream iss(line);
            std::vector<int> vertex_elements;
            std::vector<int> texture_elements;
            std::vector<int> normal_elements;
            std::string part;
            char f;
            iss >> f; // Ignore the 'f' character

            bool has_slash = false;

            while (iss >> part)
            {
                if (part.find('/') != std::string::npos) {
                    has_slash = true;
                    break;
                }
            }

            iss.clear();
            iss.seekg(2); // Skip 'f '

            if (!has_slash) {
                // Case: "f v1 v2 v3" (only vertex indices)
                int vertex_idx;
                while (iss >> vertex_idx) {
                    vertex_elements.push_back(vertex_idx - 1);
                }
            } else {
                // Case: "f v1/t1/n1 v2/t2/n2 v3/t3/n3"
                while (iss >> part) {
                    std::istringstream sub_iss(part);
                    std::string vertex_idx, texture_coord_idx, normals_idx;

                    std::getline(sub_iss, vertex_idx, '/');
                    if (std::getline(sub_iss, texture_coord_idx, '/')) {
                        if (!texture_coord_idx.empty()) {
                            texture_elements.push_back(std::stoi(texture_coord_idx) - 1);
                        }
                        if (std::getline(sub_iss, normals_idx, '/')) {
                            if (!normals_idx.empty()) {
                                normal_elements.push_back(std::stoi(normals_idx) - 1);
                            }
                        }
                    }
                    vertex_elements.push_back(std::stoi(vertex_idx) - 1);
                }
            }

            faces_verts.push_back(vertex_elements);
            faces_tex.push_back(texture_elements);
            faces_normals.push_back(normal_elements);

            // std::cout << "Line: " << line << std::endl;
            // std::cout << "Vertices: ";
            // for (int vert : faces_verts.back()) std::cout << vert << ' ';
            // std::cout << "\nTexture: ";
            // for (int tex : faces_tex.back()) std::cout << tex << ' ';
            // std::cout << "\nNormals: ";
            // for (int norm : faces_normals.back()) std::cout << norm << ' ';
            // std::cout << "\n#####\n";
        }

        


        
};

#endif
