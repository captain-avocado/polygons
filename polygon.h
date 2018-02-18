#ifndef POLYGON_H
#define POLYGON_H


class polygon
{
private:
    int x0, y0, R;
public:
    polygon(int x0, int y0, int R);
    int getR() { return R; }
    int getX0() { return x0; }
    int getY0() { return y0; }
};

#endif // POLYGON_H
