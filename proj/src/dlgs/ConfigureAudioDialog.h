#ifndef CONFIGUREAUDIODIALOG_H
#define CONFIGUREAUDIODIALOG_H

#include <QDialog>

namespace Ui {
class ConfigureAudioDialog;
}
class TreeItem;
class TreeModel;

class ConfigureAudioDialog : public QDialog
{
	Q_OBJECT
	Ui::ConfigureAudioDialog*	ui;
	TreeItem*					item;
	TreeModel*					model;
public:
	explicit ConfigureAudioDialog(TreeItem* item, TreeModel* model, QWidget* parent = 0);
	~ConfigureAudioDialog();
private slots:
	void accept();
};

#endif // CONFIGUREAUDIODIALOG_H
