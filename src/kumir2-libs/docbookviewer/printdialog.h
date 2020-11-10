#ifndef DOCBOOKVIEWER_PRINTDIALOG_H
#define DOCBOOKVIEWER_PRINTDIALOG_H

#include <QDialog>
#include <QMap>
class QTreeWidgetItem;

namespace DocBookViewer
{
class Document;
class DocBookModel;
typedef QSharedPointer<DocBookModel> ModelPtr;

namespace Ui
{
class PrintDialog;
}

class PrintDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PrintDialog(QWidget *parent = 0);
	void addDocument(const Document &document);
	~PrintDialog();
	QList<ModelPtr> selectedModels() const;

private:
	QList<ModelPtr> selectedModels(QTreeWidgetItem *root) const;
	QMap<QTreeWidgetItem *, ModelPtr> modelsOfItems_;
	void createNavigationItems(QTreeWidgetItem *item, ModelPtr model);
	Ui::PrintDialog *ui;
};


} // namespace DocBookViewer
#endif // DOCBOOKVIEWER_PRINTDIALOG_H
