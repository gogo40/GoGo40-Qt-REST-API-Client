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

#include <QDebug>

#include "rest_api_client.h"

#include "qt_rest_api_client.h"

namespace gogo40_rest_api_client {


rest_api_client::rest_api_client(const QString& url)
{
    impl_ = new rest_api_client_impl(url);
}

rest_api_client::~rest_api_client()
{
    delete impl_;
}

rest_api_client &rest_api_client::set_url(const QString &url)
{
    QUrl _url(url);
    impl_->set_url(_url);
    return *this;
}

rest_api_client& rest_api_client::add_content_type_header(const QString& header)
{
    impl_->add_content_type_header(header);
    return *this;
}

rest_api_client& rest_api_client::add_query_item(const QString& name, const QString& value)
{
    impl_->add_query_item(name, value);
    return *this;
}

rest_api_client &rest_api_client::get()
{
    impl_->get();
    return *this;
}

rest_api_client& rest_api_client::post(const QString& value)
{
    impl_->post(value.toLocal8Bit());
    return *this;
}

rest_api_client &rest_api_client::post(const QJsonObject &obj)
{
    add_content_type_header("application/json");

    QJsonDocument doc(obj);
    QByteArray value = std::move(doc.toJson());
    impl_->post(value);
    return *this;
}

rest_api_client &rest_api_client::put(const QString &value)
{
    impl_->put(value.toLocal8Bit());
    return *this;
}

rest_api_client &rest_api_client::put(const QJsonObject &obj)
{
    add_content_type_header("application/json");

    QJsonDocument doc(obj);
    QByteArray value = std::move(doc.toJson());
    impl_->put(value);
    return *this;
}

void rest_api_client::clear()
{
    impl_->clear();
}

rest_api_client &rest_api_client::on_result(reply_callback callback)
{
    impl_->on_result(callback);
    return *this;
}

rest_api_client &rest_api_client::on_error(error_callback callback)
{
    impl_->on_error(callback);
    return *this;
}


} // namespace gogo40_rest_api_client
