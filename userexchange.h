#ifndef USEREXCHANGE_H
#define USEREXCHANGE_H

#include <QDialog>
#include <QPainter>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <fstream>
#include <QMessageBox>
#include <QtDebug>
#include <QFile>

namespace Ui {
class UserExchange;
}

class UserExchange : public QDialog
{
    Q_OBJECT

public:
    explicit UserExchange(char *nowUserName,QWidget *parent);
    void paintEvent(QPaintEvent *event)override;
    void closeEvent(QCloseEvent *) override;
    ~UserExchange();

signals:
    void logIn(int usernumber,char* username);
    void UXclose();

private slots:
    void on_enterPushButton_clicked();

    void on_registerPushButtin_clicked();

private:
    bool copyAndRenameFile(const QString& sourcePath, const QString& targetPath);
    Ui::UserExchange *ui;
    char nextUserName[16]={'\0'};
    char nextPassword[32]={'\0'};
};

#endif // USEREXCHANGE_H
