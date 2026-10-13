#include "gamedialog.h"
#include "ui_gamedialog.h"

GameDialog::GameDialog(int PlaneKind,int*bulletdata,int user_num,int acturalLevel,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameDialog),
    acturalLevel(acturalLevel),
    heroKind(PlaneKind)
{
    ui->setupUi(this);

    //创建全局时钟
    timer = new QTimer(this);
    //连接timer和容器刷新槽函数
    connect(timer,&QTimer::timeout,this,&GameDialog::updateVector);
    //初始化dialog大小
    this->setFixedSize(width,heigth);

    //初始化view和scene，因为飞机子弹模型是基于QGraphicsItem类的，需要依赖这两个显示
    view = new QGraphicsView(this);
    view->setFixedSize(width,heigth);
    view->setRenderHint(QPainter::Antialiasing) ;//抗拒锯齿
    scene = new QGraphicsScene(0,0,width-10,heigth-10,this);//根据实际显示模型修改大小
    view->setScene(scene);
    view->show();

    //初始化读取文件的路径
    filePath="user";
    filePath+=std::to_string(user_num);
    filePath+="leveldata";
    //复制战机的子弹参数(新式的）
    bulletamount=0;
    for(int i=0;i<4;i++)
    {
        heroBullet[i]=bulletdata[i];
        bulletamount++;
    }


    //绘制背景图
    QPixmap background(":/scene/gamescene.jpg");
      if (!background.isNull()) {
          // 设置背景画刷
          scene->setBackgroundBrush(background.scaled(
              scene->width(), scene->height(),
              Qt::IgnoreAspectRatio, Qt::SmoothTransformation
          ));
      }

    //初始化按钮
    stopIcon = new Icon(0,this);
    continueIcon = new Icon(1,this);
    againIcon = new Icon(2,this);
    scene->addItem(stopIcon);
    scene->addItem(againIcon);
    gamestate=1;

    start();

}

GameDialog::~GameDialog()
{
    delete ui;

}


void GameDialog::start()
{
    //这里要完成根据关卡文件初始化

    //初始化tsxtItem
    text = new QGraphicsTextItem("敌人数量：0/0");
    text->setParent(this);
    text->setDefaultTextColor(Qt::yellow);
    text->setPos(280, 60); // 设置文本位置
    text->setFont(QFont("Arial", 14));
    text->setZValue(100); // 确保文本在最上层
    scene->addItem(text);
    //初始化战机血条
    solidRect = new QGraphicsRectItem;
    hollowRect = new QGraphicsRectItem;
    solidRect->setRect(100,950,550,10);
    solidRect->setPen(QPen(Qt::black));
    hollowRect->setRect(100,950,550,10);
    hollowRect->setBrush(Qt::NoBrush); // 设置为空心
    hollowRect->setPen(QPen(Qt::black));       // 设置边框颜色为红色
    hollowRect->setZValue(99);

    scene->addItem(hollowRect);
    scene->addItem(solidRect);

    //初始化爆炸模型对象池
    for(int i=0;i<10;i++)
    {
        Boom *boom = new Boom(0,this);
        bombList0.push_back(boom);
        scene->addItem(boom);
    }
    for(int i=0;i<5;i++)
    {
        Boom* boom = new Boom(1,this);
        bombList1.push_back(boom);
        scene->addItem(boom);
    }
    //敌人子弹对象池
    enemyBulletList1.resize(7);//下标为0是测试子弹，考虑后期直接滞空，敌人子弹类型和下标直接对等
    for(int i=0;i<7;i++)
    {
        if(i==0)
        {
            continue;
        }
        for(int a=0;a<10;a++)
        {
            EnemyBullet *emybu = new EnemyBullet(i,this);
            scene->addItem(emybu);
            enemyBulletList1[i].push_back(emybu);
            connect(emybu,&EnemyBullet::hitHerop,this,&GameDialog::HitFromEmbuToHerop);
            connect(emybu,&EnemyBullet::bulletBoom,this,&GameDialog::BombAddSlot);
        }
    }

    //hero发出的子弹//待优化，现在如果子弹类型重复，后面的vec根本没用
    //已优化，同一类子弹只有一个vector
    heroBulletList.resize(bulletamount);//bulletamount是新式的
    for(int i=0;i<bulletamount;i++)
    {
//        heroBulletKind[i]=bulletKinds[i];
        for(int a=0;a<20;a++)
        {
            if(heroBullet[i]!=-1)
            {
                HeroBullet *herobu = new HeroBullet(heroBullet[i],this);
                heroBulletList[i].push_back(herobu);
                scene->addItem(herobu);
                connect(herobu,&HeroBullet::hitEnemyp,this,&GameDialog::HitFromHrbuToEmp);
                connect(herobu,&HeroBullet::bulletBoom,this,&GameDialog::BombAddSlot);

            }
        }
    }

    //读取文件，根据关卡数据初始化敌机并塞入对象池
    readFile();

    //初始化战机
    heroP = new HeroPlane(heroKind,heroBullet,this);//传入新式的子弹数据
    scene->addItem(heroP);
    connect(timer,&QTimer::timeout,heroP,&HeroPlane::update);
    connect(heroP,&HeroPlane::emitHeroBullet,this,&GameDialog::heroEmitSlot);
    connect(heroP,&HeroPlane::emitHeroBullet5,this,&GameDialog::heroEmitSlot5);
    connect(heroP,&HeroPlane::heroPBoom,this,&GameDialog::BombAddSlot);
    connect(heroP,&HeroPlane::heroPDie,this,&GameDialog::loseSlot);

    //开始刷新
    timer->start(10);
}


