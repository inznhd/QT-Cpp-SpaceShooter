#include "editor.h"
#include "ui_editor.h"

editor::editor(std::string levelPath,int newLevel,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::editor),
    newLevel(newLevel),
    levelPath(levelPath)
{
    ui->setupUi(this);
    // 禁用所有编辑触发方式
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ini_row(0);
    bulletAmount.push_back(0);
    std::vector<double>vec;
    bulletData.push_back(vec);

}

editor::~editor()
{
    delete ui;
}

void editor::typeChange(int row,int type)
{
    DeleteCellWidget(row,1);
    DeleteCellWidget(row,7);
    DeleteCellWidget(row,8);
    switch (type) {
    case 1:
    {
            QComboBox *combobox2= new QComboBox(ui->tableWidget);
            combobox2->addItem({"1"});
            combobox2->addItem("2");
            combobox2->addItem("3");
            ui->tableWidget->setCellWidget(row,1,combobox2);
        break;
    }
    case 2:
    {
        QComboBox *combobox2= new QComboBox(ui->tableWidget);
        combobox2->addItem({"1"});
        combobox2->addItem("2");
        combobox2->addItem("3");
        ui->tableWidget->setCellWidget(row,1,combobox2);
        QPushButton *button = new QPushButton("设置子弹", ui->tableWidget);
        connect(button,&QPushButton::clicked,[this](){setBulletClicked(ui->tableWidget->currentRow());});
        ui->tableWidget->setCellWidget(row, 8, button);
    break;
    }
    case 3:
    {
        QComboBox *combobox3= new QComboBox(ui->tableWidget);
        combobox3->addItem("1");
        combobox3->addItem("2");
        ui->tableWidget->setCellWidget(row,1,combobox3);
        QPushButton *button = new QPushButton("设置子弹", ui->tableWidget);
        connect(button,&QPushButton::clicked,[this](){setBulletClicked(ui->tableWidget->currentRow());});
        ui->tableWidget->setCellWidget(row, 8, button);
        QDoubleSpinBox *spinbox = new QDoubleSpinBox(ui->tableWidget);
        spinbox->setRange(0,300000);
        ui->tableWidget->setCellWidget(row,7,spinbox);
    break;
    }
    default:
    {
        qDebug()<<"default";
    }
    }
}


// 检查并删除指定单元格(row, column)的控件
void editor::DeleteCellWidget( int row, int column)
{
    QWidget *widget = ui->tableWidget->cellWidget(row, column);
    if (widget)
    {
        ui->tableWidget->removeCellWidget(row, column);
        // 现在可以安全地删除控件
        delete widget;
    }
}

void editor::on_newButton_clicked()
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ini_row(ui->tableWidget->rowCount()-1);
    bulletAmount.push_back(0);
    std::vector<double>vec;
    bulletData.push_back(vec);
}

void editor::ini_row(int row)
{
    QComboBox *combobox1= new QComboBox(ui->tableWidget);
    combobox1->addItem("1");
    combobox1->addItem("2");
    combobox1->addItem("3");
    ui->tableWidget->setCellWidget(row,0,combobox1);

    QDoubleSpinBox *spinbox = new QDoubleSpinBox(ui->tableWidget);
    spinbox->setRange(0,700);
    ui->tableWidget->setCellWidget(row,2,spinbox);
    QDoubleSpinBox *spinbox2 = new QDoubleSpinBox(ui->tableWidget);
    spinbox2->setRange(0,800);
    ui->tableWidget->setCellWidget(row,3,spinbox2);
    QDoubleSpinBox *spinbox3 = new QDoubleSpinBox(ui->tableWidget);
    spinbox3->setRange(0,2);
    ui->tableWidget->setCellWidget(row,4,spinbox3);
    QDoubleSpinBox *spinbox4 = new QDoubleSpinBox(ui->tableWidget);
    spinbox4->setRange(0,2);
    ui->tableWidget->setCellWidget(row,5,spinbox4);
    QDoubleSpinBox *spinbox5 = new QDoubleSpinBox(ui->tableWidget);
    spinbox5->setRange(0,500);
    ui->tableWidget->setCellWidget(row,6,spinbox5);
    QComboBox *combobox2= new QComboBox(ui->tableWidget);
    combobox2->addItem("1");
    combobox2->addItem("2");
    combobox2->addItem("3");
    ui->tableWidget->setCellWidget(row,1,combobox2);
    connect(combobox1,&QComboBox::currentTextChanged,[this,combobox1]()
    {
        for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
        {
            if (ui->tableWidget->cellWidget(row, 0) == combobox1)
            {
                typeChange(row,combobox1->currentIndex()+1);
                return;
            }
        }
    });
}

void editor::on_pushButton_2_clicked()
{
    int row = ui->tableWidget->currentRow();
    if(ui->tableWidget->rowCount()>=2)
    {
        bulletAmount.erase(bulletAmount.begin() + row);
        bulletData.erase(bulletData.begin() + row);
        ui->tableWidget->removeRow(row);
    }
}

