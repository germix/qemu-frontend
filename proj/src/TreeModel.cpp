#include "TreeModel.h"

#define MAX_COLUMNS 1

#include <QFile>
#include <QFileInfo>
#include <QXmlStreamReader>

TreeModel::TreeModel()
	: root(0)
	, modified(false)
{
}
TreeModel::~TreeModel()
{
	clear();
}
void TreeModel::clear()
{
	beginResetModel();
	if(root)
		delete root;
	root = nullptr;
	modified = false;
	fileName = QString();
	// ...
	emit onModified();
	// ...
	endResetModel();
}
void TreeModel::newFile()
{
	beginResetModel();
	if(root)
		delete root;
	root = new TreeItem();
	root->values["version"] = "1.0";
	modified = false;
	fileName = QString();
	// ...
	emit onModified();
	// ...
	endResetModel();
}
bool TreeModel::openFile(const QString& fname)
{
	QFile file(fname);
	if(file.open(QFile::ReadOnly))
	{
		QXmlStreamReader xml(&file);

		beginResetModel();

		if(root != nullptr) delete root;

		root = readAll(xml);

		endResetModel();
		// ...
		modified = false;
		fileName = QFileInfo(fname).absoluteFilePath();

		// ...
		emit onModified();
		// ...
		return true;
	}
	return false;
}
bool TreeModel::saveFile(const QString& fname)
{
	QFile f(fname);

	if(f.open(QFile::WriteOnly))
	{
		QXmlStreamWriter stream(&f);
		stream.setAutoFormatting(true);
		stream.writeStartDocument();

		saveItem(stream, root);

		modified = false;
		fileName = QFileInfo(fname).absoluteFilePath();
		// ...
		emit onModified();
		// ...
		return true;
	}
	return false;
}
void TreeModel::setModified(bool m)
{
	if(modified != m)
	{
		modified = m;
		emit onModified();
	}
}
void TreeModel::addItem(const QModelIndex& parentIndex, int type, const QHash<QString,QString>& values)
{
	TreeItem* parent = toItem(parentIndex);
	beginInsertRows(toIndex(parent), parent->children.size(), parent->children.size()+1);
	TreeItem* item = new TreeItem(parent, type);
	item->values = values;
	endInsertRows();

	setModified(true);
}
void TreeModel::removeIndex(const QModelIndex& index)
{
	if(index.isValid())
	{
		beginRemoveRows(index.parent(), index.row(), index.row());
		TreeItem* item = toItem(index);
		delete item;
		modified = true;
		emit onModified();
		endRemoveRows();
	}
}
TreeItem* TreeModel::getItemByType(int type, TreeItem* parent)
{
	if(parent == nullptr)
		parent = root;
	if(parent != nullptr)
	{
		for(int i = 0; i < parent->children.size(); i++)
		{
			TreeItem* item = parent->children.at(i);
			if(item->type == type)
				return item;
			TreeItem* child = getItemByType(type, item);
			if(child)
				return child;
		}
	}
	return nullptr;
}
void TreeModel::addMachine(const QString& name, const QString& qemu)
{
	beginInsertRows(toIndex(root), root->children.size(), root->children.size()+1);
	TreeItem* mach = new TreeItem(root, TreeItem::TYPE_MACHINE);
	mach->values["name"] = name;
	mach->values["qemu"] = qemu;
	new TreeItem(mach, TreeItem::TYPE_MACHINE_MEMORY);
	new TreeItem(mach, TreeItem::TYPE_MACHINE_DISPLAY);
	new TreeItem(mach, TreeItem::TYPE_MACHINE_AUDIO);
	new TreeItem(mach, TreeItem::TYPE_MACHINE_MOUSE);
	new TreeItem(mach, TreeItem::TYPE_MACHINE_TABLET);
	new TreeItem(mach, TreeItem::TYPE_MACHINE_KEYBOARD);
	new TreeItem(mach, TreeItem::TYPE_MACHINE_CDROM);
	new TreeItem(mach, TreeItem::TYPE_MACHINE_FLOPPY);
	new TreeItem(mach, TreeItem::TYPE_MACHINE_PARALLEL);
	new TreeItem(mach, TreeItem::TYPE_MACHINE_NETWORK);
	endInsertRows();
	// ...
	modified = true;
	emit onModified();
}
TreeItem* TreeModel::toItem(const QModelIndex& index) const
{
	if(!index.isValid())
		return root;
	return static_cast<TreeItem*>(index.internalPointer());
}
QModelIndex TreeModel::toIndex(TreeItem* item, int column) const
{
	if(!root)
		return QModelIndex();
	if(!item)// || (item == root))
		return QModelIndex();
	TreeItem* parent = item->parent;
	if(!parent)
		parent = root;
	int row = parent->children.lastIndexOf(item);
	if(row != -1)
		return createIndex(row, column, item);
	return QModelIndex();
}
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if(!root)
		return QModelIndex();
	if(row < 0 || column < 0 || column >= MAX_COLUMNS)
		return QModelIndex();
	TreeItem* parentItem = parent.isValid() ? toItem(parent) : root;
	if(parentItem && row < parentItem->children.count())
	{
		TreeItem* item = parentItem->children.at(row);
		if(item)
			return createIndex(row, column, item);
		return QModelIndex();
	}
	return QModelIndex();
}
QModelIndex TreeModel::parent(const QModelIndex &child) const
{
	if(!root)
		return QModelIndex();
	TreeItem* item = toItem(child);
	if(!item)
		return QModelIndex();
	TreeItem* parentItem = item->parent;
	if(!parentItem)
		return QModelIndex();
	TreeItem* grandparentElement = parentItem->parent;
	if(!grandparentElement)
		return QModelIndex();

	int row = grandparentElement->children.indexOf(parentItem);
	return createIndex(row, 0, parentItem);
}
int TreeModel::rowCount(const QModelIndex &parent) const
{
	if(!root)
		return 0;
	if(!parent.isValid())
		return root->children.count();
	TreeItem* parentItem = toItem(parent);
	if(parentItem)
		return parentItem->children.count();
	return 0;
}
int TreeModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return MAX_COLUMNS;
}
bool TreeModel::hasChildren(const QModelIndex &parent) const
{
	if(!root)
		return false;
	if(!parent.isValid())
		return (root->children.count() > 0);
	TreeItem* item = toItem(parent);
	if(!item)
		return false;

	return (item->children.count() > 0);
}
QVariant TreeModel::data(const QModelIndex &index, int role) const
{
	if(!root)
		return QVariant();
	if(!index.isValid())
		return QVariant();
	TreeItem* item = toItem(index);
	if(!item)
		return QVariant();
	if(role == Qt::DisplayRole)
	{
		return item->getLabel();
	}
	else if(role == Qt::DecorationRole)
	{
		if(index.column() == 0)
		{
			return item->getIcon();
		}
	}
	return QVariant();
}
Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
	TreeItem* item = toItem(index);
	Qt::ItemFlags flags = QAbstractItemModel::flags(index);
	if(!item->isEnabled())
		flags ^= (Qt::ItemIsEnabled|Qt::ItemIsSelectable);
	else
		flags |= Qt::ItemIsEnabled;
	return flags;
}
TreeItem* TreeModel::readAll(QXmlStreamReader& xml)
{
	TreeItem* root = 0;
	try
	{
		if(xml.readNextStartElement())
		{
			if(xml.name().toString() == "Workspace" && xml.attributes().value("version").toString() == "1.0")
			{
				root = new TreeItem();
				root->values["version"] = "1.0";
				readWorkspace(xml, root);
			}
			else
			{
				xml.raiseError(tr("This file is not an Workspace version 1.0 file"));
			}
		}
	}
	catch(std::exception& e)
	{
		if(xml.hasError())
		{
			qDebug("Error: %ls\n", (wchar_t*)xml.errorString().constData());
		}
	}
	return root;
}
void TreeModel::readWorkspace(QXmlStreamReader& xml, TreeItem* parent)
{
	Q_ASSERT(xml.isStartElement() && xml.name().toString() == "Workspace");

	while(xml.readNextStartElement())
	{
		if(xml.name().toString() == "Machine")
			readMachine(xml, parent);
		else
			xml.skipCurrentElement();
	}
}

