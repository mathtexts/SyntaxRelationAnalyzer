#include "word.h"

void Intr::Print(FILE * out) {
	int i, j, k;
	fprintf(out, "\tЋексема \"%s\"; ", lxm);
	fprintf(out, "часть речи %s", Data::PSSP()[k = (data & Data::PSMA()[0]) / Data::PSMA()[1]]);
	for (i = 0; i < Data::DTAR(); ++i) if (Data::DTIA()[k][i]) {
		j = (data & Data::TDMA()[i][0]) / Data::TDMA()[i][1];
		if (Data::TDSA()[i][j][0] != '$') fprintf(out, "; %s %s", Data::DTSA()[i], Data::TDSA()[i][j]);
	}
	fprintf(out, ".\n");
}

char * const Intr::Lxm() const { return lxm; }

const long Intr::Data() const { return data; }

void Intr::SetSP(int i) { (data &= ~Data::PSMA()[0]) |= i*Data::PSMA()[1]; }

void Intr::SetData(int i, int j) {
	(data &= ~Data::TDMA()[i][0]) |= j*Data::TDMA()[i][1];
	if ((i == 7) && (j == 2)) (data &= ~Data::PSMA()[0]) |= 10*Data::PSMA()[1];
	else if ((i == 7) && (j == 3)) (data &= ~Data::PSMA()[0]) |= 13*Data::PSMA()[1];
}

void Intr::SetLxm(char * s) { lxm = s; }

int Word::Space(char c) {
	return (c == ' ') || (c == '\t') || (c == '\v')
		|| (c == '\n') || (c == '\r') || (c == '\f');
}

int Word::Punct(char c) {
	return (c == '.') || (c == ',') || (c == '\'') || (c == '"') || (c == ';')
		|| (c == ':') || (c == '(') || (c == ')') || (c == '?') || (c == '!')
		|| (c == '`') || (c == '-') || (c == '_') || (c == '[') || (c == ']')
		|| (c == '{') || (c == '}') || (c == 'Ч');
}

int Word::StrCmp(const char * s1, const char * s2) {
	int i = 0;
	while ((s1[i] != 0) && ((s1[i] == s2[i]) ||
		(s1[i] == 'е') && (s2[i] == 'Є') ||
		(s1[i] == 'Є') && (s2[i] == 'е'))) ++i;
	return (s1[i] == 0) && (s2[i] == 0);
}

char * Word::Clone(const char * s) {
	int i = 0;
	char * res;
	if (!s) return 0;
	while (s[i] != 0) ++i;
	res = new char[i + 1];
	while (i >= 0) {
		res[i] = s[i];
		--i;
	}
	return res;
}

const char *& Word::Txt() { return txt; }

Array<char> & Word::Pnct() { return pnct; }

Intr & Word::Prop() { return prop; }

Word::~Word() {
	delete txt;
}

void Word::Print(FILE * out) {
	int i;
	if (txt) fprintf(out, txt);
	else fprintf(out, "<FANTOM>");
	for (i = 0; i <= pnct.e(); ++i) fprintf(out, "%c", *(pnct[i]));
}

void Word::PrintIntr(FILE * out) {
	prop.Print(out);
}
