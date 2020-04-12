#include <QtCore/QDataStream>
#include <QtNetwork/QLocalSocket>

#include "authserver.h"

AuthServer::AuthServer() {
	server = new QLocalServer();
	server->listen("auth");
}

AuthServer::~AuthServer() {
	qDebug() << (server == nullptr);
	if (server->isListening()) {
		emit stopListening_signal();
	}
	delete server;
}

void AuthServer:: init_slot(const int port) {
	if (!server->isListening() && !server->listen("auth")) {
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

	connect(server, &QLocalServer::newConnection, this, &AuthServer::listening_slot, Qt::QueuedConnection);
	connect(this, &AuthServer::generateSerialNumber_signal, this, &AuthServer::generateSerialNumber_slot, Qt::UniqueConnection);
	connect(this, &AuthServer::initServer_signal, this, &AuthServer::init_slot, Qt::QueuedConnection);
	connect(this, &AuthServer::stopListening_signal, this, &AuthServer::stopListening_slot, Qt::QueuedConnection);
}

void AuthServer::listening_slot() {
	QByteArray block;
	QDataStream out(&block, QIODevice::ReadWrite);
	out.setVersion(QDataStream::Qt_5_12);
	QLocalSocket *connection = nullptr;
	if (server->waitForNewConnection()) {
		if (server->hasPendingConnections()) {
			connection = server->nextPendingConnection();
			connect(connection, &QLocalSocket::disconnected, connection, &QLocalSocket::deleteLater);
		}
	}

	qDebug() << "Connection status:" << connection->isOpen();
	connection->waitForBytesWritten();

	if (checkSerialNumber(QString(connection->readAll()))) {
		out << "OK";
	} else {
		out << "NOT OK";
	}

	connection->write(block);
	connection->flush();
	connection->waitForBytesWritten(3000);
	connection->close();
}

bool AuthServer::checkSerialNumber(const QString &hash) {
	qDebug() << "HASH? ->" << hash;
	for (int i = 0; i < 102400; ++i) {
		QCryptographicHash tempHash(QCryptographicHash::RealSha3_512);
		QString serial = QString("acceptable_serial_") + QString(i);
		tempHash.addData(serial.toUtf8());
		if (tempHash.result().toHex() == hash) {
			return true;
		}
	}

	return false;
}

void AuthServer::generateSerialNumber_slot(const QString &data) {
	QCryptographicHash tempHash(QCryptographicHash::RealSha3_512);
	QString serial = QString("acceptable_serial_") + data;
	tempHash.addData(serial.toUtf8());
	qDebug() << "Serial generated:" << endl << QString(tempHash.result().toHex()).toUtf8();
}

void AuthServer::stopListening_slot() {
	server->close();
}
