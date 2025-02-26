#include <iostream>
#include <cmath>
#include <limits>

#include "tgaimage.h"
#include "model.h"
#include "vec2.h"
#include "vec3.h"
#include "matrix.h"

const TGAColor white  = TGAColor(255, 255, 255, 255);
const TGAColor red    = TGAColor(255, 0, 0, 255);
const TGAColor green  = TGAColor(0, 255, 0, 255);
const TGAColor blue   = TGAColor(0, 0, 255, 255);
const TGAColor yellow = TGAColor(255, 255, 0, 255);

void line(Vec3f p0, Vec3f p1, TGAImage &image, TGAColor color); 
void draw_model(std::string name, TGAImage &image, TGAImage &texture);
void draw_test(std::string obj_name, TGAImage &image, TGAImage &texture);
void  triangle(Vec3f a, Vec3f b, Vec3f c, Vec2f *, float *, TGAImage &image, TGAImage &texture, float intensity);
Vec3f compute_barycentric_weights(Vec3f *pts, Vec3f p);
float compute_edge_function(Vec3f a, Vec3f b, Vec3f p);
void test(TGAImage &image);

float *zbuffer = NULL;
Vec3f light_dir(0, 0, -1);
Vec3f camera(0, 0, 5);
const int WIDTH = 800;
const int HEIGHT = 800;
const int DEPTH = 255;

Matf get_viewport(int x, int y, int w, int h)
{
    Matf mat = get_identity<float>(4, 4);
    mat[0][3] = x+w/2.f;
    mat[1][3] = y+h/2.f;
    mat[2][3] = DEPTH/2.f;

    mat[0][0] = w/2.f;
    mat[1][1] = h/2.f;
    mat[2][2] = DEPTH/2.f;

    return mat;
}

Matf projection = get_identity<float>(4, 4);
Matf viewport   = get_viewport(WIDTH/8, HEIGHT/8, WIDTH * 3/4, HEIGHT * 3/4); 

int main()
{
    std::cout << "++ Renderer ++" << std::endl;
    TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);
    TGAImage texture(0, 0, TGAImage::RGB);
    texture.read_tga_file("./models/african_head/Texture/african_head_diffuse.tga");
    // texture.read_tga_file("./models/skull/Texture/skull_albedo.tga");
    // texture.read_tga_file("./models/scooter/Texture/texture.tga");
    texture.flip_vertically();
    projection[3][2] = -1/camera.z();
    draw_model("./models/african_head/african_head.obj", image, texture);
    // draw_model("./models/skull/skull.obj", image, texture);
    // draw_model("./models/scooter/scooter.obj", image, texture);
    image.flip_vertically();
    image.write_tga_file("output.tga");
    return 0;
}

