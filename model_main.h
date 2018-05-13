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

    void on_pushButton_saveComplete_clicked();

    void on_pushButton_saveCompressed_clicked();

private:
    Ui::Model_main *ui;

    polygon *polyTree = nullptr;
    const int MAX_R = 256;

    std::vector<polygon> polyVector;
    QImage img;
    QImage grid;
    QImage res;
    void split(int x0, int y0, int R, int P);
    void draw(int x0, int y0, int R);
//    bool reqSplit(int x0, int y0, int R, int P);
    void process(int P);
    int getIntensityFromPic(int x0, int y0, int R);
    void formNewPic();

    void bfs(polygon* root);
protected:

public slots:


signals:

};

#endif // MODEL_MAIN_H
