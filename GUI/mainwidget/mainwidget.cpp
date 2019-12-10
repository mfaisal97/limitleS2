#include "mainwidget.h"
#include "ui_mainwidget.h"


MainWidget::MainWidget(QWidget *parent,Peer* _peer)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    setPeer(_peer);
    ui->setupUi(this);
    Login l;
    l.setPeer(_peer);
    l.setModal(true);
    l.exec(); 
    close();
    ui->listWidget->clear();
    std::vector<std::string> tempVect = peer->getOnlineUsers();
    for (int i=0;i<tempVect.size();i++)
    {
        std::string temp = tempVect[i];
        QString qstr = QString::fromStdString(temp);
        ui->listWidget->addItem(qstr);
    }
}

MainWidget::~MainWidget()
{
    peer->RemoteSignOut();
    delete ui;
}


void MainWidget::on_AddImageButton_clicked()
{
    a = new AddImageWindow(this);
    a->setPeer(peer);
    a->show();
}

void MainWidget::on_ViewImageButton_clicked()
{
    v= new ViewImageWindow(this,peer);
    v->show();
}

void MainWidget::on_pushButton_clicked()
{
    if (ui->listWidget->currentItem())
    {
        QString qstr = ui->listWidget->currentItem()->text();
        std::string s = qstr.toUtf8().constData();
        std::string userName = s.substr(0, s.find(' '));
        std::string imageName = s.substr(s.find(' '));
        QString qviews = ui->lineEdit->text();
        std::string views = qviews.toUtf8().constData();
        peer->SendNotification(peer->GetUserName()+" requests "+views+" views on image "+imageName,userName);
    }

}

void MainWidget::on_pushButton_2_clicked()
{
    ui->listWidget->clear();
    std::vector<std::string> tempVect = peer->getOnlineUsers();
    for (int i=0;i<tempVect.size();i++)
    {
        std::string temp = tempVect[i];
        QString qstr = QString::fromStdString(temp);
        ui->listWidget->addItem(qstr);
    }
}

void MainWidget::on_pushButton_3_clicked()
{
    n= new notifications(this,peer);
    n->show();
}
