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
	readSettings();
	connect(cliHandler, &CliHandler::generateSerial_signal, this, &Core::handleGenerating_slot);
	connect(cliHandler, &CliHandler::stopApp_signal, this, &Core::stopApp_slot);
	connect(cliHandler, &CliHandler::finished, this, &CliHandler::deleteLater);
	connect(this, &Core::finished, this, &Core::deleteLater);
	connect(this, &Core::generateSerial_signal, authServer, &AuthServer::generateSerialNumber_signal);
	connect(this, &Core::startServer_signal, authServer, &AuthServer::init_slot);
	connect(authServer, &AuthServer::finished, authServer, &AuthServer::deleteLater);

	return false;
}

void Core::stopApp_slot() {
	QCoreApplication::exit(0);
}
