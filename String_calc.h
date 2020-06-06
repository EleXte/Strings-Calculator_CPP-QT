#pragma once

#include <QObject>
#if QT_VERSION > QT_VERSION_CHECK(5, 0, 0)
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QtMath>
#else
#include <qregexp.h>
#include <qmath.h>
#endif

class AE_CalcString : public QObject
{
	Q_OBJECT

	public:
	AE_CalcString(QObject *parent = 0);
    ~AE_CalcString();
	//Функции запускающие расчёт
    QString getString(QString expression);
    void getString(QString expression, bool rtrn);
	
	//Базовые переменные
	QString answer; //Переменная с ответом на заданую строку или сообщение об ошибке
	int rounding; //Округление ответа (0 - никакого, 1 - вниз, 2 - вверх)
	
	private:
	//Тоже базовые, но не доступные для модификации извне
	QString notacing; //Переменная введенной нотации
	bool endSignal; //Переменная отправки сигнала об окончании подсчёта
	
    signals:
        void AE_mathEnd(QString answer);
		void parseSt();

	private slots:
		void parse_start();
	private:
        QString calculate(QStringList str, QString sym);
		void parse();
        QString parseMinus(QString str, QString paterns) ;
        QString parseZeroPrior(QString str, QString paterns);
        QString parsePrior(QString str, QString paterns);
		
		QString roundNum(QString str); //Округление ответа
	
        //Шаблоны для замены минуса формата "(-n)" и "-n"
        static QString ptrnDecr1;
        static QString ptrnDecr2;
        //Шаблон замены скобок
        static QString ptrnPt0;
        //Шаблоны приоритетов операций. 1 для "*" и "/" 2 для "+" и "-"
        static QString ptrnPt1;
        static QString ptrnPt2;
        //Шаблон выбора символа операции
        static QString ptrnSymbol;

    //boolean start; //Переменная старта обработки для правильного ввода
};
