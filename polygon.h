#ifndef POLYGON_H
#define POLYGON_H

#include <QImage>
//#include <QPixmap>

class polygon
{
    friend class Model_main;

private:
    int x0, y0, R, intensity;
public:
    bool isEmpty = true;
    polygon *children = nullptr;

    polygon(int x0, int y0, int R);
    int getR() { return R; }
    int getX0() { return x0; }
    int getY0() { return y0; }
    int getIntensity() { return intensity; }
    void setIntensity(int intns) { intensity = intns; }
    bool reqSplit(const QImage &img, int P);
    void draw(QImage &grid);
    int getIntensityFromPic(const QImage &img);
};

#endif // POLYGON_H
