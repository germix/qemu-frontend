#include "TreeItem.h"
#include <QFileInfo>

TreeItem::TreeItem()
	:
	type(TYPE_ROOT),
	parent(0),
	enabled(true)
{
}
TreeItem::TreeItem(TreeItem* ___parent, int ___type)
	:
	type(___type),
	parent(___parent),
	enabled(true)
{
	if(parent != NULL)
	{
		parent->children.append(this);
	}
}
TreeItem::~TreeItem()
{
	if(parent != NULL)
	{
		parent->children.remove(parent->children.indexOf(this));
	}
	foreach(TreeItem* child , children)
	{
		child->parent = NULL;
		delete child;
	}
	children.clear();
}
QIcon TreeItem::getIcon() const
{
	switch(type)
	{
		case TYPE_MACHINE:					return QIcon(":/images/items/machine.png");
		case TYPE_MACHINE_AUDIO:			return QIcon(":/images/items/audio.png");
		case TYPE_MACHINE_USB:				return QIcon(":/images/items/usb.png");
		case TYPE_MACHINE_MEMORY:			return QIcon(":/images/items/memory.png");
		case TYPE_MACHINE_NETWORK:			return QIcon(":/images/items/network.png");
		case TYPE_MACHINE_MOUSE:			return QIcon(":/images/items/mouse.png");
		case TYPE_MACHINE_KEYBOARD:			return QIcon(":/images/items/keyboard.png");
		case TYPE_MACHINE_DISPLAY:			return QIcon(":/images/items/monitor.png");
		case TYPE_MACHINE_CDROM:			return QIcon(":/images/items/cdrom.png");
		case TYPE_MACHINE_FLOPPY:			return QIcon(":/images/items/floppy.png");
		case TYPE_MACHINE_PARALLEL:			return QIcon(":/images/items/parallel.png");

		case TYPE_MACHINE_TABLET:			return QIcon(":/images/items/tablet.png");
		case TYPE_MACHINE_NETWORK_IFACE:	return QIcon(":/images/items/interface.png");
	}
	return QIcon();
}
QString TreeItem::getLabel() const
{
	QString s;

	switch(type)
	{
		case TYPE_MACHINE:
			s = "Machine";
			if(!values["name"].isEmpty())		{ s += " (" + values["name"] + ")";								}	break;
		case TYPE_MACHINE_MEMORY:
			s = "Memory";
			if(!values["value"].isEmpty())		{ s += " (" + values["value"] + ")";							}	break;
		case TYPE_MACHINE_DISPLAY:
			s = "Display";
			if(!values["type"].isEmpty())		{ s += " (" + values["type"] + ")";								}	break;
		case TYPE_MACHINE_AUDIO:
			s = "Audio";
			if(!values["audio"].isEmpty())		{ s += " (" + values["audio"] + ")";							}	break;
		case TYPE_MACHINE_MOUSE:
			s = "Mouse";
			if(!values["mouse"].isEmpty())		{ s += " (" + values["mouse"] + ")";							}	break;
		case TYPE_MACHINE_TABLET:
			s = "Tablet";
			if(!values["tablet"].isEmpty())		{ s += " (" + values["tablet"] + ")";							}	break;
		case TYPE_MACHINE_KEYBOARD:
			s = "Keyboard";
			if(!values["keyboard"].isEmpty())	{ s += " (" + values["keyboard"] + ")";							}	break;
		case TYPE_MACHINE_CDROM:
			s = "CD-ROM";
			if(!values["source"].isEmpty())		{ s += " (" + QFileInfo(values["source"]).fileName() + ")";		}	break;
		case TYPE_MACHINE_FLOPPY:
			s = "Floppy";
			if(!values["source"].isEmpty())		{ s += " (" + QFileInfo(values["source"]).fileName() + ")";		}	break;
		case TYPE_MACHINE_PARALLEL:
			s = "Parallel";
			break;
		case TYPE_MACHINE_NETWORK:
			s = "Network";
			break;
		case TYPE_MACHINE_NETWORK_IFACE:
			s = "Interface";
			if(!values["name"].isEmpty())		{ s += " (" + values["name"] + ")";								}	break;
	}
	return s;
}