void test(TGAImage &image)
{
    Vec3f center(50, 50, 1);

    Vec3f pa(30, 70, 1); 
    Vec3f pb(50, 70, 1); 
    Vec3f pc(30, 30, 1); 
    Vec3f pd(70, 30, 1); 
    Vec3f pe(70, 50, 1); 

    Matf pam = vec3_to_mat(pa);
    Matf pbm = vec3_to_mat(pb);
    Matf pcm = vec3_to_mat(pc);
    Matf pdm = vec3_to_mat(pd);
    Matf pem = vec3_to_mat(pe);

    Matf pamTrans = pam - vec3_to_mat(center);  // Translation for pam
    Matf pbmTrans = pbm - vec3_to_mat(center);  // Translation for pbm
    Matf pcmTrans = pcm - vec3_to_mat(center);  // Translation for pcm
    Matf pdmTrans = pdm - vec3_to_mat(center);  // Translation for pdm
    Matf pemTrans = pem - vec3_to_mat(center);  // Translation for pem

    line(mat_to_vec3(pam), mat_to_vec3(pbm), image, green);
    line(mat_to_vec3(pbm), mat_to_vec3(pem), image, green);
    line(mat_to_vec3(pdm), mat_to_vec3(pcm), image, green);
    line(mat_to_vec3(pcm), mat_to_vec3(pam), image, green);
    line(mat_to_vec3(pdm), mat_to_vec3(pem), image, green);

    Matf rotate = get_identity<float>(4, 4);
    double angle = -180.f;
    double rad = angle * M_PI / 180.0;

    rotate[0][0] = cos(rad);
    rotate[0][2] = sin(rad);
    rotate[2][0] = -sin(rad);
    rotate[2][2] = cos(rad);

    // Apply the rotation to each point
    pamTrans = rotate * pamTrans;
    pbmTrans = rotate * pbmTrans;
    pcmTrans = rotate * pcmTrans;
    pdmTrans = rotate * pdmTrans;
    pemTrans = rotate * pemTrans;

    // Translate the points back to their original position (center + rotation)
    pam = pamTrans + vec3_to_mat(center);
    pbm = pbmTrans + vec3_to_mat(center);
    pcm = pcmTrans + vec3_to_mat(center);
    pdm = pdmTrans + vec3_to_mat(center);
    pem = pemTrans + vec3_to_mat(center);

    // Draw the rotated square
    line(mat_to_vec3(pam), mat_to_vec3(pbm), image, red);
    line(mat_to_vec3(pbm), mat_to_vec3(pem), image, red);
    line(mat_to_vec3(pdm), mat_to_vec3(pcm), image, red);
    line(mat_to_vec3(pcm), mat_to_vec3(pam), image, red);
    line(mat_to_vec3(pdm), mat_to_vec3(pem), image, red);

}

void draw_test(std::string obj_name, TGAImage &image, TGAImage &texture)
{
    Model<float> model(obj_name);

    zbuffer = new float[WIDTH * HEIGHT];
    for (int i = 0; i < HEIGHT * WIDTH; i++)
        zbuffer[i] = -std::numeric_limits<float>::max();

    for (int i = 0; i < model.get_nfaces(); i++)
    {
        std::vector<int> face = model.face(i);
        std::vector<int> face_texture = model.face_texture(i);

        Vec3f screen_coords[3];
        Vec3f world_coords[3];
        Vec2f uv_coords[3];
        for (int j = 0; j < 3; j++)
        {
            Vec3f v = model.vertex(face[j]);
            // screen_coords[j] = Vec3f(int((v.x()+1) * WIDTH/2.), int((v.y() + 1) * HEIGHT/2.), int(v.z())); 
            screen_coords[j] = mat_to_vec3(viewport * projection * vec3_to_mat(v));
            world_coords[j] = v;
            if (model.get_ntex_coords() > 0){
                Vec2f uv = model.texture_uv(face_texture[j]);
                uv_coords[j] = uv;
            }

        }
        Vec3f n = cross((world_coords[2] - world_coords[0]), (world_coords[1] - world_coords[0])); 
        n = unit_vector(n);
        float intensity = dot(n, light_dir);
        if (intensity > 0)
        {
            triangle(screen_coords[0], screen_coords[1], screen_coords[2],
                    uv_coords, zbuffer, image, texture, intensity);
        }
    }
}

void draw_model(std::string obj_name, TGAImage &image, TGAImage &texture)
{
    Model<float> model(obj_name);
    zbuffer = new float[WIDTH * HEIGHT];

    for (int i = 0; i < HEIGHT * WIDTH; i++)
        zbuffer[i] = -std::numeric_limits<float>::max();

    for (int i = 0; i < model.get_nfaces(); i++)
    {
        std::vector<int> face = model.face(i);
        std::vector<int> face_texture = model.face_texture(i);

        Vec3f screen_coords[3];
        Vec3f world_coords[3];
        Vec2f uv_coords[3];
        for (int j = 0; j < 3; j++)
        {
            // screen_coords[j] = Vec3f(int((v.x()+1) * WIDTH/2.), int((v.y() + 1) * HEIGHT/2.), int(v.z())); 
            Vec3f v = model.vertex(face[j]);
            screen_coords[j] = mat_to_vec3(viewport * projection * vec3_to_mat(v));
            world_coords[j] = v;
            if (model.get_ntex_coords() > 0){
                Vec2f uv = model.texture_uv(face_texture[j]);
                uv_coords[j] = uv;
            }
        }
        Vec3f n = cross((world_coords[2] - world_coords[0]), (world_coords[1] - world_coords[0])); 
        n = unit_vector(n);
        float intensity = dot(n, light_dir);
        if (intensity > 0)
        {
            // TGAColor color(uv_coords[0].x() * intensity * 255, uv_coords[0].y() * intensity * 255, intensity * 255, 255);
            triangle(screen_coords[0], screen_coords[1], screen_coords[2],
                    uv_coords, zbuffer, image, texture, intensity);
        }
    }
}

