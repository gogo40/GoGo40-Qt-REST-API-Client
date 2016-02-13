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

#ifndef QT_REST_API_CLIENT
#define QT_REST_API_CLIENT

#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QUrlQuery>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include "rest_api_client.h"

namespace gogo40_rest_api_client {

class rest_api_client_impl : public QObject
{
    Q_OBJECT
public:
    explicit rest_api_client_impl(const QString& url = "");

    virtual ~rest_api_client_impl();

    void set_url(const QUrl& url);

    void set_reply_callback(reply_callback callback);

    void set_error_callback(error_callback callback);

    void add_query_item(const QString& item, const QString& value);

    void add_content_type_header(const QString& header);

    void add_content_length_header(int size);

    void get();

    void post(const QByteArray& value);

    void put(const QByteArray& value);

    void clear();

private slots:

    void reply_finished(QNetworkReply* reply);

private:
    void update_params();

    QNetworkAccessManager manager_;
    QNetworkRequest request_;
    QUrlQuery params_;

    reply_callback reply_callback_ = nullptr;
    error_callback error_callback_ = nullptr;
    void send_error(const QString& e);
};

} // namespace gogo40_rest_api_client

#endif // QT_REST_API_CLIENT

