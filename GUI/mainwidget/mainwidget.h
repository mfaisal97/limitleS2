#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "../addimagewindow/addimagewindow.h"
#include "../viewimagewindow/viewimagewindow.h"
#include "../notifications/notifications.h"
#include "../login/login.h"
#include "../../header/Peer.h"
#include "../../common/Constants.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr,Peer* _peer=nullptr);
    void setPeer(Peer* _peer){
        peer = _peer;
    }
    ~MainWidget();

private slots:
    void on_AddImageButton_clicked();

    void on_ViewImageButton_clicked();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Peer *peer;
    Ui::MainWidget *ui;
    AddImageWindow *a;
    ViewImageWindow *v; 
    notifications *n;
};
#endif // MAINWIDGET_H
