#ifndef CONFIGURENETWORKINTERFACEDIALOG_H
#define CONFIGURENETWORKINTERFACEDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigureNetworkInterfaceDialog;
}
class TreeItem;
class TreeModel;

class ConfigureNetworkInterfaceDialog : public QDialog
{
	Q_OBJECT
	Ui::ConfigureNetworkInterfaceDialog*	ui;
	TreeItem*								item;
	TreeModel*								model;
public:
	explicit ConfigureNetworkInterfaceDialog(TreeItem* item, TreeModel* model, QWidget* parent = 0);
	~ConfigureNetworkInterfaceDialog();
private:
	QString getName() const;
	QString getModel() const;
	QString getMacAddress() const;
	void checkName(const QString& text);
	void checkMacAddress(const QString& text);
private slots:
	void accept();
	void slotLineEdit_textChanged(const QString& text);
};

#endif // CONFIGURENETWORKINTERFACEDIALOG_H
