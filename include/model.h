#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "vec3.h"

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
                while (std::getline(file, line))
                {
                    if (line.starts_with("v "))
                        parse_vertex(line);

                    if (line.starts_with("f "))
                        parse_face(line);
                }
                file.close();
            }

        }

        int get_nvertexs() { return (int) vertexs.size(); }

        int get_nfaces() { return (int) faces.size(); }

        Vec3<T> vertex(int idx) { return vertexs[idx]; }

        std::vector<int> face(int idx) { return faces[idx]; }


    private:
        std::vector<Vec3<T>>          vertexs;
        std::vector<std::vector<int>> faces;

        void parse_vertex(const std::string& line)
        {
            // assumes line has the format of  "v number number number"
            std::istringstream iss(line);
            char v;
            T n1, n2, n3;
            iss >> v >> n1 >> n2 >> n3;
            Vec3<T> vertex(n1, n2, n3);
            vertexs.push_back(vertex);
            
            // std::cout << line << std::endl;
            // std::cout << vertex << std::endl;
            
        }

        void parse_face(const std::string& line)
        {
            // assumes line has the format of "f number/number/number number/number/ number"
            std::istringstream iss(line);
            std::vector<int> elements;
            std::string part;
            char f;
            iss >> f;
            while (iss >> part)
            {
                std::istringstream sub_iss(part);
                std::string num;
                std::getline(sub_iss, num, '/'); // only wants the first number of each part
                
                elements.push_back(std::stoi(num)-1); // .obj file start at 1 instead of 0
            }

            faces.push_back(elements);
            
            // std::cout << line << std::endl;
            // for (int i = 0; i < faces[faces.size()-1].size(); i++)
            // {
            //     std::cout << faces[faces.size()-1].at(i) << std::endl;
            // }
        }
        
};

#endif
