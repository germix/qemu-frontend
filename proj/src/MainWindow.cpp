#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "AboutDialog.h"
#include "TreeModel.h"

#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QMimeData>
#include <QDropEvent>
#include <QDragEnterEvent>

void ShowInContainerFolder(const QString& fileName);

#define APP_TITLE "Qemu Frontend"
#define SETTINGS_APPLICATION "QemuFrontend"
#define SETTINGS_ORGANIZATION "Germix"

#include "dlgs/AddMachineDialog.h"

#include "dlgs/ConfigureMachineDialog.h"
#include "dlgs/ConfigureCdromDialog.h"
#include "dlgs/ConfigureFloppyDialog.h"
#include "dlgs/ConfigureParallelDialog.h"

#include "dlgs/ConfigureAudioDialog.h"
#include "dlgs/ConfigureDisplayDialog.h"
#include "dlgs/ConfigureMouseDialog.h"
#include "dlgs/ConfigureTabletDialog.h"
#include "dlgs/ConfigureKeyboardDialog.h"
#include "dlgs/ConfigureMemoryDialog.h"
#include "dlgs/ConfigureNetworkInterfaceDialog.h"


MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);

	initLanguages(s);
	initRecentFilesMenu(s.value("RecentFiles").toByteArray());

	connect(ui->actionFileNew, SIGNAL(triggered()), this, SLOT(slotAction()));
	connect(ui->actionFileOpen, SIGNAL(triggered()), this, SLOT(slotAction()));
	connect(ui->actionFileSave, SIGNAL(triggered()), this, SLOT(slotAction()));
	connect(ui->actionFileSaveAs, SIGNAL(triggered()), this, SLOT(slotAction()));
	connect(ui->actionFileClose, SIGNAL(triggered()), this, SLOT(slotAction()));
	connect(ui->actionFileOpenContainerFolder, SIGNAL(triggered()), this, SLOT(slotAction()));
	connect(ui->actionFileExit, SIGNAL(triggered()), this, SLOT(slotAction()));
	connect(ui->actionHelpAbout, SIGNAL(triggered()), this, SLOT(slotAction()));

	model = new TreeModel();
	connect(model, SIGNAL(onModified()), this, SLOT(slotTreeModel_modified()));
	ui->treeView->setModel(model);
	connect(ui->treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotTreeView_customContextMenuRequested(QPoint)));

	// Process
	qemuProcess = new QemuProcess(ui->treeView);

	//
	// Restaurar estado anterior
	//
	restoreGeometry(s.value("WindowGeometry").toByteArray());
	restoreState(s.value("WindowState").toByteArray());

	//
	// Cargar el primer archivo pasado como argumento
	//
	QStringList args = qApp->arguments();
	if(args.size() > 1)
	{
		openFile(args.at(1));
	}
}

