#ifndef CONFIGUREDISPLAYDIALOG_H
#define CONFIGUREDISPLAYDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigureDisplayDialog;
}
class TreeItem;
class TreeModel;

class ConfigureDisplayDialog : public QDialog
{
	Q_OBJECT
	Ui::ConfigureDisplayDialog*	ui;
	TreeItem*					item;
	TreeModel*					model;
public:
	explicit ConfigureDisplayDialog(TreeItem* item, TreeModel* model, QWidget* parent = 0);
	~ConfigureDisplayDialog();
private slots:
	void accept();
};

#endif // CONFIGUREDISPLAYDIALOG_H
