#include "model_main.h"
#include "ui_model_main.h"

Model_main::Model_main(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Model_main)
{
    ui->setupUi(this);
    img.load(":/resources/img/spichki.bmp");
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
    polyVector.clear();

    polyCounter = 0;
    polyTree = nullptr;

    //создать дерево полигонов и сетку
    polyTree = new polygon(0, 0, 256);
    polyCounter++;
    polyTree->split(img, grid, P, polyCounter);
    polyTree->bfsTreeToArray(polyVector);

    //отобразить сетку
    ui->label_pic_grid->setPixmap(QPixmap::fromImage(grid));
    //отобразить количество полигонов
    ui->label_polyNum->setText(QString::number(polyCounter));
    //отобразить количество полигонов заданного размера
    on_spinBox_valueChanged(QString::number(ui->spinBox->value()));
    //формируем результирующее изображение и отображаем его
    polyTree->postOrderFormPic(res);
    ui->label_pic_res->setPixmap(QPixmap::fromImage(res));

    ui->label_deviationNum->setText(QString::number(calcStandartDeviation()));
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
    int counter = polyTree->bfsPolyCount(polySize);
    ui->label_reqPolyNum->setText(QString::number(counter));
}

void Model_main::on_pushButton_saveCompressed_clicked()
{
    if (polyTree != nullptr) {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::homePath());

        if (!fileName.isEmpty()) {
            QFile fileCompressed(fileName);
            if (fileCompressed.open(QIODevice::WriteOnly)) {
                QByteArray fileData;
                polyTree->bfsCompressed(fileData);
                fileCompressed.write(fileData);

                fileCompressed.close();
            }

        }
    }
}

double Model_main::calcStandartDeviation() {
    int sum = 0;
    for (int i = 0; i < MAX_R; i++) {
        for (int j = 0; j < MAX_R; j++) {
            sum += pow((qGray(img.pixel(i, j)) - qGray(res.pixel(i, j))), 2);
        }
    }
    return sqrt(sum/(MAX_R * MAX_R));
}

void Model_main::on_pushButton_loadPIF_clicked()
{
    QString imgFile = QFileDialog::getOpenFileName(this, tr("Открытие изображения"), QDir::homePath());
    QFile fileCompressed(imgFile);

    if (fileCompressed.open(QIODevice::ReadOnly)) {

        QByteArray data = fileCompressed.readAll();

        int x = 1;
        int x0, y0, R, intensity;

        for (int i = 0; i < data.length(); i++) {
                if (x == 5) x = 1;
                if (x == 1) x0 = (int)(unsigned char)data[i];
                if (x == 2) y0 = (int)(unsigned char)data[i];
                if (x == 3) R = (int)(unsigned char)data[i] + 1;
                if (x == 4) intensity = (int)(unsigned char)data[i];

                x++;
                if (x == 5) {
                    for (int k = x0; k < x0 + R; k++) {
                        for (int j = y0; j < y0 + R; j++) {
                            img.setPixel(k, j, qRgb(intensity, intensity, intensity));
                        }
                    }
                }
            }

        fileCompressed.close();
        ui->label_pic_src->setPixmap(QPixmap::fromImage(img));
    }

}
