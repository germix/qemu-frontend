#include "QemuProcess.h"
#include <QProcess>
#include "TreeItem.h"
#include "TreeModel.h"

#include <QDir>
#include <QFileInfo>
#include <QTreeView>

QemuProcess::QemuProcess(QTreeView* tvw)
	: process(0)
	, treeView(tvw)
{
}
QemuProcess::~QemuProcess()
{
	if(process)
		delete process;
}
void QemuProcess::start(TreeModel* mdl, TreeItem* machineItem)
{
	item = machineItem;
	model = mdl;
	treeView->setEnabled(false);
	//model->setItemEnabled(item, false);
	if(process)
		delete process;
	// ...
	QString prog = machineItem->values["qemu"];
	QStringList args;
	char* boot_value = 0;
	int floppy_count = 0;
	foreach(TreeItem* item, machineItem->children)
	{
		switch(item->type)
		{
			case TreeItem::TYPE_MACHINE_AUDIO:
				args << "-soundhw" << item->getValue("audio");
				break;
			case TreeItem::TYPE_MACHINE_MEMORY:
				args << "-m" << item->getValue("value");
				break;
			case TreeItem::TYPE_MACHINE_DISPLAY:
				args << "-display" << item->getValue("type");
				break;
			case TreeItem::TYPE_MACHINE_MOUSE:
				if(item->getValue("mouse") == "usbmouse")
				{
					args << "-usbdevice" << "mouse";
				}
				break;
			case TreeItem::TYPE_MACHINE_KEYBOARD:
				if(item->getValue("keyboard") == "usbkeyboard")
				{
					args << "-usbdevice" << "keyboard";
				}
				break;
			case TreeItem::TYPE_MACHINE_CDROM:
				args << "-cdrom" << item->getValue("source");
				if(!boot_value)
				{
					boot_value = "d";
				}
				break;
			case TreeItem::TYPE_MACHINE_FLOPPY:
				{
					QString s;

					if(!(s = item->getValue("source-fda")).isEmpty())
					{
						args << "-fda" << item->getValue("source");
						if(!boot_value)
							boot_value = "a";
					}
					if(!(s = item->getValue("source-fdb")).isEmpty())
					{
						args << "-fdb" << item->getValue("source");
						if(!boot_value)
							boot_value = "b";
					}
				}
				break;
			case TreeItem::TYPE_MACHINE_PARALLEL:
				args << "-parallel" << "file:" + item->getValue("source");
				break;
			case TreeItem::TYPE_MACHINE_TABLET:
				if(item->getValue("tablet") == "usbtablet")
				{
					args << "-usbdevice" << "tablet";
				}
				break;
			case TreeItem::TYPE_MACHINE_NETWORK_IFACE:
				{
					QString model = item->getValue("model");
					QString macaddr = item->getValue("macaddr");
#if 1
					if(model != "none")
					{
						QString s;

						s += ",model=" + model;
						if(!macaddr.isEmpty())
							s += ",macaddr=" + macaddr;

						args << "-net" << "nic"+s;
					}
#else
					args << "-net" << "nic,model=rtl8139";
#endif
				}
				break;
		}
	}
	if(boot_value)
	{
		args << "-boot" << boot_value;
	}

	QDir().cd(QFileInfo(prog).absolutePath());

#if 0
	QProcess().startDetached("\""+prog+"\"", args);
#else
	process = new QProcess();

	connect(process, SIGNAL(finished(int)), this, SLOT(slotProcess_finished(int)));
	connect(process, SIGNAL(readyReadStandardError()), this, SLOT(slotProcess_readyReadStandardError()));
	connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(slotProcess_readyReadStandardOutput()));

	process->start("\""+prog+"\"", args);

#endif
}
void QemuProcess::slotProcess_finished(int exitCode)
{
	Q_UNUSED(exitCode);
	treeView->setEnabled(true);
	//model->setItemEnabled(item, true);
}
void QemuProcess::slotProcess_readyReadStandardError()
{
	qDebug("readAllStandardError()=%s", process->readAllStandardError().constData());
}
void QemuProcess::slotProcess_readyReadStandardOutput()
{
	qDebug("readAllStandardOutput()=%s", process->readAllStandardOutput().constData());
}


