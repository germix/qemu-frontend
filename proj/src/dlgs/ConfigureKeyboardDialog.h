#ifndef CONFIGUREKEYBOARDDIALOG_H
#define CONFIGUREKEYBOARDDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigureKeyboardDialog;
}
class TreeItem;
class TreeModel;

class ConfigureKeyboardDialog : public QDialog
{
	Q_OBJECT
	Ui::ConfigureKeyboardDialog*	ui;
	TreeItem*						item;
	TreeModel*						model;
public:
	explicit ConfigureKeyboardDialog(TreeItem* item, TreeModel* model, QWidget* parent = 0);
	~ConfigureKeyboardDialog();
private slots:
	void accept();
};

#endif // CONFIGUREKEYBOARDDIALOG_H
