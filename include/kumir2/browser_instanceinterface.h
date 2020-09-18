#ifndef BROWSER_INSTANCEINTERFACE_H
#define BROWSER_INSTANCEINTERFACE_H

#include "startpage_widget_interface.h"

namespace Shared
{
namespace Browser
{

class InstanceInterface : public StartpageWidgetInterface
{
public:
	virtual QWidget *widget() = 0;

	virtual QWidget *startPageWidget()
	{
		return widget();
	}

	virtual void go(const QUrl &url) = 0;

	virtual void setContent(const QString &data) = 0;

	virtual QUrl currentLocation() const = 0;

	virtual QString title() const = 0;

	virtual QString startPageTitle() const
	{
		return title();
	}

	virtual void setTitleChangeHandler(
		const QObject *receiver,
		const char *method  /*(QString title, const InstanceInterface * sender)*/
	) = 0;
	virtual void setStartPageTitleChangeHandler(
		const QObject *receiver,
		const char *method
	) {
		setTitleChangeHandler(receiver, method);
	}

	virtual void evaluateCommand(
		const QString &method,
		const QVariantList &arguments
	) = 0;

	virtual QMap<QString, QObject *> &manageableObjects() = 0;

	QObject *&operator[](const QString &objName)
	{
		if (!manageableObjects().contains(objName)) {
			manageableObjects().insert(objName, nullptr);
		}
		return manageableObjects()[objName];
	}
};

} // namespace Browser
} // namespace Shared

Q_DECLARE_INTERFACE(Shared::Browser::InstanceInterface, "kumir2.browser.instance")

#endif // BROWSER_INSTANCEINTERFACE_H
