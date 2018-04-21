#include "polygon.h"

polygon::polygon(int x, int y, int r)
{
    x0 = x;
    y0 = y;
    R = r;
}

bool polygon::reqSplit(QImage img, int P)
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
