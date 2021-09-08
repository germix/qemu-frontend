#include "ConfigureTabletDialog.h"
#include "ui_ConfigureTabletDialog.h"

#include "../TreeItem.h"
#include "../TreeModel.h"

static struct
{
	const char* type;
	const char* name;
}
tablet[] =
{
	{	"none"	,		"None"				},
	{	"usbtablet",	"USB Tablet"		},
};
ConfigureTabletDialog::ConfigureTabletDialog(TreeItem* ___item, TreeModel* ___model, QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::ConfigureTabletDialog)
	, item(___item)
	, model(___model)
{
	ui->setupUi(this);
	setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

	QString type = item->getValue("type");
	for(unsigned int i = 0; i < sizeof(tablet)/sizeof(tablet[0]); i++)
	{
		ui->comboBox->addItem(tablet[i].name);
	}
	for(unsigned int i = 0; i < sizeof(tablet)/sizeof(tablet[0]); i++)
	{
		if(tablet[i].type == type)
		{
			ui->comboBox->setCurrentIndex(i);
			break;
		}
	}
}
ConfigureTabletDialog::~ConfigureTabletDialog()
{
	delete ui;
}
void ConfigureTabletDialog::accept()
{
	item->setValue("tablet", tablet[ui->comboBox->currentIndex()].type);
	model->setModified(true);
	// ...
	QDialog::accept();
}


