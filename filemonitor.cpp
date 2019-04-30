/* Author: Yuriy Kuzin
 */
#include "filemonitor.h"

FileMonitor::FileMonitor(QObject *parent) : QObject(parent)
{
    watcher = new QFileSystemWatcher(this);
    connect(watcher, SIGNAL(fileChanged(const QString &)), this, SLOT(on_fileChanged(const QString &)));
    //connect(watcher, SIGNAL(directoryChanged(const QString &)), this, SLOT(on_directoryChanged(const QString &)));

}

void FileMonitor::init()
{
    //watcher->addPath(qApp->applicationDirPath());
    //watcher->addPath(qApp->applicationDirPath()+"/" + getfileName());
    watcher->addPath(getfileName());
}

void FileMonitor::on_directoryChanged(const QString &path)
{
    qDebug() << path;

}

void FileMonitor::on_fileChanged(const QString &path)
{
    //qDebug() << path;
    Q_EMIT ReadFullFile();
}

void FileMonitor::ReadFullFile()
{
    //QFile file(qApp->applicationDirPath() + "/" +getfileName());
    QFile file(getfileName());
    QString filetext = QString();
    if (!file.exists())
        {
            qDebug() << "no such file";
        }
    else
        {
            file.open(QIODevice::ReadOnly);
            filetext = file.readAll();
            file.close();

        }
    if (prevfiletext != filetext)
        {
            prevfiletext = filetext;
            qDebug() << getfileName();
            emit fileMessage(filetext);
        }
}