void GameDialog::readFile()
{
    //关卡数据的结构
//    敌机类存储规范：
//    在敌人类这里

//    int		int		double x,double y,double vx,double vy,double sx,double sy,double st,
//    敌人类	kind种类
//    如果首位为	2    多加读取：int num，			 然后再加读取 num×6：double *
//    如果首位为	3    多加读取：double health,int num  然后再加读取  num×6:  double *

//    关卡文件结构：user leveldata
//    int  关卡编号
//    int 关卡检查 *2+12345
//    int 当前关卡剩余大小（跳转）
//    int 敌人数量
//    循环的敌人数据
//    int 关卡结束检查*3+12345
    EnemyPlaneBase *emy;

    std::fstream fs;
    fs.open(filePath,std::ios::in|std::ios::binary);
    if(!fs)
    {
        QMessageBox::warning(this,"警告","该用户关卡数据丢失",QMessageBox::Ok);
        this->close();
    }
    //查找对应关卡数据的存放开始位置
    int read_level=0;//读取到的关卡编号
    fs.read(reinterpret_cast<char*>(&read_level),sizeof (int));
    qDebug()<<read_level;
    for(;read_level!=acturalLevel&&read_level!=-1;)
    {
        //如果关卡编号不匹配，读取跳转字节数，跳转到下一个关卡的数据
        fs.seekg(sizeof (int),std::ios_base::cur);//跳过关卡检查
        //读取要跳转的字节数
        int read_jump=0;
        fs.read(reinterpret_cast<char*>(&read_jump),sizeof (int));
        fs.seekg(read_jump,std::ios_base::cur);
        //读取下一关卡编号
        fs.read(reinterpret_cast<char*>(&read_level),sizeof (int));
    }
    //读到尾了都还没有找到：
    if(read_level==-1)
    {
        QMessageBox::warning(this,"警告","该关卡数据丢失",QMessageBox::Ok);
        return;
    }
    int read_levelcheck=0;
    fs.read(reinterpret_cast<char*>(&read_levelcheck),sizeof (int));
    if(read_levelcheck!=acturalLevel*2+12345)
    {
        QMessageBox::warning(this,"警告","该关卡数据损坏",QMessageBox::Ok);
        return;
    }
    //读取关卡剩余字节，暂时没用
    int read_rest_size=0;
    fs.read(reinterpret_cast<char*>(&read_rest_size),sizeof (int));

    //读取关卡敌人数量，后面控制敌人读取次数
    fs.read(reinterpret_cast<char*>(&enemyamount),sizeof (int));

    //开始循环读取敌人数据
    int enemyType,kind;
    int num;
    double x,y,vx,vy,st,health;
    for(int i=0;i<enemyamount;i++)
    {

        fs.read(reinterpret_cast<char*>(&enemyType),sizeof(int));
        if(enemyType==acturalLevel*3+12345)
        {
            qDebug()<<"提前检测到关卡结束标志,未处理错误";
            return;
        }
        fs.read(reinterpret_cast<char*>(&kind),sizeof(int));
        fs.read(reinterpret_cast<char*>(&x),sizeof(double));
        fs.read(reinterpret_cast<char*>(&y),sizeof(double));
        fs.read(reinterpret_cast<char*>(&vx),sizeof(double));
        fs.read(reinterpret_cast<char*>(&vy),sizeof(double));
        fs.read(reinterpret_cast<char*>(&st),sizeof(double));
        qDebug()<<enemyType<<kind<<x<<y<<vx<<vy<<st;;
        if(enemyType==1)
        {
            emy = new Enemy1(kind,x,y,vx,vy,st,this);//不用delete，有父类对象给我自动管理
            enemyList.append(emy);
        }
        if(enemyType==2)
        {
            //先读取敌人子弹数量
            fs.read(reinterpret_cast<char*>(&num), sizeof(int));
            if(num<=1000)//最大数量检测
            {
                //再读取敌人子弹数据
                double *dat=new double[num*6];//这个要delete
                fs.read(reinterpret_cast<char*>(dat),sizeof(double)*num*6);
                emy = new Enemy2(kind,x,y,vx,vy,st,num,dat,this);//生成敌机
                enemyList.append(emy);
                //释放double*
                delete[] dat;
                dat=NULL;
            }
        }
        if(enemyType==3)
        {
            fs.read(reinterpret_cast<char*>(&health),sizeof(double));
            fs.read(reinterpret_cast<char*>(&num),sizeof (int));
            if(num<=1000)
            {
                double *dat = new double[num*6];
                fs.read(reinterpret_cast<char*>(dat),sizeof (double)*num*6);
                qDebug()<<"子弹数据";
                for(int i=0;i<num;i++)
                {
                qDebug()<<dat[0+i*6]<<dat[1+i*6]<<dat[2+i*6]<<dat[3+i*6]<<dat[4+i*6]<<dat[5+i*6];
                }
                emy = new Enemy3(kind,x,y,vx,vy,st,health,num,dat,this);
                enemyList.append(emy);
                delete[] dat;
                dat=NULL;
            }
            else{
                qDebug()<<"num";
            }
        }
        //连接好敌机的信号
        connect(emy,&EnemyPlaneBase::emitBullet3,this,&GameDialog::enemyEmitSlot3);
        connect(emy,&EnemyPlaneBase::emitBullet5,this,&GameDialog::enemyEmitSlot5);
        connect(emy,&EnemyPlaneBase::EnemyPBoom,this,&GameDialog::BombAddSlot);
        //添加到GraphicsScene
        scene->addItem(emy);
    }//敌机读取循环结尾

    fs.read(reinterpret_cast<char*>(&read_levelcheck),sizeof (int));
    if(read_levelcheck!=acturalLevel*3+12345)
    {
        qDebug()<<"关卡结束标志检测失败";//好像没用什么用
    }

    fs.close();
}


