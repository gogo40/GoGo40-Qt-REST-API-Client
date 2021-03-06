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
#include <ctime>
#include <cstdlib>
#include <memory>

#include "rest_api_client.h"


#define DEFAULT_HOST "http://143.54.155.233:9891"

const std::vector<const char*> status = {"ERROR", "RUNNING", "FINISHED", "UNDEFINED", "TIMEOUT"};
enum process_status { ERROR = -1, RUNNING = 1, FINISHED = 2, UNDEFINED = 3, TIMEOUT = 4 };


class remote_call {
public:
    remote_call(const char* host, size_t test_size,
                const std::chrono::time_point<std::chrono::system_clock>& start)
        : client_(host), s_(""), start_(start) {
        for (int i = 0; i < test_size; ++i) {
            s_ += 'a' + std::rand() % ('z' - 'a');
        }
    }

    virtual ~remote_call() {}

    void call() {
        QJsonObject args;
        QJsonObject request;

        args.insert("str", s_);

        request.insert("cmd", "echo");
        request.insert("args", args);

        client_.on_result(
                    [=](const QByteArray& response) {
            QJsonObject r = std::move(QJsonDocument::fromJson(response).object());
            QJsonObject res;
            QJsonObject a;

            if (r.contains("id")) {
                id_ = r["id"].toInt();
            }

            a.insert("id", id_);

            if (r.contains("result")) {
                QString result = std::move(r["result"]
                        .toObject()["str"]
                        .toString());
                bool ok = (result == s_);
                qDebug() << "result:" << ((ok)?"ok":"n_ok") << " id:" << id_;
                using namespace std::chrono;
                auto end = std::chrono::system_clock::now();
                auto diff = duration_cast<milliseconds>(end - start_);
                qDebug() << "dt: " << diff.count() << " ms\n";
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
            } else {
                qDebug() << "error during query execution";
                qDebug() << r;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            client_.post(res);

        }).on_error(
                    [](const QString& error) {
            qDebug() << "ERROR: " << error;
        }).post(request);
    }

private:
    int id_ = -1;
    gogo40_rest_api_client::rest_api_client client_;
    QString s_;
    const std::chrono::time_point<std::chrono::system_clock>& start_;
};


int main(int argc, char *argv[])
{
    std::srand(std::time(NULL));

    QCoreApplication a(argc, argv);

    size_t t = 40000000; // 10 MB
    size_t n_run = 5; //

    auto start = std::chrono::system_clock::now();

    std::vector<std::shared_ptr<remote_call>> calls;

    for (int i = 0; i < n_run; ++i) {
        qDebug() << "run " << i << "\n";
        calls.push_back(std::shared_ptr<remote_call>(new remote_call(DEFAULT_HOST, t, start)));
        calls.at(i)->call();
    }

    return a.exec();
}

