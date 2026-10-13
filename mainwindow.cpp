#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(width,height);
    //先进入默认用户
    readFile(0);
    userNumber=0;
    //默认用户名已经初始化
    exchangeUser();
    //加载并放缩选择战机的图片
    for(int i=0;i<4;i++)
    {
        QString heroPath = ":/hero/";
        QPixmap pix;
        pix.load(heroPath+=QString::number(i+1)+".png");
        scalledPlanePix[i] = pix.scaled(pixRect.at(1).size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    for(int i=0;i<7;i++)
    {
        QString bulletPath = ":/heroBullet/heroBullet0";
        QPixmap pix;
        pix.load(bulletPath+=QString::number(i+1)+".png");
        scalledBulletPix[i] = pix.scaled(pixRect.at(1).size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

//主页的绘制函数（Qt自动控制）
void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);//暂时没用到

    //绘制背景图
    QPainter painter(this);
    painter.drawPixmap(rect(),QPixmap(":/scene/mainscene.jpg"));//等待放入资源文件并更改路径
    //初始化一些笔刷
    QPen levelpen(QColor(0,0,0,50));
    painter.setPen(levelpen);
    QBrush levelbrush(QColor(255,255,255,50));
    painter.setBrush(levelbrush);
    //绘制左右翻页的箭头
    QPoint pointR[3]={QPoint(1090,274),QPoint(1130,324),QPoint(1090,374)};
    QPoint pointL[3]={QPoint(60,274),QPoint(20,324),QPoint(60,374)};
    painter.drawPolygon(pointR,3);
    painter.drawPolygon(pointL,3);
    //初始化一些字体
    QFont levelfont("Arial",18,3,false);//字体、字号、粗细、斜体
    painter.setFont(levelfont);
    painter.setPen(Qt::black);

    //绘制关卡入口
    int i=nowPage*5;//关卡的下标（0开始的）
    //绘制关卡圆形还有文字
    for(;i<(levelCount<nowPage*5+5 ? levelCount : nowPage*5+5);i++)//i是0开始的下标，i+1是实际关卡
    {
        QFont levelfont("Arial",18,3,false);//字体、字号、粗细、斜体
        painter.setFont(levelfont);
        painter.setPen(Qt::black);
        painter.drawEllipse(lvRect.at(i-nowPage*5));
        QString name="关卡";
        name+=QString::number(i+1);
        painter.drawText(lvRect.at(i-nowPage*5),Qt::AlignCenter,name);
        QFont levelfont2("Arial",12,1,false);//字体、字号、粗细、斜体
        painter.setFont(levelfont2);
        painter.setPen(Qt::black);
        QString rate;
        rate+=QString::number(levelRecord.at(i)*100);
        rate+="%";
        painter.drawText(lvRect2.at(i-nowPage*5),Qt::AlignCenter,rate);
    }
    //在最后面绘制新建关卡入口
    QFont levelfont22("Arial",16,3,false);//字体、字号、粗细、斜体
    painter.setFont(levelfont22);
    painter.setPen(Qt::black);
    if(i!=nowPage*5+5)
    {
        painter.drawEllipse(lvRect.at(i-nowPage*5));
        QString name="新建\n自定义关卡";
        painter.drawText(lvRect.at(i-nowPage*5),Qt::AlignCenter,name);
    }
    QFont levelfont222("Arial",12,3,false);//字体、字号、粗细、斜体
    painter.setFont(levelfont222);
    painter.setPen(Qt::black);
    painter.drawRect(exchangeUseRect);
    QString exText="切换用户";
    painter.drawText(exchangeUseRect,Qt::AlignCenter,exText);


    //绘制战机和子弹选择，在左上角
    painter.drawPixmap(pixRect[0],scalledPlanePix[nowPlaneType-1]);
    QFont levelfont2("Arial",12,3,false);//字体、字号、粗细、斜体
    painter.setFont(levelfont2);
    painter.setPen(Qt::yellow);
    painter.drawText(textRect.at(0),Qt::AlignCenter,"点击设置战机和子弹");
    for(int i=1;i<=max_bulletKind[nowPlaneType-1];i++)
    {
        painter.drawPixmap(pixRect[i],scalledBulletPix[nowBulletType[i-1]-1]);
    }
    if(changePlaneType==1)
    {
        for(int i=0;i<heroPlaneAmount;i++)
        {
            painter.drawPixmap(choiseRect[i],scalledPlanePix[i]);
        }
    }
    if(changeBulletType!=-1)
    {
        for(int i=0;i<heroBulletAmount;i++)
        {
            painter.drawPixmap(choiseRect[i],scalledBulletPix[i]);
        }
    }

}


void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint position=event->pos();
    //是否点击鼠标左键
    if(event->button()==Qt::LeftButton)
    {
        int i=nowPage*5;
        //检测是否点击关卡图案所在的矩形
         for(;i<(levelCount<nowPage*5+5?levelCount:nowPage*5+5);i++)
        {

            if(lvRect.at(i-nowPage*5).contains(position))
            {
                realLevel=i+1;
                startGame();
                return;
            }
        }
         //检测是否点击编辑关卡
        if(i!=nowPage*5+5)
        {
            if(lvRect.at(i-nowPage*5).contains(position))
            {
                //进入编辑新关卡
                newLevel();
                return;
            }
        }
        //检测切换用户
        if(exchangeUseRect.contains(position))
        {
            exchangeUser();
        }
        //检测左右翻页
        QRect turnPageL(20,274,40,100);
        QRect turnPageR(1090,274,40,100);
        if(turnPageL.contains(position))
        {
            if(nowPage>0)
            {
                nowPage--;
                update();
            }
            return;
        }
        if(turnPageR.contains(position))
        {
            if(nowPage<totalPage)
            {
                nowPage++;
                update();
            }
            return;
        }
        //检测是否点击切换战机或者子弹的矩形，代码（下标）有点复杂，能不动别动
        if(pixRect[0].contains(position))
        {
            changePlaneType=1;
            changeBulletType=-1;
            update();
            return;
        }
        for(int i=1;i<=max_bulletKind[nowPlaneType-1];i++)
        {
            if(pixRect[i].contains(position))
            {
                changeBulletType=i;
                changePlaneType=-1;
                update();
                return;
            }
        }
        if(changePlaneType==1)
        {
            for(int i=0;i<heroPlaneAmount;i++)
            {
                if(choiseRect[i].contains(position))
                {
                    nowPlaneType=i+1;
                    for(int i=0;i<max_bulletKind[nowPlaneType-1];i++)
                        nowBulletType[i]=1;
                    for(int i=max_bulletKind[nowPlaneType-1];i<4;i++)
                        nowBulletType[i]=-1;
                    changePlaneType=-1;
                    update();
                    return;
                }
            }
        }
        if(changeBulletType!=-1)
        {
            for(int i=0;i<heroBulletAmount;i++)
            {
                if(choiseRect[i].contains(position))
                {
                    nowBulletType[changeBulletType-1]=i+1;
                    changeBulletType=-1;
                    update();
                    return;
                }
            }
        }
    }
}


//开启关卡内游戏界面 //还没写
void MainWindow::startGame()//改之前是relevantLevel
{
    //进入游戏关卡
    GameDialog *dig = new GameDialog(nowPlaneType,nowBulletType,userNumber,realLevel,this);
    connect(dig,&QDialog::finished,this,&QMainWindow::show);
    connect(dig,&GameDialog::lose,this,&MainWindow::loseSlot);
    connect(dig,&GameDialog::win,this,&MainWindow::winSlot);
    //开启模态窗口
    this->hide();
    dig->exec();
    //退出后自动析构和释放子类对象内存
    delete dig;
}

void MainWindow::readFile(int userNumber)
{

    heroPlaneAmount=0;
    nowPlaneType=1;
    heroBulletAmount=0;
    nowBulletType[0]=1;
    nowBulletType[1]=1;
    nowBulletType[2]=-1;
    nowBulletType[3]=-1;

    std::string path="user";
    path+=std::to_string(userNumber);
    path+="data";
    std::fstream fs;
    fs.open(path,std::ios::in|std::ios::binary);
    if(fs.is_open()==0)
    {
        QMessageBox::warning(this, "警告", "用户文件数据丢失");
        if(userNumber!=0)
        {
            readFile(0);
        }
        else
        {
            qDebug()<<"默认用户也不见了！";
        }
        return;
    }
    fs.read(reinterpret_cast<char*>(&levelCount),sizeof (int));
    levelNumber.clear();
    levelNumber.resize(levelCount);
    levelRecord.clear();
    levelRecord.resize(levelCount);
    for(int i=0;i<levelCount;i++)
    {
        fs.read(reinterpret_cast<char*>(&levelNumber.at(i)),sizeof (int));
        fs.read(reinterpret_cast<char*>(&levelRecord.at(i)),sizeof (double));
    }
    updateUnlock();
    //回到第0页
    nowPage=0;
    //计算总页数
    totalPage=(levelCount+1)/5;
    if((levelCount+1)%5==0)
    {
        totalPage--;
    }
    fs.close();
    update();
}

void MainWindow::updateUnlock()
{
    if(levelRecord.at(3)==1)
    {
        heroBulletAmount=7;
        heroPlaneAmount=4;
    }
    else if(levelRecord.at(2)==1)
    {
        heroBulletAmount=5;
        heroPlaneAmount=3;
    }
    else if(levelRecord.at(1)==1)
    {
        heroBulletAmount=4;
        heroPlaneAmount=2;
    }
    else if(levelRecord.at(0)==1)
    {
        heroBulletAmount=3;
        heroPlaneAmount=2;
    }
    else
    {
        heroBulletAmount=2;
        heroPlaneAmount=1;
    }
}

void MainWindow::loseSlot(double r)
{
    double rate=1-r;
    //更差的记录不保存
    if(rate<=levelRecord.at(realLevel-1))
    {
        return;
    }
    std::string path="user";
    path+=std::to_string(userNumber);
    path+="data";
    std::fstream fs;
    fs.open(path,std::ios::out|std::ios::in|std::ios::binary);
    if(fs.is_open()==0)
    {
        QMessageBox::warning(this, "警告", "用户文件数据丢失");
        if(userNumber!=0)
        {
            readFile(0);
        }
        else
        {
            qDebug()<<"默认用户也不见了！";
        }
        return;
    }
    //跳转到对应区域覆写
    fs.seekp(4+12*(realLevel-1)+4,std::ios_base::beg);
    fs.write(reinterpret_cast<const char*>(&rate),sizeof (double));
    fs.close();
    levelRecord.at(realLevel-1)=rate;
    updateUnlock();
    update();
}

void MainWindow::winSlot()
{
    loseSlot(0);
}

void MainWindow::newLevel()
{
    std::string path="user";
    path+=std::to_string(userNumber);
    path+="leveldata";
    editor *edi = new editor(path,levelCount+1,this);
    connect(edi,&editor::success,[this](){
        //先在user data中加入新关卡的索引
        std::string path="user";
        path+=std::to_string(userNumber);
        path+="data";
        std::fstream fs;
        fs.open(path,std::ios::out|std::ios::in|std::ios::binary);
        int newlevelCount=levelCount+1;
        fs.write(reinterpret_cast<const char*>(&newlevelCount),sizeof (int));
        fs.seekp(levelCount*12,std::ios_base::cur);
        double newRecord=0;
        fs.write(reinterpret_cast<const char*>(&newlevelCount),sizeof (int));
        fs.write(reinterpret_cast<const char*>(&newRecord),sizeof (double));
        fs.close();
        //再重新读取文件
        readFile(userNumber);
//        this->show();
//        delete edi;
    });
    connect(edi,&editor::editorClose,[this,edi](){
        this->show();
        delete edi;
    });
    edi->show();
    this->hide();
}

void MainWindow::exchangeUser()
{
    UserExchange *usc = new UserExchange(userName,this);
    connect(usc,&UserExchange::UXclose,[this,usc](){
        this->show();
        delete usc;
    });
    connect(usc,&UserExchange::logIn,[this,usc](int nextUserNumber,char* nextUsername){
        userNumber=nextUserNumber;
        for(int i=0;i<16;i++)
        {
            userName[i]=nextUsername[i];
        }
        readFile(userNumber);
        delete usc;
    });
    usc->show();
}
