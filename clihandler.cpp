#include "clihandler.h"

#include <iostream>


void CliHandler::run() {
	std::string input;
	while (!stopCli) {
		//qDebug() << "Waiting for input...";
		std::getline(std::cin, input);
		if (input == "generate") {
			QString temp = QString::fromStdString(input);
			temp.remove(0, 9);
			emit generateSerial_signal(QString::fromStdString(input));
		}

		if (input == "exit") {
			emit stopApp_signal();
		}

	}
	QThread::run();
}

void CliHandler::stopCli_slot() {
	stopCli = true;
}
