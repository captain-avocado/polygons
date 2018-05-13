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

void polygon::split(const QImage &img, QImage &grid, std::vector<polygon> &polyVector, int P)
{
    if (reqSplit(img, P)) {
        isEmpty = false;
        draw(grid);

        LT = new polygon(x0, y0, R/2);
        LD = new polygon(x0, y0 + R/2, R/2);
        RT = new polygon(x0 + R/2, y0, R/2);
        RD = new polygon(x0 + R/2, y0 + R/2, R/2);

        LT->split(img, grid, polyVector, P);
        LD->split(img, grid, polyVector, P);
        RT->split(img, grid, polyVector, P);
        RD->split(img, grid, polyVector, P);

    } else {
        setIntensity(getIntensityFromPic(img));
    }
    polyVector.push_back(*this);
}

void polygon::bfs(QString &fileData) {

    //очередь обрабатываемых элементов дерева
    QQueue<polygon*> queue;

//    QFile file("/Users/artemkaloev/file");
//    file.open(QIODevice::WriteOnly);

    queue.enqueue(this);

    while (!queue.isEmpty())
    {
        polygon* tmpNode = queue.dequeue();

        fileData += QString::number(tmpNode->isEmpty);
        fileData += ",";
        fileData += QString::number(tmpNode->x0);
        fileData += ",";
        fileData += QString::number(tmpNode->y0);
        fileData += ",";
        fileData += QString::number(tmpNode->R);
        fileData += "|";

        if (tmpNode->LT) {
            queue.enqueue(tmpNode->LT);
        }
        if (tmpNode->LD) {
            queue.enqueue(tmpNode->LD);
        }
        if (tmpNode->RT) {
            queue.enqueue(tmpNode->RT);
        }
        if (tmpNode->RD) {
            queue.enqueue(tmpNode->RD);
        }
    }

}

void polygon::postOrder(QString &fileData) {
    if (isEmpty) {
        fileData += QString::number(x0);
        fileData += ",";
        fileData += QString::number(y0);
        fileData += ",";
        fileData += QString::number(R);
        fileData += "|";
    } else {
        LT->postOrder(fileData);
        LD->postOrder(fileData);
        RT->postOrder(fileData);
        RD->postOrder(fileData);
    }
    return;
}

