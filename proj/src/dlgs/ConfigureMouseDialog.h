#ifndef CONFIGUREMOUSEDIALOG_H
#define CONFIGUREMOUSEDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigureMouseDialog;
}
class TreeItem;
class TreeModel;

class ConfigureMouseDialog : public QDialog
{
	Q_OBJECT
	Ui::ConfigureMouseDialog*	ui;
	TreeItem*					item;
	TreeModel*					model;
public:
	explicit ConfigureMouseDialog(TreeItem* item, TreeModel* model, QWidget* parent = 0);
	~ConfigureMouseDialog();
private slots:
	void accept();
};

#endif // CONFIGUREMOUSEDIALOG_H
