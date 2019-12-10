#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H
#include "../../header/Peer.h"
#include <QDialog>

namespace Ui {
class notifications;
}

class notifications : public QDialog
{
    Q_OBJECT

public:
    explicit notifications(QWidget *parent = nullptr, Peer* _peer=nullptr);
    void setPeer(Peer* _peer){
        peer = _peer;
    }
    ~notifications();

private:
    Peer *peer;
    Ui::notifications *ui;
};

#endif // NOTIFICATIONS_H
