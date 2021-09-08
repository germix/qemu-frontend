#ifndef TREEITEM_H
#define TREEITEM_H
#include <QIcon>
#include <QHash>
#include <QVector>

class TreeItem
{
public:
	enum
	{
		TYPE_ROOT,

		TYPE_MACHINE,
		TYPE_MACHINE_AUDIO,
		TYPE_MACHINE_USB,
		TYPE_MACHINE_MEMORY,
		TYPE_MACHINE_NETWORK,
		TYPE_MACHINE_MOUSE,
		TYPE_MACHINE_KEYBOARD,
		TYPE_MACHINE_CDROM,
		TYPE_MACHINE_FLOPPY,
		TYPE_MACHINE_DISPLAY,
		TYPE_MACHINE_PARALLEL,
		TYPE_MACHINE_TABLET,
		TYPE_MACHINE_NETWORK_IFACE,
	};
public:
	int						type;
	TreeItem*				parent;
	QVector<TreeItem*>		children;
	QHash<QString, QString>	values;
	bool					enabled;
public:
	TreeItem();
	TreeItem(TreeItem* parent, int type);
	~TreeItem();
public:
	QIcon getIcon() const;
	QString getLabel() const;

	QString getValue(const QString& key)
	{
		return values[key];
	}
	void setValue(const QString& key, const QString& value)
	{
		values[key] = value;
	}

	bool isRoot() const { return type == TYPE_ROOT; }
	bool isMachine() const { return type == TYPE_MACHINE; }

	bool isEnabled()
	{
		if(type == TYPE_MACHINE)
			return enabled;
		if(!parent->isEnabled())
			return false;
		return enabled;
	}
};

#endif // TREEITEM_H
