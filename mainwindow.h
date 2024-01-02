#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ServerManager.h"
#include "ClientChatWidget.h"
#include <QMainWindow>
#include <QFileSystemModel>
#include <QDir>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void newClientConnected(QTcpSocket *client);
    void clientDisconnected(QTcpSocket *client);
    void setClientName(QString name);
    void setClientStatus(ChatProtocol::Status status);
    void on_listView_doubleClicked(const QModelIndex &index);

private: //fields
    Ui::MainWindow *ui;
    ServerManager *_server;
private: //methods
    void seupServer();
     QFileSystemModel *model;
};
#endif // MAINWINDOW_H
