#ifndef CONFIGURECDROMDIALOG_H
#define CONFIGURECDROMDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigureCdromDialog;
}
class TreeItem;
class TreeModel;

class ConfigureCdromDialog : public QDialog
{
	Q_OBJECT
	Ui::ConfigureCdromDialog*	ui;
	TreeItem*					item;
	TreeModel*					model;
public:
	explicit ConfigureCdromDialog(TreeItem* item, TreeModel* model, QWidget* parent = 0);
	~ConfigureCdromDialog();
private:
	void checkExists();
	QString getSource() const;
private slots:
	void accept();
	void slotChooseCdrom();
	void slotRemoveCdrom();
};

#endif // CONFIGURECDROMDIALOG_H
