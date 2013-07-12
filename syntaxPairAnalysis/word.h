// S______T_____A___________R       A_________G_______________E__S
// Syntax Texts Analysis of Russian Automated Generator of rulEs Systems

#ifndef STAR_AGES_WORD_H
#define STAR_AGES_WORD_H

#include "data.h"

class Intr {
	char * lxm;
	long data;
public:
	Intr() : lxm(0), data(0x00000000) {}
	Intr(char * lexem, long i) : lxm(lexem), data(i) {}
	void Print(FILE * out);
	char * const Lxm() const;
	const long Data() const;
	void SetSP(int i);
	void SetData(int i, int j);
	void SetLxm(char * s);
};

class Word {
	int id;
	Intr prop;
	Array<char> pnct;
	const char * txt;
public:
	static int Space(char c);
	static int Punct(char c);
	static int StrCmp(const char * s1, const char * s2);
	static char * Clone(const char * s);
	int ID() const {	return id;	}
	const char *& Txt();
	Array<char> & Pnct();
	Intr & Prop();
	Word(int i = -1) : id(i), prop(), pnct(), txt(0) {}
	Word(int i, char * lxm, long data, const char * t)
		: id(i), prop(lxm, data), pnct(), txt(t) {}
	void Set(int i) {	id = i;	}
	~Word();
	void Print(FILE * out);
	void PrintIntr(FILE * out);
};

#endif
