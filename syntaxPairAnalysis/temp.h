// S______T_____A___________R       A_________G_______________E__S
// Syntax Texts Analysis of Russian Automated Generator of rulEs Systems

#ifndef STAR_AGES_TEMP_H
#define STAR_AGES_TEMP_H

#define WINDOWS

#include <exception>
#include <iostream>
#include <fstream>
#include <vector>
#include <C:\Program Files\waffles\src\GClasses\GApp.h>
#include <C:\Program Files\waffles\src\GClasses\GError.h>
#include <C:\Program Files\waffles\src\GClasses\GRand.h>
#include <C:\Program Files\waffles\src\GClasses\GDecisionTree.h>
#include <C:\Program Files\waffles\src\GClasses\GActivation.h>
#include <C:\Program Files\RussianGrammaticalDictionary\include\lem\solarix\solarix_grammar_engine.h>
#include <C:\Program Files\RussianGrammaticalDictionary\include\lem\solarix\_sg_api.h>
#include "formula.h"

using namespace GClasses;
using std::cerr;
using std::cout;
using std::vector;

const char done[] = "Успех\n";

class TempEntry {
	int ob, sb, id;
	char * txt;
	static int DTAR, * I2C, * C2I;
public:
	TempEntry(int o, int s, int i, char * t) : ob(o), sb(s), id(i), txt(t) {}
	int O() const {	return ob;	}
	int S() const {	return sb;	}
	int I() const {	return id;	}
	const char * Txt() const {	return txt;	}
	static void Train(GDecisionTree * model, const Array <Sentense> & sa);
	static void Download();
private:
	static int EntryTranslate(FILE * in, wchar_t & c,
		Array <TempEntry> & arr, Array <char> & la);
	static int State2Int(int coord, int state);
	static int Int2State(int coord, int state);
	static int Class2Int(int state);
	static void Translate(HGREN & eng, FILE * in, FILE * out);
};

#endif
