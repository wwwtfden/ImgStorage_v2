#ifndef DBWRITER_H
#define DBWRITER_H
#include <QtWidgets>
#include <QFile>
#include <QObject>
#include "note.h"
#include <QList>

class DBWriter  : public QObject
{

public:
    explicit DBWriter(QObject *parent = 0);
    QList<Note> readFromFile();
    void saveFile(QList<Note*> list);
    void deleteFile(QString name);
    QStringList getFilesList();
    QPixmap getPixmapFromFile(QString filePath);
;

private:
    QFile* settingsFile;
    QDir dbDir;
    QString dbDirPath;
    QFileInfoList lst;
  //  QByteArray bar;

};

#endif // DBWRITER_H
