#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QtDebug>
#include <vector>
#include "editorbullet.h"
#include <QMessageBox>
#include <fstream>
#include <QCloseEvent>
#include <QEvent>

namespace Ui {
class editor;
}

class editor : public QMainWindow
{
    Q_OBJECT

public:
    explicit editor(std::string levelPath,int newLevel,QWidget *parent);
    ~editor();

signals:
    void success();
    void editorClose();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void setBulletClicked(int row);

    void on_newButton_clicked();

    void on_pushButton_2_clicked();

    void on_writeButton_clicked();

private:
    Ui::editor *ui;
    void typeChange(int row,int tpye);
    void DeleteCellWidget( int row, int column);
    void ini_row(int row);
    std::vector<std::vector<double>>bulletData;
    std::vector<int>bulletAmount;
    int newLevel;
    std::string levelPath;

};

#endif // EDITOR_H
