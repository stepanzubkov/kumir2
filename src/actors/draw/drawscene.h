#ifndef DRAWSCENE_H
#define DRAWSCENE_H

#include <QGraphicsScene>
class QGraphicsItem;
class QMutex;

#define KUM_MULTI 50

namespace ActorDraw {

class DrawModule;

class DrawScene : public QGraphicsScene
{
	Q_OBJECT
public:
	DrawScene(DrawModule *draw, QMutex *_mutex, QObject *parent = 0);
	~DrawScene();

	void drawNet(
		double startx, double endx,
		double starty, double endy,
		QColor color,
		double stepX, double stepY,
		bool net,
		qreal nw, qreal aw
	);

	void addDrawLine(QLineF lineF, QColor color, qreal width);
	qreal drawText(const QString &Text, qreal widthChar, QPointF from, QColor color); //Returns offset of pen.

	void reset();

	void upd()
	{
		QGraphicsScene::update();
	}

	bool isLineAt(const QPointF &pos, qreal radius);

	QRectF getRect() const;

	int saveToFile(const QString &p_FileName) const;
	int loadFromFile(const QString &p_FileName);

	void fromBufferToScene();

	int buffSize() const
	{
		return itemsBuffer.count();
	}

private:
	void drawOnlyAxis(double startx, double endx, double starty, double endy, qreal aw);
	void DestroyNet();
	bool isUserLine(QGraphicsItem *) const; //Return true if item is user item;

	DrawModule *DRAW;
	QMutex *mutex;

	QList<QGraphicsLineItem *> lines;
	QList<QGraphicsLineItem *> Netlines;
	QList<QGraphicsSimpleTextItem *> texts;
	QList<QGraphicsItem *> itemsBuffer;
};

} // namespace ActorDraw
#endif // DRAWSCENE_H
