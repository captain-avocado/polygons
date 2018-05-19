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

void polygon::split(const QImage &img, QImage &grid, int P, int &polyCounter)
{
    if (reqSplit(img, P)) {

        polyCounter += 4;
        isEmpty = false;
        draw(grid);

        LT = new polygon(x0, y0, R/2);
        LD = new polygon(x0, y0 + R/2, R/2);
        RT = new polygon(x0 + R/2, y0, R/2);
        RD = new polygon(x0 + R/2, y0 + R/2, R/2);

        LT->split(img, grid, P, polyCounter);
        LD->split(img, grid, P, polyCounter);
        RT->split(img, grid, P, polyCounter);
        RD->split(img, grid, P, polyCounter);

    } else {
        setIntensity(getIntensityFromPic(img));
    }
}

void polygon::bfs(QString &fileData) {

    //очередь обрабатываемых элементов дерева
    QQueue<polygon*> queue;
    queue.enqueue(this);

    while (!queue.isEmpty())
    {
        polygon* tmpNode = queue.dequeue();

        fileData += QString::number(tmpNode->isEmpty) + ","
                + QString::number(tmpNode->x0) + ","
                + QString::number(tmpNode->y0) + ","
                + QString::number(tmpNode->R) + "|";

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

void polygon::bfsTreeToArray(QVector<polygon*> &polyVector) {

    //очередь обрабатываемых элементов дерева
    QQueue<polygon*> queue;
    queue.enqueue(this);

    while (!queue.isEmpty())
    {
        polygon* tmpNode = queue.dequeue();
        polyVector.push_back(tmpNode);

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

int polygon::bfsPolyCount(int size) {

    //очередь обрабатываемых элементов дерева
    QQueue<polygon*> queue;
    queue.enqueue(this);
    int counter = 0;

    while (!queue.isEmpty())
    {
        polygon* tmpNode = queue.dequeue();

        if (tmpNode->getR() < size) return counter;

        if (tmpNode->getR() == size) {
            counter++;
        }

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

    return counter;
}

void polygon::bfsCompressed(QByteArray &fileData) {

    QQueue<polygon*> queue;
    queue.enqueue(this);

    while (!queue.isEmpty())
    {
        polygon* tmpNode = queue.dequeue();

        if (tmpNode->isEmpty) {
            fileData += char(tmpNode->x0);
            fileData += char(tmpNode->y0);
            fileData += char((tmpNode->R)-1);
            fileData += char(tmpNode->intensity);

            qDebug() << (char)(tmpNode->x0) <<
                        (char)(tmpNode->y0) <<
                        (char)((tmpNode->R) - 1) <<
                        (char)(tmpNode->intensity);
        }

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

void polygon::postOrder(QByteArray &fileData) {
    if (isEmpty) {
        fileData += QByteArray::number(x0) + " "
                + QByteArray::number(y0) + " "
                + QByteArray::number(R) + " ";

//        QByteArray newx = x0;
//        qDebug() << x0 << newx;

    } else {
        LT->postOrder(fileData);
        LD->postOrder(fileData);
        RT->postOrder(fileData);
        RD->postOrder(fileData);
    }
    return;
}

void polygon::postOrderFormPic(QImage &res) {
    if (isEmpty) {
        for (int i = x0; i < x0 + R; i++) {
            for (int j = y0; j < y0 + R; j++) {
                res.setPixel(i, j, qRgb(intensity, intensity, intensity));
            }
        }
    } else {
        LT->postOrderFormPic(res);
        LD->postOrderFormPic(res);
        RT->postOrderFormPic(res);
        RD->postOrderFormPic(res);
    }
    return;
}

