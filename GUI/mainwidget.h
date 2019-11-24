#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "addimagewindow.h"
#include "viewimagewindow.h"
#include "../header/Peer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private slots:
    void on_AddImageButton_clicked();

    void on_ViewImageButton_clicked();

private:
    Peer *peer;
    Ui::MainWidget *ui;
    AddImageWindow *a;
    ViewImageWindow *v;
};
#endif // MAINWIDGET_H
