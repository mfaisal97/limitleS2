#include "mainwidget.h"
#include "ui_mainwidget.h"


MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    Login l(peer);
    l.setPeer(peer);
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
    a->show();

}

void MainWidget::on_ViewImageButton_clicked()
{
    v= new ViewImageWindow(this);
    v->setPeer(peer);
    v->show();
}
