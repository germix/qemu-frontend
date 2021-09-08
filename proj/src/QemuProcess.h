#ifndef QEMUPROCESS_H
#define QEMUPROCESS_H
#include <QObject>

class QProcess;
class TreeItem;
class TreeModel;
class QTreeView;

class QemuProcess : public QObject
{
	Q_OBJECT
	TreeItem*	item;
	TreeModel*	model;
	QProcess*	process;
	QTreeView*	treeView;
public:
	QemuProcess(QTreeView* tvw);
	~QemuProcess();
public:
	void start(TreeModel* mdl, TreeItem* machineItem);
private slots:
	void slotProcess_finished(int exitCode);
	void slotProcess_readyReadStandardError();
	void slotProcess_readyReadStandardOutput();
};

#endif // QEMUPROCESS_H
