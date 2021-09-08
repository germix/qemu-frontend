#ifndef TREEMODEL_H
#define TREEMODEL_H
#include <QAbstractItemModel>

#include "TreeItem.h"

class QXmlStreamReader;
class QXmlStreamWriter;

class TreeModel : public QAbstractItemModel
{
	Q_OBJECT
	TreeItem*	root;
	bool		modified;
	QString		fileName;
public:
	TreeModel();
	~TreeModel();
public:
	void clear();
	void newFile();
	bool openFile(const QString& fileName);
	bool saveFile(const QString& fileName);

	bool isEmpty() const { return root == 0; }
	bool isModified() const { return modified; }
	QString getFileName() const { return fileName; }

	void setModified(bool modified);

	void addItem(TreeItem* parent, int type, const QHash<QString,QString>& values)
	{
		addItem(toIndex(parent), type, values);
	}
	void addItem(const QModelIndex& parentIndex, int type, const QHash<QString,QString>& values);
	void removeIndex(const QModelIndex& index);

	TreeItem* getItemByType(int type, TreeItem* parent = 0);

	void addMachine(const QString& name, const QString& qemu);

	void setItemEnabled(TreeItem* item, bool enabled)
	{
		if(item->enabled != enabled)
		{
			item->enabled = enabled;
			dataChanged(toIndex(item), toIndex(item));
		}
	}
signals:
	void onModified();
public:
	TreeItem* toItem(const QModelIndex& index) const;
	QModelIndex toIndex(TreeItem* item, int column = 0) const;
private:
	virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	virtual int rowCount(const QModelIndex &parent) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual bool hasChildren(const QModelIndex &parent) const;

	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
private:
	TreeItem* readAll(QXmlStreamReader& xml);
	void readWorkspace(QXmlStreamReader& xml, TreeItem* parent);
	void readMachine(QXmlStreamReader& xml, TreeItem* parent);

	void readItemXml(QXmlStreamReader& in, TreeItem* parent, int type);
	void readItemXml(QXmlStreamReader& in, TreeItem* parent, int type, const QString& keys);

	void saveItem(QXmlStreamWriter& out, TreeItem* item);
	void saveItems(QXmlStreamWriter& out, QVector<TreeItem*>& items);
	void saveItemXml(QXmlStreamWriter& out, TreeItem* item, const QString& label);
};

#endif // TREEMODEL_H
