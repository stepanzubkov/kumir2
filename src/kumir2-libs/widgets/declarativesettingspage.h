#ifndef DECLARATIVE_SETTINGS_PAGE
#define DECLARATIVE_SETTINGS_PAGE

#include <QtGlobal>
#include <QVariant>
#include <QWidget>

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

class DeclarativeSettingsPageImpl;

class WIDGETS_EXPORT DeclarativeSettingsPage : public QWidget
{
	friend DeclarativeSettingsPageImpl;
	Q_OBJECT

public:
	enum Type { Integer, Double, String, Char, Bool, Color, Font, Choice };

	struct Entry {
		Type type;
		QString title;
		QVariant defaultValue;
		QVariant minimumValue;
		QVariant maximumValue;
		QStringList items;
		qreal displayOrder;
		QString key;

		bool operator < (const struct Entry &other) const
		{
			return this->displayOrder < other.displayOrder;
		}
	};

	explicit DeclarativeSettingsPage(
		const QString &title,
		ExtensionSystem::SettingsPtr settings,
		const QMap<QString, Entry> entries
	);

	void setSettingsObject(ExtensionSystem::SettingsPtr settings);
	ExtensionSystem::SettingsPtr settingsObject() const;

public slots:
	void init();
	void accept();
	void resetToDefaults();

signals:
	void settingsChanged(const QStringList &keys);

private:
	DeclarativeSettingsPageImpl *pImpl_;
};

}

#endif
