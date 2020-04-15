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

	bool init(const QString &settingsFilePath, bool *isInited);

private:
	bool readSettings();

	bool *isInited;

	QSettings *settings = nullptr;

	QString rootDir;

	CliHandler *cliHandler;

	AuthServer *authServer;

private slots:
	void stopApp_slot();

	void isInited_slot(bool isGood);

signals:

	void startServer_signal(int port = 50137);
};
