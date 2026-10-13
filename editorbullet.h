#ifndef EDITORBULLET_H
#define EDITORBULLET_H

#include <QDialog>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QtDebug>
#include <QTableWidget>
#include <vector>


namespace Ui {
class editorBullet;
}

class editorBullet : public QDialog
{
    Q_OBJECT

signals:
    void sendData(int bulletNumber,double *data);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

public:
    explicit editorBullet(QWidget *parent);
    ~editorBullet();

private:
    Ui::editorBullet *ui;
    void ini_section(int row);
    void directionChange(int row);
    int bulletNumber;
    double *bulletdata;
};

#endif // EDITORBULLET_H
