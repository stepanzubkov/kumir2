#ifndef ROBOTCELL_H
#define ROBOTCELL_H

#include <QtGlobal>
class QGraphicsItem;


namespace Robot25D
{

class CellGraphicsItem;

struct RobotCell {
	RobotCell()
	{
		painted = pointed = flag = false;
		wallUp = wallDown = wallLeft = wallRight = false;
		cellItem = NULL;
		pointItem = NULL;
		wallUpItem = wallDownItem = wallLeftItem = wallRightItem = NULL;
		paintState = 0;
	}

	bool painted;
	bool wallUp;
	bool wallDown;
	bool wallLeft;
	bool wallRight;
	bool pointed;
	bool flag;

	quint8 paintState;
	qreal baseZOrder;

	CellGraphicsItem *cellItem;

	QGraphicsItem *pointItem;
	QGraphicsItem *wallUpItem;
	QGraphicsItem *wallDownItem;
	QGraphicsItem *wallLeftItem;
	QGraphicsItem *wallRightItem;
};

} // namespace Robot25D

#endif // ROBOTCELL_H
