#ifndef DRAWVIEW_H
#define DRAWVIEW_H

#include <QGraphicsView>
class QLabel;
class QMutex;

namespace ActorDraw
{

class DrawModule;

class DrawView : public QGraphicsView
{
	Q_OBJECT
public:
	DrawView(DrawModule *draw, QMutex *_mutex, QWidget *parent = 0);
	~DrawView();

	double zoom() const
	{
		return c_scale;
	}

	void setZoom(double zoom);

	void setNet();//RESIZE NET

	bool isNet() const
	{
		return net;
	}

	void forceRedraw();

protected:
	void resizeEvent(QResizeEvent *event);
	void wheelEvent(QWheelEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	DrawModule *DRAW;
	QMutex *dr_mutex;
	double c_scale;
	bool pressed;
	bool net;
	QPoint press_pos;
	bool firstResize;
	QLabel *smallNetLabel;
};

} // namespace ActorDraw
#endif // DRAWVIEW_H
