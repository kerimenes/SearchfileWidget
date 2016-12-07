#include "serialno.h"
#include "ui_serialno.h"

#include <QDebug>
#include <QProcess>
#include <QSqlQuery>
#include <QFileDialog>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlField>

SerialNo::SerialNo(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::SerialNo)
{
	ui->setupUi(this);
	ui->progressBar->setVisible(true);
}

SerialNo::~SerialNo()
{
	delete ui;
}

void SerialNo::on_browse_clicked()
{
	QString path = QFileDialog::getOpenFileName(this, "Click on *.txt.. ", "/home/kerim/myfs/");
	ui->browseFile->setText(path);

	if ( !path.contains(".txt") )
		return;

	ui->pathOpen->clear();
	QStringList str = fileOpen(path);
	ui->pathOpen->addItems(str);
}

QStringList SerialNo::fileOpen(const QString &path)
{
	QFile fd(path);
	QStringList ls;
	if ( fd.open(QIODevice::ReadOnly | QIODevice::Text) ) {

		QTextStream in(&fd);

		while (!in.atEnd()) {
			ls << in.readLine();
		}
	}
	return ls;
}

void SerialNo::on_browseDir_clicked()
{
	QString path = QFileDialog::getExistingDirectory(this ,"Click on direction", "/home/kerim/myfs/");

	ui->SearchPath->clear();
	ui->SearchPath->setText(path);
	searchingfiledir = listDir(path);
}

void SerialNo::on_startSearch_clicked()
{
	searchInTxt(searchingfiledir);
}

QStringList SerialNo::listDir(const QString &desc)
{
	QProcess p;
	p.start(QString("ls %1").arg(desc));
	p.waitForFinished();

	QString str = p.readAllStandardOutput().data();
	QStringList fld = str.split("\n");

	/* kontrol et */
	for (int i = 0; i < fld.size(); i++) {
		if (fld.at(i).contains("log") == false ) {
			fld.removeAt(i);
		}
	}
	return fld;
}

void SerialNo::searchInTxt(const QStringList ls)
{
	QStringList fileinside;

	startProgress(ls.size());
	for (int i = 0; i < ls.size(); i++) {
		fileinside << fileOpen( QString("%1/%2").arg(ui->SearchPath->text()).arg(ls.at(i)) );
		updateProgress(i, ls.size() - 1);
		for (int k = 0; k < fileinside.size(); k++) {
			for (int j = 0; j < ui->pathOpen->count(); j++) {
				if ( fileinside.at(k).contains(ui->pathOpen->item(j)->text()) == true )
					ui->pathOpen->item(j)->setForeground(Qt::green);
			}
		}
		fileinside.clear();
	}
	qDebug() << "next";
}

void SerialNo::startProgress(int total)
{
	/* total value is ip numbers in encoderlist*/
	ui->progressBar->setValue(0);
	ui->progressBar->setMaximum(total);
	ui->progressBar->setVisible(true);
}

void SerialNo::updateProgress(int progress, int total)
{
	ui->progressBar->setValue(progress);
	if (total != -1)
		ui->progressBar->setMaximum(total);
	ui->progressBar->setVisible(true);
}


QSqlDatabase SerialNo::erpInit(const QString &dbServer, const QString &dbDatabase, const QString &dbUser, const QString &dbPass)
{
	QSqlDatabase db = QSqlDatabase::database();
	if (db.isOpen())
		qDebug() << "0";
	db = QSqlDatabase::addDatabase("QPSQL");
	db.setHostName(dbServer);
	db.setDatabaseName(dbDatabase);
	db.setUserName(dbUser);
	db.setPassword(dbPass);
	if (!db.open())
		qDebug() << -EINVAL;
	return db;

}

const QStringList SerialNo::getEncoderSerialAllList()
{
	QStringList encoderlist;

	QSqlQuery q(QString("SELECT * FROM encoder_test_logging"));
	while(q.next()) {
		encoderlist << q.value("encoder_serial").toString();
	}
	return encoderlist;
}

void SerialNo::databaseClose(QSqlDatabase database)
{
	QString connection;
	connection = database.connectionName();
	database.close();
	database = QSqlDatabase();
	database.removeDatabase(connection);
}

void SerialNo::on_serialOpen_clicked()
{
	QSqlDatabase database = erpInit("maxwell.bilkon.arge", "BilkonErp", "BilkonErpAdmin", "Aibotah8");
	getSerialNo();
}

void SerialNo::getSerialNo()
{
	QString pathexcel = QFileDialog::getOpenFileName(this, "Select Excel file ", "/home/kerim/myfs/");
	ui->serialedit->setText(pathexcel);
	QStringList listexcel = fileOpen(pathexcel);

	startProgress(listexcel.size());
	QSqlQuery q;
	q.prepare(QString("Delete  from new_table"));

	if(!q.exec()) {
		ui->cmdOutput->setText(q.lastError().text());
		qDebug() << q.lastError().text();
	}

	int i = 0;
	foreach (QString serino, listexcel) {
		i++;
		updateProgress(i, listexcel.size());
		createTempCalcTable(serino);
	}
}

const QString SerialNo::createTempCalcTable(QString serino)
{
	QSqlQuery q;
	q.prepare("INSERT INTO new_table( "
			  "serial1) "
			  "VALUES (?);");

	q.addBindValue(serino);
	if (!q.exec()) {
		ui->cmdOutput->setText(q.lastError().text());
		return q.lastError().text();
	}
	return "";
}

void SerialNo::checkSerialNumber()
{
	QSqlQuery q("SELECT serial1 FROM new_table "
				"WHERE NOT EXISTS (SELECT * FROM encoder_test_logging "
				"WHERE encoder_test_logging.encoder_serial = new_table.serial1)");
	startProgress(q.numRowsAffected());
	ui->pathOpen->clear();
	int i = 0;
	while(q.next()) {
		ui->pathOpen->addItem(q.value("serial1").toString());
		i++;
		updateProgress(i, q.numRowsAffected());
	}
}

void SerialNo::on_startSqlSearching_clicked()
{
	QSqlDatabase database = erpInit("maxwell.bilkon.arge", "BilkonErp", "BilkonErpAdmin", "Aibotah8");
	ui->cmdOutput->setText(database.lastError().text());
	checkSerialNumber();
}
