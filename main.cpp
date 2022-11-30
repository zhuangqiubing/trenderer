#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);
const int width  = 200;
const int height = 200;

void line(Vec2i p0, Vec2i p1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (std::abs(p0.x-p1.x)<std::abs(p0.y-p1.y)) {
        std::swap(p0.x, p0.y);
        std::swap(p1.x, p1.y);
        steep = true;
    }
    if (p0.x>p1.x) {
        std::swap(p0, p1);
    }

    for (int x=p0.x; x<=p1.x; x++) {
        float t = (x-p0.x)/(float)(p1.x-p0.x);
        int y = p0.y*(1.-t) + p1.y*t;
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
    }
}

//void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color) {
//    line(t0, t1, image, color);
//    line(t1, t2, image, color);
//    line(t2, t0, image, color);
//}

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color)
{
    if (t0.y > t1.y) std::swap(t0, t1);
    if (t0.y > t2.y) std::swap(t0, t2);
    if (t1.y > t2.y) std::swap(t1, t2);

    float alph = float(t2.x - t0.x) / (t2.y - t0.y);
    float beta = float(t1.x - t0.x) / (t1.y - t0.y);
    float x1 = t0.x;
    float x2 = t0.x;
    float x_left = 0;
    float x_right = 0;
    image.set(t0.x, t0.y, color);
    for (int y = t0.y; y < t1.y+1; y++)
    {
        x_left = x1;
        x_right = x2;
        if (x_left > x_right) std::swap(x_left, x_right);
        for (int x = int(x_left)+1; x < x_right+1; x++)
        {
            image.set(x, y, color);
        }
        x1 += alph;
        x2 += beta;
    }
    alph = float(t0.x - t2.x) / (t2.y - t0.y);
    beta = float(t1.x - t2.x) / (t2.y - t1.y);
    x1 = t2.x;
    x2 = t2.x;
    for (int y = t2.y; y > t1.y; y--)
    {
        x_left = x1;
        x_right = x2;
        if (x_left > x_right) std::swap(x_left, x_right);
        for (int x = int(x_left) + 1; x < x_right + 1; x++)
        {
            image.set(x, y, color);
        }
        x1 += alph;
        x2 += beta;
    }
}

int main(int argc, char** argv) {
    TGAImage image(width, height, TGAImage::RGB);

    Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
    Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};

    triangle(t0[0], t0[1], t0[2], image, red);
    triangle(t1[0], t1[1], t1[2], image, white);
    triangle(t2[0], t2[1], t2[2], image, green);


    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}

