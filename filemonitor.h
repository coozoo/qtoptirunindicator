/* Author: Yuriy Kuzin
 */

#ifndef FILEMONITOR_H
#define FILEMONITOR_H

#include <QObject>
#include <QCoreApplication>
#include <QFileSystemWatcher>
#include <QString>
#include <QFile>
#include <QDebug>

class FileMonitor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fileName
               READ getfileName
               WRITE setfileName
              )
public:
    explicit FileMonitor(QObject *parent = nullptr);
    void setfileName(QString m_fileName)
    {
        fileName = m_fileName;
    }
    QString getfileName() const
    { return fileName; }
    void init();
    void ReadFullFile();
    QString prevfiletext;
private:
  QString fileName;
  QFileSystemWatcher * watcher;

signals:
  void fileMessage(const QString &message);

public slots:

private slots:

  void on_directoryChanged(const QString & path);
  void on_fileChanged(const QString & path);


};

#endif // FILEMONITOR_H
