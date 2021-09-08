#ifndef CONFIGUREMEMORYDIALOG_H
#define CONFIGUREMEMORYDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigureMemoryDialog;
}
class TreeItem;
class TreeModel;

class ConfigureMemoryDialog : public QDialog
{
	Q_OBJECT
	Ui::ConfigureMemoryDialog*	ui;
	TreeItem*					item;
	TreeModel*					model;
public:
	explicit ConfigureMemoryDialog(TreeItem* item, TreeModel* model, QWidget* parent = 0);
	~ConfigureMemoryDialog();
private slots:
	void accept();
};

#endif // CONFIGUREMEMORYDIALOG_H
