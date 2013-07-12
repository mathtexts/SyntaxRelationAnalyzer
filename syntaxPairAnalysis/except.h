// S______T_____A___________R       A_________G_______________E__S
// Syntax Texts Analysis of Russian Automated Generator of rulEs Systems

#ifndef STAR_AGES_EXCEPT_H
#define STAR_AGES_EXCEPT_H

#include <stdio.h>

const char * const msg[] = {
	"Тип иcключения Exception распознан как WithChar!\n",
	"Тип иcключения Exception распознан как WithShort!\n",
	"Тип иcключения Exception распознан как WithStr!\n",
	"Отрицательная дизъюнкция расположена не в первом\n\
поддереве дерева, визуализирующего дизънкцию, а в %i-м!\n",
	"Тип иcключения Exception распознан как With3Str!\n",
	"Тип иcключения WithChar распознан как WithShort!\n",
	"Тип иcключения WithChar распознан как WithStr!\n",
	"Тип иcключения WithChar распознан как With2Str!\n",
	"Тип иcключения WithChar распознан как With3Str!\n",
	"Обращение к массиву с индексом %i, лежащим вне его границ!\n",
	"Тип иcключения WithShort распознан как WithChar!\n",
	"Не удаётся открыть файл \"%s\"!\n",
	"Тип иcключения WithShort распознан как WithStr!\n",
	"Тип иcключения WithShort распознан как With2Str!\n",
	"Тип иcключения WithShort распознан как With3Str!\n",
	"Доступ к правилу после его завершения запрещён!\n\
Перезагрузите параметры правила.\n",
	"Это сообщение об ошибке не должно было быть выдано!\n\
Проблема с перехватом строки\"%s\", не являющейся связкой.\n",
	"Нераспознаваемая строка в файле данных: \"%s\". Не найдено \"%s\".\n",
	"Неопознанная часть речи в строке: \"%s\".\n",
	"Неопознанный тип связки в строке: \"%s\".\n",
	"Ошибка во входном файле! Не найдено \"%s\".\n",
	"Заголовок \"%s\" не соответствует ожидаемому предложению с номером %i!\n",
	"Неожиданный символ '%c' в строке \"%s\".\n",
	"Неизвестная команда \"%s\". Попробуйте \"help\".\n",
	"\"exit\" - выйти из программы.\n\
\"help\" - вывести это сообщение.\n\
\"stat {-csv} <filename>\" - вывести статистику в файл;\n\
	если указано -csv, выводить в формате csv.\n\
\"top {-x} n <filename>\" - только лучшие n признаков для каждого шаблона;\n\
	если указано -x, вывести дополнительную информацию по выбору признаков.\n\
\"tree <filename>?<filename>\" - вывести решающее дерево в файл;\n\
\"frml {-x n} p1 p2 <filename>\" - вывести булевы формулы для всех правил в файл;\
	если указано -x, выводить n примеров применения правил;\n\
	p1 = -no, чтобы отключить проверку полноты, или\n\
		константа, ограничивающая полноту формулы;\n\
	p2 = -no, чтобы отключить проверку точности, или\n\
		константа, ограничивающая точность формулы.\n\
\"text <filename>\" - вывести текст в файл.\n\
\"cat <filename>\" - вывести лексическую информацию в файл.\n\
\"entry {-csv} <filename>\" - вывести в файл все пары слов, входящие в связи;\n\
	если указано -csv, выводить в формате csv.\n\
\"ngtv {-csv} <filename>\" - вывести в файл пары слов, не входящих в связи;\n\
	если указано -csv, выводить в формате csv.\n\
\"load <filename>\" - загрузить файл.\n\
\"data <filename>{?<filename>}\" - загрузить собранную статистику из файла;\n\
	если указан дополнительный файл, загрузить из него вхождения правил.\n\
\"reset\" - очистить память.\n",
	"Неопознанный параметр слова в строке \"%s\".\n",
	"Нужны параметры: число выводимых признаков и имя файла вывода.\n",
	"Файл повреждён: нераспознаваемый символ \'%c\'.\n",
	"Файл повреждён: не найдено \"%s\".\n",
	"Файл повреждён: не найдено окончание заголовка.\n",
	"Файл повреждён: не найдена закрывающая кавычка.\n",
	"Найден объект класса Entry с одинаковыми номерами объекта и субъекта %i!\n",
	"Попытка удалить элемент из пустого массива!\n",
	"Тип иcключения WithStr распознан как WithChar!\n",
	"Тип иcключения WithStr распознан как WithShort!\n",
	"Тип иcключения WithStr распознан как With2Str!\n",
	"Тип иcключения WithStr распознан как With3Str!\n",
	"Тип иcключения With2Str распознан как WithChar!\n",
	"Тип иcключения With2Str распознан как WithShort!\n",
	"Тип иcключения With2Str распознан как With3Str!\n",
	"Тип иcключения With3Str распознан как WithChar!\n",
	"Тип иcключения With3Str распознан как WithShort!\n",
	"Это сообщение об ошибке не должно было быть выдано!\n\
Проблема с перехватом сообщения об отмене открытия файла.\n",
	"Тип иcключения Exception распознан как With2Str!\n",
	"Это сообщение об ошибке не должно было быть выдано!\n\
Проблема с перехватом сообщения об отмене редактирования формулы.\n",
	"Потеряно название правила №%i в файле правил!\n",
	"Нераспознанное расстояние в строке %s!\n",
	"Неопознанная часть речи объекта в строке: \"%s\".\n",
	"Неопознанная часть речи субъекта в строке: \"%s\".\n",
	"Неопознанное отношение частей речи в строке: \"%s\".\n",
	"Неопознанное значение параметра \"%s\" объекта в строке \"%s\".\n",
	"Неопознанное значение параметра \"%s\" субъекта в строке \"%s\".\n",
	"Неопознанное отношение параметра \"%s\" слов в строке \"%s\".\n",
	"Неопознанное слово с параметром \"%s\" в строке \"%s\".\n",
	"Слишком длинная строка \"%s\" в файле данных!\n",
	"Неподходящее количество параметров %i.\n",
	"Неожиданный конец файла на строке \"%s\"!\n",
	"Невозможно открыть файл данных data.txt для чтения!\n",
	"Знак пунктуации внутри слова %s!\n",
	"Более одного графа морфологии в предложении %s!\n",
	"Более одной вершины в связной области %i в предложении %s!\n",
	"Более одного варианта разбора для слова %i в предложении %s!\n",
	"Разобрано %i слов в предложении %s!\n",
	"Неопознанная часть слова %i!\n",
	"В командной строке \"%s\" не хватает второго параметра - имени файла вывода!",
	"Проблема с переводом строки \"%s\" в кодировку Windows-1251.\n",
	"Проблема с переводом строки \"%s\" из кодировки Windows-1251.\n"
};

