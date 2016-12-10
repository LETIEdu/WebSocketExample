#include <QCoreApplication>
#include "echoserver.h"

int main(int argc, char *argv[])
{
    // Основной класс Qt для консольного приложения
    // создаёт event loop (цикл обработки событий),
    // реагирующий на события типа прихода новых
    // данных по сети и т.д.
    QCoreApplication a(argc, argv);

    // Создаём наш сервер
    EchoServer *server = new EchoServer(1234);
    // Соединяем сигнал закрытия сервера со
    // слотом закрытия всей программы
    QObject::connect(server, &EchoServer::closed, &a,
                     &QCoreApplication::quit);

    return a.exec();
}
