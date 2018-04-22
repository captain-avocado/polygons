#include "polygon.h"

polygon::polygon(int x, int y, int r)
{
    x0 = x;
    y0 = y;
    R = r;
}

bool polygon::reqSplit(const QImage &img, int P)
{
    if (R == 1) return false;
    int min = 255;
    int max = 0;
    for (int i = x0; i < x0 + R; i++) {
        for (int j = y0; j < y0 + R; j++) {
            int intensity = qGray(img.pixel(i, j));
            if (intensity < min) min = intensity;
            if (intensity > max) max = intensity;
            if (max - min >= P) {
                return true;
            }
        }
    }
    return false;
}

void polygon::draw(QImage &grid)
{
    for (int i = x0; i < x0 + R; i++) grid.setPixel(i, y0 + R/2 - 1, qRgb(0, 0, 0));
    for (int j = y0; j < y0 + R; j++) grid.setPixel(x0 + R/2 - 1, j, qRgb(0, 0, 0));
}

int polygon::getIntensityFromPic(const QImage &img) {
    int intensity = 0;
    for (int i = x0; i < x0 + R; i++) {
        for (int j = y0; j < y0 + R; j++) {
            intensity += qGray(img.pixel(i, j));
        }
    }
    return intensity /= R * R;
}