void GameDialog::enemyEmitSlot3(int bulletKind,double x,double y)
{
    int check=0;//1表示完成操作，不然就要append
    //根据bulletkind选择容器
    //switch
    for(EnemyBullet *emBu : enemyBulletList1[bulletKind] )//子弹类型和下标直接对等的，没问题，下标0滞空
    {
        //找到空闲的对象并复用
        if(emBu->getState()==0)
        {
            emBu->reUseBullet(x,y);
            check++;
            return;
        }
    }
    //如果没用空闲对象就生成并插入
    if(check==0)
    {
        EnemyBullet *embu = new EnemyBullet(bulletKind,x,y,this);
        enemyBulletList1[bulletKind].push_back(embu);
        scene->addItem(embu);
        connect(embu,&EnemyBullet::hitHerop,this,&GameDialog::HitFromEmbuToHerop);
        connect(embu,&EnemyBullet::bulletBoom,this,&GameDialog::BombAddSlot);
    }
}
void GameDialog::enemyEmitSlot5(int bulletKind,double x,double y,double tox,double toy)
{
    int check=0;//1表示完成，不然就要append
    for(EnemyBullet *emBu : enemyBulletList1[bulletKind])
    {
        if(emBu->getState()==0)
        {
            emBu->reUseBullet(x,y,tox,toy);
            check++;
            return;
        }
    }
    if(check==0)
    {
        EnemyBullet *embu = new EnemyBullet(bulletKind,x,y,this,tox,toy);
        enemyBulletList1[bulletKind].push_back(embu);
        scene->addItem(embu);
        connect(embu,&EnemyBullet::hitHerop,this,&GameDialog::HitFromEmbuToHerop);
        connect(embu,&EnemyBullet::bulletBoom,this,&GameDialog::BombAddSlot);
    }
}

