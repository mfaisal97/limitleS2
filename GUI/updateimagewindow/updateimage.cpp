#include "updateimage.h"
#include "ui_updateimage.h"

UpdateImage::UpdateImage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateImage)
{
    ui->setupUi(this);
}

UpdateImage::~UpdateImage()
{
    delete ui;
}
