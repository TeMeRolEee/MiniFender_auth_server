#pragma once

#include <QtCore/QThread>

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

	QString rootDir;

	DBManager *dbManager;

	CliHandler *cliHandler;

	AuthServer *authServer;

private slots:

signals:

};