//战机发射子弹信号槽函数
void GameDialog::heroEmitSlot(int kind,double x,double y)
{

    int check=0;//1表示完成，不然就要append
    int i=0;//i是为了找到对应的对象池容器
    for(;i<bulletamount;i++)
    {
        if(heroBullet[i]==kind)//新式
        {
            break;
        }
    }
    if(i==bulletamount)
    {
        qDebug()<<"管理类中的heroEmitSlot子弹容器匹配错误,return";
        return;
    }
    //复用或者新生成子弹
    for(HeroBullet *emBu : heroBulletList[i] )
    {
        if(emBu->getState()==0)
        {
            emBu->reUseBullet(x,y);
            check++;
            return;
        }
    }
    if(check==0)
    {
        HeroBullet *embu = new HeroBullet(kind,x,y,this);
        heroBulletList[i].push_back(embu);
        scene->addItem(embu);
        connect(embu,&EnemyBullet::hitEnemyp,this,&GameDialog::HitFromHrbuToEmp);

    }
}
void GameDialog::heroEmitSlot5(int kind,double x,double y,double toX,double toY)
{
    int check=0;//1表示完成，不然就要append
    int i=0;
    for(;i<bulletamount;i++)
    {
        if(heroBullet[i]==kind)
        {
            break;
        }
    }
    if(i==bulletamount)
    {
        qDebug()<<"管理类中的heroEmitSlot子弹容器匹配错误,return";
        return;
    }
    for(HeroBullet *emBu : heroBulletList[i] )
    {
        if(emBu->getState()==0)
        {
            emBu->reUseBullet(x,y,toX,toY);
            check++;
            return;
        }
    }
    if(check==0)
    {
        HeroBullet *embu = new HeroBullet(kind,x,y,this,toX,toY);
        heroBulletList[i].push_back(embu);
        scene->addItem(embu);
        connect(embu,&EnemyBullet::hitEnemyp,this,&GameDialog::HitFromHrbuToEmp);

    }
}

//添加爆炸动画
void GameDialog::BombAddSlot(int kind,double x,double y)
{
    int check=0;
    if(kind==0)
    {
        for(Boom *bomb : bombList0)
        {
            if(bomb->getState()==0)
            {
                bomb->reUse(x,y);
                check++;
                return;
            }
        }
        if(check==0)
        {
            Boom *bomb = new Boom(0,this,x,y);
            bombList0.push_back(bomb);
            scene->addItem(bomb);
            return;
        }
    }
    if(kind==1)
    {
        for(Boom *bomb : bombList1)
        {
            if(bomb->getState()==0)
            {
                bomb->reUse(x,y);
                check++;
                return;
            }
        }
        if(check==0)
        {
            Boom *bomb = new Boom(0,this,x,y);
            bombList1.push_back(bomb);
            scene->addItem(bomb);
            return;
        }
    }
}

