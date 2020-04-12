#include "core.h"


Core::Core(const QString &rootDir) {

}

Core::~Core() {
	delete settings;
}

void Core::run() {
	QThread::run();
}

bool Core::readSettings(const QString &filePath) {
	settings = new QSettings(filePath, QSettings::IniFormat);
	QStringList keys = settings->childGroups();

	if (!keys.contains("Port") && !keys.contains("Database")) {
		return false;
	}

	settings->beginGroup("Port");
	return (settings->childKeys().contains("port") && (settings->value("port").toInt() < 65535 && settings->value("port").toInt() > 0));
}
