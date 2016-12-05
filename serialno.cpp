#include "serialno.h"
#include "ui_serialno.h"

#include <QFileDialog>
#include <QDebug>
#include <QProcess>

SerialNo::SerialNo(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::SerialNo)
{
	ui->setupUi(this);
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



}