//许多信号的中转：
//敌人子弹打到战机
void GameDialog::HitFromEmbuToHerop(double hp)
{
    heroP->getHit(hp);
}
//战机子弹打到敌人
void GameDialog::HitFromHrbuToEmp(EnemyPlaneBase *emyp,double hp)
{
    emyp->getHit(hp);
}


void GameDialog::updateVector()
{
    //刷新所有对象
    //刷新敌人子弹
    for(int i=0;i<7;i++)
    {
        for(EnemyBullet *embu : enemyBulletList1[i])
        {
            embu->upDate();
        }
    }
    //刷新战机子弹
    for(int i=0;i<bulletamount;i++)
    {
        for(HeroBullet *herobu : heroBulletList[i])
        {
            herobu->upDate();
        }
    }
    //数剩余敌人数量
    int rest=0;
    for(EnemyPlaneBase *emy : enemyList)
    {
        if(emy->getState()<=2)
        {
            rest++;
        }
        emy->upDate();
    }
//    //如果剩余敌人数改变了，刷新显示
//    if(restEnemyamount!=rest)
//    {
        restEnemyamount=rest;
        upDateText();
        if(restEnemyamount==0)
        {
            winSlot();
        }
//    }
    //刷新爆炸显示
    for(Boom *boom : bombList0)
    {
        boom->upDate();
    }
    for(Boom *boom : bombList1)
    {
        boom->upDate();
    }
    //刷新战机血条
    upDateSolidRect();
}

//刷新剩余敌人数的显示
void GameDialog::upDateText()
{
    QString qs("敌人数量：");
    qs += QString::number(restEnemyamount);
    qs += '/';
    qs += QString::number(enemyamount);
    text->setPlainText(qs);
}

//刷新战机血条的显示
void GameDialog::upDateSolidRect()
{
    double healthRate = heroP->getRestHealthRate();
    solidRect->setRect(100,950,550*healthRate,10);
    if(healthRate>=0.5)
    {
        solidRect->setBrush(QBrush(QColor((2-healthRate*2)*212,212,0,255)));
    }
    else
    {
        solidRect->setBrush(QBrush(QColor(212,(healthRate*2)*212,0,255)));
    }
}

//重载鼠标事件，主要用来实现暂停之类的操作
void GameDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if(iconRect[0].contains(event->pos()))//暂停或者继续被点击了
        {
            if(gamestate==0)//在暂停状态
            {
                scene->removeItem(continueIcon);
                scene->addItem(stopIcon);
                messageState=2;
                gamestate=1;
                updateMessageText();
                return;
            }
            if(gamestate==1)//在游戏中的状态
            {
                gamestate=0;
                timer->stop();
                scene->removeItem(stopIcon);
                scene->addItem(continueIcon);
                heroP->clearKeySet();
                messageState=1;
                updateMessageText();
                return;
            }
        }
        if(iconRect[1].contains(event->pos()))//重来被点击
        {
            timer->stop();
            deleteAllReNewObject();
            gamestate=1;
            messageState=0;
            updateMessageText();
            start();
            return;
        }//重来区域结束
    }//左键点击判断结束
}

void GameDialog::winSlot()
{
    emit win();
    messageState=3;
    timer->stop();
    updateMessageText();
}

void GameDialog::loseSlot()
{

    emit lose((double)restEnemyamount/enemyamount);
    messageState=4;
    timer->stop();
    updateMessageText();
}

