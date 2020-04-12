#include "clihandler.h"

#include <iostream>


void CliHandler::run() {
	std::string input;
	while (!stopCli) {
		//qDebug() << "Waiting for input...";
		std::getline(std::cin, input);
		if (input == "generate") {
			emit generateSerial_signal(QString::fromStdString(input));
		}

	}
	QThread::run();
}

void CliHandler::stopCli_slot() {
	stopCli = true;
}
