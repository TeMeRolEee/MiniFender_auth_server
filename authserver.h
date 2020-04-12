#pragma once

#include <QtCore/QThread>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QNetworkSession>
#include <QtNetwork/QLocalServer>
#include <QtCore/QCryptographicHash>

class AuthServer : public QThread {
Q_OBJECT

public:
	~AuthServer() override;

private:
	QLocalServer *server = nullptr;

	bool checkSerialNumber(const QString &hash);

public slots:
	void init_slot(const int port);

	void listening_slot();

	void stopListening_slot();

	void generateSerialNumber_slot(const QString &data);

signals:
	void generateSerialNumber_signal(const QString &data);

	void initServer_signal(const int port);

	void initSuccess_signal(bool isGood);

	void stopListening_signal();
};

