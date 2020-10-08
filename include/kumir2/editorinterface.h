#ifndef EDITOR_INTERFACE
#define EDITOR_INTERFACE

#include <QtPlugin>
class QUrl;

namespace Shared
{

namespace Editor {
	class InstanceInterface;
}

namespace Analizer {
	struct SFData;
}

class EditorInterface
{
public:
	virtual QFont defaultEditorFont() const = 0;

	virtual Editor::InstanceInterface *loadDocument(
		QIODevice *device,
		const QString &fileNameSuffix,
		const QString &sourceEncoding,
		const QUrl &sourceUrl, QString *error
	) = 0;

	virtual Editor::InstanceInterface *loadDocument(
		const QString &fileName, QString *error
	) = 0;

	virtual Editor::InstanceInterface *loadDocument(
		const Analizer::SFData &data,
		QString *error
	) = 0;

	virtual Editor::InstanceInterface *newDocument(
		const QString &canonicalLanguageName = "",
		const QString &documentDir = ""
	) = 0;
};

}

Q_DECLARE_INTERFACE(Shared::EditorInterface, "kumir2.editor")

#endif
