#ifndef WIDGETS_SECONDARYWINDOW_H
#define WIDGETS_SECONDARYWINDOW_H

#include <QObject>
#include <QSharedPointer>

class QIcon;

#ifdef WIDGETS_LIBRARY
#define WIDGETS_EXPORT Q_DECL_EXPORT
#else
#define WIDGETS_EXPORT Q_DECL_IMPORT
#endif

namespace ExtensionSystem
{
	class Settings;
	typedef QSharedPointer<Settings> SettingsPtr;
}

namespace Widgets
{
class SecondaryWindowImplementationInterface;
class DockWindowPlace;

class WIDGETS_EXPORT SecondaryWindow : public QObject
{
	Q_OBJECT
public:
	SecondaryWindowImplementationInterface *dockContainer() const;
	SecondaryWindowImplementationInterface *windowContainer() const;

	const QString &settingsKey() const;

	static SecondaryWindow *createSecondaryWindow(
		QWidget *centralWidget,
		const QString &title,
		const QIcon &icon,
		QWidget *topLevelParent,
		DockWindowPlace *dockPlace,
		const QString &settingsKey,
		bool resizable
	);

	void changeDockPlace(DockWindowPlace *dockPlace);
	bool isSeparateWindow() const;

signals:

public slots:
	void activate();
	void updateSettings(
		ExtensionSystem::SettingsPtr settings,
		const QStringList &keys
	);
	void saveState();
	void restoreState();

private /*methods*/:
	bool event(QEvent *evt);

	explicit SecondaryWindow(
		QWidget *topLevelParent,
		SecondaryWindowImplementationInterface *windowContainer,
		SecondaryWindowImplementationInterface *dockContainer,
		const QString &settingsKey,
		QWidget *centralWidget
	);

	static SecondaryWindowImplementationInterface *createWindowContainer(
		const QString &title,
		const QIcon &icon,
		QWidget *topLevelParent,
		bool resizable
	);

	static SecondaryWindowImplementationInterface *createDockContainer(
		const QString &title,
		DockWindowPlace *dockPlace
	);

	SecondaryWindowImplementationInterface *currentContainer() const;

private /*fields*/:
	SecondaryWindowImplementationInterface *dockContainer_;
	SecondaryWindowImplementationInterface *windowContainer_;
	QString settingsKey_;
	ExtensionSystem::SettingsPtr settings_;
	QWidget *topLevelParent_;
	QWidget *centralWidget_;

};

} // namespace Widgets

#endif // WIDGETS_SECONDARYWINDOW_H
