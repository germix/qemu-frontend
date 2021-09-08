#include "ConfigureParallelDialog.h"
#include "ui_ConfigureParallelDialog.h"

#include "../TreeItem.h"
#include "../TreeModel.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>

ConfigureParallelDialog::ConfigureParallelDialog(TreeItem* ___item, TreeModel* ___model, QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::ConfigureParallelDialog)
	, item(___item)
	, model(___model)
{
	ui->setupUi(this);
	setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

	QFileInfo fi(item->getValue("source"));
	ui->txtFileName->setText(fi.fileName());
	ui->txtFileDirectory->setText(fi.absolutePath());
	if(item->getValue("enabled") == "true")
		ui->frmParallel->setChecked(true);
	else
		ui->frmParallel->setChecked(false);
	checkValid();
	checkEnabled(ui->frmParallel->isChecked());

	connect(ui->frmParallel, SIGNAL(toggled(bool)), this, SLOT(slotFrame_toggled(bool)));
	connect(ui->txtFileName, SIGNAL(textChanged(QString)), this, SLOT(slotTextEdit_textChanged(QString)));
}
ConfigureParallelDialog::~ConfigureParallelDialog()
{
	delete ui;
}
void ConfigureParallelDialog::checkValid()
{
	checkFileValid();
	checkDirectoryExists();
}
void ConfigureParallelDialog::checkFileValid()
{
	QPalette palette = ui->txtFileName->palette();
	if(ui->txtFileName->text().isEmpty())
		palette.setColor(QPalette::Base, QColor(255, 205, 200));
	else
		palette.setColor(QPalette::Base, QColor(255, 255, 255));
	ui->txtFileName->setPalette(palette);
}
void ConfigureParallelDialog::checkDirectoryExists()
{
	QFileInfo fi(ui->txtFileDirectory->text());
	QPalette palette = ui->txtFileDirectory->palette();
	if(!fi.exists())
		palette.setColor(QPalette::Base, QColor(255, 205, 200));
	else
		palette.setColor(QPalette::Base, QColor(150, 255, 150));
	ui->txtFileDirectory->setPalette(palette);
}
void ConfigureParallelDialog::checkEnabled(bool on)
{
	ui->lblFileName->setEnabled(on);
	ui->txtFileName->setEnabled(on);
	ui->lblFileDirectory->setEnabled(on);
	ui->txtFileDirectory->setEnabled(on);
	ui->btnChooseDirectory->setEnabled(on);
}
void ConfigureParallelDialog::accept()
{
	if(ui->frmParallel->isChecked())
	{
		if(ui->txtFileName->text().isEmpty())
		{
			QMessageBox::information(0, "", tr("File name can't be empty"), QMessageBox::Information);
			return;
		}
		else if(ui->txtFileDirectory->text().isEmpty())
		{
			QMessageBox::information(0, "", tr("File directorty can't be empty"), QMessageBox::Information);
			return;
		}
		else
		{
			QFileInfo fi(ui->txtFileDirectory->text());
			if(!fi.exists())
			{
				QMessageBox::information(0, "", tr("Directory does not exists"), QMessageBox::Information);
				return;
			}
		}
		item->setValue("source", ui->txtFileDirectory->text() + QDir::separator() + ui->txtFileName->text());
		item->setValue("enabled", "true");
	}
	else
	{
		item->setValue("enabled", "false");
	}
	model->setModified(true);
	// ...
	QDialog::accept();
}
void ConfigureParallelDialog::slotChooseDirectory()
{
	QString dir = QFileDialog::getExistingDirectory(0, tr("Select directory"));
	if(!dir.isEmpty())
	{
		ui->txtFileDirectory->setText(dir);
		checkDirectoryExists();
	}
}
void ConfigureParallelDialog::slotFrame_toggled(bool on)
{
	checkEnabled(on);
}
void ConfigureParallelDialog::slotTextEdit_textChanged(const QString& text)
{
	Q_UNUSED(text);
	checkFileValid();
}


