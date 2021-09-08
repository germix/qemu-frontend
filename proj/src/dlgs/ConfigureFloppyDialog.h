#ifndef CONFIGUREFLOPPYDIALOG_H
#define CONFIGUREFLOPPYDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigureFloppyDialog;
}
class TreeItem;
class TreeModel;
class QLineEdit;

class ConfigureFloppyDialog : public QDialog
{
	Q_OBJECT
	Ui::ConfigureFloppyDialog*	ui;
	TreeItem*					item;
	TreeModel*					model;
public:
	explicit ConfigureFloppyDialog(TreeItem* item, TreeModel* model, QWidget* parent = 0);
	~ConfigureFloppyDialog();
private:
	void checkExists();
	void checkExists(QLineEdit* txt);
	void chooseFloppy(QLineEdit* txt);
	void removeFloppy(QLineEdit* txt);
	QString getSourceFda() const;
	QString getSourceFdb() const;
private slots:
	void accept();
	void slotChooseFloppy();
	void slotRemoveFloppy();
};

#endif // CONFIGUREFLOPPYDIALOG_H
