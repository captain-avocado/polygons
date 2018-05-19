#ifndef MODEL_MAIN_H
#define MODEL_MAIN_H

#include <vector>
#include <polygon.h>
#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QString>
#include <QFileDialog>
#include <QDebug>
#include <QPushButton>
#include <QByteArray>
#include <math.h>


namespace Ui {
class Model_main;
}

class Model_main : public QMainWindow
{
    Q_OBJECT

public:
    explicit Model_main(QWidget *parent = 0);
    ~Model_main();


private slots:
    void on_pushButton_load_clicked();

    void on_horizontalSlider_threshold_actionTriggered();

    void on_spinBox_threshold_valueChanged(const QString &arg1);

    void on_spinBox_valueChanged(const QString &arg1);

    void on_pushButton_saveCompressed_clicked();

    void on_pushButton_loadPIF_clicked();

private:
    Ui::Model_main *ui;

    polygon *polyTree = nullptr;
    int polyCounter = 0;
    QVector<polygon*> polyVector;


    const int MAX_R = 256;

    QImage img;
    QImage grid;
    QImage res;
    void process(int P);
    double calcStandartDeviation();

protected:

public slots:


signals:

};

#endif // MODEL_MAIN_H
