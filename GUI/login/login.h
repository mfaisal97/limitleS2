#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "../../header/Peer.h"
#include "../../common/Constants.h"

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    void setPeer(Peer* _peer){
        //std::cout << "Peer passed!";
        peer = _peer;
    }
    ~Login();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Peer* peer;
    Ui::Login *ui;
};

#endif // LOGIN_H
