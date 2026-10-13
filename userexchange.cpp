#include "userexchange.h"
#include "ui_userexchange.h"

UserExchange::UserExchange(char* nowname,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserExchange)
{
    ui->setupUi(this);

    //设置颜色
    QPalette palette = ui->nowUserName->palette();
    palette.setColor(QPalette::WindowText, Qt::yellow);
    ui->nowUserName->setPalette(palette);
    // 设置字体大小
    QFont font = ui->nowUserName->font();
    font.setPointSize(12);
    ui->nowUserName->setFont(font);
    //插入当前用户名
    QString currentName = QString(nowname);
    ui->nowUserName->setText(currentName);
    //限制账号密码输入类型
    QRegularExpression regex("^[a-zA-Z0-9]*$");  // 数字和字母
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex, this);
    ui->passWordlineEdit->setValidator(validator);
    QRegularExpression regex2("^[a-zA-Z0-9\\s]*$");
    QRegularExpressionValidator* validator2 = new QRegularExpressionValidator(regex2, this);
    ui->userNamelineEdit->setValidator(validator2);
}

UserExchange::~UserExchange()
{
    delete ui;
}

void UserExchange::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(rect(),QPixmap(":/scene/user_exchange_scene.jpg"));
}

void UserExchange::on_enterPushButton_clicked()
{
    //清空
    for(int i=0;i<16;i++)
    {
        nextUserName[i]='\0';
    }
    for(int i=0;i<32;i++)
    {
        nextPassword[i]='\0';
    }
    //获取输入的用户名和密码
    QString userName = ui->userNamelineEdit->text();
    QString password = ui->passWordlineEdit->text();
    for (int i = 0; i < (userName.length()<16 ? userName.length() : 16); i++) {
        nextUserName[i] = static_cast<char>(userName[i].unicode());
    }
    for (int i = 0; i < (password.length()<32 ? password.length() : 32); i++) {
        nextPassword[i] = static_cast<char>(password[i].unicode());
    }
    //开始读文件
    std::fstream fs;
    fs.open("userheadfile",std::ios::in|std::ios::binary);
    if(fs.is_open()==0)
    {
        QMessageBox::warning(this,"","账户数据丢失",QMessageBox::Ok);
        fs.close();
        return;
    }
    int userCount;
    fs.read(reinterpret_cast<char*>(&userCount),sizeof (int));
    for(int i=0;i<userCount;i++)
    {
        int a=0;
        for(;a<16;a++)
        {
            char namebit;
            fs.read(reinterpret_cast<char*>(&namebit),sizeof (char));
//            qDebug()<<namebit<<nextUserName[a];
            if(namebit!=nextUserName[a])
            {
                break;
            }
        }
        //如果名字核对失败，继续下一条
        if(a!=16)
        {
            fs.seekg((15-a)+8+32,std::ios::cur);
            continue;
        }
        //核对成功
        int nextUserNumber;
        fs.read(reinterpret_cast<char*>(&nextUserNumber),sizeof (int));
        //看看有没有密码
        int ifPassword;
        fs.read(reinterpret_cast<char*>(&ifPassword),sizeof (int));
        if(ifPassword==0)
        {
            //没有密码直接登录成功
            QMessageBox::information(this,"","登录成功",QMessageBox::Ok);
            logIn(nextUserNumber,nextUserName);
            fs.close();
            return;
        }
        //有密码继续核对
        int b=0;
        for(;b<32;b++)
        {
            char passwordbit;
            fs.read(reinterpret_cast<char*>(&passwordbit),sizeof (char));
            if(passwordbit!=nextPassword[b])
            {
                break;
            }
        }
        //密码核对错误直接退出
        if(b!=32)
        {
            QMessageBox::warning(this,"","密码错误",QMessageBox::Ok);
            fs.close();
            return;
        }
        else
        {
            //核对成功
            emit logIn(nextUserNumber,nextUserName);
            fs.close();
            return;
        }
    }//所有用户都读完了
    QMessageBox::information(this,"","没有此用户",QMessageBox::Ok);
    fs.close();
}

