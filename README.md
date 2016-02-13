# GoGo40-Qt-REST-API-Client

This is a small api using Qt Network Manager to create HTTP requests. The library is implemented as a fluent api.

EXAMPLE
-------

```c++

#include <QCoreApplication>
#include <QDebug>
#include <thread>
#include <chrono>

#include "rest_api_client.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    gogo40_rest_api_client::rest_api_client client("http://localhost:8891");

    QJsonObject request;
    QJsonObject args;

    args.insert("a", 1);
    args.insert("b", 2);

    request.insert("cmd", "hd");
    request.insert("args", args);

    const std::vector<const char*> status = {"ERROR", "RUNNING", "FINISHED", "UNDEFINED", "TIMEOUT"};
    enum process_status { ERROR = 0, RUNNING = 1, FINISHED = 2, UNDEFINED = 3, TIMEOUT = 4 };

    client.on_result(
                [&client, &status](const QByteArray& response) {
                    QJsonObject r = std::move(QJsonDocument::fromJson(response).object());
                    QJsonObject res;
                    QJsonObject a;

                    static int id = -1;

                    if (r.contains("id")) {
                        id = r["id"].toInt();
                    }

                    a.insert("id", id);

                    if (r.contains("result")) {
                        qDebug() << "result:" << r["result"]
                                    .toObject()["a*b"].toDouble();
                        return;
                    } else if (r.contains("status")) {
                        int s = r["status"].toInt();
                        if (s == RUNNING) {
                            res.insert("status", a);
                        } else if (s == FINISHED) {
                            res.insert("result", a);
                        } else if (s == UNDEFINED) {
                            qDebug() << "error: bad query";
                            qDebug() << r;
                            return;
                        }
                        qDebug() << "status: " << status[s];
                    } else if (r.contains("id")) {
                        res.insert("status", a);
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

                    client.post(res);

                }).on_error(
                [](const QString& error) {
                    qDebug() << "ERROR: " << error;
                }).post(request);

    return a.exec();
}


```


TODO
----

- Add support to SSL
- Test GET and PUT
- Add support to routes
- Add a simple stub generator to create json rpc clients automagically (or other kind of rpc clients based on HTTP)

