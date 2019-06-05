#include "formattransdlg.h"

FormatTransDlg::FormatTransDlg(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	connect(ui.btnCalc, SIGNAL(clicked(bool)), this, SLOT(onBtnClicked()));

	ui.cmbDataType->addItem("二字节整形(12)", QVariant("CInt16By12Factory"));
	ui.cmbDataType->addItem("二字节整形(21)", QVariant("CInt16By21Factory"));

	ObjectFactory::registerClass<CInt16By12Factory>();
	ObjectFactory::registerClass<CInt16By21Factory>();
}

void FormatTransDlg::onBtnClicked()
{
	QString factoryStr = ui.cmbDataType->itemData(ui.cmbDataType->currentIndex()).toString();
	IFormatFactory* factory = (IFormatFactory*)ObjectFactory::createObject(factoryStr.toLocal8Bit());
	ITrans	*trans = factory->CreateTrans();
	ui.edtDstData->setPlainText(trans->calc(ui.edtSrcData->toPlainText()));
}

FormatTransDlg::~FormatTransDlg()
{

}

QString ITrans::calc( const QString &str )
{
	key = str;
	if (!TransCheck())
	{
		return "size error";
	}
	return TransCalc();
}

bool ITrans::TransCheck(const QStringList &list)
{
	for (int i=0; i<list.size(); i++)
	{
		if (list[i].toUInt() <= 0xff && list[i].toUInt() >= 0 )
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	return true;
}

QString CInt16By12Trans::TransCalc()
{
	QStringList list = key.split(" ", QString::SkipEmptyParts);
	bool bOk;
	int value = list[0].toInt(&bOk, 16);
	if (!bOk)
	{
		return list[0]+QString(" trans failed");
	}
	value += list[1].toInt(&bOk, 16) << 8;
	if (!bOk)
	{
		return list[1]+QString(" trans failed");
	}
	return QString("%1").arg(value);
}

QString CInt16By21Trans::TransCalc()
{
	QStringList list = key.split(" ", QString::SkipEmptyParts);
	bool bOk;
	int value = list[1].toInt(&bOk, 16);
	if (!bOk)
	{
		return list[1]+QString(" trans failed");
	}
	value += list[0].toInt(&bOk, 16) << 8;
	if (!bOk)
	{
		return list[0]+QString(" trans failed");
	}
	return QString("%1").arg(value);
}

