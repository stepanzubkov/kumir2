#ifndef CONTENTRENDERER_H
#define CONTENTRENDERER_H

#include <QSharedPointer>

namespace DocBookViewer
{

class DocBookModel;
typedef QSharedPointer<DocBookModel> ModelPtr;

class ContentRenderer
{
public:
	virtual bool isEmpty() const = 0;
	virtual void reset() = 0;
	virtual void renderData(ModelPtr data) = 0;
};

}

#endif
