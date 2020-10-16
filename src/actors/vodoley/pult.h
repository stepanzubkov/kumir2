/****************************************************************************
**
** Copyright (C) 2004-2008 NIISI RAS. All rights reserved.
**
** This file is part of the KuMir.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef VPULT_H
#define VPULT_H

#include "ui_pult.h"
class Vodoley;
class pultLogger;
class linkLight;
class MainButton;

#include <QWidget>

#if 0
#define SCROLL_STEP 10
#define RESP_PANEL 40
#define LOGGER_BUTTONS 140

#undef TEXT
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define TEXT 5
#endif

#if 0
class OvenTimer : public QWidget
{
	Q_OBJECT

public:
	OvenTimer(QWidget *parent = 0);

	void setDuration(int secs);
	int duration() const;
	void draw(QPainter *painter);

signals:
	void angChange(int value);
public slots:
	void setValue(int value);
protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
private:
	//  QDateTime finishTime;
	//  QTimer *updateTimer;
	//  QTimer *finishTimer;
	bool mouseFlag;
	QPointF old_mouse_pos;
	int gradValue;
	int oldValue;
};
#endif

class VodoleyPult : public QWidget, public Ui::VodoleyPult
{
	Q_OBJECT
public:
	/**
	 * Конструктор
	 * @param parent ссыка на объект-владелец
	 * @param fl флаги окна
	 */
	VodoleyPult(QWidget *parent, Qt::WindowFlags fl, Vodoley *vodoley);

	/**
	 * Деструктор
	 */
	~VodoleyPult() {}

	QSize minimumSizeHint() const
	{
		return QSize(250, 450);
	}

	bool Link()
	{
		return link;
	}

	pultLogger *pltLogger()
	{
		return Logger;
	}

	void AutoClose()
	{
		autoClose = true;
	}

	public
slots:
	void noLink();
	void LinkOK();

	void newClient(QString);
	void clientDisconnect();

	void Up();

	void AOutS();
	void BOutS();
	void COutS();

	void TempS();

	void AtoBS();
	void AtoCS();
	void CtoBS();
	void CtoAS();

	void resetVodoley();
	void showMessage(QString message)
	{
		label->setText(message);
	}

	void logToKumir();

	void FillAs();
	void FillBs();
	void FillCs();
	void lockCButtons();

	void UnLockCButtons();
signals:
	void goUp();
	void goDown();
	void goLeft();
	void goRight();

	void hasLeftWall();
	void hasRightWall();

	void noUpWall();
	void noDownWall();
	void noLeftWall();
	void noRightWall();

	void Color();
	void Clean();

	void Rad();
	void Temp();
	void logToK();
	void PultCmd(QString text);
	void sendText(QString text);

protected:
	virtual void paintEvent(QPaintEvent *);

private:
	pultLogger *Logger;
	Vodoley *VodoleyObj;
	bool link;
	bool autoClose;
	linkLight *greenLight;
	MainButton *BtoC, *buttBack, *AtoC, *turnRight;
	MainButton *Coutb, *askFree, *AtoB, *buttTemp, *aFill, *bFill, *cFill, *CtoB, *CtoA;
};

#endif // VPULT_H
