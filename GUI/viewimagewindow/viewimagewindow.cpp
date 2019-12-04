#include "viewimagewindow.h"
#include "ui_viewimagewindow.h"

ViewImageWindow::ViewImageWindow(QWidget *parent) : QDialog(parent),
                                                    ui(new Ui::ViewImageWindow)
{
    ui->setupUi(this);
    // QPixmap pix("/home/khatter/Desktop/GUI_Distrib/GUI/prof.jpg");
    // ui->label->setPixmap(pix);
}

void ViewImageWindow::showLists(std::map<std::string, std::string> *exMap, std::map<std::string, std::string> *exMap2)
{
    // (*exMap)["baua"] = "1";
    // (*exMap)["aona"] = "10";
    // (*exMap)["oana"] = "100";
    // (*exMap)["ojao"] = "1000";

    // (*exMap2)["iobd"] = "2";
    // (*exMap2)["iobs"] = "20";
    // (*exMap2)["ioba"] = "200";
    // (*exMap2)["iobi"] = "2000";

    for (auto cursor = exMap->begin(); cursor != exMap->end(); ++cursor)
    {
        std::string temp = cursor->first + " " + cursor->second;
        QString qstr = QString::fromStdString(temp);
        ui->listWidget->addItem(qstr);
    }

    for (auto cursor = exMap2->begin(); cursor != exMap2->end(); ++cursor)
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
        ui->listWidget->currentItem()->text();
    }
}

void ViewImageWindow::on_pushButton_3_clicked()
{
    if (ui->listWidget_2->currentItem())
    {
        ui->listWidget_2->currentItem()->text();
    }
}

ViewImageWindow::~ViewImageWindow()
{
    delete ui;
}
