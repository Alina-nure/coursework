#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    seupServer();
    model = new QFileSystemModel (this) ;
    model->setFilter (QDir::QDir::AllEntries) ;
    model->setRootPath ( " " ) ;
    ui->listView->setModel(model) ;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newClientConnected(QTcpSocket *client)
{
    auto id = client->property("id").toInt();
    ui->lstClients->addItem(QString("New Client added: %1").arg(id));
    auto chatWidget= new ClientChatWidget(client, ui->tbClientsChat);
    ui->tbClientsChat->addTab(chatWidget, QString("Client (%1)").arg(id));

    connect(chatWidget, &ClientChatWidget::clientNameChanged, this, &MainWindow::setClientName);
    connect(chatWidget, &ClientChatWidget::statusChanged, this, &MainWindow::setClientStatus);
    connect(chatWidget, &ClientChatWidget::isTyping, [this](QString name){
        this->statusBar()->showMessage(name, 750);
    });

}

void MainWindow::clientDisconnected(QTcpSocket *client)
{
    auto id = client->property("id").toInt();
    ui->lstClients->addItem(QString("Client disconnected: %1").arg(id));
}

void MainWindow::setClientName(QString name)
{
    auto widget = qobject_cast<QWidget *>(sender());
    auto index = ui->tbClientsChat->indexOf(widget);
    ui->tbClientsChat->setTabText(index, name);
}

void MainWindow::setClientStatus(ChatProtocol::Status status)
{
    auto widget = qobject_cast<QWidget *>(sender());
    auto index = ui->tbClientsChat->indexOf(widget);
    QString iconName = ":/icons/";
    switch (status) {
    case ChatProtocol::Available:
        iconName.append("available.png");
        break;
    case ChatProtocol::Away:
        iconName.append("away.png");
        break;
    case ChatProtocol::Busy:
        iconName.append("busy.png");
        break;
    default:
        iconName = "";
        break;

    }

    auto icon = QIcon(iconName);
    ui->tbClientsChat->setTabIcon(index, icon);
}

void MainWindow::seupServer()
{
    _server = new ServerManager();
    connect(_server, &ServerManager::newClientConnected, this, &MainWindow::newClientConnected);
    connect(_server, &ServerManager::clientDisconnected, this, &MainWindow::clientDisconnected);

}


void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    QListView* listV = (QListView*)sender();
    QFileInfo fileInfo = model->fileInfo(index);

    if (fileInfo.fileName() =="..")
    {
        QDir dir=fileInfo.dir();
        dir.cdUp() ;
        listV->setRootIndex(model->index(dir.absolutePath()));
    }
    else if (fileInfo.fileName() ==".")
    {
        QDir dir = fileInfo.dir() ;
        listV->setRootIndex(model->index(""));
    }

    else if (fileInfo.isDir())
    {
        QDir dir = fileInfo.dir() ;
        listV->setRootIndex(index);
    }
}

