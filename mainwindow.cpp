#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <deletenotdialog.h>


#include <QHash>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createMenuData();
 //   savedFlag = false;

    aResWidget = new AddOnRes();

    this->setWindowTitle("Image Viewer");
    activeNoteIndex = 0;

    QList<Note> lst = dbTools.readFromFile(); //читаем файл базы данных


    for (int i = 0; i < lst.length(); i++){ // инициализируем коллекцию заметок из буфера, который мы создали из файла
        Note* note = new Note(lst.at(i).getText(), lst.at(i).getImg());
        noteList.append(note);
        qDebug() << note->getText();
    }
    noteList.at(activeNoteIndex)->setImg(dbTools.getPixmapFromFile(noteList.at(activeNoteIndex)->getText()), noteList.at(activeNoteIndex)->getText());
    displayNoteData();

    connect(ui->label, &QExLabel::imgDeleted, this, &MainWindow::deleteImg);
    connect(ui->label, &QExLabel::imgDClicked, this, &MainWindow::addResWgtInit);
    connect(ui->label, &QExLabel::imgExported, this, &MainWindow::exportImg);
    ui->listWidget->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addNewNote()
{
    Note* note = new Note();
    if (noteList.length() <= 1 && (noteList.at(0)->getText() == "")){
        noteList.clear();
    }
    noteList.append(note); //заметки добавляются в конец QList
    qDebug() << "noteList.length()" << noteList.length();
    activeNoteIndex = noteList.length() - 1; //меняем флаг активной заметки на индекс новой заметки
    qDebug() << "activeNoteIndex" << activeNoteIndex;
    ui->listWidget->setCurrentRow(activeNoteIndex); //делаем активной позицию данной заметки в виджете списка
    displayNoteData();
}

void MainWindow::displayNoteData() //тут обновляем элементы интерфейса
{
    qDebug() << "Updating listView";
    ui->listWidget->clear();
    for (int i = 0; i < noteList.length(); i++){
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(parseStr(noteList.at(i)->getText()));
        ui->listWidget->addItem(item);
    }
    ui->listWidget->setCurrentRow(activeNoteIndex, QItemSelectionModel::ToggleCurrent);

    drawImage();

    //savedFlag = false;
}

QString MainWindow::parseStr(QString str)  //парсинг элемента виджета списка, для стандартизации внешнего вида
{
    QFileInfo fileinfo(str);
    str = fileinfo.fileName();

    if (str.length() == 0) { return "..."; }
    if (str.length()> 25){
        str.truncate(25);
        str = str + "...";
    }
    for (int i = 0; i < str.length(); i++){
        QChar c = str.at(i);
        if (c == '\n'){
       //  qDebug() << "Found tag at index position" << i;
      //   str.remove(i, 1);
         str.replace(i,1,' ');
        }
    }
    return str;
}

void MainWindow::setImageToNote() // добавляем картинку к объекту Note
{
 //   QString fName = QFileDialog::getOpenFileName();
  //  QString fName = QFileDialog::getOpenFileName(this, QString("Открыть файл"), QString(), QString("Изображения (*.jpg,*.png,*.jpeg,*.bmp);; Все файлы (*.*)"));
    QString fName = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "", tr("Images (*.png *.jpeg *.jpg)"));
    QString fileName = fName.section("/",-1,-1);
    qDebug() << fName;
//    qDebug() << fileName;
    if(tmpImg.load(fName)){
        noteList[activeNoteIndex]->setImg(tmpImg, fName);
        qDebug() << "image loaded";
        displayNoteData();
        //сделать save отдельного файла
        saveDbFile();
    }
    else {
//        qDebug() << "error while image loaded";
        deleteImgWithoutDialog();
    }
}

void MainWindow::drawImage()
{
    ui->label->clear();
    tmpImg2 = noteList[activeNoteIndex]->getImg().scaled(QSize(ui->label->width(), ui->label->height()), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label->setPixmap(tmpImg2);
    ui->label->repaint();
    cleaner();

    // EXPERIMENTAL
     aResWidget->setPix(noteList.at(activeNoteIndex)->getImg());
}


void MainWindow::on_pushButton_clicked() //кнопка добавить заметку
{
    addNewNote();
    setImageToNote();
    if (noteList.at(activeNoteIndex)->getText() == ""){
 //       qDebug() << "Not added!!!";
    }
    dbTools.saveFile(noteList);
}


void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)//при нажатии на элемент в списке
{
    activeNoteIndex = ui->listWidget->currentRow();
    ui->listWidget->setCurrentRow(activeNoteIndex, QItemSelectionModel::ToggleCurrent); // удалить если что, проба
 //   ui->plainTextEdit->setPlainText(noteList.at(activeNoteIndex)->getText());
    QPixmap pix = dbTools.getPixmapFromFile(noteList.at(activeNoteIndex)->getText());
    noteList.at(activeNoteIndex)->setImg(pix, noteList.at(activeNoteIndex)->getText());

    currentImgName = noteList.at(activeNoteIndex)->getText();
    //test function
    displayNoteData();
   // drawImage();
}

