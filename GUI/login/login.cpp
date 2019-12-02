#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    //setPeer(peer);
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_clicked()
{
    std::cout << "button pressed\n";
    QString userName=ui->lineEdit->text();
    std::cout << userName.toUtf8().constData()<<std::endl;
    QString password=ui->lineEdit_2->text();
    std::cout << password.toUtf8().constData()<<std::endl;
    peer->SetAuthInfo(ui->lineEdit->text().toUtf8().constData(), ui->lineEdit_2->text().toUtf8().constData());
    std::cout<<"In logincpp checking if username and password is set"<<peer->GetAuthInfo();
    //peer->SetAuthInfo("mickey", "mouse");
    peer->RemoteSignUp();
    //std::cout << "hello hello\n";
}

void Login::on_pushButton_clicked(bool checked)
{
    if(checked){
        std::cout<<"hello 2\n";
    }
}
