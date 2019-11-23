#ifndef VIEWIMAGEWINDOW_H
#define VIEWIMAGEWINDOW_H

#include <QDialog>

namespace Ui {
class ViewImageWindow;
}

class ViewImageWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ViewImageWindow(QWidget *parent = nullptr);
    ~ViewImageWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::ViewImageWindow *ui;
};

#endif // VIEWIMAGEWINDOW_H
