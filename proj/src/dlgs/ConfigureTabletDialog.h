#ifndef CONFIGURETABLETDIALOG_H
#define CONFIGURETABLETDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigureTabletDialog;
}
class TreeItem;
class TreeModel;

class ConfigureTabletDialog : public QDialog
{
	Q_OBJECT
	Ui::ConfigureTabletDialog*	ui;
	TreeItem*					item;
	TreeModel*					model;
public:
	explicit ConfigureTabletDialog(TreeItem* item, TreeModel* model, QWidget* parent = 0);
	~ConfigureTabletDialog();
private slots:
	void accept();
};

#endif // CONFIGURETABLETDIALOG_H