void TreeModel::readMachine(QXmlStreamReader& xml, TreeItem* parent)
{
	TreeItem* item = new TreeItem(parent, TreeItem::TYPE_MACHINE);
	item->values["qemu"] = xml.attributes().value("qemu").toString();
	item->values["name"] = xml.attributes().value("name").toString();

	while(xml.readNextStartElement())
	{
		if(xml.name() == "Audio")			{ readItemXml(xml, item, TreeItem::TYPE_MACHINE_AUDIO);			}
		else if(xml.name() == "Mouse")		{ readItemXml(xml, item, TreeItem::TYPE_MACHINE_MOUSE);			}
		else if(xml.name() == "Tablet")		{ readItemXml(xml, item, TreeItem::TYPE_MACHINE_TABLET);		}
		else if(xml.name() == "Keyboard")	{ readItemXml(xml, item, TreeItem::TYPE_MACHINE_KEYBOARD);		}

		else if(xml.name() == "Memory")		{ readItemXml(xml, item, TreeItem::TYPE_MACHINE_MEMORY,		"value");					}
		else if(xml.name() == "Display")	{ readItemXml(xml, item, TreeItem::TYPE_MACHINE_DISPLAY,	"type");					}
		else if(xml.name() == "CdRom")		{ readItemXml(xml, item, TreeItem::TYPE_MACHINE_CDROM,		"source");					}
		else if(xml.name() == "Floppy")		{ readItemXml(xml, item, TreeItem::TYPE_MACHINE_FLOPPY,		"source-fda,source-fdb");	}
		else if(xml.name() == "Parallel")	{ readItemXml(xml, item, TreeItem::TYPE_MACHINE_PARALLEL,	"source,enabled");			}
		else if(xml.name() == "Network")
		{
			TreeItem* netItem = new TreeItem(item, TreeItem::TYPE_MACHINE_NETWORK);

			xml.readNext();
			while(xml.readNextStartElement())
			{
				if(xml.name() == "Interface")
				{
					readItemXml(xml, netItem, TreeItem::TYPE_MACHINE_NETWORK_IFACE);
				}
				else
				{
					xml.skipCurrentElement();
				}
			}
		}
		else
		{
			xml.skipCurrentElement();
		}
	}
	//xml.skipCurrentElement();
}
void TreeModel::readItemXml(QXmlStreamReader& in, TreeItem* parent, int type)
{
	TreeItem* item = new TreeItem(parent, type);
	foreach (const QXmlStreamAttribute& attr, in.attributes())
	{
		QString key = attr.name().toString();
		QString value = attr.value().toString();

		item->values[key] = value;
	}
	in.skipCurrentElement();
}
void TreeModel::readItemXml(QXmlStreamReader& in, TreeItem* parent, int type, const QString& keys)
{
	TreeItem* item = new TreeItem(parent, type);
	foreach(QString k, keys.split(","))
	{
		item->values[k] = in.attributes().value(k).toString();
	}
	in.skipCurrentElement();
}
void TreeModel::saveItem(QXmlStreamWriter& out, TreeItem* item)
{
	if(item->type == TreeItem::TYPE_ROOT)
	{
		saveItemXml(out, item, "Workspace");
	}
	// ...
	else if(item->type == TreeItem::TYPE_MACHINE)
	{
		saveItemXml(out, item, "Machine");
	}
	else if(item->type == TreeItem::TYPE_MACHINE_MEMORY)		{	saveItemXml(out, item, "Memory");		}
	else if(item->type == TreeItem::TYPE_MACHINE_AUDIO)			{	saveItemXml(out, item, "Audio");		}
	else if(item->type == TreeItem::TYPE_MACHINE_MOUSE)			{	saveItemXml(out, item, "Mouse");		}
	else if(item->type == TreeItem::TYPE_MACHINE_KEYBOARD)		{	saveItemXml(out, item, "Keyboard");		}
	else if(item->type == TreeItem::TYPE_MACHINE_DISPLAY)		{	saveItemXml(out, item, "Display");		}
	else if(item->type == TreeItem::TYPE_MACHINE_CDROM)			{	saveItemXml(out, item, "CdRom");		}
	else if(item->type == TreeItem::TYPE_MACHINE_FLOPPY)		{	saveItemXml(out, item, "Floppy");		}
	else if(item->type == TreeItem::TYPE_MACHINE_PARALLEL)		{	saveItemXml(out, item, "Parallel");		}
	else if(item->type == TreeItem::TYPE_MACHINE_TABLET)		{	saveItemXml(out, item, "Tablet");		}
	else if(item->type == TreeItem::TYPE_MACHINE_NETWORK)		{	saveItemXml(out, item, "Network");		}
	else if(item->type == TreeItem::TYPE_MACHINE_NETWORK_IFACE)	{	saveItemXml(out, item, "Interface");	}
	else
	{
		return;
	}
	saveItems(out, item->children);
	out.writeEndElement();
}
void TreeModel::saveItems(QXmlStreamWriter& out, QVector<TreeItem*>& items)
{
	foreach(TreeItem* item, items)
	{
		saveItem(out, item);
	}
}
void TreeModel::saveItemXml(QXmlStreamWriter& out, TreeItem* item, const QString& label)
{
	out.writeStartElement(label);
	QHash<QString,QString>::const_iterator it;
	for(it = item->values.constBegin(); it != item->values.constEnd(); it++)
	{
		out.writeAttribute(it.key(), it.value());
	}
}
