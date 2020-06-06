#include <String_calc.h>


//Шаблоны для замены минуса формата "(-n)" и "-n"
QString AE_CalcString::ptrnDecr1 = "([(][-][0-9.]+[)])";
QString AE_CalcString::ptrnDecr2 = "([\\/\\*\\-\\+][\\-][0-9.]+)";
//Шаблон замены скобок
QString AE_CalcString::ptrnPt0 = "([(][0-9\\/\\*\\-\\+\\±.]+[)])";
//Шаблоны приоритетов операций. 1 для "*" и "/" 2 для "+" и "-"
QString AE_CalcString::ptrnPt1 = "([±]*)([0-9.]+)([\\/]|[\\*])([±]*)([0-9.]+)";
QString AE_CalcString::ptrnPt2 = "([±]*)([0-9.]+)([\\-]|[\\+])([±]*)([0-9.]+)";
//Шаблон выбора символа операции
QString AE_CalcString::ptrnSymbol = "[\\/]|[\\*]|[\\-]|[\\+]";

AE_CalcString::AE_CalcString(QObject *parent)
    : QObject(parent)
{
	rounding = 0;
	endSignal = false;
    connect(this, SIGNAL(parseSt()), this, SLOT(parse_start()));
}

QString AE_CalcString::getString(QString expression) {
    notacing = expression;
    parse();
	return answer;
}

void AE_CalcString::getString(QString expression, bool rtrn) {
    notacing = expression;
    endSignal = rtrn;
    emit parseSt();
}
	//Генеральная функция обработки
void AE_CalcString::parse() {
	QString str = notacing; //Копируем строку для разбора (оригинал остаётся для дебага или определения последней операции)
	str = parseMinus(str, ptrnDecr1); //Первый отрицательный разбор
	str = parseMinus(str, ptrnDecr2); //Второй отрицательный разбор
	str = parseZeroPrior(str, ptrnPt0); //Поиск и разбор выражения в скобках
	str = parsePrior(str, ptrnPt1); //Поиск и подсчёт первого приоритета
	str = parsePrior(str, ptrnPt2); //Поиск и подсчёт второго приоритета
	//Отсюда и до конца процедура вывода результата
	if (str.contains("±") == true) { 
		str = str.replace(QRegularExpression("±"), "-");
	}	
	str = roundNum(str); //При необходимости округляем по заданому типу (по умолчанию без округления)
	//Возврат расчитаного значения
	if (endSignal == true) {
		emit AE_mathEnd(str);
        endSignal = false;
	} else {
		answer = str;
	}
}
	//Подсчитываем пару значений
QString AE_CalcString::calculate(QStringList str, QString sym) {
	double x;
	double y;
	QString end;
	for (int i = 0; i < str.length(); i++) { //Обратная замена временного знака минус
		if (str[i].contains("±") == true) {
			str[i] = str[i].replace(QRegularExpression("±"), "-");
		}
	}
	x = str[0].toDouble();
	y = str[1].toDouble();
	if(sym.contains("+")) { //Считаем
		x += y;
	} else if(sym.contains("-")) {
		x -= y;
	} else if(sym.contains("*")) {
		x *= y;
	} else if(sym.contains("/")) {
		x /= y;
	}
	if (x < 0) { //Замена на временный знак минус
		end = QString::number(x);
		return end = end.replace(QRegularExpression("-"), "±");
	} else {
		return QString::number(x);
	}
}
    //Конвертация форм минуса в не мешающую форму
QString AE_CalcString::parseMinus(QString str, QString paterns) {
    QRegularExpression p(paterns); //Подготавливаем шаблон поиска пар
	QRegularExpressionMatch m = p.match(str);
	while(m.hasMatch() == true){ //Нашли
		QString newStr = m.captured(0);
		newStr = newStr.replace(QRegularExpression("[(]|[)]"), "");
		newStr = newStr.replace(QRegularExpression("-"), "±");
		str = str.replace(m.capturedStart(0), m.capturedLength(0), newStr);
		m = p.match(str);
	}
	return str;
}
	//Обработка значение в скобках
QString AE_CalcString::parseZeroPrior(QString str, QString paterns) {
    QRegularExpression p(paterns); //Подготавливаем шаблон поиска пар
	QRegularExpressionMatch m = p.match(str);
	while(m.hasMatch() == true ){  //Нашли
		QString newStr;
		newStr = m.captured(0);
		newStr = newStr.replace(QRegularExpression("[(]|[)]"), ""); //Убрали
		newStr = parsePrior(newStr, ptrnPt1); //Проверяем/вычисляем шаблон первого приоритета
		newStr = parsePrior(newStr, ptrnPt2); //Проверяем/вычисляем шаблон второго приоритета
		str = str.replace(m.capturedStart(0), m.capturedLength(0), newStr); //Возвращяем расчитанное значение
		m = p.match(str);
	}
	return str;
}
	//Поиск пар и расчёт
QString AE_CalcString::parsePrior(QString str, QString paterns) {
    QRegularExpression p(paterns); //Подготавливаем шаблон поиска пар
	QRegularExpressionMatch m = p.match(str); 
	while(m.hasMatch() == true ){ //Нашли
		QString newStr;
		newStr = m.captured(0);
		QRegularExpression p2(ptrnSymbol); //Настраиваем поиск точного знака
		QRegularExpressionMatch sym = p2.match(newStr);
        //sym.find();
        newStr = calculate(newStr.split(sym.captured(0)), sym.captured(0)); //Отправляем пару с определенным знаком на подсчёт
		str = str.replace(m.capturedStart(0), m.capturedLength(0), newStr); //Возвращяем значение
		m = p.match(str);
	}           
	return str;
}

QString AE_CalcString::roundNum(QString str) {
	if (rounding > 0) {
		double x = str.toDouble();
		switch (rounding) {
		case 1:
			x = qFloor(x);
		break;
		case 2:
			x = qCeil(x);
		break;
		case 3:
			x = qFloor(x);
		break;
		case 4:
			x = qFloor(x);
		break;
		}
		return QString::number(x);
	}
	return str;
}

void AE_CalcString::parse_start(){
	parse();
}

AE_CalcString::~AE_CalcString()
{
   
}
