#include "ConfigureCdromDialog.h"
#include "ui_ConfigureCdromDialog.h"

#include "../TreeItem.h"
#include "../TreeModel.h"
#include <QFileInfo>
#include <QFileDialog>

ConfigureCdromDialog::ConfigureCdromDialog(TreeItem* ___item, TreeModel* ___model, QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::ConfigureCdromDialog)
	, item(___item)
	, model(___model)
{
	ui->setupUi(this);
	setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

	ui->txtCdrom->setText(item->getValue("source"));
	checkExists();
}

ConfigureCdromDialog::~ConfigureCdromDialog()
{
	delete ui;
}
void ConfigureCdromDialog::checkExists()
{
	QPalette palette = ui->txtCdrom->palette();
	if(ui->txtCdrom->text().isEmpty())
	{
		palette.setColor(QPalette::Base, QColor(255, 255, 255));
	}
	else
	{
		QFileInfo fi(ui->txtCdrom->text());
		if(!fi.exists())
			palette.setColor(QPalette::Base, QColor(255, 205, 200));
		else
			palette.setColor(QPalette::Base, QColor(150, 255, 150));
	}
	ui->txtCdrom->setPalette(palette);
}
QString ConfigureCdromDialog::getSource() const
{
	return ui->txtCdrom->text();
}
void ConfigureCdromDialog::accept()
{
	item->setValue("source", getSource());
	model->setModified(true);
	// ...
	QDialog::accept();
}
void ConfigureCdromDialog::slotChooseCdrom()
{
	QString fileName = QFileDialog::getOpenFileName(
								0,
								tr("Find ISO file"),
								QFileInfo(ui->txtCdrom->text()).absolutePath(),
								tr("ISO files (%1)").arg("*.iso"));
	if(!fileName.isEmpty())
	{
		ui->txtCdrom->setText(QFileInfo(fileName).absoluteFilePath());
		checkExists();
	}
}
void ConfigureCdromDialog::slotRemoveCdrom()
{
	ui->txtCdrom->setText("");
	checkExists();
}
