#ifndef CONFIGUREMACHINEDIALOG_H
#define CONFIGUREMACHINEDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigureMachineDialog;
}
class TreeItem;
class TreeModel;

class ConfigureMachineDialog : public QDialog
{
	Q_OBJECT
	Ui::ConfigureMachineDialog*		ui;
	TreeItem*						item;
	TreeModel*						model;
public:
	explicit ConfigureMachineDialog(TreeItem* item, TreeModel* model, QWidget* parent = 0);
	~ConfigureMachineDialog();
private:
	void checkExists();
private slots:
	void accept();
	void slotChooseQemu();
};

#endif // CONFIGUREMACHINEDIALOG_H
