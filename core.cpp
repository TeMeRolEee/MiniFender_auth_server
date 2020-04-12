#include <QtCore/QFile>
#include <QtCore/QCoreApplication>
#include "core.h"


Core::Core(const QString &rootDir) {

}

Core::~Core() {
	delete settings;
	delete authServer;
	delete cliHandler;
}

void Core::run() {
	QThread::run();
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
	if (settingsFilePath.isEmpty()) {
		return false;
	}

	authServer = new AuthServer();
	cliHandler = new CliHandler();
	settings = new QSettings();

	rootDir = settingsFilePath;

	connect(cliHandler, &CliHandler::generateSerial_signal, this, &Core::generateSerial_signal, Qt::QueuedConnection);
	connect(cliHandler, &CliHandler::stopApp_signal, this, &Core::stopApp_slot, Qt::QueuedConnection);
	connect(cliHandler, &CliHandler::finished, this, &CliHandler::deleteLater, Qt::QueuedConnection);
	connect(this, &Core::finished, this, &Core::deleteLater);
	connect(this, &Core::generateSerial_signal, authServer, &AuthServer::generateSerialNumber_signal, Qt::QueuedConnection);
	connect(this, &Core::startServer_signal, authServer, &AuthServer::init_slot, Qt::QueuedConnection);
	connect(authServer, &AuthServer::finished, authServer, &AuthServer::deleteLater);

	if (!readSettings()) {
		return false;
	}

	authServer->start();
	cliHandler->start();

	return true;
}

void Core::stopApp_slot() {
	QCoreApplication::exit(0);
}
