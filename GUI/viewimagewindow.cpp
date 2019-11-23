#include "viewimagewindow.h"
#include "ui_viewimagewindow.h"
#include <QPixmap>
#include <map>
#include <string>
using namespace std;

ViewImageWindow::ViewImageWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewImageWindow)
{
    ui->setupUi(this);
    QPixmap pix("/home/khatter/Desktop/GUI_Distrib/GUI/prof.jpg");
    ui->label->setPixmap(pix);

    map<string,string> exMap;
    map<string,string> exMap2;

    exMap["baua"]="1";
    exMap["aona"]="10";
    exMap["oana"]="100";
    exMap["ojao"]="1000";

    exMap2["iobd"]="2";
    exMap2["iobs"]="20";
    exMap2["ioba"]="200";
    exMap2["iobi"]="2000";

    for(auto cursor = exMap.begin(); cursor!=exMap.end(); ++cursor){
        string temp =cursor->first + " " +cursor->second;
        QString qstr = QString::fromStdString(temp);
        ui->listWidget->addItem(qstr);
    }

    for(auto cursor = exMap2.begin(); cursor!=exMap2.end(); ++cursor){
        string temp =cursor->first + " " +cursor->second;
        QString qstr = QString::fromStdString(temp);
        ui->listWidget_2->addItem(qstr);
    }

}

ViewImageWindow::~ViewImageWindow()
{
    delete ui;
}

void ViewImageWindow::on_pushButton_clicked()
{
    QPixmap pix2("/home/khatter/Desktop/GUI_Distrib/GUI/GRE.png");
    ui->label->setPixmap(pix2);
}

void ViewImageWindow::on_pushButton_2_clicked()
{
    if(ui->listWidget->currentItem()){
        ui->listWidget->currentItem()->text();}
}

void ViewImageWindow::on_pushButton_3_clicked()
{
    if(ui->listWidget_2->currentItem()){
        ui->listWidget_2->currentItem()->text();}
}
