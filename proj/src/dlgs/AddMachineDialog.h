#ifndef ADDMACHINEDIALOG_H
#define ADDMACHINEDIALOG_H

#include <QDialog>

namespace Ui {
class AddMachineDialog;
}

class TreeModel;

class AddMachineDialog : public QDialog
{
	Q_OBJECT
	Ui::AddMachineDialog*	ui;
	TreeModel*				model;
public:
	explicit AddMachineDialog(TreeModel* mdl, QWidget* parent = 0);
	~AddMachineDialog();
private:
	void checkExists();
private slots:
	void accept();
	void slotChooseQemu();
};

#endif // ADDMACHINEDIALOG_H