void GameDialog::deleteAllReNewObject()
{
    //清除掉所有new的对象，在重新读取
    delete text;//会自动从scene中删掉
    delete solidRect;
    delete hollowRect;
    //清除爆炸对象
    for(Boom* boom: bombList0)
    {
        delete boom;
    }
    bombList0.clear();
    for(Boom* boom: bombList1)
    {
        delete boom;
    }
    bombList1.clear();
    //清除敌机子弹对象池
    for(int i=0;i<7;i++)
    {
        for(BulletBase* emybu : enemyBulletList1[i])
        {
            disconnect(emybu,&EnemyBullet::hitHerop,this,&GameDialog::HitFromEmbuToHerop);
            disconnect(emybu,&EnemyBullet::bulletBoom,this,&GameDialog::BombAddSlot);
            delete emybu;
        }
        enemyBulletList1[i].clear();
    }
    //清除战机子弹对象池
    for(int i=0;i<bulletamount;i++)
    {
        for(BulletBase *hrbu : heroBulletList[i])
        {
            disconnect(hrbu,&HeroBullet::hitEnemyp,this,&GameDialog::HitFromHrbuToEmp);
            disconnect(hrbu,&HeroBullet::bulletBoom,this,&GameDialog::BombAddSlot);
            delete  hrbu;
        }
        heroBulletList[i].clear();
    }
    //清除战机
    disconnect(timer,&QTimer::timeout,heroP,&HeroPlane::update);
    disconnect(heroP,&HeroPlane::emitHeroBullet,this,&GameDialog::heroEmitSlot);
    disconnect(heroP,&HeroPlane::emitHeroBullet5,this,&GameDialog::heroEmitSlot5);
    disconnect(heroP,&HeroPlane::heroPBoom,this,&GameDialog::BombAddSlot);
    disconnect(heroP,&HeroPlane::heroPDie,this,&GameDialog::loseSlot);
    delete heroP;

    //清除敌机对象池
    for(EnemyPlaneBase *emyP : enemyList)
    {
        disconnect(emyP,&EnemyPlaneBase::emitBullet3,this,&GameDialog::enemyEmitSlot3);
        disconnect(emyP,&EnemyPlaneBase::emitBullet5,this,&GameDialog::enemyEmitSlot5);
        disconnect(emyP,&EnemyPlaneBase::EnemyPBoom,this,&GameDialog::BombAddSlot);
        delete emyP;
    }
    enemyList.clear();
}

