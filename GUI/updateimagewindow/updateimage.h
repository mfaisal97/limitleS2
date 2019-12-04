#ifndef UPDATEIMAGE_H
#define UPDATEIMAGE_H

#include <QDialog>

namespace Ui {
class UpdateImage;
}

class UpdateImage : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateImage(QWidget *parent = nullptr);
    ~UpdateImage();

private:
    Ui::UpdateImage *ui;
};

#endif // UPDATEIMAGE_H
