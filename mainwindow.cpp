/* Author: Yuriy Kuzin
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->interval_spinBox, SIGNAL(valueChanged(int)), this, SLOT(on_interval_spinbox_valueChanged(int)));
    setinterval(ui->interval_spinBox->value() * 1000);
    connect(this, SIGNAL(bumblebeestateChanged(QString)), this, SLOT(on_bumblebeestateChanged(QString)));
    connect(this, SIGNAL(xstateChanged(QString)), this, SLOT(on_xstateChanged(QString)));
    connect(this, SIGNAL(discretestateChanged(QString)), this, SLOT(on_discretestateChanged(QString)));
    createActions();
    createTrayIcon();
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);

    procexec = new QProcess();
    connect(procexec, SIGNAL(finished(int)), this, SLOT(on_procexec_closed(int)));

    //read all outputs from command line executor
    connect(procexec, SIGNAL(readyReadStandardError()), this, SLOT(cmdUpdateError()));
    connect(procexec, SIGNAL(readyReadStandardOutput()), this, SLOT(cmdUpdateText()));

    connect(&executeUseLoopTimer, SIGNAL(timeout()), this, SLOT(on_executeUseLoopTimer()));

    bbswitchFile = new FileMonitor(this);
    bbswitchFile->setfileName("/proc/acpi/bbswitch");
    bbswitchFile->init();
    connect(bbswitchFile, SIGNAL(fileMessage(QString)), this, SLOT(on_bbswitchFile_Changed(QString)));

    if (!executeUseLoopTimer.isActive())
        {
            executeUseLoopTimer.setSingleShot(true);
            executeUseLoopTimer.start(0);
        }
    QIcon icon = QIcon(":/images/intel_circle.png");
    trayIcon->setIcon(icon);
    trayIcon->show();
}

MainWindow::~MainWindow()
{
    if (!executeUseLoopTimer.isActive())
        {
            executeUseLoopTimer.stop();
        }
    executeUseLoopTimer.deleteLater();
    if (procexec->processId() > 0)
        {
            procexec->close();
        }
    procexec->deleteLater();
    delete ui;
}

void MainWindow::on_executeUseLoopTimer()
{
    if (!procexec->isOpen())
        {
            procexec->start("optirun --status");
        }


}


/* Slot to execute on command line process closed */
void MainWindow::on_procexec_closed(int reason)
{
    Q_UNUSED(reason)
    //QTextStream cout(stdout);
    //cout<<reason<<endl;
    if (procexec->isOpen())
        {
            procexec->close();
        }
}

//on error stream after excute command
void MainWindow::cmdUpdateError()
{
    QString appendText(procexec->readAll());
    QTextStream cout(stdout);
    cout << appendText << endl;
}

void MainWindow::cmdUpdateText()
{
    QTextStream cout(stdout);
    QString appendText = "";
    while (procexec->canReadLine())
        {
            QString line = procexec->readLine().trimmed();
            if (!line.isNull() && line != "" && !line.isEmpty())
                {
                    appendText = appendText + line + "\n";
                }
        }
    QRegularExpression reg("(.+\\(.+\\))\\.\\sX(.+)\\.");
    QString bumble = "";
    QString xpid = "";
    QString discrete = "";

//Bumblebee status: Ready (3.2.1). X inactive. Discrete video card is off.
//Bumblebee status: Ready (3.2.1). X is PID 8806, 1 applications using bumblebeed.
    QRegularExpressionMatchIterator i = reg.globalMatch(appendText);
    if (i.isValid())
        {
            while (i.hasNext())
                {
                    QRegularExpressionMatch match = i.next();
                    //logcatFiltered_plaintextedit->appendHtml("<html><font color=\"orange\">"+match.captured(1).trimmed()+"</font></html>");
                    //emit regexpMessageReceived(match.captured(1).right(match.captured(1).size()-match.captured(1).indexOf("bundle",0,Qt::CaseInsensitive)-6).trimmed());
                    bumble = match.captured(1).trimmed().split(":")[1].trimmed();

                    if (match.captured(2).contains("inactive"))
                        {
                            xpid = "inactive";
                            if (match.captured(2).split(".")[1].contains("off"))
                                {
                                    discrete = "off";
                                }
                        }
                    if (match.captured(2).contains("PID"))
                        {
                            QRegularExpression pidreg(".+PID\\s([0-9]+),\\s([0-9]+)?\\s.+");

                            QRegularExpressionMatchIterator pidregiterrator = pidreg.globalMatch(match.captured(2));
                            if (pidregiterrator.isValid())
                                {

                                    QRegularExpressionMatch pidregmatch = pidregiterrator.next();
                                    //cout<<pidregmatch.lastCapturedIndex();
                                    xpid = pidregmatch.captured(1);
                                    discrete = pidregmatch.captured(2);
                                }

                        }
                }
        }
    setbumblebeestate(bumble);
    setxstate(xpid);
    if (getdiscretestate().indexOf(discrete) == -1 || getdiscretestate().isEmpty())
        {
            setdiscretestate(discrete);
            if (!executeUseLoopTimer.isActive() && !getdiscretestate().toLower().contains("off"))
                {
                    executeUseLoopTimer.setSingleShot(false);
                    executeUseLoopTimer.start(getinterval());
                }
        }

}


void MainWindow::on_bumblebeestateChanged(QString bumblebeestate)
{
    ui->bumblebeeState_lineEdit->setText(bumblebeestate);
}

void MainWindow::on_xstateChanged(QString xstate)
{
    ui->xserverState_lineEdit->setText(xstate);
}

void MainWindow::on_discretestateChanged(QString discretestate)
{
    ui->discreteCardState_lineEdit->setText(discretestate);
    bool ok;
    int first = discretestate.toInt(&ok);
    Q_UNUSED(first);
    if (ok)
        {
            QIcon icon = QIcon(":/images/nvidia_circle.png");
            trayIcon->setIcon(icon);
            setWindowIcon(icon);
        }
    else
        {
            QIcon icon = QIcon(":/images/intel_circle.png");
            trayIcon->setIcon(icon);
            setWindowIcon(icon);
        }

}


void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    //trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

void MainWindow::createActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
        {
        case QSystemTrayIcon::Trigger:
            if (this->isVisible())
                {this->hide();}
            else
                {this->showNormal();}
            break;
//    case QSystemTrayIcon::DoubleClick:
//        break;
//    case QSystemTrayIcon::MiddleClick:
//        break;
        default:
            ;
        }
}

void MainWindow::on_interval_spinbox_valueChanged(int value)
{
    setinterval(value * 1000);
    if (value > 1)
        {
            ui->interval_spinBox->setSuffix(" seconds");
        }
    else
        {
            ui->interval_spinBox->setSuffix(" second");
        }
    if (executeUseLoopTimer.isActive())
        {
            executeUseLoopTimer.stop();
            executeUseLoopTimer.setSingleShot(false);
            executeUseLoopTimer.start(getinterval());
        }
}

void MainWindow::on_bbswitchFile_Changed(const QString &content)
{
    QTextStream cout(stdout);
    cout << content;
    if (content.toLower().contains("off"))
        {
            setdiscretestate("off");
        }
    executeUseLoopTimer.stop();
    executeUseLoopTimer.setSingleShot(true);
    executeUseLoopTimer.start(0);
}



