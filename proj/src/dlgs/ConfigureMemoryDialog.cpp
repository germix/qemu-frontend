#include "ConfigureMemoryDialog.h"
#include "ui_ConfigureMemoryDialog.h"

#include "../TreeItem.h"
#include "../TreeModel.h"

static struct
{
	const char* value;
	const char* description;
}
memory[] =
{
	{ "64M",	"64 megabytes"		},
	{ "128M",	"128 megabytes"		},
	{ "256M",	"256 megabytes"		},
	{ "512M",	"512 megabytes"		},
	{ "1G",		"1 gigabytes"		},
	{ "2G",		"2 gigabytes"		},
	{ "4G",		"4 gigabytes"		},
};

ConfigureMemoryDialog::ConfigureMemoryDialog(TreeItem* ___item, TreeModel* ___model, QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::ConfigureMemoryDialog)
	, item(___item)
	, model(___model)
{
	ui->setupUi(this);
	setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

	QString value = item->getValue("value");

	for(unsigned int i = 0; i < sizeof(memory)/sizeof(memory[0]); i++)
	{
		ui->comboBox->addItem(memory[i].description);
	}
	for(unsigned int i = 0; i < sizeof(memory)/sizeof(memory[0]); i++)
	{
		if(memory[i].value == value)
		{
			ui->comboBox->setCurrentIndex(i);
			break;
		}
	}
}
ConfigureMemoryDialog::~ConfigureMemoryDialog()
{
	delete ui;
}
void ConfigureMemoryDialog::accept()
{
	item->setValue("value", memory[ui->comboBox->currentIndex()].value);
	model->setModified(true);
	// ...
	QDialog::accept();
}
