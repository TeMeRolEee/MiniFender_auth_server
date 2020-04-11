#include <memory>

#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>

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

	return 0;
}
