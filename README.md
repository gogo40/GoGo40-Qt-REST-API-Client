# GoGo40-Qt-REST-API-Client

This is a small api using Qt Network Manager to create HTTP requests. The library is implemented as a fluent api.

EXAMPLE
-------

```c++
    gogo40_rest_api_client::rest_api_client client("http://localhost:8891");

    QJsonObject request;
    QJsonObject args;

    args.insert("a", 1);
    args.insert("b", 2);

    request.insert("cmd", "my_function");
    request.insert("args", args);

    static const std::vector<QString> status = {"ERROR", "RUNNING", "FINISHED", "UNDEFINED", "TIMEOUT"};
    enum process_status { ERROR = 0, RUNNING = 1, FINISHED = 2, UNDEFINED = 3, TIMEOUT = 4 };

    client.set_reply_callback(
                [&client, &status](const QByteArray& response) {
                    qDebug() << "response:" << response << "\n";
                }).set_error_callback(
                [](const QString& error) {
                    qDebug() << "error: " << error;
                }).post(request);
```


TODO
----

- Add support to SSL
- Test GET and PUT
- Add support to routes
- Add a simple stub generator to create json rpc clients automagically (or other kind of rpc clients based on HTTP)

