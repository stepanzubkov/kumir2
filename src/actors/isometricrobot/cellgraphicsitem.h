#ifndef CELLGRAPHICSITEM_H
#define CELLGRAPHICSITEM_H

#include <QGraphicsPolygonItem>

namespace Robot25D
{

class RobotView;

class CellGraphicsItem : public QGraphicsPolygonItem
{
public:
	CellGraphicsItem(
		const QPolygonF &poly,
		const QPolygonF &polySouth,
		const QPolygonF &polyEast,
		const QPolygonF &polyNorth,
		const QPolygonF &polyWest,
		QGraphicsItem *parent = 0
	);

protected:
	void paint(
		QPainter *painter,
		const QStyleOptionGraphicsItem *option,
		QWidget *widget
	);

private:
	QPolygonF p_south;
	QPolygonF p_east;
	QPolygonF p_north;
	QPolygonF p_west;
};

} // namespace Robot25D

#endif // CELLGRAPHICSITEM_H
