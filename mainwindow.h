/* Author: Yuriy Kuzin
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QProcess>
#include <QTextStream>
#include <QRegularExpression>
#include <QSystemTrayIcon>
#include "filemonitor.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(int interval
               READ getinterval
               WRITE setinterval
              )
    Q_PROPERTY(QString bumblebeestate
               READ getbumblebeestate
               WRITE setbumblebeestate
              )
    Q_PROPERTY(QString xstate
               READ getxstate
               WRITE setxstate
              )
    Q_PROPERTY(QString discretestate
               READ getdiscretestate
               WRITE setdiscretestate
              )

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QProcess *procexec;
    FileMonitor *bbswitchFile;

    void setinterval(int m_interval)
    {
        interval = m_interval;
    }
    int getinterval() const
    { return interval; }

    void setbumblebeestate(QString m_bumblebeestate)
    {
        int changed = QString::compare(bumblebeestate, m_bumblebeestate);
        bumblebeestate = m_bumblebeestate;
        if (changed != 0)
            {
                emit bumblebeestateChanged(bumblebeestate);
            }
    }
    QString getbumblebeestate() const
    { return bumblebeestate; }

    void setxstate(QString m_xstate)
    {
        int changed = QString::compare(xstate, m_xstate);
        xstate = m_xstate;
        if (changed != 0)
            {
                emit xstateChanged(xstate);
            }
    }
    QString getxstate() const
    { return xstate; }

    void setdiscretestate(QString m_discretestate)
    {
        int changed = QString::compare(discretestate, m_discretestate);
        discretestate = m_discretestate;
        if (changed != 0)
            {
                emit discretestateChanged(discretestate);
            }
    }
    QString getdiscretestate() const
    { return discretestate; }

private:
    Ui::MainWindow *ui;
    QTimer executeUseLoopTimer;
    int interval;
    QString bumblebeestate;
    QString xstate;
    QString discretestate;

    void createActions();
    void createTrayIcon();
    void updateTrayTooltip();

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

signals:
    void bumblebeestateChanged(QString bumblebeestate);
    void xstateChanged(QString xstate);
    void discretestateChanged(QString discretestate);

private slots:
    void on_executeUseLoopTimer();
    void on_procexec_closed(int reason);
    void on_interval_spinbox_valueChanged(int value);

    void cmdUpdateText();
    void cmdUpdateError();

    void on_bumblebeestateChanged(QString bumblebeestate);
    void on_xstateChanged(QString xstate);
    void on_discretestateChanged(QString discretestate);

    void iconActivated(QSystemTrayIcon::ActivationReason reason);

public slots:
    void on_bbswitchFile_Changed(const QString &content);

};

#endif // MAINWINDOW_H
