#ifndef SERIALNO_H
#define SERIALNO_H

#include <QMainWindow>
#include <QSqlDatabase>

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
	QSqlDatabase erpInit(const QString &dbServer, const QString &dbDatabase, const QString &dbUser, const QString &dbPass);
	const QStringList getEncoderSerialAllList();
	const QString createTempCalcTable(QString serino);
	const QString copyTableToTable(const QString columnname, const QString oldtablename, const QString newtablename);
	void checkSerialNumber();
	void getSerialNo();
	void cmdOutputEditor(const QString cmdoutput);
protected:
	void startProgress(int total);
	void updateProgress(int progress, int total);
	void databaseClose(QSqlDatabase database);
private slots:
	void on_browse_clicked();

	void on_browseDir_clicked();

	void on_startSearch_clicked();

	void on_serialOpen_clicked();

	void on_startSqlSearching_clicked();

private:
	Ui::SerialNo *ui;
	QStringList searchingfiledir;
};

#endif // SERIALNO_H
