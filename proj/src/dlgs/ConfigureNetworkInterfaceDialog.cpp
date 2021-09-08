#include "ConfigureNetworkInterfaceDialog.h"
#include "ui_ConfigureNetworkInterfaceDialog.h"

#include "../TreeItem.h"
#include "../TreeModel.h"

#include <QRegExpValidator>
#include <QRegExp>

#include <QMessageBox>

static const char* netmodels[] =
{
	"none",
	// ...
	"dp83816",
	"e100",
	"ne2k_pci",
	"i82551",
	"i82557a",
	"i82557b",
	"i82557c",
	"i82558b",
	"i82559c",
	"i82559er",
	"i82801",
	"rtl8139",
	"e1000",
	"pcnet",
	"tnetw1130",
	"virtio",
};
static QRegExp getRegex()
{
	return QRegExp("([0-9a-fA-F]{2})(:([0-9a-fA-F]{2})){5}");
}
ConfigureNetworkInterfaceDialog::ConfigureNetworkInterfaceDialog(TreeItem* ___item, TreeModel* ___model, QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::ConfigureNetworkInterfaceDialog)
	, item(___item)
	, model(___model)
{
	ui->setupUi(this);
	setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

	QString name;
	QString macaddr;
	QString netmodel;

	if(item)
	{
		name = item->getValue("name");
		macaddr = item->getValue("macaddr");
		netmodel = item->getValue("model");
	}
	else
	{
		netmodel = "pcnet";
		setWindowTitle(tr("Add network interface"));
	}
#if 0
	QRegExp rx(getRegex());
	QRegExpValidator* validator = new QRegExpValidator(rx, this);
	ui->txtMacAddress->setValidator(validator);
#endif
	for(unsigned int i = 0; i < sizeof(netmodels)/sizeof(netmodels[0]); i++)
	{
		ui->cboModel->addItem(netmodels[i]);
	}
	for(unsigned int i = 0; i < sizeof(netmodels)/sizeof(netmodels[0]); i++)
	{
		if(netmodels[i] == netmodel)
		{
			ui->cboModel->setCurrentIndex(i);
			break;
		}
	}
	// ...
	ui->txtName->setText(name);
	checkName(name);
	// ...
	ui->txtMacAddress->setText(macaddr);
	checkMacAddress(macaddr);

	connect(ui->txtName, SIGNAL(textChanged(QString)), this, SLOT(slotLineEdit_textChanged(QString)));
	connect(ui->txtMacAddress, SIGNAL(textChanged(QString)), this, SLOT(slotLineEdit_textChanged(QString)));
}
ConfigureNetworkInterfaceDialog::~ConfigureNetworkInterfaceDialog()
{
	delete ui;
}
QString ConfigureNetworkInterfaceDialog::getName() const
{
	return ui->txtName->text();
}
QString ConfigureNetworkInterfaceDialog::getModel() const
{
	return netmodels[ui->cboModel->currentIndex()];
}
QString ConfigureNetworkInterfaceDialog::getMacAddress() const
{
	return ui->txtMacAddress->text().toUpper();
}
void ConfigureNetworkInterfaceDialog::checkName(const QString& text)
{
	QPalette pal = ui->txtName->palette();
	if(text.isEmpty())
		pal.setColor(QPalette::Base, QColor(Qt::red).lighter());
	else
		pal.setColor(QPalette::Base, QColor(Qt::green).lighter());
	ui->txtName->setPalette(pal);
}
void ConfigureNetworkInterfaceDialog::checkMacAddress(const QString& text)
{
	QPalette pal = ui->txtMacAddress->palette();
	if(text.isEmpty())
	{
		pal.setColor(QPalette::Window, Qt::black);
	}
	else
	{
		if(!getRegex().exactMatch(text))
			pal.setColor(QPalette::Base, QColor(Qt::red).lighter());
		else
			pal.setColor(QPalette::Base, QColor(Qt::green).lighter());
	}
	ui->txtMacAddress->setPalette(pal);
}
void ConfigureNetworkInterfaceDialog::accept()
{
	QString s;

	//
	// Comprobar nombre
	//
	s = ui->txtName->text();
	if(s.isEmpty())
	{
		QMessageBox::information(0, 0, tr("Name can't be empty"), QMessageBox::Ok);
		return;
	}

	//
	// Comprobar dirección MAC
	//
	s = ui->txtMacAddress->text();
	if(!s.isEmpty() && !getRegex().exactMatch(s))
	{
		QMessageBox::information(0, 0, tr("Bad MAC Addresss"), QMessageBox::Ok);
		return;
	}
	//
	// ...
	//
	if(item)
	{
		item->setValue("name", getName());
		item->setValue("model", getModel());
		item->setValue("macaddr", getMacAddress());
		model->setModified(true);
	}
	else
	{
		QHash<QString, QString> values;
		values["name"] = getName();
		values["model"] = getModel();
		values["macaddr"] = getMacAddress();

		model->addItem(model->getItemByType(TreeItem::TYPE_MACHINE_NETWORK), TreeItem::TYPE_MACHINE_NETWORK_IFACE, values);
	}
	// ...
	QDialog::accept();
}
void ConfigureNetworkInterfaceDialog::slotLineEdit_textChanged(const QString& text)
{
	QLineEdit* edt = qobject_cast<QLineEdit*>(sender());
	if(edt == ui->txtName)				checkName(text);
	else if(edt == ui->txtMacAddress)	checkMacAddress(text);
}

