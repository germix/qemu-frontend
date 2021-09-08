#include "ConfigureFloppyDialog.h"
#include "ui_ConfigureFloppyDialog.h"

#include  "../TreeItem.h"
#include "../TreeModel.h"
#include <QFileDialog>

ConfigureFloppyDialog::ConfigureFloppyDialog(TreeItem* ___item, TreeModel* ___model, QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::ConfigureFloppyDialog)
	, item(___item)
	, model(___model)
{
	ui->setupUi(this);
	setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

	ui->txtFloppyFda->setText(item->getValue("source-fda"));
	checkExists(ui->txtFloppyFda);
	ui->txtFloppyFdb->setText(item->getValue("source-fdb"));
	checkExists(ui->txtFloppyFdb);
}

ConfigureFloppyDialog::~ConfigureFloppyDialog()
{
	delete ui;
}
void ConfigureFloppyDialog::checkExists()
{
	checkExists(ui->txtFloppyFda);
	checkExists(ui->txtFloppyFdb);
}
void ConfigureFloppyDialog::checkExists(QLineEdit* txt)
{
	QPalette palette = txt->palette();
	if(txt->text().isEmpty())
	{
		palette.setColor(QPalette::Base, QColor(255, 255, 255));
	}
	else
	{
		QFileInfo fi(txt->text());
		if(!fi.exists())
			palette.setColor(QPalette::Base, QColor(255, 205, 200));
		else
			palette.setColor(QPalette::Base, QColor(150, 255, 150));
	}
	txt->setPalette(palette);
}
void ConfigureFloppyDialog::chooseFloppy(QLineEdit* txt)
{
	QString fileName = QFileDialog::getOpenFileName(
								0,
								tr("Find floppy file"),
								QFileInfo(txt->text()).absolutePath(),
								tr("Floppy files (%1)").arg("*.flp"));
	if(!fileName.isEmpty())
	{
		txt->setText(QFileInfo(fileName).absoluteFilePath());
		checkExists(txt);
	}
}
void ConfigureFloppyDialog::removeFloppy(QLineEdit* txt)
{
	txt->setText("");
	checkExists(txt);
}
QString ConfigureFloppyDialog::getSourceFda() const
{
	return ui->txtFloppyFda->text();
}
QString ConfigureFloppyDialog::getSourceFdb() const
{
	return ui->txtFloppyFdb->text();
}
void ConfigureFloppyDialog::accept()
{
	item->setValue("source-fda", getSourceFda());
	item->setValue("source-fdb", getSourceFdb());
	model->setModified(true);
	// ...
	QDialog::accept();
}
void ConfigureFloppyDialog::slotChooseFloppy()
{
	QToolButton* btn = qobject_cast<QToolButton*>(sender());

	if(btn == ui->btnChooseFloppyFda)		chooseFloppy(ui->txtFloppyFda);
	else if(btn == ui->btnChooseFloppyFdb)	chooseFloppy(ui->txtFloppyFdb);
}
void ConfigureFloppyDialog::slotRemoveFloppy()
{
	QToolButton* btn = qobject_cast<QToolButton*>(sender());

	if(btn == ui->btnChooseFloppyFda)		removeFloppy(ui->txtFloppyFda);
	else if(btn == ui->btnChooseFloppyFdb)	removeFloppy(ui->txtFloppyFdb);

}



