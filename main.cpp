#include "formattransdlg.h"
#include <QtGui/QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));
	FormatTransDlg w;
	w.show();
	return a.exec();
}
