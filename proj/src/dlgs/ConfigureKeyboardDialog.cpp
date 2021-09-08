#include "ConfigureKeyboardDialog.h"
#include "ui_ConfigureKeyboardDialog.h"

#include "../TreeItem.h"
#include "../TreeModel.h"

static struct
{
	const char* type;
	const char* name;
}
keyboard[] =
{
	{	"ps2keyboard",		"PS/2 Keyboard"			},
	{	"usbkeyboard",		"USB Keyboard"			},
};

ConfigureKeyboardDialog::ConfigureKeyboardDialog(TreeItem* ___item, TreeModel* ___model, QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::ConfigureKeyboardDialog)
	, item(___item)
	, model(___model)
{
	ui->setupUi(this);
	setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

	QString type = item->getValue("keyboard");

	for(unsigned int i = 0; i < sizeof(keyboard)/sizeof(keyboard[0]); i++)
	{
		ui->comboBox->addItem(keyboard[i].name);
	}
	for(unsigned int i = 0; i < sizeof(keyboard)/sizeof(keyboard[0]); i++)
	{
		if(keyboard[i].type == type)
		{
			ui->comboBox->setCurrentIndex(i);
			break;
		}
	}
}
ConfigureKeyboardDialog::~ConfigureKeyboardDialog()
{
	delete ui;
}
void ConfigureKeyboardDialog::accept()
{
	item->setValue("keyboard", keyboard[ui->comboBox->currentIndex()].type);
	model->setModified(true);
	// ...
	QDialog::accept();
}
