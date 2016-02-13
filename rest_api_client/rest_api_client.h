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

#ifndef REST_API_CLIENT_H
#define REST_API_CLIENT_H

#include <QString>
#include <functional>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

namespace gogo40_rest_api_client {

using error_callback = std::function<void(const QString&)>;
using reply_callback = std::function<void(const QByteArray&)>;

class rest_api_client_impl;
class rest_api_client
{
public:
    //TODO: Add support to SSL and other http elements
    rest_api_client(const rest_api_client&) = delete;

    rest_api_client(const QString& url = "");
    virtual ~rest_api_client();

    rest_api_client& set_url(const QString& url);
    rest_api_client& add_content_type_header(const QString& header);
    rest_api_client& add_query_item(const QString& name, const QString& value);

    rest_api_client& get();
    rest_api_client& post(const QString& value);
    rest_api_client& post(const QJsonObject& obj);
    rest_api_client& put(const QString& value);
    rest_api_client& put(const QJsonObject& obj);

    void clear();

    rest_api_client& set_reply_callback(reply_callback callback);
    rest_api_client& set_error_callback(error_callback callback);

private:
    rest_api_client_impl* impl_ = nullptr;
};


} // namespace gogo40_rest_api_client

#endif // REST_API_CLIENT_H
