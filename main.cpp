#include <memory>

#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QDebug>

#include "core.h"

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);
	QCoreApplication::setApplicationName("MiniFender authentication server");
	QCoreApplication::setApplicationVersion("0.1");

	QCommandLineParser parser;
	parser.setApplicationDescription("Handling authentication of the MiniFender server");
	parser.addHelpOption();
	parser.addVersionOption();

	QString rootDir = QCoreApplication::applicationDirPath();

	auto core = std::make_unique<Core>(rootDir);

	core->start();

	if (!core->init(rootDir)) {
		qDebug() << "SHIT HAPPENED";
		core->quit();
		core->wait();
		QCoreApplication::exit(1);
	}

	return QCoreApplication::exec();
}
