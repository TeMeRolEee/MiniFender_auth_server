#include <QtCore/QDataStream>
#include "authserver.h"

void AuthServer::init_slot(const int port) {
	server = new QLocalServer(this);
	if (!server->listen("auth")) {
		qCritical() << "Unable to start server";
		emit initSuccess_signal(false);
		return;
	}

	QString ipAddress;
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
	for (const auto & address : ipAddressesList) {
		if (address != QHostAddress::LocalHost && address.toIPv4Address()) {
			ipAddress = address.toString();
			break;
		}
	}

	if (ipAddress.isEmpty()) {
		ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
	}

	connect(server, &QLocalServer::newConnection, this, &AuthServer::listening_slot);
}

void AuthServer::listening_slot() {
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_12);
	QLocalSocket *connection = server->nextPendingConnection();


}

bool AuthServer::checkSerialNumber(const QCryptographicHash &hash) {
	for (int i = 0; i < 102400; ++i) {
		QCryptographicHash tempHash(QCryptographicHash::RealSha3_512);
		QString serial = QString("acceptable_serial_") + i;
		tempHash.addData(serial.toUtf8());
		if (tempHash.result() == hash.result()) {
			return true;
		}
	}

	return false;
}
