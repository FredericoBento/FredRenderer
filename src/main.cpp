#include <iostream>
#include <cmath>

#include "tgaimage.h"
#include "model.h"
#include "vec2.h"
#include "vec3.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
const TGAColor yellow = TGAColor(255, 255, 0, 255);


void line(Vec2i p0, Vec2i p1,  TGAImage &image, TGAColor color);
void draw_model(std::string obj_name, TGAImage &image, TGAColor color);
void draw_model_colored(std::string obj_name, TGAImage &image);
void draw_model_lighten(std::string obj_name, TGAImage &image);
void draw_triangle(Vec2i , Vec2i , Vec2i , TGAImage &image, TGAColor color);
int edge_function(Vec2i a, Vec2i b, Vec2i c);

int main()
{
    std::cout << "++ Renderer ++" << std::endl;
    TGAImage image(1000, 1000, TGAImage::RGB);

    // draw_model("./obj/skull.obj", image, white);
    // draw_triangle(Vec2i(10, 70), Vec2i(50, 160), Vec2i(70, 80), image, red);
    // draw_triangle(Vec2i(180, 50), Vec2i(150, 1), Vec2i(70, 180), image, blue);
    // draw_triangle(Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180), image, green);
    // draw_triangle(Vec2i(10, 10), Vec2i(35, 50), Vec2i(60, 10), image, green);
    // draw_model_colored("./obj/skull.obj", image);
    draw_model_lighten("./obj/skull.obj", image);


    image.flip_vertically();
    image.write_tga_file("./skull_lighten.tga");
    return 0;
}

void draw_triangle(Vec2i a, Vec2i b, Vec2i c, TGAImage &image, TGAColor color)
{
    // Ensures there is a clockwise order
    if (edge_function(a, b, c) < 0)
        std::swap(b, c);

    int lowest_x = std::min(a.x(), b.x());
    lowest_x = std::min(lowest_x, c.x());

    int lowest_y = std::min(a.y(), b.y());
    lowest_y = std::min(lowest_y, c.y());

    int highest_x = std::max(a.x(), b.x());
    highest_x = std::max(highest_x, c.x());

    int highest_y = std::max(a.y(), b.y());
    highest_y = std::max(highest_y, c.y());
    
    int edge_abc = edge_function(a, b, c);

    for (int y = lowest_y; y <= highest_y; y++)
    {
        for (int x = lowest_x; x <= highest_x; x++)
        {
            Vec2i p(x, y);
            int edge_abp = edge_function(a, b, p);
            int edge_bcp = edge_function(b, c, p);
            int edge_cap = edge_function(c, a, p);

            float weight_a = (float) edge_bcp / edge_abc;
            float weight_b = (float) edge_cap / edge_abc;
            float weight_c = (float) edge_abp / edge_abc;

            // float r = 255 * weight_a;  
            // float g = 255 * weight_b;  
            // float b = 255 * weight_c;  
                            
            if (weight_a >= 0 && weight_b >= 0 && weight_c >=0)
                image.set(p.x(), p.y(), color); 
        }
    }

    // line(a, b, image, color);
    // line(b, c, image, color);
    // line(c, a, image, color);
}

int edge_function(Vec2i a, Vec2i b, Vec2i c)
{
    return ((b.x() - a.x()) * (c.y() - a.y())) - ((b.y() - a.y()) * (c.x() - a.x())); 
}


void draw_model_wireframe(std::string obj_name, TGAImage &image, TGAColor color)
{
    Model<float> model(obj_name);
    int width = image.get_width();
    int height = image.get_height();
    
    for (int i = 0; i < model.get_nfaces(); i++)
    {
        std::vector<int> face = model.face(i);
        for (int j = 0; j < 3; j++)
        {
            Vec3<float> v0 = model.vertex(face[j]);
            Vec3<float> v1 = model.vertex(face[(j+1)%3]);

            int x0 = (v0.x() + 1) * width / 2;
            int y0 = (v0.y() + 1) * height / 2;

            int x1 = (v1.x() + 1) * width / 2;
            int y1 = (v1.y() + 1) * height / 2;

            line(Vec2i(x0, y0), Vec2i(x1, y1), image, color);
        }
    }
}

void draw_model_colored(std::string obj_name, TGAImage &image)
{
    Model<float> model(obj_name);
    int width = image.get_width();
    int height = image.get_height();
    
    for (int i = 0; i < model.get_nfaces(); i++)
    {
        std::vector<int> face = model.face(i);
        Vec2i screen_coords[3];
        for (int j = 0; j < 3; j++)
        {
            Vec3f world_coords = model.vertex(face[j]);
            screen_coords[j] = Vec2i((world_coords.x()+1) * width/2,
                                    (world_coords.y() + 1) * height/2); 
        }
        TGAColor color(rand()%255, rand()%255, rand()%255, 255);
        draw_triangle(screen_coords[0], screen_coords[1], screen_coords[2],
                    image, color);
    }
}

void draw_model_lighten(std::string obj_name, TGAImage &image)
{
    Model<float> model(obj_name);
    int width = image.get_width();
    int height = image.get_height();

    Vec3f light_dir(0, 0, -1);
    
    for (int i = 0; i < model.get_nfaces(); i++)
    {
        std::vector<int> face = model.face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3];
        for (int j = 0; j < 3; j++)
        {
            Vec3f v = model.vertex(face[j]);
            screen_coords[j] = Vec2i((v.x()+1) * width/2, (v.y() + 1) * height/2); 
            world_coords[j] = v;
        }
        Vec3f n = cross((world_coords[2] - world_coords[0]), (world_coords[1] - world_coords[0])); 
        n = unit_vector(n);
        float intensity = dot(n, light_dir);
        if (intensity > 0)
        {
            TGAColor color(intensity * 255, intensity * 255, intensity * 255, intensity * 255);
            draw_triangle(screen_coords[0], screen_coords[1], screen_coords[2],
                        image, color);
        }
    }
}

void line(Vec2i p0, Vec2i p1, TGAImage &image, TGAColor color) { 
    bool steep = false; 

    int x0 = p0.x();
    int y0 = p0.y();

    int x1 = p1.x();
    int y1 = p1.y();

    if (std::abs(x0 - x1)<std::abs(y0 - y1))
    { 
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    } 

    if (x0 > x1)
    { 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 

    int dx = x1 - x0; 
    int dy = y1 - y0; 
    int derror2 = std::abs(dy) * 2; 
    int error2 = 0; 
    int y = y0; 
    for (int x = x0; x <= x1; x++)
    { 
        if (steep)
        { 
            image.set(y, x, color); 
        } else
        { 
            image.set(x, y, color); 
        } 
        error2 += derror2; 
        if (error2 > dx)
        { 
            y += (y1 > y0 ? 1: -1); 
            error2 -= dx * 2; 
        } 
    } 
} 