void editor::setBulletClicked(int row)
{
    editorBullet *dia = new editorBullet(this);
    dia->setAttribute(Qt::WA_DeleteOnClose); // 设置关闭时自动删除
    dia->show();
    connect(dia,&editorBullet::sendData,[row,this](int amount,double* array)
    {
        this->bulletAmount[row]=amount;
        for(int i=0;i<amount*6;i++)
        {
            (bulletData.at(row)).push_back(array[i]);
        }
    });


}

void editor::on_writeButton_clicked()
{
    int enemySize=8;//在头部还要存一个int敌人数量,和int结尾检查
    int check=0;
    std::string message1="下列敌机子弹未设置：";
    std::string message2="下列敌机生命值未设置：";
    std::string message;

    //先自查数据是否提供全
    for(int r=0;r<ui->tableWidget->rowCount();r++)
    {
        int type;
        QComboBox *qcb = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(r,0));
        type=qcb->currentIndex()+1;
        switch (type) {
        case 1:
        {
            enemySize+=48;
            qDebug()<<"1";
            break;
        }
        case 2:
        {
            if(bulletAmount[r]==0)
            {
                message1+=std::to_string(r+1);
                check=1;
            }
            enemySize+=48+4+8*6*bulletAmount[r];
            break;
        }
        case 3:
        {
            if(bulletAmount[r]==0)
            {
                message1+=std::to_string(r+1);
                message1+=',';
                check=1;
            }
            QDoubleSpinBox *spinbox = qobject_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget(r,7));
            if(spinbox->value()==0)
            {
                message2+=std::to_string(r+1);
                message2+=',';
                check=1;
            }
            enemySize+=48+8+4+8*6*bulletAmount[r];
            break;
        }
        }
    }
    if(check==1)
    {
        //数据不全
        message=message1+message2;
        QMessageBox::information(this, "提示",QString::fromStdString(message), QMessageBox::Ok);
    }

    //开始写文件
    else
    {
        std::fstream fs;
        fs.open(levelPath,std::ios::in|std::ios::out|std::ios::binary);
        fs.seekp(-4,std::ios::end);

        fs.write(reinterpret_cast<const char*>(&newLevel),sizeof (int));
        int levelcheck=newLevel*2+12345;
        fs.write(reinterpret_cast<const char*>(&levelcheck),sizeof (int));
        fs.write(reinterpret_cast<const char*>(&enemySize),sizeof (int));
        int enemyNumber = ui->tableWidget->rowCount();
        fs.write(reinterpret_cast<const char*>(&enemyNumber),sizeof (int));

        for(int r=0;r<ui->tableWidget->rowCount();r++)
        {
            int type,kind;
            double x,y,vx,vy,st,health;

            QComboBox *qcb = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(r,0));
            type=qcb->currentIndex()+1;
            QComboBox *qcb1 = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(r,1));
            kind=qcb1->currentIndex()+1;
            QDoubleSpinBox *qdsb2 = qobject_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget(r,2));
            x=qdsb2->value();
            QDoubleSpinBox *qdsb3 = qobject_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget(r,3));
            y=qdsb3->value();
            QDoubleSpinBox *qdsb4 = qobject_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget(r,4));
            vx=qdsb4->value();
            QDoubleSpinBox *qdsb5 = qobject_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget(r,5));
            vy=qdsb5->value();
            QDoubleSpinBox *qdsb6 = qobject_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget(r,6));
            st=qdsb6->value();

            fs.write(reinterpret_cast<const char*>(&type),sizeof(int));
            fs.write(reinterpret_cast<const char*>(&kind),sizeof(int));
            fs.write(reinterpret_cast<const char*>(&x),sizeof(double));
            fs.write(reinterpret_cast<const char*>(&y),sizeof(double));
            fs.write(reinterpret_cast<const char*>(&vx),sizeof(double));
            fs.write(reinterpret_cast<const char*>(&vy),sizeof(double));
            fs.write(reinterpret_cast<const char*>(&st),sizeof(double));

            switch (type) {
            case 1:
            {
                break;
            }
            case 2:
            {
                fs.write(reinterpret_cast<const char*>(&bulletAmount.at(r)),sizeof (int));
                fs.write(reinterpret_cast<const char*>((bulletData.at(r)).data()),sizeof(double)*bulletAmount[r]*6);
                break;
            }
            case 3:
            {

                QDoubleSpinBox *spinbox = qobject_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget(r,7));
                health = spinbox->value();
                fs.write(reinterpret_cast<const char*>(&health),sizeof (double));
                fs.write(reinterpret_cast<const char*>(&bulletAmount.at(r)),sizeof (int));
                fs.write(reinterpret_cast<const char*>((bulletData.at(r)).data()),sizeof(double)*bulletAmount[r]*6);
                break;
            }
            }
        }
        int endcheck=newLevel*3+12345;
        fs.write(reinterpret_cast<const char*>(&endcheck),sizeof (int));
        int fileEndSign = -1;
        fs.write(reinterpret_cast<const char*>(&fileEndSign),sizeof (int));
        fs.close();
        QMessageBox::information(this, "提示","成功", QMessageBox::Ok);
        emit success();
        this->close();
    }
}

void editor::closeEvent(QCloseEvent *event)
{
    emit editorClose();
    event->accept();//告诉qt事件处理完成
}
