#ifndef VIAL_H
#define VIAL_H

#include <QGraphicsItem>
class QMutex;


class Vial: public QGraphicsItem
{

public:
	Vial(int x, int y, uint size, float lsize, QMutex *mutex);
	~Vial() {};

	QRectF boundingRect() const
	{
		qreal penWidth = 1;
		return QRectF(
			-10 - penWidth / 2, -10 - penWidth / 2,
			20 + penWidth / 2, 20 + penWidth / 2
		);
	}

	void paint(
		QPainter *painter,
		const QStyleOptionGraphicsItem *option,
		QWidget *widget
	);

	void setCurFill(uint value)
	{
		curFil = value;
	}

	void setNeedFill(uint value)
	{
		needFill = value;
	}

	void setSize(uint value)
	{
		SizeInLiters = value;
	}

	void setLiterSize(float value)
	{
		literSize = value;
	}

	void setYoffset(float value)
	{
		offsetY = value;
	}

private:
	float literSize;
	uint SizeInLiters;
	uint curFil;
	int needFill;
	int offsetX;
	int offsetY;
	QMutex *M;
};

#endif // VIAL_H
