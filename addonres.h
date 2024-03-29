#ifndef ADDONRES_H
#define ADDONRES_H

#include <QWidget>
#include <QLayout>
#include <QLabel>

#include <QCloseEvent>
#include <QDebug>

namespace Ui {
class AddOnRes;
}

class AddOnRes : public QWidget
{
    Q_OBJECT

public:
    explicit AddOnRes(QWidget *parent = nullptr);
    ~AddOnRes();

    QPixmap getPix() const;
    void setPix(const QPixmap &value);
    void resizeEvent(QResizeEvent* event);

private:
    Ui::AddOnRes *ui;
    QPixmap pix;
    QPixmap pixF;
    //   QLayout layout;
    void closeEvent(QCloseEvent *e);
};

#endif // ADDONRES_H
