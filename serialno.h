#ifndef SERIALNO_H
#define SERIALNO_H

#include <QMainWindow>

namespace Ui {
class SerialNo;
}

class SerialNo : public QMainWindow
{
	Q_OBJECT

public:
	explicit SerialNo(QWidget *parent = 0);
	~SerialNo();

	QStringList fileOpen(const QString &path);
	QStringList listDir(const QString &desc);
	void searchInTxt(const QStringList ls);
private slots:
	void on_browse_clicked();

	void on_browseDir_clicked();

	void on_startSearch_clicked();

private:
	Ui::SerialNo *ui;
	QStringList searchingfiledir;
};

#endif // SERIALNO_H
