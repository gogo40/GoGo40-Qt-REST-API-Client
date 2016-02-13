/*
Copyright 2016 Péricles Lopes Machado (eu [at] gogo40.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

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

