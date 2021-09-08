#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class TreeModel;
class QSettings;

#include "Languages.h"
#include "RecentFilesMenu.h"
#include "QemuProcess.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
	Ui::MainWindow*		ui;
	TreeModel*			model;
	QString				lastDirectory;

	Languages			languages;

	RecentFilesMenu*	recentFiles;

	QemuProcess*		qemuProcess;
public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();
private:
	void newFile();
	void openFile(const QString& fileName);
	bool saveFile(bool saveAs = false);
	bool closeFile();
	void updateTitle();
	void updateActions();

	void initLanguages(QSettings& s);
	void initRecentFilesMenu(const QByteArray& state);
private:
	void closeEvent(QCloseEvent* e);
	void changeEvent(QEvent* e);
	void dropEvent(QDropEvent* e);
	void dragEnterEvent(QDragEnterEvent* e);
private slots:
	void slotAction();
	void slotRecentFiles_fileTriggered(const QString& fileName);
	void slotTreeView_customContextMenuRequested(const QPoint& pos);
	void slotTreeModel_modified();
};

#endif // MAINWINDOW_H