void MainWindow::on_pushButton_2_clicked() //кнопка удалить заметку
{
    deleteImg();
}


//void MainWindow::on_pushButton_3_clicked() //добавляем изображение
//{
//    setImageToNote();
//}


void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
//   qDebug() << ui->label->size();
   displayNoteData();
}

void MainWindow::createMenuData()
{
    //Создаем менюху с пунктом save
    QMenu *fileMenu = new QMenu("File");
  //  QAction *saveAction = new QAction("Save", fileMenu);
 //   connect(saveAction, &QAction::triggered, this, &MainWindow::saveDbFile);
  //  fileMenu->addAction(saveAction);

    QAction *closeAction = new QAction("Close", fileMenu);
    connect(closeAction, &QAction::triggered, this, &MainWindow::forceCloseApp);
    fileMenu->addAction(closeAction);

    ui->menubar->addMenu(fileMenu);

}

void MainWindow::forceCloseApp() //обработчик кнопки закрыть
{
//    if (!savedFlag){
//       SaveOnCloseDialog* saveOnCloseDialog = new SaveOnCloseDialog;
//       if (saveOnCloseDialog->exec() == QDialog::Accepted){
//           saveDbFile();
//            qDebug() << "Save on close completed!";
//            QApplication::quit();
//       } else if (saveOnCloseDialog->exec() == QDialog::Rejected) {
//           QApplication::quit();
//       }
//    } else {
//        QApplication::quit();
//    }
    saveDbFile();
}


bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{

        if (event->type() == QEvent::KeyPress) // тут переопределяем нажатие клавиш вверх и вниз
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

            if (keyEvent->key() == Qt::Key_Down)
            {
          //      qDebug() << "Key Down Pressed";
                int row = ui->listWidget->currentRow();
          //      qDebug() << "current row" << row;
                if(row < noteList.length() - 1){
                    ui->listWidget->setCurrentRow(row+1);
                }
                else {
                    ui->listWidget->setCurrentRow(0);
                }
                activeNoteIndex = ui->listWidget->currentRow();

                noteList.at(activeNoteIndex)->setImg(dbTools.getPixmapFromFile(noteList.at(activeNoteIndex)->getText()), noteList.at(activeNoteIndex)->getText());
                drawImage();

                // EXPERIMENTAL
               //  aResWidget->setPix(noteList.at(activeNoteIndex)->getImg());

           //     qDebug() << "Now current row " << ui->listWidget->currentRow() << " activeNoteIndex" << activeNoteIndex;
  //              ui->plainTextEdit->setPlainText(noteList.at(activeNoteIndex)->getText());
                return true;
            }
            else if (keyEvent->key() == Qt::Key_Up)
            {
           //     qDebug() << "Key Up Pressed";
                int row = ui->listWidget->currentRow();
           //     qDebug() << row;
                if(row > 0){
                    ui->listWidget->setCurrentRow(row - 1);
                }
                else {
                    ui->listWidget->setCurrentRow(noteList.length() - 1);
                }
                activeNoteIndex = ui->listWidget->currentRow();
                noteList.at(activeNoteIndex)->setImg(dbTools.getPixmapFromFile(noteList.at(activeNoteIndex)->getText()), noteList.at(activeNoteIndex)->getText());
                drawImage();


                // EXPERIMENTAL
                // aResWidget->setPix(noteList.at(activeNoteIndex)->getImg());


           //     qDebug() << "Now current row " << ui->listWidget->currentRow() << " activeNoteIndex" << activeNoteIndex;
  //              ui->plainTextEdit->setPlainText(noteList.at(activeNoteIndex)->getText());
                return true;
            } else if (keyEvent->key() == Qt::Key_Delete) // обработчик кнопки Delete
            {
                deleteImg();
            }
        }


    if(watched == ui->label)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            //обрабатываем щелчок мышки по лейблу
      //      qDebug() << event->type();
        }
    }
     return QWidget::eventFilter(watched, event);
}

