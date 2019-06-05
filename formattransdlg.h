#ifndef FORMATTRANSDLG_H
#define FORMATTRANSDLG_H

#include <QObject>
#include <QtGui/QDialog>
#include "ui_formattransdlg.h"

// 反射工厂,使用反射工厂注意事项：
// 需要先注册ObjectFactory::registerClass<MyObject>()，MyObject需要继承QObject并添加Q_OBJECT
// 反射构建对象MyObject* object = (MyObject*)ObjectFactory::createObject( "MyObject" );
#include <QByteArray>
#include <QMetaObject>
#include <QHash>

class ObjectFactory
{
public:
	template<typename T>
	static void registerClass()
	{
		constructors().insert( T::staticMetaObject.className(), &constructorHelper<T> );
	}

	static QObject* createObject( const QByteArray& className, QObject* parent = NULL )
	{
		Constructor constructor = constructors().value( className );
		if ( constructor == NULL )
			return NULL;
		return (*constructor)( parent );
	}

private:
	typedef QObject* (*Constructor)( QObject* parent );

	template<typename T>
	static QObject* constructorHelper( QObject* parent )
	{
		return new T( parent );
	}

	static QHash<QByteArray, Constructor>& constructors()
	{
		static QHash<QByteArray, Constructor> instance;
		return instance;
	}
};

// 工厂方法：格式化转换对象放在子工厂中创建
// 格式化转换接口
class ITrans
{
public:
	QString calc(const QString &str);
	virtual QString TransCalc()=0;
	virtual bool TransCheck() = 0;
	bool TransCheck(const QStringList &list);

public:
	QString key;
};

class CInt16Trans :public ITrans
{
public:
	bool TransCheck()
	{
		QStringList list = key.split(" ", QString::SkipEmptyParts);
		if (list.size() != 2)
		{
			return false;
		}
		return ITrans::TransCheck(list);
	}
	virtual QString TransCalc()
	{
		return "";
	}
};

// 二字节整数(低前高后)
class CInt16By12Trans : public CInt16Trans
{
public:
	QString TransCalc();
};

// 二字节整数(高前低后)
class CInt16By21Trans : public CInt16Trans
{
public:
	QString TransCalc();
};

// 格式化工厂接口
class IFormatFactory : public QObject
{
	Q_OBJECT
public:
	IFormatFactory(QObject *parent=0):QObject(parent){}
	virtual ITrans* CreateTrans()=0;
};


class CInt16By12Factory : public IFormatFactory
{
	Q_OBJECT
public:
	CInt16By12Factory(QObject *parent):IFormatFactory(parent){}
	ITrans* CreateTrans()
	{
		return new CInt16By12Trans();
	}
};

class CInt16By21Factory : public IFormatFactory
{
	Q_OBJECT
public:
	CInt16By21Factory(QObject *parent):IFormatFactory(parent){}
	ITrans* CreateTrans()
	{
		return new CInt16By21Trans();
	}
};


class FormatTransDlg : public QDialog
{
	Q_OBJECT

public:
	FormatTransDlg(QWidget *parent = 0, Qt::WFlags flags = 0);
	~FormatTransDlg();

private slots:
	void onBtnClicked();

private:
	Ui::FormatTransDlgClass ui;
};

#endif // FORMATTRANSDLG_H
