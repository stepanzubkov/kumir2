//
// C++ Implementation: new task dialog
//
// Description:
//
//
// Author: Denis Khachko <mordol@lpm.org.ru>
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "dialog.h"
#include <QDebug>

Dialog::Dialog(QWidget *parent, Qt::WindowFlags fl): QDialog(parent, fl), Ui::Dialog()
{
	setupUi(this);
	connect(BaseA, SIGNAL(valueChanged(int)), this, SLOT(setMax()));
	connect(BaseB, SIGNAL(valueChanged(int)), this, SLOT(setMax()));
	connect(pushButton, SIGNAL(clicked()), this, SLOT(addFlag()));
	connect(pushButton_2, SIGNAL(clicked()), this, SLOT(removeFlag()));
	connect(listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(enableDelete()));
	translateButtons();
}

Dialog::~Dialog() {}

void Dialog::setMax()
{
	int max = BaseA->value();
	if (BaseB->value() > max) {
		max = BaseB->value();
	}
}

void Dialog::addFlag()
{
	QList<QListWidgetItem *> flags = listWidget->findItems(NeedA->cleanText(), Qt::MatchExactly);
	if (flags.count() > 0) {
		return;
	}
	listWidget->addItem(NeedA->cleanText());
}

void Dialog::enableDelete()
{
	pushButton_2->setEnabled(true);
}

void Dialog::removeFlag()
{
	QList<QListWidgetItem *>  selectedItems = listWidget->selectedItems();
	qDebug() << "Item Count" << selectedItems.count();
	for (int i = 0; i < selectedItems.count(); i++) {
		listWidget->takeItem(listWidget->row(selectedItems[i]));
	}
	if (listWidget->selectedItems().count() == 0) {
		pushButton_2->setEnabled(false);
	}
}


int Dialog::ASize() const
{
	return BaseA->value();
}

int Dialog::BSize() const
{
	return BaseB->value();
}

int Dialog::ANeed() const
{
	return NeedA->value();
}

void Dialog::setSizes(int A, int B)
{
	BaseA->setValue(A);
	BaseB->setValue(B);
}

int Dialog::getStart() const
{
	if (!taskNeeded->isChecked()) {
		return 0;
	}
	return StartPos->value();
}

QList<int> Dialog::getFlags()
{
	QList<int> toRet;
	if (!taskNeeded->isChecked()) {
		return toRet;
	}
	for (int i = 0; i < listWidget->count(); i++) {
		toRet.append(listWidget->item(i)->text().toInt());
	}
	return toRet;
}

int Dialog::getLeftBorder() const
{
	if (!taskNeeded->isChecked()) {
		return 0;
	}
	return spinBox->value();
}

int Dialog::getRightBorder() const
{
	if (!taskNeeded->isChecked()) {
		return 0;
	}
	return spinBox_2->value();
}

void Dialog::setLeftBorder(int pos)
{
	spinBox->setValue(pos);
}

void Dialog::setRightBorder(int pos)
{
	spinBox_2->setValue(pos);
}

void Dialog::setStart(int pos)
{
	StartPos->setValue(pos);
}

void Dialog::setFlags(QList<int> flags)
{
	listWidget->clear();
	for (int i = 0; i < flags.count(); i++) {
		listWidget->addItem(QString::number(flags[i]));
	}
}

bool Dialog::borders() const
{
	return Borders->isChecked();
}

void Dialog::setBordersChB(bool brd)
{
	Borders->setChecked(brd);
	if (!brd) {
		return;
	}
	spinBox->setEnabled(true);
	spinBox_2->setEnabled(true);
}

bool Dialog::isTaskNeeded() const
{
	return taskNeeded->isChecked();
}

void Dialog::setTaskNeeded(bool task)
{
	taskNeeded->setChecked(task);
	if (!task) {
		return;
	}
	frame_3->setEnabled(true);

	//qDebug() << "frame_3->ENABLED!";
}

void Dialog::translateButtons()
{
	QList<QAbstractButton *>btns = buttonBox->buttons();
	for (int i = 0; i < btns.count(); i++) {
		if (buttonBox->buttonRole(btns[i]) == QDialogButtonBox::RejectRole) {
			btns[i]->setText(trUtf8("Отмена"));
		}
	}
}