class Exception {
protected:
	int id;
public:
	Exception(int s) : id(s) {}
	virtual ~Exception() {}
	virtual int Print(FILE * out);
	const int ID() const {	return id;	}
	virtual const char Char1() const {	throw new Exception(0);	}
	virtual const int Num1() const {	throw new Exception(1);	}
	virtual const char * const Data() const {	throw new Exception(2);	}
	virtual const char * const Data2() const {	throw new Exception(43);	}
	virtual const char * const Data3() const {	throw new Exception(4);	}
};

class WithChar : public virtual Exception {
protected:
	char char1;
public:
	WithChar(int s, char c1) : Exception(s), char1(c1) {}
	~WithChar() {}
	int Print(FILE * out);
	const char Char1() const {	return char1;	}
	const int Num1() const {	throw new Exception(5);	}
	const char * const Data() const {	throw new Exception(6);	}
	const char * const Data2() const {	throw new Exception(7);	}
	const char * const Data3() const {	throw new Exception(8);	}
};

class WithShort : public virtual Exception {
protected:
	int num1;
public:
	WithShort(int s, int n1) : Exception(s), num1(n1) {}
	~WithShort() {}
	int Print(FILE * out);
	const char Char1() const {	throw new Exception(10);	}
	const int Num1() const {	return num1;	}
	const char * const Data() const {	throw new Exception(12);	}
	const char * const Data2() const {	throw new Exception(13);	}
	const char * const Data3() const {	throw new Exception(14);	}
};

class WithStr : public virtual Exception {
	int OTT1;
protected:
	const char * data1;
public:
	WithStr(int s, const char * s1, int o1 = 1)
		: Exception(s), OTT1(o1), data1(s1) {}
	~WithStr();
	int Print(FILE * out);
	const char Char1() const {	throw new Exception(33);	}
	const int Num1() const {	throw new Exception(34);	}
	const char * const Data() const {	return data1;	}
	const char * const Data2() const {	throw new Exception(35);	}
	const char * const Data3() const {	throw new Exception(36);	}
};

class WithCharStr : public WithChar, WithStr {
public:
	WithCharStr(int s, char c1, const char * s1, int o1 = 1)
		: Exception(s), WithChar(s, c1), WithStr(s, s1, o1) {}
	~WithCharStr() {}
	int Print(FILE * out);
	const char Char1() const {	return ((WithChar *)this)->Char1();	}
	const int Num1() const {	throw new Exception(5);	}
	const char * const Data() const {	return ((WithStr *)this)->Data();	}
	const char * const Data2() const {	throw new Exception(7);	}
	const char * const Data3() const {	throw new Exception(8);	}
};

class WithStrShort : public WithStr, WithShort {
public:
	WithStrShort(int s, int n1, const char * s1, int o1 = 1)
		: Exception(s), WithStr(s, s1, o1), WithShort(s, n1) {}
	~WithStrShort() {}
	int Print(FILE * out);
	const char Char1() const {	throw new Exception(10);	}
	const int Num1() const {	return ((WithShort *)this)->Num1();	}
	const char * const Data() const {	return ((WithStr *)this)->Data();	}
	const char * const Data2() const {	throw new Exception(13);	}
	const char * const Data3() const {	throw new Exception(14);	}
};

class With2Str : public WithStr {
	int OTT2;
protected:
	const char * data2;
public:
	With2Str(int s, const char * s1, const char * s2,
		int o1 = 1, int o2 = 1) : Exception(s),
		WithStr(s, s1, o1), OTT2(o2), data2(s2) {}
	~With2Str();
	int Print(FILE * out);
	const char Char1() const {	throw new Exception(37);	}
	const int Num1() const {	throw new Exception(38);	}
	const char * const Data2() const {	return data2;	}
	const char * const Data3() const {	throw new Exception(39);	}
};

class With3Str : public With2Str {
	int OTT3;
protected:
	const char * data3;
public:
	With3Str(int s, const char * s1, const char * s2, const char * s3,
		int o1 = 1, int o2 = 1, int o3 = 1) : Exception(s),
		With2Str(s, s1, s2, o1, o2), OTT3(o3), data3(s3) {}
	~With3Str();
	int Print(FILE * out);
	const char Char1() const {	throw new Exception(40);	}
	const int Num1() const {	throw new Exception(41);	}
	const char * const Data3() const {	return data3;	}
};

#endif