void GameDialog::updateMessageText()
{
    if(messageRectItem!=nullptr)
    {
        delete messageRectItem;
        messageRectItem = nullptr;
    }
    if(messageText!=nullptr)
    {
        delete messageText;
        messageText = nullptr;
    }
    if(messageText2!=nullptr)
    {
        delete messageText2;
        messageText2 = nullptr;
    }
    switch (messageState) {
    case 0:
    {
        return;
    }
    case 1://暂停中
    {
        //暂停中的文字item
        messageText = new QGraphicsTextItem("暂停中");
        messageText->setParent(this);
        messageText->setDefaultTextColor(Qt::yellow);
        messageText->setPos(260,450);
        messageText->setFont(QFont("Arial", 42));
        messageText->setZValue(100);
        scene->addItem(messageText);
        //背景方框
        messageRectItem = new QGraphicsRectItem;
        messageRectItem->setRect(messageRect);
        messageRectItem->setBrush(QBrush(QColor(255,255,255,100)));
        messageRectItem->setZValue(99);
        scene->addItem(messageRectItem);
        break;
    }
    case 2:
    {
        //倒计时的时钟
        QTimer *timer = new QTimer(this);
        timer->start(10);
        //背景方框
        messageRectItem = new QGraphicsRectItem;
        messageRectItem->setRect(messageRect);
        messageRectItem->setBrush(QBrush(QColor(255,255,255,100)));
        messageRectItem->setZValue(99);
        scene->addItem(messageRectItem);
        //即将继续的item
        messageText2 = new QGraphicsTextItem("即将继续");
        messageText2->setParent(this);
        messageText2->setDefaultTextColor(Qt::yellow);
        messageText2->setPos(245,425);
        messageText2->setFont(QFont("Arial", 36));
        messageText2->setZValue(100);
        scene->addItem(messageText2);
        //倒计时的item
        messageText = new QGraphicsTextItem("3");
        messageText->setParent(this);
        messageText->setDefaultTextColor(Qt::yellow);
        messageText->setPos(325,510);
        messageText->setFont(QFont("Arial", 28));
        messageText->setZValue(100);
        scene->addItem(messageText);

        double downCount;
        downCount=3.01;
        connect(timer,&QTimer::timeout,[this,timer,downCount]()mutable{//允许lambda表达式修改捕获的变量
            if(messageState!=2)//如果继续倒计时的状态被改变了
            {
                timer->stop();
                timer->deleteLater();
                return;
            }
            if (downCount <= 0.0) {
                timer->stop();
                timer->deleteLater();
                gamestate=1;
                GameDialog::timer->start(10);
                messageState=0;
                updateMessageText();
                return;
            }
            downCount-=0.01;
            messageText->setPlainText(QString::number(downCount));
        });
        break;
    }//case2结束
    case 3://胜利
    {
        //倒计时的时钟
        QTimer *timer = new QTimer(this);
        timer->start(10);
        //背景方框
        messageRectItem = new QGraphicsRectItem;
        messageRectItem->setRect(messageRect);
        messageRectItem->setBrush(QBrush(QColor(255,255,255,100)));
        messageRectItem->setZValue(99);
        scene->addItem(messageRectItem);
        //胜利的item
        messageText2 = new QGraphicsTextItem("胜利");
        messageText2->setParent(this);
        messageText2->setDefaultTextColor(Qt::yellow);
        messageText2->setPos(280,425);
        messageText2->setFont(QFont("Arial", 42));
        messageText2->setZValue(100);
        scene->addItem(messageText2);
        //倒计时的item
        messageText = new QGraphicsTextItem("10");
        messageText->setParent(this);
        messageText->setDefaultTextColor(Qt::yellow);
        messageText->setPos(350,530);
        messageText->setFont(QFont("Arial", 18));
        messageText->setZValue(100);
        scene->addItem(messageText);

        double downCount2;
        downCount2=10.01;
        connect(timer,&QTimer::timeout,[this,timer,downCount2]()mutable{//允许lambda表达式修改捕获的变量
            if(messageState!=3)//如果继续倒计时的状态被改变了
            {
                timer->stop();
                timer->deleteLater();
                return;
            }
            if (downCount2 <= 0.0) {
                this->close();
            }
            downCount2-=0.01;
            messageText->setPlainText(QString::number((int)downCount2));
        });
        break;
    }//case3胜利结束
    case 4://失败
    {
        //倒计时的时钟
        QTimer *timer = new QTimer(this);
        timer->start(10);
        //背景方框
        messageRectItem = new QGraphicsRectItem;
        messageRectItem->setRect(messageRect);
        messageRectItem->setBrush(QBrush(QColor(255,255,255,100)));
        messageRectItem->setZValue(99);
        scene->addItem(messageRectItem);
        //胜利的item
        double rate =100 - (double)restEnemyamount/enemyamount*100;
        QString qs = "      失败\n本次记录:";
        qs+=QString::number((int)rate);
        qs+="%";
        messageText2 = new QGraphicsTextItem(qs);
        messageText2->setParent(this);
        messageText2->setDefaultTextColor(Qt::yellow);
        messageText2->setPos(245,400);
        messageText2->setFont(QFont("Arial", 28));
        messageText2->setZValue(100);
        scene->addItem(messageText2);
        //倒计时的item
        messageText = new QGraphicsTextItem("10");
        messageText->setParent(this);
        messageText->setDefaultTextColor(Qt::yellow);
        messageText->setPos(355,550);
        messageText->setFont(QFont("Arial", 18));
        messageText->setZValue(100);
        scene->addItem(messageText);

        double downCount2;
        downCount2=10.01;
        connect(timer,&QTimer::timeout,[this,timer,downCount2]()mutable{//允许lambda表达式修改捕获的变量
            if(messageState!=4)//如果继续倒计时的状态被改变了
            {
                timer->stop();
                timer->deleteLater();
                return;
            }
            if (downCount2 <= 0.0) {
                this->close();
            }
            downCount2-=0.01;
            messageText->setPlainText(QString::number((int)downCount2));
        });
        break;
    }//case4失败结束
    case 5:
    {
        this->close();
    }
    }//switch结束
}

