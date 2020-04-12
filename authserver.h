#pragma once

#include <QtCore/QThread>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QNetworkSession>
#include <QtNetwork/QLocalServer>
#include <QtCore/QCryptographicHash>

class AuthServer : public QThread {
Q_OBJECT

private:
	QLocalServer *server = nullptr;

	bool checkSerialNumber(const QString &hash);

public slots:
	void init_slot(const int port);

	void listening_slot();

signals:
	void initServer_signal(const int port);

	void initSuccess_signal(bool isGood);

	void startListening_signal();

	void stopListening_signal();

	void newRequest_signal(const QJsonObject &request);



};

