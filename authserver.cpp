#include <QtCore/QDataStream>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QNetworkProxy>

#include "authserver.h"

AuthServer::AuthServer() {
}

AuthServer::~AuthServer() {
	if (server->isListening()) {
		emit stopListening_signal();
	}
	delete server;
}

void AuthServer::init_slot(const int port) {
	server = new QTcpServer();
	server->setMaxPendingConnections(10);
	server->setProxy(QNetworkProxy::NoProxy);

	QHostAddress hostAddress;
	hostAddress.setAddress("0.0.0.0");
	hostAddress.toIPv4Address();
	server->listen(hostAddress, port);

	if (!server->isListening()) {
		qCritical() << "Unable to start server";
		qCritical() << server->errorString();
		emit initSuccess_signal(false);
		return;
	} else {
		qInfo() << "server is listening on port:" << server->serverPort();
	}

	QString ipAddress;
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
	for (const auto &address : ipAddressesList) {
		if (address != QHostAddress::LocalHost && address.toIPv4Address()) {
			ipAddress = address.toString();
			break;
		}
	}

	if (ipAddress.isEmpty()) {
		ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
	}

	connect(server, &QTcpServer::newConnection, this, &AuthServer::listening_slot, Qt::UniqueConnection);
	connect(this, &AuthServer::generateSerialNumber_signal, this, &AuthServer::generateSerialNumber_slot,Qt::UniqueConnection);
	connect(this, &AuthServer::initServer_signal, this, &AuthServer::init_slot, Qt::QueuedConnection);
	connect(this, &AuthServer::stopListening_signal, this, &AuthServer::stopListening_slot, Qt::QueuedConnection);
	emit initSuccess_signal(true);
}

void AuthServer::listening_slot() {
	if (server->hasPendingConnections()) {
		QByteArray block;
		QDataStream out(&block, QIODevice::ReadWrite);
		out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
		QTcpSocket *socket = server->nextPendingConnection();

		if (socket != nullptr) {
			socket->waitForReadyRead();
			out << socket->readAll();
			QByteArray response;
			QDataStream out2(&response, QIODevice::ReadWrite);

			if (checkSerialNumber(QString(QByteArray::fromBase64(block)))) {
				out2 << QString(QByteArray(QString("ACCEPTED").toUtf8()).toBase64());
			} else {
				out2 << QByteArray(QString("DECLINED").toUtf8()).toBase64();
			}

			socket->write(response);
			socket->waitForBytesWritten();
			socket->flush();
			socket->close();
		} else {
			qCritical() << "socket is bad";
		}
	}
}

bool AuthServer::checkSerialNumber(const QString &hash) {
	for (int i = 0; i <= 128000; ++i) {
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
	qInfo() << "Serial generated:" << endl << QString(tempHash.result().toHex()).toUtf8();
}

void AuthServer::stopListening_slot() {
	server->close();
}
