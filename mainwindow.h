#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <note.h>
#include <QtWidgets>
#include <QList>
#include <dbwriter.h>
#include "addonres.h"

#include <QPointer>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void displayNoteData();
    QString parseStr(QString str);
    void resizeEvent(QResizeEvent* event);
    void createMenuData();

public slots:
    void addNewNote();
    void setImageToNote();
    void drawImage();
    void forceCloseApp();
//    void deleteNote();

    bool eventFilter(QObject *watched, QEvent *event);

    void deleteImg();
    void exportImg();
    void deleteImgWithoutDialog();

    void addResWgtInit();

    void saveDbFile();

    void closeEvent(QCloseEvent* e) override;

    void cleaner();


private slots:

    void on_pushButton_clicked();
   // void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_pushButton_2_clicked();
//    void on_pushButton_3_clicked();
//    void on_listWidget_itemSelectionChanged();
  //  void keyPressEvent(QKeyEvent *event) override;
    void on_debugButton_clicked();

private:
    Ui::MainWindow *ui;
    QList<Note*> noteList;
    int activeNoteIndex;
  //  bool savedFlag;
  //  bool constructorFlag;

    DBWriter dbTools;

    QPointer<AddOnRes> aResWidget;
   // AddOnRes* aResWidget;

    QPixmap tmpImg;
    QPixmap tmpImg2;

    //тут будет название текущего файла
    QString currentImgName;

};
#endif // MAINWINDOW_H
