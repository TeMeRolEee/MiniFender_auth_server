#include <memory>

#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QDebug>

#include "core.h"

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);
	QCoreApplication::setApplicationName("MiniFender authentication server");
	QCoreApplication::setApplicationVersion("0.8");

	QCommandLineParser parser;
	parser.setApplicationDescription("Handling authentication of the MiniFender server");
	parser.addHelpOption();
	parser.addVersionOption();

	QString rootDir = QCoreApplication::applicationDirPath();

	auto core = std::make_unique<Core>(rootDir);

	core->start();

	auto isInited = new bool();
	*isInited = false;

	if (!core->init(rootDir, isInited)) {
		qCritical() << "[MAIN]\t" << "Unable to initialize. Shutting down.";
		core->quit();
		core->wait();
		QCoreApplication::exit(1);
	}

	for (int i = 0; i < 10; ++i) {
		if (*isInited) {
			break;
		} else {
			std::this_thread::sleep_for(std::chrono_literals::operator ""ms(250));
		}
	}

	if (!*isInited) {
		qCritical() << "[MAIN]\t" << "Unable to startup the server. Shutting down.";
		core->quit();
		core->wait();
		QCoreApplication::exit(1);
	}

	return QCoreApplication::exec();
}
