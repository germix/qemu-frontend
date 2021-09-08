#include "ConfigureMachineDialog.h"
#include "ui_ConfigureMachineDialog.h"

#include "../TreeItem.h"
#include "../TreeModel.h"
#include <QFileDialog>

ConfigureMachineDialog::ConfigureMachineDialog(TreeItem* ___item, TreeModel* ___model, QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::ConfigureMachineDialog)
	, item(___item)
	, model(___model)
{
	ui->setupUi(this);
	setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

	ui->txtName->setText(item->getValue("name"));
	ui->txtQemu->setText(item->getValue("qemu"));
	checkExists();
}
ConfigureMachineDialog::~ConfigureMachineDialog()
{
	delete ui;
}
void ConfigureMachineDialog::checkExists()
{
	QFileInfo fi(ui->txtQemu->text());
	QPalette palette = ui->txtQemu->palette();
	if(!fi.exists())
		palette.setColor(QPalette::Base, QColor(255, 205, 200));
	else
		palette.setColor(QPalette::Base, QColor(150, 255, 150));
	ui->txtQemu->setPalette(palette);
}
void ConfigureMachineDialog::accept()
{
	item->setValue("name", ui->txtName->text());
	item->setValue("qemu", ui->txtQemu->text());
	model->setModified(true);
	// ...
	QDialog::accept();
}
void ConfigureMachineDialog::slotChooseQemu()
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
