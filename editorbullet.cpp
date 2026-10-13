#include "editorbullet.h"
#include "ui_editorbullet.h"

editorBullet::editorBullet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editorBullet)
{
    ui->setupUi(this);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ini_section(0);
}

editorBullet::~editorBullet()
{
    delete ui;
}

void editorBullet::on_pushButton_clicked()//新增一行
{

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ini_section(ui->tableWidget->rowCount()-1);
}

void editorBullet::ini_section(int row)
{
    QSpinBox *qsb = new QSpinBox(ui->tableWidget);
    ui->tableWidget->setCellWidget(row,0,qsb);
    QDoubleSpinBox *qdsb = new QDoubleSpinBox(ui->tableWidget);
    qdsb->setRange(0.1,10);
    ui->tableWidget->setCellWidget(row,1,qdsb);
    QComboBox *qcb = new QComboBox(ui->tableWidget);
    qcb->addItem("1");
    qcb->addItem("2");
    qcb->addItem("3");
    qcb->addItem("4");
    qcb->addItem("5");
    qcb->addItem("6");
    ui->tableWidget->setCellWidget(row,2,qcb);
    QComboBox *qcb2 = new QComboBox(ui->tableWidget);
    qcb2->addItem("垂直方向");
    qcb2->addItem("手动控制方向");
    ui->tableWidget->setCellWidget(row,3,qcb2);
    QDoubleSpinBox *qdsb2 = new QDoubleSpinBox(ui->tableWidget);
    qdsb2->setRange(-100,100);
    ui->tableWidget->setCellWidget(row,4,qdsb2);
    QDoubleSpinBox *qdsb3 = new QDoubleSpinBox(ui->tableWidget);
    qdsb3->setRange(-100,100);
    ui->tableWidget->setCellWidget(row,5,qdsb3);
}
void editorBullet::on_pushButton_2_clicked()
{
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}

void editorBullet::on_pushButton_3_clicked()
{
    bulletNumber=ui->tableWidget->rowCount();
    bulletdata = new double[bulletNumber*6];
    for(int row=0;row<bulletNumber;row++)
    {
        QSpinBox *qsp= qobject_cast<QSpinBox*>(ui->tableWidget->cellWidget(row,0));
        if(qsp)
        {
            bulletdata[row*6]=qsp->value();
        }
        else{
            qDebug()<<"bullet第0格赋值出错";
        }
        QDoubleSpinBox *qdsb = qobject_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget(row,1));
        if(qdsb)
        {
            bulletdata[row*6+1]=qdsb->value();
        }
        else{
            qDebug()<<"bullet第1格赋值出错";
        }
        QComboBox *qcb = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(row,2));
        if(qcb)
        {
            bulletdata[row*6+2]=qcb->currentIndex()+1;
        }
        else{
            qDebug()<<"bullet第2格赋值出错";
        }
        QComboBox *qcb2 = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(row,3));
        if(qcb2)
        {
            qDebug()<<qcb2->currentIndex();
            bulletdata[row*6+3]=qcb2->currentIndex();
        }
        else{
            qDebug()<<"bullet第3格赋值出错";
        }
        QDoubleSpinBox *qdsb2 = qobject_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget(row,4));
        if(qdsb2)
        {
            bulletdata[row*6+4]=qdsb2->value();
        }
        else{
            qDebug()<<"bullet第4格赋值出错";
        }
        QDoubleSpinBox *qdsb3 = qobject_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget(row,5));
        if(qdsb3)
        {
            bulletdata[row*6+5]=qdsb3->value();
        }
        else{
            qDebug()<<"bullet第5格赋值出错";
        }
    }
    emit sendData(bulletNumber,bulletdata);
    this->close();
}



