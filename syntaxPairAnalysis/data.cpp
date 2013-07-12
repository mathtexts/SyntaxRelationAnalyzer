#include "data.h"

int Data::pspr, * Data::psla, Data::pslm, Data::dtar, * Data::dtsl, Data::dtlm,
	* Data::tdar, ** Data::tdsl, * Data::tdsm, ** Data::tdla, * Data::tdlm,
	Data::slar, Data::sum, * Data::tdas, * Data::tdpa;
char ** Data::pssp, * Data::tdua, * Data::tdxa, ** Data::dtsa,
	** Data::dtma, *** Data::tdsa, *** Data::tdna, ** Data::slsa;
long * Data::psma, ** Data::tdma;
bool ** Data::dtia;

int Data::ReadStr(FILE * in, const char * s, char & c, int & i) {
	while ((c == s[i]) && (s[i] != 0)) {	c = fgetc(in);	++i;	}
	return (s[i] == 0);
}

int Data::Str2Int(FILE * in, char & c) {
	int r = 0;
	while ((c >= '0') && (c <= '9')) { (r *= 10) += c - '0';	 c = fgetc(in); }
	return r;
}

void Data::MakeTDUXA(FILE * in) {
	int i, j, * l, m;
	char ** dtna;
	Array <char> u, x;
	ReadStrArray(in, dtna, dtar, true, l, m);
	u += new char('{');
	for (i = 0; i < dtar; ++i) {
		(u += new char('[')) += new char(' ');
		x += new char(' ');
		for (j = l[i]; j < m; ++j) {
			u += new char(' ');
			x += new char(' ');
		}
		for (j = 0; j < l[i]; ++j) {
			u += new char(dtna[i][j]);
			x += new char(dtna[i][j]);
		}
		((u += new char('E')) += new char('q')) += new char(' ');
		(((x += new char('E')) += new char('q'))
			+= new char(';')) += new char(' ');
		for (j = l[i]; j < m; ++j) {
			u += new char(' ');
			x += new char(' ');
		}
		for (j = 0; j < l[i]; ++j) {
			u += new char(dtna[i][j]);
			x += new char(dtna[i][j]);
		}
		(((u += new char('N')) += new char('E'))
			+= new char(']')) += new char(' ');
		((x += new char('N')) += new char('E')) += new char(';');
	}
	delete --u;
	(((u += new char('}')) += new char('~')) += new char('{')) += new char(0);
	x += new char(0);
	tdua = Array<char>::Arr2Str(u);
	tdxa = Array<char>::Arr2Str(x);
	for (i = 0; i < dtar; ++i) delete [] dtna[i];
	delete [] dtna;
	delete [] l;
}

void Data::ReadBoolMatrix(FILE * in) {
	int i, l;
	char c;
	dtia = new bool *[pspr];
	for (i = 0; i < pspr; ++i) {
		dtia[i] = new bool[dtar];
		for (l = 0; l < dtar; ++l) {
			if ('0' != (c = fgetc(in))) if ('1' == c) dtia[i][l] = true;
			else throw new WithChar(27, c);
			else dtia[i][l] = false;
			if (' ' != (c = fgetc(in)))
				if ((dtar - 1 == l) && ('\n' == c)) break;
			else throw new WithChar(27, c);
		}
	}
}

void Data::MakePSTDMA() {
	int i;
	long l;
	psma = new long[2];
	tdma = new long *[dtar];
	for (i = 0; i < dtar; ++i) tdma[i] = new long[2];
	tdma[dtar - 1][1] = 1;
	for (i = dtar - 1; i > 0; --i) {
		l = 1;
		while (l < tdar[i]) l *= 2;
		tdma[i - 1][1] = l*tdma[i][1];
		tdma[i][0] = (l - 1)*tdma[i][1];
	}
	l = 1;
	while (l < tdar[0]) l *= 2;
	psma[1] = l*tdma[0][1];
	tdma[0][0] = (l - 1)*tdma[0][1];
	l = 1;
	while (l < pspr) l *= 2;
	psma[0] = (l - 1)*psma[1];
}

void Data::ReadStrArray(FILE * in, char **& arr,
	int & n, bool x, int *& len, int & m, bool z)
{
	int l;
	char c, * buf;
	Array <char> tArr;
	Array <int> tLen;
	for (;;) {
		if ('"' != (c = fgetc(in))) if (feof(in) && z) break;
		else throw new WithChar(27, c);
		buf = new char[(l = 0) + BUF_SIZE + 1];
		while (('"' != (c = fgetc(in))) && !feof(in))
			if (l >= BUF_SIZE) break; else buf[l++] = c;
		buf[l] = 0;
		if (!z && feof(in)) throw new WithStr(56, buf);
		if (l >= BUF_SIZE) throw new WithStr(54, buf);
		tArr += buf;
		tLen += new int(l);
		c = fgetc(in);
		if (' ' != c) if ('\n' != c) if (feof(in) && z) break;
		else throw new WithChar(27, c);
		else if (!z) break;
	}
	if ((-1 != n) && (n != tArr.e() + 1))
		throw new WithShort(55, tArr.e() + 1);
	tArr.Terminate(0);
	if (x) {
		len = new int[n = tArr.e() + 1];
		m = 0;
		for (l = 0; l < n; ++l) {
			len[l] = *tLen[l];
			if (len[l] > m) m = len[l];
		}
	} else n = tArr.e() + 1;
	arr = new char *[n];
	for (l = 0; l < n; ++l) arr[l] = tArr[l];
}

void Data::Read(FILE * in) {
	int i;
	ReadStrArray(in, pssp, pspr = -1, true, psla, pslm);
	ReadStrArray(in, dtsa, dtar = -1, true, dtsl, dtlm);
	ReadStrArray(in, dtma, dtar, false, dtsl, dtlm);
	MakeTDUXA(in);
	tdsa = new char **[dtar];
	tdna = new char **[dtar];
	tdar = new int[dtar];
	tdsl = new int *[dtar];
	tdla = new int *[dtar];
	tdsm = new int[dtar];
	tdlm = new int[dtar];
	for (i = 0; i < dtar; ++i) ReadStrArray
		(in, tdsa[i], tdar[i] = -1, true, tdsl[i], tdsm[i]);
	for (i = 0; i < dtar; ++i) ReadStrArray
		(in, tdna[i], tdar[i], true, tdla[i], tdlm[i]);
	tdas = new int[dtar];
	tdas[0] = pspr;
	for (i = 1; i < dtar; ++i) tdas[i] = tdas[i - 1] + tdar[i - 1];
	sum = tdas[dtar - 1] + tdar[dtar - 1];
	MakePSTDMA();
	tdpa = new int[dtar];
	for (i = 0; i < dtar; ++i) tdpa[i] = 0;
	ReadBoolMatrix(in);
	ReadStrArray(in, slsa, slar = -1, false, dtsl, dtlm, true);
	fclose(in);
}
