#include "viewimagewindow.h"
#include "ui_viewimagewindow.h"

ViewImageWindow::ViewImageWindow(QWidget *parent,Peer* _peer) : QDialog(parent),
                                                    ui(new Ui::ViewImageWindow)
{
    ui->setupUi(this);
    setPeer(_peer);
    std::map<std::string, std::string> temp;
    std::map<std::string, std::string> temp2;
    std:: string username = peer->GetUserName();
    temp = peer->SearchForStegNames(username);
    //temp2 =peer->RemoteSearchForStegNames(username);
    localMap=&temp;
    remoteMap = &temp2;
    showLists();
    // QPixmap pix("/home/khatter/Desktop/GUI_Distrib/GUI/prof.jpg");
    // ui->label->setPixmap(pix);
}

void ViewImageWindow::showLists()
{

    for (auto cursor = localMap->begin(); cursor != localMap->end(); ++cursor)
    {
        std::string temp = cursor->first + " " + cursor->second;
        QString qstr = QString::fromStdString(temp);
        ui->listWidget->addItem(qstr);
    }

    for (auto cursor = remoteMap->begin(); cursor != remoteMap->end(); ++cursor)
    {
        std::string temp = cursor->first + " " + cursor->second;
        QString qstr = QString::fromStdString(temp);
        ui->listWidget_2->addItem(qstr);
    }
}

void ViewImageWindow::on_pushButton_clicked()
{
    QPixmap pix2("/home/khatter/Desktop/GUI_Distrib/GUI/GRE.png");
    ui->label->setPixmap(pix2);
}

void ViewImageWindow::on_pushButton_2_clicked()
{
    if (ui->listWidget->currentItem())
    {
        QString qstr = ui->listWidget->currentItem()->text();
        std::string s = qstr.toUtf8().constData();
        std::string first_token = s.substr(0, s.find(' '));
        std::cout<<"Before Get Image"<<std::endl;
        StegImage temp = peer->GetImage(first_token);
        std::string pathToImage =PlainImagesDirectory+'/'+temp.getPlainName();
        std::cout<<pathToImage<<std::endl;
        QString qstrimg = QString::fromStdString(pathToImage);
        QPixmap pix(qstrimg);
        ui->label->setPixmap(pix);

    }
}

void ViewImageWindow::on_pushButton_3_clicked()
{
    if (ui->listWidget_2->currentItem())
    {
        QString qstr = ui->listWidget_2->currentItem()->text();
        std::string s = qstr.toUtf8().constData();
        std::string first_token = s.substr(0, s.find(' '));
        peer->RemoteRetrieveImage(first_token);
    }
}

ViewImageWindow::~ViewImageWindow()
{
    delete ui;
}

void ViewImageWindow::on_pushButton_4_clicked()
{
    std::map<std::string, std::string> temp;
    std:: string username = peer->GetUserName();
    temp = peer->SearchForStegNames(username);
    localMap=&temp;
    showLists();
}

void ViewImageWindow::on_pushButton_5_clicked()
{
    std::map<std::string, std::string> temp;
    std:: string username = peer->GetUserName();
    temp = peer->RemoteSearchForStegNames(username);
    remoteMap = &temp;
    showLists();
}
