#include "dbwriter.h"
#include <QStandardPaths>

DBWriter::DBWriter(QObject* parent): QObject(parent)
{

 //   qDebug() << "DBWriter initialize";
    dbDirPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    dbDir.setPath(dbDirPath);
  //  dbDir = new QDir(dbDirPath);
    if(dbDir.exists())
        qDebug("folder exists");
       else if(dbDir.mkpath(dbDir.path()))
           qDebug("folder created");
  //  dbDir->setPath(dbDirPath);
 //   qDebug() << dbDir.path();



    settingsFile = new QFile();
    settingsFile->setFileName(QApplication::applicationDirPath() + "/settings");
    if (settingsFile->exists()){
 //       qDebug() << "Settings file opened";
    }
    else {
 //       qDebug() << "Settings file created";
    }
}




void DBWriter::saveFile(QList<Note*> list)
{
    qDebug() << "Function of saving file";

    //пишем в папку
    if (list.length() == 1 && list.at(0)->getText() == ""){
        qDebug() << "No images";
        return;
    }

    for (int i = 0; i < list.length(); i++){
        QFileInfo f(list.at(i)->getText());
        QString fs = f.fileName();
        QFile tmpFile(dbDirPath + "/" + fs);
        if (list.at(i)->getImg().size().height() <= 0){
            continue;
        }
     //   QFile tmpFile(list.at(i)->getText());
        qDebug() << tmpFile.fileName();
        if (!tmpFile.exists()){
            if (tmpFile.open(QIODevice::WriteOnly | QIODevice::Truncate)){
                QPixmap tmpPix = list.at(i)->getImg();
                qDebug() << "tmppix param" << tmpPix;
                QFileInfo fi(tmpFile);
                QString ext = fi.suffix();
            //    QString ext = fi.completeSuffix();
                QByteArray ba = fi.suffix().toLocal8Bit();
            //    QByteArray ba = fi.completeSuffix().toLocal8Bit();
                const char *c_str2 = ba.data();
                qDebug() << " Format file: " << c_str2;

                tmpPix.save(&tmpFile, c_str2);
                tmpFile.close();
            }
        }
    }
}

void DBWriter::deleteFile(QString name)
{
//    QFile file(dbDirPath + "/" + name);
    QFile file(name);
 //   qDebug() << "to remove " << name;
   // QFile file(name);
//    qDebug() << "to remove " << file.fileName();
    file.remove();
}

QStringList DBWriter::getFilesList()
{
    QFileInfoList ls = dbDir.entryInfoList(QStringList()<< "*",QDir::Files);
    QStringList ls2;
    for (int i = 0; i < ls.length(); i++){
        ls2.append(ls.at(i).filePath());
    }
//    qDebug() << "ls2";
//    qDebug() << ls2;
    return ls2;
}


QList<Note> DBWriter::readFromFile()
{

    QList <Note> temp2;

    // смотрим что в папке

    lst = dbDir.entryInfoList(QStringList()<< "*",QDir::Files);
   // qDebug() << lst.at(0).filePath();
  //  qDebug() << lst.at(1).filePath();

    for (int i = 0; i < lst.length(); i++){
      //  QFile file(lst.at(i).filePath());
        Note ttNote;
  //      QPixmap tmpPixMap;
     //   if(tmpPixMap.load(lst.at(i).filePath())){
        //    ttNote.setImg(tmpPixMap, lst.at(i).fileName());
            ttNote.addText(lst.at(i).filePath());
  //          qDebug() << "image loaded";
  //      }
        temp2.append(ttNote);
    }

//    for (int i = 0; i < temp2.length(); i++){
//           qDebug() << "Builded QList2 test:" << i << " " << temp2.at(i).getText();
//    }


    if (temp2.length() == 0) {
        Note emptyNote;
        temp2.append(emptyNote);
    }
    return temp2;
}



QPixmap DBWriter::getPixmapFromFile(QString filePath)
{
    QPixmap tmpPixMap;
    if(tmpPixMap.load(filePath)){
        return tmpPixMap;
    }
    else return QPixmap(0,0);
}


