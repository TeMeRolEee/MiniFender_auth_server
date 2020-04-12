#pragma once

#include <QtCore/QThread>
#include <QtCore/QSettings>
#include <QtCore/QCryptographicHash>

#include "clihandler.h"
#include "authserver.h"

class Core : public QThread {
Q_OBJECT

public:
	explicit Core(const QString &rootDir);
	~Core() override;

	bool init(const QString &settingsFilePath);

private:
	bool readSettings();

	QSettings *settings = nullptr;

	QString rootDir;

	CliHandler *cliHandler;

	AuthServer *authServer;

private slots:
	void stopApp_slot();


signals:

	void startServer_signal(int port);

	void generateSerial_signal(const QString &data);
};
