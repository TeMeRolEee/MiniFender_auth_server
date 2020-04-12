#include "clihandler.h"

#include <iostream>
#include <QDebug>

void CliHandler::run() {
	std::string input;
	qDebug() << "Waiting for input...";
	while (!stopCli) {
		std::getline(std::cin, input);
		QString temp = QString::fromStdString(input);
		if (temp.contains("generate")) {
			temp.remove(0, 9);
			qInfo() << "Generating serial... from" << temp;
			qInfo() << __FUNCTION__ << __LINE__ << __FILE__ << temp.toInt();
			emit generateSerial_signal(QString(temp.toInt()));
		}

		if (temp.contains("exit")) {
			stopCli = true;
			emit stopApp_signal();
		}

	}
	QThread::run();
}

void CliHandler::stopCli_slot() {
	stopCli = true;
}
