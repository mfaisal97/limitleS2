#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "../../header/Peer.h"

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(Peer* peer, QWidget *parent = nullptr);
    void setPeer(Peer* _peer){
        std::cout << "Peer passed!";
        peer = _peer;
    }
    ~Login();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_clicked(bool checked);

private:
    Peer* peer;
    Ui::Login *ui;
};

#endif // LOGIN_H
