#include "cellgraphicsitem.h"
#include "robotview.h"
#include "robotcell.h"
#include "robotitem.h"
#include "util.h"

namespace Robot25D
{

CellGraphicsItem::CellGraphicsItem(
	const QPolygonF &poly,
	const QPolygonF &polySouth,
	const QPolygonF &polyEast,
	const QPolygonF &polyNorth,
	const QPolygonF &polyWest,
	QGraphicsItem *parent
) :
	QGraphicsPolygonItem(poly, parent),
	p_south(polySouth),
	p_east(polyEast),
	p_north(polyNorth),
	p_west(polyWest)
{
}


void CellGraphicsItem::paint(
	QPainter *painter,
	const QStyleOptionGraphicsItem *option,
	QWidget *widget
) {
	QGraphicsPolygonItem::paint(painter, option, widget);
}


} // namespace Robot25D
