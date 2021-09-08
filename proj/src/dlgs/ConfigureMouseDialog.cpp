#include "ConfigureMouseDialog.h"
#include "ui_ConfigureMouseDialog.h"

#include "../TreeItem.h"
#include "../TreeModel.h"

static struct
{
	const char* type;
	const char* name;
}
mouse[] =
{
	{	"ps2mouse",		"PS/2 Mouse"			},
	{	"usbmouse",		"USB Mouse"				},
};
ConfigureMouseDialog::ConfigureMouseDialog(TreeItem* ___item, TreeModel* ___model, QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::ConfigureMouseDialog)
	, item(___item)
	, model(___model)
{
	ui->setupUi(this);
	setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

	QString type = item->getValue("mouse");
	for(unsigned int i = 0; i < sizeof(mouse)/sizeof(mouse[0]); i++)
	{
		ui->comboBox->addItem(mouse[i].name);
	}
	for(unsigned int i = 0; i < sizeof(mouse)/sizeof(mouse[0]); i++)
	{
		if(mouse[i].type == type)
		{
			ui->comboBox->setCurrentIndex(i);
			break;
		}
	}
}
ConfigureMouseDialog::~ConfigureMouseDialog()
{
	delete ui;
}
void ConfigureMouseDialog::accept()
{
	item->setValue("mouse", mouse[ui->comboBox->currentIndex()].type);
	model->setModified(true);
	// ...
	QDialog::accept();
}
