#include "addimagewindow.h"
#include "ui_addimagewindow.h"

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

    QString imagePath=ui->lineEdit->text();
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

void AddImageWindow::on_pushButton_clicked()
{
    QString imagePath=ui->lineEdit->text();
    //std::cout<<"Image path in QT "<<imagePath.toUtf8().constData()<<std::endl;
    //std::string im = peer->GetUserName();
    //std::cout << "test: " << im << std::endl;
    imageToBeAdded = new StegImage(imagePath.toUtf8().constData(),peer->GetUserName());
    for(int i=0;i<pickedUsers.size();i++){
        if(imageToBeAdded->addUser(pickedUsers[i][0],stoi(pickedUsers[i][1]))){
            std::cout<<"Added user "<<pickedUsers[i][0]<<" Successfully"<<std::endl;
        }
        else{
            std::cout<<"Adding user "<<pickedUsers[i][0]<<"failed"<<std::endl;
        }
    }
    imageToBeAdded->saveStegImage();
    std::cout<<"Hash"<<imageToBeAdded->GetHash();
    //peer->RemoteUpdateStegImage();
    //close();
}

