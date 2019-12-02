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

}

MainWidget::~MainWidget()
{
    delete ui;
}


void MainWidget::on_AddImageButton_clicked()
{
    
    a = new AddImageWindow(this);
    a->setPeer(peer);
    //a->setPeer(peer);
    a->show();

}

void MainWidget::on_ViewImageButton_clicked()
{
    v= new ViewImageWindow(this);
    //v->setPeer(peer);
    v->show();
}