void MainWindow::deleteImg() //удаление изображения
{
    DeleteNotDialog* deleteDialog = new DeleteNotDialog;
    if(deleteDialog->exec() == QDialog::Accepted){
        if (noteList.length() >  1){
     //   qDebug() << "current Item Index" << activeNoteIndex;
        dbTools.deleteFile(noteList.at(activeNoteIndex)->getText());
        dbTools.deleteFile(noteList.at(activeNoteIndex)->getText()); // удаление реального файла
        noteList.removeAt(activeNoteIndex);
        if(activeNoteIndex > 0){
        activeNoteIndex--;
        } else {
        activeNoteIndex = 0;
        }
        ui->listWidget->setCurrentRow(activeNoteIndex);
        displayNoteData();

        } else {
            activeNoteIndex = 0;
            dbTools.deleteFile(noteList.at(0)->getText());
            noteList.at(activeNoteIndex)->setImg(QPixmap(0,0), "");
            ui->label->clear();
          //  setImageToNote();
            displayNoteData();
        }
    }
}

void MainWindow::exportImg()
{
 //   qDebug() << "exportImg()";
    // "/database.bin"
    QString exFileName = noteList.at(activeNoteIndex)->getText();
    QFileInfo f(exFileName);
    exFileName = f.fileName();
    QDir saveDir = QFileDialog::getExistingDirectory();
    QString saveDirPath = saveDir.path();
    qDebug() << saveDirPath;
    qDebug() << exFileName;
    QFile exFile(QString(saveDirPath + "/" + exFileName));
    if (exFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QPixmap tmpPix = noteList.at(activeNoteIndex)->getImg();
        QFileInfo fi(exFile);
    //    QString ext = fi.completeSuffix();
        QString ext = fi.suffix();
   //     QByteArray ba = fi.completeSuffix().toLocal8Bit();
        QByteArray ba = fi.suffix().toLocal8Bit();

        const char *c_str2 = ba.data();
        qDebug() << "c_str2" << c_str2;
        tmpPix.save(&exFile, c_str2);
        qDebug() << "tmpPix.size() " << tmpPix.size();
        exFile.close();
    }
}

void MainWindow::deleteImgWithoutDialog()
{
    if (noteList.length() >  1){
//    qDebug() << "current Item Index" << activeNoteIndex;
    dbTools.deleteFile(noteList.at(activeNoteIndex)->getText());
    noteList.removeAt(activeNoteIndex);
    if(activeNoteIndex > 0){
    activeNoteIndex--;
    } else {
    activeNoteIndex = 0;
    }
    ui->listWidget->setCurrentRow(activeNoteIndex);
    displayNoteData();

    } else {
        activeNoteIndex = 0;
        dbTools.deleteFile(noteList.at(0)->getText());
        noteList.at(activeNoteIndex)->setImg(QPixmap(0,0), "");
        ui->label->clear();
       // setImageToNote();
        displayNoteData();
    }
}

void MainWindow::addResWgtInit()
{
//    qDebug() << "Double click from MainWindow";
//    AddOnRes aResWidget;
 //   aResWidget.setPix(noteList.at(activeNoteIndex)->getImg());
 //   aResWidget.initData();
 //   aResWidget = new AddOnRes();
    aResWidget->show();
    aResWidget->setPix(noteList.at(activeNoteIndex)->getImg());
    aResWidget->repaint();

    // добавить флаг что areswidget активен
}

void MainWindow::saveDbFile()
{
//    qDebug() << "SLOT(saveDbFile()";
    dbTools.saveFile(noteList);
}

void MainWindow::closeEvent(QCloseEvent *e)
{
//    qDebug() << "Close event terminated.";
    forceCloseApp();
}

void MainWindow::cleaner()
{
    for (int i  = 0; i < noteList.length(); i++){
        if (i != activeNoteIndex){
            noteList.at(i)->clrImg();
           // qDebug() << "cleaned " << noteList.at(i)->getText();
            //continue;
        } else {
            qDebug() << "not cleaned " << noteList.at(activeNoteIndex)->getText();
            qDebug() << noteList.at(activeNoteIndex)->getImg();
            continue;
        }
    }
}

void MainWindow::on_debugButton_clicked()
{
    qDebug() << currentImgName;
//    qDebug() << "Program info";
//    qDebug() << "NoteList in Widget: length " << noteList.length();
//    for (int i = 0; i < noteList.length(); i++){
//        qDebug() << noteList.at(i)->getText();
//        qDebug() << noteList.at(i)->getImg();
//        qDebug() << qHash(noteList.at(i));
//    }
//    dbTools.getFilesList();

//    cleaner();

    qDebug() << "noteList.length()" << noteList.length();
    qDebug() << "dbTools.getFilesList().length()" << dbTools.getFilesList().length();

    for (int i = 0; i < noteList.length(); i++){
        QSize s = (noteList.at(i)->getImg().size());
        QSize sravn(0,0);
        if (s.height() > sravn.height()){
            qDebug() << "picture in mem " << s;
        }
    }

}
