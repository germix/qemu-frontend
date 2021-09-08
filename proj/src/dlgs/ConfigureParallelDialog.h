#ifndef CONFIGUREPARALLELDIALOG_H
#define CONFIGUREPARALLELDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigureParallelDialog;
}
class TreeItem;
class TreeModel;

class ConfigureParallelDialog : public QDialog
{
	Q_OBJECT
	Ui::ConfigureParallelDialog*	ui;
	TreeItem*						item;
	TreeModel*						model;
public:
	explicit ConfigureParallelDialog(TreeItem* item, TreeModel* model, QWidget* parent = 0);
	~ConfigureParallelDialog();
private:
	void checkValid();
	void checkFileValid();
	void checkDirectoryExists();
	void checkEnabled(bool on);
private slots:
	void accept();
	void slotChooseDirectory();
	void slotFrame_toggled(bool on);
	void slotTextEdit_textChanged(const QString& text);
};

#endif // CONFIGUREPARALLELDIALOG_H
