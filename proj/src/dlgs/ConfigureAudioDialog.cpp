#include "ConfigureAudioDialog.h"
#include "ui_ConfigureAudioDialog.h"

#include "../TreeItem.h"
#include "../TreeModel.h"

static struct
{
	const char* type;
	const char* name;
}
soundhw[] =
{
	{	"none",			0								},
	{	"pcspk",		"PC speaker"					},
	{	"hda",			"Intel HD Audio"				},
	{	"cs4231a",		"CS4231A"						},
	{	"gus",			"Gravis Ultrasound GF1"			},
	{	"adlib",		"Yamaha YM3812 (OPL2)"			},
	{	"ac97",			"Intel 82801AA AC97 Audio"		},
	{	"es1370",		"ENSONIQ AudioPCI ES1370"		},
	{	"sb16",			"Creative Sound Blaster 16"		},
};

ConfigureAudioDialog::ConfigureAudioDialog(TreeItem* ___item, TreeModel* ___model, QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::ConfigureAudioDialog)
	, item(___item)
	, model(___model)
{
	ui->setupUi(this);
	setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

	QString type = item->getValue("audio");
	for(unsigned int i = 0; i < sizeof(soundhw)/sizeof(soundhw[0]); i++)
	{
		QString s;

		s = soundhw[i].type;
		if(soundhw[i].name != NULL)
		{
			s += " (";
			s += soundhw[i].name;
			s += ")";
		}
		ui->comboBox->addItem(s);
	}
	for(unsigned int i = 0; i < sizeof(soundhw)/sizeof(soundhw[0]); i++)
	{
		if(soundhw[i].type == type)
		{
			ui->comboBox->setCurrentIndex(i);
			break;
		}
	}
}
ConfigureAudioDialog::~ConfigureAudioDialog()
{
	delete ui;
}
void ConfigureAudioDialog::accept()
{
	item->setValue("audio", soundhw[ui->comboBox->currentIndex()].type);
	model->setModified(true);
	// ...
	QDialog::accept();
}
