#pragma once

#include <QtCore/QThread>
#include <QtCore/QSettings>

#include "dbmanager.h"
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

	QSettings *settings = nullptr;

	QString rootDir;

	DBManager *dbManager;

	CliHandler *cliHandler;

	AuthServer *authServer;

private slots:

signals:

};


