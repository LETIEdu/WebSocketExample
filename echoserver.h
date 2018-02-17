#ifndef ECHOSERVER_H
#define ECHOSERVER_H

#include <QObject>
#include <QList>
#include <QByteArray>

// Объявляем два класса, с сервером и сокетом
class QWebSocketServer;
class QWebSocket;

// Создаём наш класс сервера и наследуемся от QObject для
// реализации сигналов и слотов
class EchoServer : public QObject
{
    Q_OBJECT
public:
    // В конструктор получаем номер порта
    explicit EchoServer(quint16 port, QObject *parent = 0);
    ~EchoServer();

signals:
    void closed();

private slots:
    // Слоты на новое подключение,
    // чтение текстового и бинарного сообщений и
    // отключение клиента
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();

private:
    // Один объект сервера
    QWebSocketServer *server;
    // И список сокетов, один сокет на одного клиента
    QList<QWebSocket *> clients;
};

#endif // ECHOSERVER_H