MainWindow::~MainWindow()
{
	// ...
	QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);
	s.setValue("Language", languages.language());
	s.setValue("WindowState", saveState());
	s.setValue("WindowGeometry", saveGeometry());
	s.setValue("RecentFiles", recentFiles->saveState());
	// ...
	delete qemuProcess;
	// ...
	delete ui;
}
void MainWindow::newFile()
{
	if(closeFile())
	{
		model->newFile();
		ui->treeView->setEnabled(true);
	}
	updateActions();
}
void MainWindow::openFile(const QString& fileName)
{
	if(closeFile())
	{
		ui->treeView->setEnabled(false);
		if(model->openFile(fileName))
		{
			ui->treeView->setEnabled(true);
		}
		recentFiles->removeFile(fileName);
	}
	updateActions();
}
bool MainWindow::saveFile(bool saveAs)
{
	QString fileName = model->getFileName();

	if(fileName.isEmpty() || saveAs)
	{
		fileName = QFileDialog::getSaveFileName(
									this,
									tr("Save project"),
									lastDirectory,
									tr("Project file (%1)").arg("*.xqemu"));

		// Si est� vac�o, se cancel�
		if(fileName.isEmpty())
		{
			return false;
		}
	}

	// Guardar
	if(model->saveFile(fileName))
	{
		lastDirectory = QFileInfo(fileName).absolutePath();
		return true;
	}

	// Error
	QMessageBox::information(this, tr("Error"), tr("Cant't save file"), QMessageBox::Information);

	return false;
}
bool MainWindow::closeFile()
{
	if(model->isModified())
	{
		QFileInfo fi(model->getFileName());
		QString fileName = fi.fileName();

		if(fileName.isEmpty())
		{
			fileName = tr("Untitled");
		}
		switch(QMessageBox::question(this,
									 tr("Save"),
									 tr("Save file \"%1\"?").arg(fileName),
									 QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel))
		{
			case QMessageBox::Yes:
				if(!saveFile())
				{
					return false;
				}
				break;
			case QMessageBox::Cancel:
				return false;
			default:
			case QMessageBox::No:
				break;
		}
	}
	if(!model->getFileName().isEmpty())
	{
		recentFiles->addFile(model->getFileName());
	}
	model->clear();

	return true;
}
void MainWindow::updateTitle()
{
	QString s = APP_TITLE;

	if(!model->isEmpty())
	{
		s += " - ";
		if(model->getFileName().isEmpty())
			s += tr("Untitled");
		else
			s += QFileInfo(model->getFileName()).fileName();

		if(model->isModified())
		{
			s += " *";
		}
	}
	if(model->isModified())
		ui->actionFileSave->setEnabled(true);
	else
		ui->actionFileSave->setEnabled(false);
	// ...
	setWindowTitle(s);
}
void MainWindow::updateActions()
{
	bool hasFilename = !model->getFileName().isEmpty();

	ui->actionFileSave->setEnabled(model->isModified());
	ui->actionFileSaveAs->setEnabled(!model->isEmpty());
	ui->actionFileClose->setEnabled(!model->isEmpty());
	ui->actionFileOpenContainerFolder->setEnabled(hasFilename);
}
void MainWindow::initLanguages(QSettings& s)
{
	languages.init(ui->menu_Languages, "translations", "qmenufronted", s.value("Language").toString());
}
void MainWindow::initRecentFilesMenu(const QByteArray& state)
{
	recentFiles = new RecentFilesMenu(tr("Recent files"), QIcon(":/images/action-clear.png"));

	ui->menu_File->insertMenu(ui->actionFileExit, recentFiles);
	ui->menu_File->insertSeparator(ui->actionFileExit);

	connect(recentFiles, SIGNAL(onFileTriggered(QString)), this, SLOT(slotRecentFiles_fileTriggered(QString)));

	recentFiles->restoreState(state);
}
void MainWindow::closeEvent(QCloseEvent* e)
{
	if(closeFile())
		e->accept();
	else
		e->ignore();
}
void MainWindow::changeEvent(QEvent* e)
{
	if(e != NULL)
	{
		switch(e->type())
		{
#if 1
			case QEvent::LocaleChange:
				{
					QString locale = QLocale::system().name();
					locale.truncate(locale.lastIndexOf('_'));
					languages.load(locale);
				}
				break;
#endif
			case QEvent::LanguageChange:
				ui->retranslateUi(this);
				updateTitle();
				recentFiles->rebuild(tr("Recent files"));
				break;
			default:
				break;
		}
	}
	QMainWindow::changeEvent(e);
}
void MainWindow::dropEvent(QDropEvent* e)
{
	QString s;
	const QList<QUrl> urls = e->mimeData()->urls();

	if(urls.size() == 1)
	{
		QString s = urls.at(0).toLocalFile();
		if(!s.isEmpty())
		{
			openFile(s);
		}
	}
	else
	{
		QMessageBox::information(
				this,
				"",
				tr("So many files"),
				QMessageBox::Ok);
	}
	// ...
	e->acceptProposedAction();
}
void MainWindow::dragEnterEvent(QDragEnterEvent* e)
{
	if(e->mimeData()->hasFormat("text/uri-list"))
	{
		e->acceptProposedAction();
	}
}
void MainWindow::slotAction()
{
	QAction* action = qobject_cast<QAction*>(sender());

	if(action == ui->actionFileNew)
	{
		newFile();
	}
	else if(action == ui->actionFileOpen)
	{
		QString fileName = QFileDialog::getOpenFileName(
									this,
									tr("Load project"),
									lastDirectory,
									tr("Project file (%1)").arg("*.xqemu"));
		if(!fileName.isEmpty())
		{
			openFile(fileName);
			// ...
			lastDirectory = QFileInfo(fileName).absolutePath();
		}
	}
	else if(action == ui->actionFileSave)
	{
		saveFile();
	}
	else if(action == ui->actionFileSaveAs)
	{
		saveFile(true);
	}
	else if(action == ui->actionFileClose)
	{
		if(closeFile())
		{
			updateActions();
			ui->treeView->setEnabled(false);
		}
	}
	else if(action == ui->actionFileOpenContainerFolder)
	{
		ShowInContainerFolder(model->getFileName());
	}
	else if(action == ui->actionFileExit)
	{
		close();
	}
	else if(action == ui->actionHelpAbout)
	{
		AboutDialog().exec();
	}
}
void MainWindow::slotRecentFiles_fileTriggered(const QString& fileName)
{
	openFile(fileName);
}
void MainWindow::slotTreeView_customContextMenuRequested(const QPoint& pos)
{
	QMenu menu;
	QAction* action;
	QModelIndex index = ui->treeView->indexAt(pos);
	if(!index.isValid())
	{
		menu.addAction(ui->actionAddMachine);
		action = menu.exec(QCursor::pos());
		if(action == ui->actionAddMachine)
		{
			AddMachineDialog(model).exec();
		}
		return;
	}
	TreeItem* item = model->toItem(index);

	switch(item->type)
	{
		case TreeItem::TYPE_MACHINE:
			menu.addAction(ui->actionEmulate);
			menu.addSeparator();
			menu.addAction(ui->actionConfigureMachine);
			menu.addSeparator();
			menu.addAction(ui->actionRemoveMachine);
			break;
		case TreeItem::TYPE_MACHINE_CDROM:		menu.addAction(ui->actionConfigureCdrom);		break;
		case TreeItem::TYPE_MACHINE_FLOPPY:		menu.addAction(ui->actionConfigureFloppy);		break;
		case TreeItem::TYPE_MACHINE_PARALLEL:	menu.addAction(ui->actionConfigureParallel);	break;

		case TreeItem::TYPE_MACHINE_MEMORY:		menu.addAction(ui->actionConfigureMemory);		break;
		case TreeItem::TYPE_MACHINE_AUDIO:		menu.addAction(ui->actionConfigureAudio);		break;
		case TreeItem::TYPE_MACHINE_DISPLAY:	menu.addAction(ui->actionConfigureDisplay);		break;
		case TreeItem::TYPE_MACHINE_MOUSE:		menu.addAction(ui->actionConfigureMouse);		break;
		case TreeItem::TYPE_MACHINE_TABLET:		menu.addAction(ui->actionConfigureTablet);		break;
		case TreeItem::TYPE_MACHINE_KEYBOARD:	menu.addAction(ui->actionConfigureKeyboard);	break;

		case TreeItem::TYPE_MACHINE_NETWORK:
			menu.addAction(ui->actionAddNetworkInterface);
			break;
		case TreeItem::TYPE_MACHINE_NETWORK_IFACE:
			menu.addAction(ui->actionConfigureNetworkInterface);
			menu.addSeparator();
			menu.addAction(ui->actionRemoveNetworkInterface);
			break;
	}
	action = menu.exec(QCursor::pos());
	if(action)
	{
		TreeItem* item = model->toItem(ui->treeView->currentIndex());
		if(action == ui->actionEmulate)
		{
			QString qemu = item->values["qemu"];
			if(qemu.isEmpty())
			{
				QMessageBox::information(this, tr("Error"), tr("The Qemu program is not assigned"), QMessageBox::Ok);
				return;
			}
			if(!QFile(qemu).exists())
			{
				QMessageBox::information(this, tr("Error"), tr("The Qemu program can't be found"), QMessageBox::Ok);
				return;
			}
			qemuProcess->start(model, item);
		}
		else if(action == ui->actionConfigureMachine)			{ ConfigureMachineDialog(item, model).exec();			}
		else if(action == ui->actionConfigureCdrom)				{ ConfigureCdromDialog(item, model).exec();				}
		else if(action == ui->actionConfigureFloppy)			{ ConfigureFloppyDialog(item, model).exec();			}
		else if(action == ui->actionConfigureParallel)			{ ConfigureParallelDialog(item, model).exec();			}

		else if(action == ui->actionConfigureAudio)				{ ConfigureAudioDialog(item, model).exec();				}
		else if(action == ui->actionConfigureDisplay)			{ ConfigureDisplayDialog(item, model).exec();			}
		else if(action == ui->actionConfigureMouse)				{ ConfigureMouseDialog(item, model).exec();				}
		else if(action == ui->actionConfigureTablet)			{ ConfigureTabletDialog(item, model).exec();			}
		else if(action == ui->actionConfigureKeyboard)			{ ConfigureKeyboardDialog(item, model).exec();			}

		else if(action == ui->actionConfigureMemory)			{ ConfigureMemoryDialog(item, model).exec();			}

		else if(action == ui->actionConfigureNetworkInterface)	{ ConfigureNetworkInterfaceDialog(item, model).exec();	}

		else if(action == ui->actionAddNetworkInterface)		{ ConfigureNetworkInterfaceDialog(NULL, model).exec();	}

		else if(action == ui->actionRemoveMachine)
		{
			if(QMessageBox::Yes ==
					QMessageBox::question(this,
					tr("Remove machine"),
					tr("Do you really want to remove the machine \"%1\"?").arg(item->getValue("name")),
					QMessageBox::Yes | QMessageBox::No))
			{
				model->removeIndex(index);
			}
		}
		else if(action == ui->actionRemoveNetworkInterface)
		{
			if(QMessageBox::Yes ==
					QMessageBox::question(this,
					tr("Remove network interface"),
					tr("Do you really want to remove the interface \"%1\"?").arg(item->getValue("name")),
					QMessageBox::Yes | QMessageBox::No))
			{
				model->removeIndex(index);
			}
		}
	}
}
void MainWindow::slotTreeModel_modified()
{
	updateTitle();
}
