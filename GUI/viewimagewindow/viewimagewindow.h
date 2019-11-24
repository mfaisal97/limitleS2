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
    explicit ViewImageWindow(QWidget *parent = nullptr);
    void setPeer(Peer* _peer){
        peer = _peer;
    }
    ~ViewImageWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

private:
    Peer* peer;
    Ui::ViewImageWindow *ui;
};

#endif // VIEWIMAGEWINDOW_H
