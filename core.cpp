#include <QtCore/QFile>
#include <QtCore/QCoreApplication>
#include "core.h"


Core::Core(const QString &rootDir) {

}

Core::~Core() {
	cliHandler->quit();
	cliHandler->wait();

	authServer->quit();
	authServer->wait();

	delete settings;
	delete cliHandler;
	delete authServer;
}

bool Core::readSettings() {
	if (!QFile(rootDir + "/settings/settings.ini").exists()) {
		return false;
	}

	settings = new QSettings(rootDir + "/settings/settings.ini", QSettings::IniFormat);
	QStringList keys = settings->childGroups();

	if (!keys.contains("Port") && !keys.contains("Database")) {
		return false;
	}

	settings->beginGroup("Port");
	return (settings->childKeys().contains("port") && (settings->value("port").toInt() < 65535 && settings->value("port").toInt() > 0));
}

bool Core::init(const QString &settingsFilePath) {
	connect(this, &Core::finished, this, &Core::deleteLater);
	if (settingsFilePath.isEmpty()) {
		qDebug() << "SHIT";
		return false;
	}

	authServer = new AuthServer();
	cliHandler = new CliHandler();
	settings = new QSettings();

	rootDir = settingsFilePath;

	authServer->start();
	cliHandler->start();

	connect(cliHandler, &CliHandler::generateSerial_signal, authServer, &AuthServer::generateSerialNumber_signal, Qt::UniqueConnection);
	connect(cliHandler, &CliHandler::stopApp_signal, this, &Core::stopApp_slot, Qt::QueuedConnection);
	connect(cliHandler, &CliHandler::finished, this, &CliHandler::deleteLater, Qt::QueuedConnection);
	connect(this, &Core::startServer_signal, authServer, &AuthServer::init_slot, Qt::QueuedConnection);
	connect(authServer, &AuthServer::finished, authServer, &AuthServer::deleteLater);
	connect(this, &Core::startServer_signal, authServer, &AuthServer::init_slot);

	if (!readSettings()) {
		return false;
	}

	emit startServer_signal();

	return true;
}

void Core::stopApp_slot() {
	this->quit();
	this->wait();

	QCoreApplication::exit(0);
}