void UserExchange::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit UXclose();
}

void UserExchange::on_registerPushButtin_clicked()
{
    const QString userName=ui->userNamelineEdit->text();
    const QString password=ui->passWordlineEdit->text();
    if(userName.length()==0)
    {
        QMessageBox::information(this,"","请输入用户名",QMessageBox::Ok);
        return;
    }
    char newname[16]={'\0'};
    for(int i=0;i<(userName.length()<16 ? userName.length() : 16);i++)
    {
        newname[i]=static_cast<char>(userName[i].unicode());
    }
    //检查用户是否存在
    std::fstream fs;
    fs.open("userheadfile",std::ios::in|std::ios::out|std::ios::binary);
    if(fs.is_open()==0)
    {
        QMessageBox::warning(this,"","账户数据文件丢失",QMessageBox::Ok);
        return;
    }
    int userCount;
    fs.read(reinterpret_cast<char*>(&userCount),sizeof (int));
    for(int i=0;i<userCount;i++)
    {
        int a=0;
        for(;a<16;a++)
        {
            char namebit;
            fs.read(reinterpret_cast<char*>(&namebit),sizeof (char));
            qDebug()<<namebit<<newname[a];
            if(namebit!=newname[a])
            {
                break;
            }
        }
        //如果名字核对失败，继续下一条
        if(a!=16)
        {
            fs.seekg((15-a)+8+32,std::ios::cur);
            continue;
        }
        else
        {
            //核对成功
            QMessageBox::information(this,"","该用户已存在",QMessageBox::Ok);
            fs.close();
            return;
        }
    }//所有用户都读完了
    //没有重复的
    int ifPassword=0;
    char newPassword[32]={'\0'};
    if(password.length()!=0)
    {
        ifPassword=1;
        for (int i = 0; i < (password.length()<32 ? password.length() : 32); i++)
        {
            newPassword[i] = static_cast<char>(password[i].unicode());
        }
    }
    //开始写文件
    fs.seekg(0,std::ios_base::beg);
    fs.seekp(0,std::ios_base::beg);
    int usercount;
    fs.read(reinterpret_cast<char*>(&usercount),sizeof (int));
    fs.seekp(0,std::ios_base::beg);
    //改写用户数量
    usercount++;
    fs.write(reinterpret_cast<char*>(&usercount),sizeof (int));
    usercount--;
    fs.seekp(usercount*56,std::ios_base::cur);
    //写用户名
    for(int i=0;i<16;i++)
    {
        fs.write(reinterpret_cast<const char*>(&newname[i]),sizeof(char));
    }
    //写用户number
    fs.write(reinterpret_cast<const char*>(&usercount),sizeof(int));
    fs.write(reinterpret_cast<const char*>(&ifPassword),sizeof(int));
    for(int i=0;i<32;i++)
    {
        fs.write(reinterpret_cast<const char*>(&newPassword[i]),sizeof(char));
    }
    fs.close();

    QString data = "user";
    data+=QString::number(usercount);
    data+="data";
    copyAndRenameFile("userdata",data);
    QString leveldata = "user";
    leveldata+=QString::number(usercount);
    leveldata+="leveldata";
    copyAndRenameFile("userleveldata",leveldata);
    QMessageBox::information(this,"","创建成功",QMessageBox::Ok);
}

bool UserExchange::copyAndRenameFile(const QString& sourcePath, const QString& targetPath) {
    // 检查源文件是否存在
    if (!QFile::exists(sourcePath)) {
        QMessageBox::warning(this,"","数据参考文件缺失");
        return false;
    }

    // 复制文件并改名（targetPath包含新文件名）
    bool success = QFile::copy(sourcePath, targetPath);

    return success;
}
