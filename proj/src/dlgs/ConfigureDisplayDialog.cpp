#include "ConfigureDisplayDialog.h"
#include "ui_ConfigureDisplayDialog.h"

#include "../TreeItem.h"
#include "../TreeModel.h"

static struct
{
	const char* type;
	const char* name;
}
displays[] =
{
	{	"sdl",		"Display video output via SDL"				},
	{	"curses",	"Display video output via curses"			},
	{	"gtk",		"Display video output in a GTK window"		},
};
ConfigureDisplayDialog::ConfigureDisplayDialog(TreeItem* ___item, TreeModel* ___model, QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::ConfigureDisplayDialog)
	, item(___item)
	, model(___model)
{
	ui->setupUi(this);
	setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

	QString type = item->getValue("type");
	for(unsigned int i = 0; i < sizeof(displays)/sizeof(displays[0]); i++)
	{
		ui->comboBox->addItem(displays[i].name);
	}
	for(unsigned int i = 0; i < sizeof(displays)/sizeof(displays[0]); i++)
	{
		if(displays[i].type == type)
		{
			ui->comboBox->setCurrentIndex(i);
			break;
		}
	}
	connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}
ConfigureDisplayDialog::~ConfigureDisplayDialog()
{
	delete ui;
}
void ConfigureDisplayDialog::accept()
{
	item->setValue("type", displays[ui->comboBox->currentIndex()].type);
	model->setModified(true);
	// ...
	QDialog::accept();
}

