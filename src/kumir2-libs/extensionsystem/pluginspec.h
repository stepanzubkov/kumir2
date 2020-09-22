#ifndef PLUGINSPEC_H
#define PLUGINSPEC_H

#include <QStringList>

namespace ExtensionSystem
{

struct PluginSpec {
	// Mandatory fields filled by plugin itself
	bool gui;
	bool main;

	QByteArray name;
	QList<QByteArray> dependencies;
	QList<QByteArray> provides;

	// Fields filled by launcher
	QByteArray libraryFileName;
	QString nonStandardPluginDir;
	QStringList arguments;

	explicit PluginSpec()
	{
		gui = false;
		main = false;
	}
};

} // namespace ExtensionSystem

#endif // PLUGINSPEC_H
