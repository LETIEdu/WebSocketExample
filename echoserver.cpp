#include "echoserver.h"
#include <QWebSocket>
#include <QWebSocketServer>
#include <QDebug>

// Констркутор класса
EchoServer::EchoServer(quint16 port, QObject *parent) :
    QObject(parent),
    // В списке инициализации создаём объект сервера и
    // пустой массив клиентов
    server(new QWebSocketServer(QStringLiteral("Echo Server"),
                                QWebSocketServer::NonSecureMode,
                                this)),
    clients()
{
    // Говорим серверу слушать выбранный порт
    if(server->listen(QHostAddress::Any, port))
    {
        // Если ошибки не произошло, то выводим в консоль
        // сообщение и связываем сигналы сервера о новом
        // подключении и закрытии с соответствующими
        // слотами (функциями)
        qDebug() << "Server listen to port " << port;
        connect(server, &QWebSocketServer::newConnection,
                this, &EchoServer::onNewConnection);
        connect(server, &QWebSocketServer::closed,
                this, &EchoServer::closed);
    }
}

EchoServer::~EchoServer()
{
    // В деструкторе закрываем сервер и удаляем
    // Все сокеты из массива
    server->close();
    qDeleteAll(clients.begin(), clients.end());
}

void EchoServer::onNewConnection()
{
    // При новом подключении получаем сокет подключения
    // от сервера
    QWebSocket *socket = server->nextPendingConnection();

    // Соединяем сигналы нового сокета со слотами
    // чтения сообщений и отсоединения
    connect(socket, &QWebSocket::textMessageReceived,
            this, &EchoServer::processTextMessage);
    connect(socket, &QWebSocket::binaryMessageReceived,
            this, &EchoServer::processBinaryMessage);
    connect(socket, &QWebSocket::disconnected,
            this, &EchoServer::socketDisconnected);

    // Добавляем сокет в массив
    clients << socket;
}

// Слот чтения текстовых сообщений
void EchoServer::processTextMessage(QString message)
{
    // Сначла определяем от какого сокета пришло и получаем
    // его адрес в указатель на сокет
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    // Выводим полученное сообщение в консоль
    qDebug() << "Message received: " << message;
    if(client)
    {
        // Если указатель существующий (проверка на NULL (nullptr)
        // на всякий случай), то посылаем ему обратно текст
        client->sendTextMessage(message);
    }
}

// То же самое с бинарным сообщением
void EchoServer::processBinaryMessage(QByteArray message)
{
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Message received: " << message;
    if(client)
    {
        client->sendBinaryMessage(message);
    }
}

// Сокет отключения
void EchoServer::socketDisconnected()
{
    // Определяем какой из сокетов отключился
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    // Выводим сообщение в консоль
    qDebug() << "Socket disconnected " << client;
    if(client)
    {
        // Удаляем копии (если их вдруг больше 1) из массива
        // клиентов
        clients.removeAll(client);
        // даём команду на удаление самого сокета
        client->deleteLater();
    }
}
