//****************************************************************************
//**
//** Copyright (C) 2004-2008 IMPB RAS. All rights reserved.
//**
//** This file is part of the KuMir.
//**
//** This file may be used under the terms of the GNU General Public
//** License version 2.0 as published by the Free Software Foundation
//** and appearing in the file LICENSE.GPL included in the packaging of
//** this file.
//**
//** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
//** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//**
//****************************************************************************/
#ifndef VODOLEY_H
#define VODOLEY_H

#include <QMainWindow>
#include <QMutex>

class QGraphicsSimpleTextItem;
class QGraphicsScene;
class QGraphicsView;
class QLabel;
class QFrame;
class QToolButton;
class QAction;

class pultLogger;
class VodoleyPult;
class Vial;

#if 0
class NewDialog: public QDialog
{
	Q_OBJECT

public:
	NewDialog();
	~NewDialog() {};
public slots:

private:
	QFrame *sizeFrame;
	QFrame *fillFrame;
	QFrame *valueFrame;
	QFrame *buttonFrame;
	QToolButton *btnOK;
	QToolButton *btnCancel;
};
#endif

class Vodoley: public QMainWindow
{
	Q_OBJECT

public:
	Vodoley();
	~Vodoley();

	bool isReady()
	{
		if ((CurA() == AfillR) || (CurB() == AfillR) || (CurC() == AfillR)) {
			return true;
		} else {
			return false;
		}
	}

	void showVodoley()
	{
		show();
		setVisible(true);
	}

	uint CurA() const
	{
		return Curfill[0];
	}

	uint CurB() const
	{
		return Curfill[1];
	}

	uint CurC() const
	{
		return Curfill[2];
	}

	void setBaseFill(int A, int B, int C)
	{
		Curfill[0] = A;
		Curfill[1] = B;
		Curfill[2] = C;
	}

	uint Asize() const
	{
		return Maxfill[0];
	}

	uint Bsize() const
	{
		return Maxfill[1];
	}

	uint Csize() const
	{
		return Maxfill[2];
	}

	uint maxSize()
	{
		uint max = 1;
		if (max < Asize()) {
			max = Asize();
		}
		if (max < Bsize()) {
			max = Bsize();
		}
		if (max < Csize()) {
			max = Csize();
		}
		return max;
	}

	bool loadIoDevice(QIODevice *source);
	bool loadFile(QString fileName);

	void AutoClose()
	{
		autoClose = true;
		setVisible(false);
	}

	void createActions(QList<QAction *> actions);
	bool ready()
	{
		return ((Curfill[0] == AfillR) || (Curfill[1] == AfillR) || (Curfill[2] == AfillR));
	}

public:
	QGraphicsScene *scene;
	QGraphicsView *view;

	VodoleyPult *pult;

	QAction *actNew;
	QAction *actLoad;
	QAction *actSave;

protected:
	void mousePressEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent *event);

public slots:
	void reset();

	void newZ();
	void loadZ();
	void saveZ();

	void FillA();
	void FillB();
	void FillC();

	void MoveFromTo(uint from, uint to);
	void redraw();

signals:
	void Otkaz(QString msg);
	void Ok();
	void CNull();
	void CNotNull();
	void FileLoaded(QString fname);

private:
	void updateMenzur();
	void updateNeedBirka();

	void CreateVodoley(void);
	void CreateDummyTask();

	QGraphicsSimpleTextItem *Atext;
	QGraphicsSimpleTextItem *Btext;
	QGraphicsSimpleTextItem *Ctext;

	QLabel *needLabel;
	QFrame *needFrame;

	//VODOLEY
	uint Afill, Bfill, Cfill; //Сколько  налито изначально
	uint Curfill[3]; //Сколько сейчас налито
	uint Maxfill[3]; //Размер емкости
	uint AfillR, BfillR, CfillR; //Сколько должно быть

	Vial *Amen, *Bmen, *Cmen;
	QMutex mutex;
	QString curDir;
	bool autoClose;
};

#endif // VODOLEY_H
