#pragma once

#include <QtCore/QThread>

class CliHandler : public QThread {
Q_OBJECT

protected:
	void run() override;

private:
	bool stopCli = false;

public slots:

	void stopCli_slot();

signals:

	void generateSerial_signal(const QString &serial);

	void stopCli_signal();

	void stopApp_signal();
};


