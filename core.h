#pragma once

#include <QtCore/QThread>
#include <QtCore/QSettings>
#include <QtCore/QCryptographicHash>

#include "clihandler.h"
#include "authserver.h"

class Core : public QThread {
Q_OBJECT

public:
	Core(const QString &rootDir);
	~Core() override;

protected:
	void run() override;

private:
	bool readSettings(const QString &filePath);

	bool checkSerialNumber(const QCryptographicHash &hash);

	QSettings *settings = nullptr;

	QString rootDir;

	CliHandler *cliHandler;

	AuthServer *authServer;

private slots:

signals:

};


