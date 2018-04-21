#include "model_main.h"
#include "ui_model_main.h"

Model_main::Model_main(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Model_main)
{
    ui->setupUi(this);

    img.load(":/resouces/img/spichki.bmp");
    img = img.scaled(MAX_R, MAX_R);
    if (img.format() != QImage::Format_Grayscale8) {
        img = img.convertToFormat(QImage::Format_Grayscale8);
    }
    ui->label_pic_src->setPixmap(QPixmap::fromImage(img));


    grid = res = QImage(MAX_R, MAX_R, QImage::Format_Grayscale8);
    grid.fill(qRgb(255,255,255));
    res.fill(qRgb(255, 255, 255));
    ui->label_pic_grid->setPixmap(QPixmap::fromImage(grid));
    ui->label_pic_res->setPixmap(QPixmap::fromImage(res));
}

Model_main::~Model_main()
{
    delete ui;
}

void Model_main::on_pushButton_load_clicked()
{
//    QDir::homePath();
    QString imgFile = QFileDialog::getOpenFileName(this, tr("Открытие изображения"), "/Users/artemkaloev/faxmepic", tr("Image Files (*.gif *.tiff *.png *.jpg *.jpeg *.bmp)"));
    if (imgFile.isNull()) return;
    img.load(imgFile);

    img = img.scaled(MAX_R, MAX_R);
    if (img.format() != QImage::Format_Grayscale8) {
        img = img.convertToFormat(QImage::Format_Grayscale8);
    }
    ui->label_pic_src->setPixmap(QPixmap::fromImage(img));
}

void Model_main::process(int P)
{
    //очистить вектор полигонов
    polyVector.clear();
    //создать сетку
    split(0, 0, 256, P);
    //отобразить сетку
    ui->label_pic_grid->setPixmap(QPixmap::fromImage(grid));
    //отобразить количество полигонов
    ui->label_polyNum->setText(QString::number(polyVector.size()));
    //отобразить количество полигонов заданного размера
    on_spinBox_valueChanged(QString::number(ui->spinBox->value()));
    //формируем результирующее изображение и отображаем его
    formNewPic();
}
void Model_main::on_horizontalSlider_threshold_actionTriggered()
{
    grid.fill(qRgb(255, 255, 255));
    res.fill(qRgb(255, 255, 255));
    int P = ui->horizontalSlider_threshold->value();
    process(P);
    ui->spinBox_threshold->setValue(P);
}

void Model_main::on_spinBox_threshold_valueChanged(const QString &arg1)
{
    grid.fill(qRgb(255,255,255));
    res.fill(qRgb(255, 255, 255));
    int P = arg1.toInt();
    process(P);
    ui->horizontalSlider_threshold->setValue(P);
}

void Model_main::on_spinBox_valueChanged(const QString &arg1)
{
    int polySize = arg1.toInt();
    int counter = 0;
    for (int i = 0; i < polyVector.size(); i++) {
        if (polyVector[i].getR() == polySize) {
            counter++;
        }
    }
    ui->label_reqPolyNum->setText(QString::number(counter));
}

void Model_main::draw(int x0, int y0, int R)
{
    for (int i = x0; i < x0 + R; i++) grid.setPixel(i, y0 + R/2 - 1, qRgb(0, 0, 0));
    for (int j = y0; j < y0 + R; j++) grid.setPixel(x0 + R/2 - 1, j, qRgb(0, 0, 0));
}

//bool Model_main::reqSplit(int x0, int y0, int R, int P)
//{
//    if (R == 1) return false;
//    int min = 255;
//    int max = 0;
//    for (int i = x0; i < x0 + R; i++) {
//        for (int j = y0; j < y0 + R; j++) {
//            int intensity = qGray(img.pixel(i, j));
//            if (intensity < min) min = intensity;
//            if (intensity > max) max = intensity;
//            if (max - min >= P) {
//                return true;
//            }
//        }
//    }
//    return false;
//}

void Model_main::split(int x0, int y0, int R, int P)
{
    polygon poly(x0, y0, R);
    if (poly.reqSplit(img, P)) {
        poly.isEmpty = false;
        draw(x0, y0, R);
        for (int x = x0; x <= x0 + R/2; x += R/2) {
            for (int y = y0; y <= y0 + R/2; y += R/2) {
                split(x, y, R/2, P);
            }
        }
    } else {
        poly.setIntensity(getIntensityFromPic(x0, y0, R));
    }
    polyVector.push_back(poly);
}

int Model_main::getIntensityFromPic(int x0, int y0, int R) {
    int intensity = 0;
    for (int i = x0; i < x0 + R; i++) {
        for (int j = y0; j < y0 + R; j++) {
            intensity += qGray(img.pixel(i, j));
        }
    }
    return intensity /= R * R;
}

void Model_main::formNewPic()
{
    qDebug() << polyVector[polyVector.size() - 1].getR() << polyVector[polyVector.size() - 2].getR() << polyVector[polyVector.size() - 3].getR() << polyVector[polyVector.size() - 4].getR();
    for (int k = 0; k < polyVector.size(); k++) {
        if (polyVector[k].isEmpty) {
            int x0 = polyVector[k].getX0();
            int y0 = polyVector[k].getY0();
            int R = polyVector[k].getR();
            int intensity = polyVector[k].getIntensity();
            for (int i = x0; i < x0 + R; i++) {
                for (int j = y0; j < y0 + R; j++) {
                    res.setPixel(i, j, qRgb(intensity, intensity, intensity));
                }
            }
        }
    }
    ui->label_pic_res->setPixmap(QPixmap::fromImage(res));
}
