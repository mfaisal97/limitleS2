#include "viewimagewindow.h"
#include "ui_viewimagewindow.h"

ViewImageWindow::ViewImageWindow(QWidget *parent,Peer* _peer) : QDialog(parent),
                                                    ui(new Ui::ViewImageWindow)
{
    ui->setupUi(this);
    setPeer(_peer);

    std:: string username = peer->GetUserName();
    //temp2 =peer->RemoteSearchForStegNames(username);
    localMap = new std::map<std::string, std::string>;
    *localMap = peer->SearchForStegNames(username);
    remoteMap = new std::map<std::string, std::string>;
    *remoteMap = peer->RemoteSearchForStegNames(username);
    showLists();
    // QPixmap pix("/home/khatter/Desktop/GUI_Distrib/GUI/prof.jpg");
    // ui->label->setPixmap(pix);
}

void ViewImageWindow::showLists()
{
    ui->listWidget->clear();
    ui->listWidget_2->clear();
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
        StegImage temp = peer->GetImage(first_token);
        std::string pathToImage =PlainImagesDirectory+'/'+temp.getPlainName();
        std::cout<< "Showing image from:\t" << pathToImage<<std::endl;
        QString qstrimg = QString::fromStdString(pathToImage);
        QPixmap pix(qstrimg);
        if (pix.isNull()){
          std::cout << "eh dh y beh" << std::endl;
        }
        int w = ui->label->width();
        int h = ui->label->height();
        ui->label->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        temp.removePlainImage();
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
        StegImage temp = peer->GetImage(first_token);
        std::string pathToImage =PlainImagesDirectory+'/'+temp.getPlainName();
        std::cout<< "Showing image from:\t" << pathToImage<<std::endl;
        QString qstrimg = QString::fromStdString(pathToImage);
        QPixmap pix(qstrimg);
        if (pix.isNull()){
          std::cout << "eh dh y beh" << std::endl;
        }
        int w = ui->label->width();
        int h = ui->label->height();
        ui->label->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        temp.removePlainImage();
    }
}

ViewImageWindow::~ViewImageWindow()
{
    delete ui;
}

void ViewImageWindow::on_pushButton_4_clicked()
{
    std:: string username = peer->GetUserName();
    *localMap = peer->SearchForStegNames(username);
    showLists();
}

void ViewImageWindow::on_pushButton_5_clicked()
{

    std:: string username = peer->GetUserName();
    *remoteMap = peer->RemoteSearchForStegNames(username);
    showLists();
}
