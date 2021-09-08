#include "AddMachineDialog.h"
#include "ui_AddMachineDialog.h"

#include "../TreeModel.h"
#include <QFileInfo>
#include <QFileDialog>

AddMachineDialog::AddMachineDialog(TreeModel* mdl, QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::AddMachineDialog)
	, model(mdl)
{
	ui->setupUi(this);
	setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

	checkExists();
}

AddMachineDialog::~AddMachineDialog()
{
	delete ui;
}
void AddMachineDialog::checkExists()
{
	QFileInfo fi(ui->txtQemu->text());
	QPalette palette = ui->txtQemu->palette();
	if(!fi.exists())
		palette.setColor(QPalette::Base, QColor(255, 205, 200));
	else
		palette.setColor(QPalette::Base, QColor(150, 255, 150));
	ui->txtQemu->setPalette(palette);
}
void AddMachineDialog::accept()
{
	model->addMachine(ui->txtName->text(), ui->txtQemu->text());
	QDialog::accept();
}
void AddMachineDialog::slotChooseQemu()
{
	QString fileName = QFileDialog::getOpenFileName(
								0,
								tr("Find qemu"),
								QFileInfo(ui->txtQemu->text()).absolutePath(),
								tr("Executable files (%1)").arg("*.exe"));
	if(!fileName.isEmpty())
	{
		ui->txtQemu->setText(QFileInfo(fileName).absoluteFilePath());
		checkExists();
	}
}
