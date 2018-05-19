#ifndef POLYGON_H
#define POLYGON_H

#include <QImage>
#include <QDebug>
#include <QQueue>
#include <QFile>
#include <QVector>
//#include <QPixmap>

class polygon
{
    friend class Model_main;

private:
    int x0, y0, R, intensity;
public:
    bool isEmpty = true;
//    polygon *children = nullptr;
    polygon *LT = nullptr, *RT= nullptr, *RD= nullptr, *LD= nullptr;


    polygon(int x0, int y0, int R);
    int getR() { return R; }
    int getX0() { return x0; }
    int getY0() { return y0; }
    int getIntensity() { return intensity; }
    void setIntensity(int intns) { intensity = intns; }
    bool reqSplit(const QImage &img, int P);
    void draw(QImage &grid);
    int getIntensityFromPic(const QImage &img);
    void split(const QImage &img, QImage &grid, int P, int &polyCounter);

    //алгоритм обхода дерева полигонов в ширину
    void bfs(QString &fileData);
    int bfsPolyCount(int size);
    void bfsTreeToArray(QVector<polygon*> &polyVector);
    void bfsCompressed(QByteArray &fileData);

    //алгоритм поиска листьев в дереве полигонов
    void postOrder(QByteArray &fileData);
    void postOrderFormPic(QImage &res);

    QByteArray convertIntToByteArray(int n);

};

#endif // POLYGON_H
