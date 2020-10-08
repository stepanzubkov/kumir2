#ifndef MACRO_H
#define MACRO_H

#include "keycommand.h"

#include <QList>
#include <QSharedPointer>
#include <QDomElement>

class QAction;
class QDomDocument;


namespace Editor
{

struct Macro {
	QString title;
	QChar key;
	Qt::Key extKey;
	QList<KeyCommand> commands;
	QAction *action;
	bool showInLastBlock;
	explicit Macro()
	{
		action = nullptr;
		extKey = Qt::Key(0u);
		showInLastBlock = false;
	}
};

extern QDomElement dumpMacro(QSharedPointer<Macro> m, QDomDocument &document, QDomElement &root);
extern bool loadMacro(const QDomElement &v, QSharedPointer<Macro> m);
extern QList<QSharedPointer<Macro>> loadFromFile(const QString &fileName);
extern bool saveToFile(const QString &fileName, const QList<QSharedPointer<Macro>> &macros);

}

#endif
