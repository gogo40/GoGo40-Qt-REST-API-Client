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

#include "qt_rest_api_client.h"

namespace gogo40_rest_api_client {

/// Rest_api implementation
rest_api_client_impl::rest_api_client_impl(const QString &url)
    : QObject(),
      manager_(this),
      params_()  {
    request_.setUrl(QUrl(url));

    QObject::connect(&manager_, SIGNAL(finished(QNetworkReply*)),
                     this, SLOT(reply_finished(QNetworkReply*)));
}

rest_api_client_impl::~rest_api_client_impl()
{

}

void rest_api_client_impl::set_url(const QUrl &url) {
    request_.setUrl(url);
}

void rest_api_client_impl::on_result(reply_callback callback) {
    reply_callback_ = callback;
}

void rest_api_client_impl::on_error(error_callback callback) {
    error_callback_ = callback;
}

void rest_api_client_impl::add_query_item(const QString &item, const QString &value) {
    params_.addQueryItem(item, value);
}

void rest_api_client_impl::add_content_type_header(const QString &header) {
    request_.setHeader(QNetworkRequest::ContentTypeHeader, header);
}

void rest_api_client_impl::add_content_length_header(int size) {
    request_.setHeader(QNetworkRequest::ContentLengthHeader, size);
}

void rest_api_client_impl::get() {
    update_params();
    QNetworkReply* r = manager_.get(request_);

    if (r->error()) {
        QString e = std::move(r->errorString());
        send_error(e);
    }

}

void rest_api_client_impl::post(const QByteArray &value) {
    update_params();
    QNetworkReply* r = manager_.post(request_, value);

    if (r->error()) {
        QString e = std::move(r->errorString());
        send_error(e);
    }
}

void rest_api_client_impl::put(const QByteArray &value) {
    update_params();
    QNetworkReply* r = manager_.put(request_, value);

    if (r->error()) {
        QString e = std::move(r->errorString());
        send_error(e);
    }
}

void rest_api_client_impl::clear() {
    params_.clear();
}

void rest_api_client_impl::send_error(const QString& e)
{
    if (error_callback_) {
        error_callback_(e);
    } else {
        qDebug() << "[rest_api_client][error] : " << e << "\n";
    }
}

void rest_api_client_impl::reply_finished(QNetworkReply *reply) {
    if (reply->error()) {
        QString e = std::move(reply->errorString());
        send_error(e);
    } else {
        QByteArray r = std::move(reply->readAll());
        if (reply_callback_) {
            reply_callback_(r);
        } else {
            qDebug() << "[rest_api_client][reply] : " << r << "\n";
        }
    }
}

void rest_api_client_impl::update_params()
{
    if (!params_.isEmpty()) {
        QUrl url = request_.url();
        url.setQuery(params_);
        request_.setUrl(url);
    }
}

} // namespace rest_api_client
