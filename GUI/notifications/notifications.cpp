#include "notifications.h"
#include "ui_notifications.h"

notifications::notifications(QWidget *parent, Peer* _peer) :
    QDialog(parent),
    ui(new Ui::notifications)
{
    setPeer(_peer);
    ui->setupUi(this);

    ui->listWidget->clear();
    std::vector<std::string> tempVect = peer->GetNotifications();
    for (int i=0;i<tempVect.size();i++)
    {
        std::string temp = tempVect[i];
        QString qstr = QString::fromStdString(temp);
        ui->listWidget->addItem(qstr);
    }
}

notifications::~notifications()
{
    delete ui;
}
