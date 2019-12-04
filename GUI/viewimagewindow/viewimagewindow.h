#ifndef VIEWIMAGEWINDOW_H
#define VIEWIMAGEWINDOW_H

#include <QDialog>
#include <QPixmap>
#include <map>
#include <string>
#include "../../header/Peer.h"

namespace Ui {
class ViewImageWindow;
}

class ViewImageWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ViewImageWindow(QWidget *parent = nullptr,Peer* _peer=nullptr);
    void showLists();
    void setPeer(Peer* _peer){
        peer = _peer;
    }
    ~ViewImageWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Peer* peer;
    std::map<std::string, std::string> *localMap;
    std::map<std::string, std::string> *remoteMap;
    Ui::ViewImageWindow *ui;
};

#endif // VIEWIMAGEWINDOW_H
