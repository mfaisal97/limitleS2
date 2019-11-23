#include "addimagewindow.h"
#include "ui_addimagewindow.h"
#include <QTableWidgetItem>
#include <string>
#include <vector>
using namespace std;
AddImageWindow::AddImageWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddImageWindow)
{
    ui->setupUi(this);
}


AddImageWindow::~AddImageWindow()
{
    delete ui;
}

void AddImageWindow::on_pushButton_2_clicked()
{
    QTableWidgetItem *userItem = new QTableWidgetItem();
    QTableWidgetItem *viewsItem = new QTableWidgetItem();


    userItem->setText(ui->lineEdit_2->text());
    viewsItem->setText(ui->lineEdit_3->text());
    std::vector<std::string> tempVect;
    tempVect.push_back(ui->lineEdit_2->text().toUtf8().constData());
    tempVect.push_back(ui->lineEdit_3->text().toUtf8().constData());
    pickedUsers.push_back(tempVect);
    int insertRow = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(insertRow);
    ui->tableWidget->setItem(insertRow,0,userItem);
    ui->tableWidget->setItem(userItem->row(),1,viewsItem);

}
