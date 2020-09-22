//
//  draw_turtle_field.h
//  Kumir2
//
//  Created by Denis Khachko on 14.12.15.
//


#ifndef Kumir2_draw_turtle_field_h
#define Kumir2_draw_turtle_field_h

#include <QGraphicsView>

class QLabel;
class QCheckBox;
class QDoubleSpinBox;
class QToolButton;

#ifdef WIDGETS_LIBRARY
#define WIDGETS_EXPORT Q_DECL_EXPORT
#else
#define WIDGETS_EXPORT Q_DECL_IMPORT
#endif

class WIDGETS_EXPORT DrawNavigator : public QGraphicsView
{
	Q_OBJECT
public:
	DrawNavigator(QWidget *parent = 0);

	void setZoom(double zoom)
	{
		Zoom = zoom;
	}

	void reDraw(const double zoom, const double netStepX, const double netStepY)
	{
		setZoom(zoom);
		updateSelf(netStepX, netStepY);
	}

	void updateSelf(double netStepX, double netStepY);

public:
	QDoubleSpinBox *netStepXS;
	QDoubleSpinBox *netStepYS;
	QToolButton *zoomUp, *zoomDown, *zoomNormal, *zoomFullDraw;
public slots:
	void XvalueChange(double value);
	void YvalueChange(double value);
	void autoNet(int state);
signals:
	void redrawNet();
	void autoNetChange(bool value);
	void netStepChange(double value);
private:
	double Zoom;
	QGraphicsScene *myScene;
	QGraphicsLineItem *mainLineX, *mainLineY;
	QGraphicsTextItem *zoomText, *zoomLab, *netLab;


	QCheckBox *isAutonet;
	QLabel *isAnLabel;
};

#endif
