/*
This file is generated, but you can safely change it
until you run "gen_actor_source.py" with "--project" flag.

Generated file is just a skeleton for module contents.
You should change it corresponding to functionality.
*/

#ifndef VODOLEYMODULE_H
#define VODOLEYMODULE_H

#include "vodoleymodulebase.h"
class QTimer;


namespace ExtensionSystem {
class KPlugin;
}

class Vodoley;

namespace ActorVodoley
{


class VodoleyModule : public VodoleyModuleBase
{
	Q_OBJECT

public /* methods */:
	VodoleyModule(ExtensionSystem::KPlugin *parent);

	static QList<ExtensionSystem::CommandLineParameter> acceptableCommandLineParameters();
	QWidget *mainWidget() const;
	QWidget *pultWidget() const;

	QString initialize(
		const QStringList &configurationParameters,
		const ExtensionSystem::CommandLine &runtimeParameters
	);

public slots:
	void changeGlobalState(ExtensionSystem::GlobalState old, ExtensionSystem::GlobalState current);
	void loadActorData(QIODevice *source);
	void reloadSettings(ExtensionSystem::SettingsPtr settings, const QStringList &keys);
	void reset();
	void terminateEvaluation() {}
	void setAnimationEnabled(bool enabled);
	void runFillA();
	void runFillB();
	void runFillC();
	void runEmptyA();
	void runEmptyB();
	void runEmptyC();
	void runFromAToB();
	void runFromAToC();
	void runFromBToA();
	void runFromBToC();
	void runFromCToB();
	void runFromCToA();
	int runSizeA();
	int runSizeB();
	int runSizeC();
	int runInA();
	int runInB();
	int runInC();
	bool runTaskComplited();

	void openRecent();
	void updateLastFiles(const QString newFile);

public:
	void tryToSleep();
	static QDir getResourcesDir()
	{
		return self->myResourcesDir();
	}

private:
	void createGui();
	void createRecentMenu();

	static VodoleyModule *self;
	Vodoley *MainWindow;
	QMutex mutex;
	ExtensionSystem::SettingsPtr my_settings;
	QMenu *recentMenu;
	QTimer *redrawTimer;
};

} // namespace ActorVodoley

#endif // VODOLEYMODULE_H