void triangle(Vec3f a, Vec3f b, Vec3f c, Vec2f *uvs, float *zbuffer, TGAImage &image, TGAImage &texture, float intensity)
{
    // Ensures there is a clockwise order
    if (compute_edge_function(a, b, c) < 0)
        std::swap(b, c);

    float lowest_x = std::fmin(a.x(), b.x());
    lowest_x = std::fmin(lowest_x, c.x());

    float lowest_y = std::fmin(a.y(), b.y());
    lowest_y = std::fmin(lowest_y, c.y());

    float highest_x = std::fmax(a.x(), b.x());
    highest_x = std::fmax(highest_x, c.x());

    float highest_y = std::fmax(a.y(), b.y());
    highest_y = std::fmax(highest_y, c.y());
    
    Vec3f pts[3] = {a, b, c};
    // std::cout << highest_x << " " << highest_y << std::endl;
    for (int y = lowest_y; y <= highest_y; y++)
    {
        for (int x = lowest_x; x <= highest_x; x++)
        {
            Vec3f p(x, y, 0);
            Vec3f weights = compute_barycentric_weights(pts, p);

            if (weights[0] >= 0 && weights[1] >= 0 && weights[2] >=0)
            {
                for (int i = 0; i < 3; i++)
                    p[2] += pts[i].z() * weights[i];

                int idx = int(p.x() + p.y() * WIDTH);
                // std::cout << zbuffer[idx] << " " << p.z() << std::endl;
                if (zbuffer[idx] < p.z())
                {
                    zbuffer[idx] = p.z();
                    if (texture.get_width() == 0)
                    {
                        TGAColor color = red;
                        color.r *= intensity;
                        color.g *= intensity;
                        color.b *= intensity;
                        image.set(p.x(), p.y(), color); 
                    } else {
                        Vec2f uv = uvs[0] * weights[0] + uvs[1] * weights[1] + uvs[2] * weights[2];
                        int text_x = uv.x() * texture.get_width();
                        int text_y = uv.y() * texture.get_height();
                        TGAColor color = texture.get(text_x, text_y);
                        color.r *= intensity;
                        color.g *= intensity;
                        color.b *= intensity;
                        
                        image.set(p.x(), p.y(), color); 
                
                    }
                }
            }
        }
    }
    
}

float compute_edge_function(Vec3f a, Vec3f b, Vec3f p)
{
    return ((b.x() - a.x()) * (p.y() - a.y())) - ((b.y() - a.y()) * (p.x() - a.x())); 
}

Vec3f compute_barycentric_weights(Vec3f *pts, Vec3f p)
{
    float edge = compute_edge_function(pts[0], pts[1], pts[2]);
    if (edge == 0.0f)
        return Vec3f(0.0f, 0.0f, 0.0f);

    Vec3f weights;
    weights[0] = (float) compute_edge_function(pts[1], pts[2], p) / edge;
    weights[1] = compute_edge_function(pts[2], pts[0], p) / edge;
    weights[2] = compute_edge_function(pts[0], pts[1], p) / edge;

    return weights;
}

void line(Vec3f p0, Vec3f p1, TGAImage &image, TGAColor color) { 
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
