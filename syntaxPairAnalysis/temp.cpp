#include "temp.h"

int TempEntry::EntryTranslate(FILE * in, wchar_t & c,
	Array <TempEntry> & arr, Array <char> & la)
{
	Array<wchar_t> buf;
	char t, * str, * tmp;
	int i = 0, n = -1, s, o;
	if (feof(in)) return 0;
	if (c != '$') buf += new wchar_t(c);
	else buf += new wchar_t(fgetwc(in));
	if (feof(in)) throw new WithStr(20, emsa[35], 0);
	buf += new wchar_t(fgetwc(in));
	while ((*buf[buf.e()] != '<') && !feof(in))
		buf += new wchar_t(fgetwc(in));
	if (*buf[buf.e()] == '<') c = '<';
	*buf[buf.e()] = 0;
	str = Array<wchar_t>::Arr2Str(buf);
	buf.Reset();
	if (!Entry::ReadStr(str, emsa[35], i)) throw new WithStr(16, str, 0);
	s = Entry::Str2Int(str, i);
	if (!Entry::ReadStr(str, emsa[5], i))
		throw new With2Str(17, str, emsa[5], 1, 0);
	if (Word::Punct(t = str[i])) {
		if (str[++i]) throw new WithStr(58, str);
		delete str;
		tmp = 0;
	} else tmp = str + i;
	buf += new wchar_t(c);
	if (feof(in)) throw new WithStr(20, emsa[38], 0);
	buf += new wchar_t(fgetwc(in));
	while ((*buf[buf.e()] != '<') && !feof(in))
		buf += new wchar_t(fgetwc(in));
	if (*buf[buf.e()] == '<') c = '<';
	*buf[buf.e()] = 0;
	str = Array<wchar_t>::Arr2Str(buf);
	i = 0;
	buf.Reset();
	if (!Entry::ReadStr(str, emsa[38], i))
		throw new With2Str(17, str, emsa[38], 1, 0);
	o = Entry::Str2Int(str, i);
	if (!Entry::ReadStr(str, emsa[39], i))
		throw new With2Str(17, str, emsa[39], 1, 0);
	if ('"' != str[i]) {
		for (n = 0; n <= la.e(); ++n)
			if (Entry::ReadStr(str, la[n], i, false)) break;
		if (n > la.e()) {
			while ('"' != str[i]) buf += new wchar_t(str[i++]);
			buf += new wchar_t(0);
			la += Array<wchar_t>::Arr2Str(buf);
			buf.Reset();
		}
	}
	if (!Entry::ReadStr(str, emsa[40], i))
		throw new With2Str(17, str, emsa[40], 1, 0);
	buf += new wchar_t(c);
	if (feof(in)) throw new WithStr(20, emsa[41], 0);
	buf += new wchar_t(fgetwc(in));
	while ((*buf[buf.e()] != '<') && !feof(in))
		buf += new wchar_t(fgetwc(in));
	if (*buf[buf.e()] == '<') c = '<';
	*buf[buf.e()] = 0;
	str = Array<wchar_t>::Arr2Str(buf);
	i = 0;
	buf.Reset();
	if (!Entry::ReadStr(str, emsa[41], i))
		throw new With2Str(17, str, emsa[41], 1, 0);
	delete str;
	if (!tmp) arr += new TempEntry(0, s, -1, new char(t));
	else arr += new TempEntry(o, s, n, tmp);
	return 1;
}

int TempEntry::DTAR, * TempEntry::I2C, * TempEntry::C2I;

int TempEntry::State2Int(int coord, int state) {
	int res = state;
	if (16 == coord) {
		if (3 < res) --res;
		if (7 < res) --res;
	}
	return res;
}

int TempEntry::Int2State(int coord, int state) {
	int res = state;
	if (16 == coord) {
		if (3 < res) ++res;
		if (8 < res) ++res;
	}
	return res;
}

int TempEntry::Class2Int(int state) {
	int i;
	for (i = 0; i < Data::PSPR(); ++i) if (C2I[i] == state) return i;
	throw new WithShort(63, state);
}

void TempEntry::Translate(HGREN & eng, FILE * in, FILE * out) {
	int i, s, o = 0, j, k, l, e, w;
	long m;
	wchar_t c = fgetwc(in), * txt, * utf;
	const char * str;
	char * lxm;
	Array <wchar_t> ca;
	Array <char> u, x, la(0);
	Array <TempEntry> arr;
	HGREN_RESPACK res;
	HGREN_TREENODE node;
	HGREN_INTARRAY list;
	DTAR = 26;
	I2C = new int[DTAR];
	I2C[0] = 5;
	I2C[1] = 7;
	I2C[2] = 8;
	I2C[3] = 9;
	I2C[4] = 10;
	I2C[5] = 11;
	I2C[6] = 12;
	I2C[7] = 13;
	I2C[8] = 14;
	I2C[9] = 15;
	I2C[10] = 16;
	I2C[11] = 17;
	I2C[12] = 18;
	I2C[13] = 20;
	I2C[14] = 21;
	I2C[15] = 22;
	I2C[16] = 24;
	I2C[17] = 25;
	I2C[18] = 26;
	I2C[19] = 27;
	I2C[20] = 28;
	I2C[21] = 29;
	I2C[22] = 30;
	I2C[23] = 31;
	I2C[24] = 32;
	I2C[25] = 33;
	list = sol_ListPartsOfSpeech(eng, RUSSIAN_LANGUAGE);
	Data::SetPSPR() = sol_CountInts(list) + 1;
	Data::SetPSSP() = new char *[Data::PSPR()];
	Data::SetPSLA() = new int[Data::PSPR()];
	C2I = new int[Data::PSPR()];
	Data::SetPSSP()[0] = "Число";
	Data::SetPSLA()[0] = 5;
	Data::SetPSLM() = 5;
	C2I[0] = 3;
	for (j = 1; j < Data::PSPR(); ++j) {
		utf = new wchar_t[sol_MaxLexemLen(eng)];
		sol_GetClassName(eng, sol_GetInt(list, j - 1), utf);
		for (k = 0; utf[k++]; ) ;
		Data::SetPSSP()[j] = new char[k];
		if (!WideCharToMultiByte(1251, 0, utf, k, Data::SetPSSP()[j], k, 0, 0))
			throw new WithStr(65, (char *)utf, 1);
		delete [] utf;
		C2I[j] = sol_GetInt(list, j - 1);
		Data::SetPSLA()[j] = --k;
		if (Data::PSLM() < k) Data::SetPSLM() = k;
	}
	Data::SetDTAR() = DTAR;
	Data::SetDTSA() = new char *[Data::DTAR()];
	Data::SetDTMA() = new char *[Data::DTAR()];
	Data::SetDTSL() = new int[Data::DTAR()];
	Data::SetDTLM() = 0;
	for (j = 0; j < Data::DTAR(); ++j) {
		utf = new wchar_t[sol_MaxLexemLen(eng)];
		sol_GetCoordName(eng, I2C[j], utf);
		for (k = 0; utf[k++]; ) ;
		Data::SetDTSA()[j] = new char[k];
		Data::SetDTMA()[j] = new char[k];
		if (!WideCharToMultiByte(1251, 0, utf, k, Data::SetDTSA()[j], k, 0, 0))
			throw new WithStr(65, (char *)utf, 1);
		delete [] utf;
		for (l = 0; l < k; ++l) Data::SetDTMA()[j][l] = Data::DTSA()[j][l];
		Data::SetDTSL()[j] = --k;
		if (Data::DTLM() < k) Data::SetDTLM() = k;
	}
	u += new char('{');
	for (j = 0; j < Data::DTAR(); ++j) {
		(u += new char('[')) += new char(' ');
		x += new char(' ');
		for (k = Data::DTSL()[j]; k < Data::DTLM(); ++k) {
			u += new char(' ');
			x += new char(' ');
		}
		for (k = 0; k < Data::DTSL()[j]; ++k) {
			u += new char(Data::DTSA()[j][k]);
			x += new char(Data::DTSA()[j][k]);
		}
		((u += new char('E')) += new char('q')) += new char(' ');
		(((x += new char('E')) += new char('q'))
			+= new char(';')) += new char(' ');
		for (k = Data::DTSL()[j]; k < Data::DTLM(); ++k) {
			u += new char(' ');
			x += new char(' ');
		}
		for (k = 0; k < Data::DTSL()[j]; ++k) {
			u += new char(Data::DTSA()[j][k]);
			x += new char(Data::DTSA()[j][k]);
		}
		(((u += new char('N')) += new char('E'))
			+= new char(']')) += new char(' ');
		((x += new char('N')) += new char('E')) += new char(';');
	}
	delete --u;
	(((u += new char('}')) += new char('~')) += new char('{')) += new char(0);
	x += new char(0);
	Data::SetTDUA() = Array<char>::Arr2Str(u);
	Data::SetTDXA() = Array<char>::Arr2Str(x);
	Data::SetTDAR() = new int[Data::DTAR()];
	Data::SetTDSA() = new char **[Data::DTAR()];
	Data::SetTDNA() = new char **[Data::DTAR()];
	Data::SetTDSL() = new int *[Data::DTAR()];
	Data::SetTDSM() = new int[Data::DTAR()];
	Data::SetTDLA() = new int *[Data::DTAR()];
	Data::SetTDLM() = new int[Data::DTAR()];
	for (j = 0; j < Data::DTAR(); ++j) {
		Data::SetTDAR()[j] = sol_CountCoordStates(eng, I2C[j]);
		if (!Data::TDAR()[j]) Data::SetTDAR()[j] = 2;
		Data::SetTDSA()[j] = new char *[Data::TDAR()[j]];
		Data::SetTDNA()[j] = new char *[Data::TDAR()[j]];
		Data::SetTDSL()[j] = new int[Data::TDAR()[j]];
		Data::SetTDLA()[j] = new int[Data::TDAR()[j]];
		Data::SetTDSM()[j] = 0;
		for (k = 0; k < Data::TDAR()[j]; ++k) {
			utf = new wchar_t[sol_MaxLexemLen(eng)];
			sol_GetCoordStateName(eng, I2C[j], Int2State(j, k), utf);
			for (l = 0; utf[l++]; ) ;
			Data::SetTDSA()[j][k] = new char[l];
			Data::SetTDNA()[j][k] = new char[l];
			if (!WideCharToMultiByte(1251, 0, utf, l, Data::SetTDSA()[j][k],
				l, 0, 0)) throw new WithStr(65, (char *)utf, 1);
			delete [] utf;
			for (w = 0; w < l; ++w)
				Data::SetTDNA()[j][k][w] = Data::TDSA()[j][k][w];
			
			Data::SetTDLA()[j][k] = (Data::SetTDSL()[j][k] = --l);
			if (Data::TDSM()[j] < l) Data::SetTDSM()[j] = l;
		}
		Data::SetTDLM()[j] = Data::TDSM()[j];
	}
	Data::SetTDAS() = new int[Data::DTAR()];
	Data::SetTDAS()[0] = Data::PSPR();
	for (j = 1; j < Data::DTAR(); ++j) Data::SetTDAS()[j]
		= Data::TDAS()[j - 1] + Data::TDAR()[j - 1];
	Data::SetSUM() = Data::TDAS()[Data::DTAR() - 1]
		+ Data::TDAR()[Data::DTAR() - 1];
	Data::SetPSMA() = new long[2];
	Data::SetTDMA() = new long *[Data::DTAR()];
	for (j = 0; j < Data::DTAR(); ++j) Data::SetTDMA()[j] = new long[2];
	Data::SetTDMA()[Data::DTAR() - 1][1] = 1;
	for (j = Data::DTAR() - 1; j > 0; --j) {
		m = 1;
		while (m < Data::TDAR()[j]) m *= 2;
		Data::SetTDMA()[j - 1][1] = m*Data::TDMA()[j][1];
		Data::SetTDMA()[j][0] = (m - 1)*Data::TDMA()[j][1];
	}
	m = 1;
	while (m < Data::TDAR()[0]) m *= 2;
	Data::SetPSMA()[1] = m*Data::TDMA()[0][1];
	Data::SetTDMA()[0][0] = (m - 1)*Data::TDMA()[0][1];
	m = 1;
	while (m < Data::PSPR()) m *= 2;
	Data::SetPSMA()[0] = (m - 1)*Data::PSMA()[1];
	Data::SetTDPA() = new int[Data::DTAR()];
	for (i = 0; i < Data::DTAR(); ++i) Data::SetTDPA()[i] = 0;
	Data::SetDTIA() = new bool *[Data::PSPR()];
	for (j = 0; j < Data::PSPR(); ++j) {
		Data::SetDTIA()[j] = new bool[Data::DTAR()];
		for (k = 0; k < Data::DTAR(); ++k) Data::SetDTIA()[j][k]
			= 0 <= sol_GetCoordType(eng, I2C[k], C2I[j]);
	}
	do {	ca += new wchar_t(c);
	} while ((c = fgetwc(in)) != '<');
	ca += new wchar_t(0);
	str = Array<wchar_t>::Arr2Str(ca);
	ca.Reset();
	if (!Entry::ReadStr(str, emsa[7], i = 0))
		throw new With2Str(17, str, emsa[7], 1, 0);
	fprintf(out, "%s%s\n", emsa[7], emsa[5]);
	delete [] str;
	do {	ca += new wchar_t(c);
	} while ((c = fgetwc(in)) != '<');
	ca += new wchar_t(0);
	str = Array<wchar_t>::Arr2Str(ca);
	ca.Reset();
	if (!Entry::ReadStr(str, emsa[31], i = 0))
		throw new With2Str(17, str, emsa[31], 1, 0);
	delete [] str;
	do {	ca += new wchar_t(c);
	} while ((c = fgetwc(in)) != '<');
	ca += new wchar_t(0);
	str = Array<wchar_t>::Arr2Str(ca);
	ca.Reset();
	if (!Entry::ReadStr(str, emsa[42], i = 0))
		throw new With2Str(17, str, emsa[42], 1, 0);
	fprintf(out, "%s%s\n%s\n", emsa[8], emsa[5], emsa[21]);
	delete [] str;
	for (;;) {
		fprintf(stdout, ".");
		do {	ca += new wchar_t(c);
		} while ((c = fgetwc(in)) != '<');
		ca += new wchar_t(0);
		str = Array<wchar_t>::Arr2Str(ca);
		ca.Reset();
		if (Entry::ReadStr(str, emsa[32], i = 0)) {
			fprintf(out, "%s\n", emsa[25]);
			break;
		}
		if (!Entry::ReadStr(str, emsa[33], i))
			throw new With2Str(17, str, emsa[33], 1, 0);
		fprintf(out, "%s", emsa[22]);
		s = Entry::Str2Int(str, i);
		fprintf(out, "%i", ++o);
		if (!Entry::ReadStr(str, emsa[5], i))
			throw new With2Str(17, str, emsa[5], 1, 0);
		fprintf(out, "%s\n", emsa[5]);
		delete [] str;
		do {	ca += new wchar_t(c);
		} while ((c = fgetwc(in)) != '<');
		ca += new wchar_t(0);
		str = Array<wchar_t>::Arr2Str(ca);
		ca.Reset();
		if (!Entry::ReadStr(str, emsa[36], i = 0))
			throw new With2Str(17, str, emsa[36], 1, 0);
		delete [] str;
		do {	ca += new wchar_t(c);
		} while ((c = fgetwc(in)) != '<');
		ca += new wchar_t(0);
		str = Array<wchar_t>::Arr2Str(ca);
		ca.Reset();
		if (!Entry::ReadStr(str, emsa[37], i = 0))
			throw new With2Str(17, str, emsa[37], 1, 0);
		while (str[i] != 0)
			if (!Word::Space(str[i])) fprintf(out, "%c", str[i++]); else ++i;
		delete [] str;
		str = 0;
		try {	for (;;) if (!EntryTranslate(in, c, arr, la)) break;
		} catch (WithStr * e) {
			if (e->ID() != 16) throw;
			str = e->Data();
			delete e;
		}
		l = 0;
		for (j = 0; j <= arr.e(); ++j) if (-1 != arr[j]->I()) {
			k = 0;
			while (arr[j]->Txt()[k]) ++k;
			l += k + 1;
		}
		txt = new wchar_t[l];
		l = 0;
		w = 0;
		for (j = 0; j <= arr.e(); ++j) if (-1 != arr[j]->I()) {
			k = 0;
			while (arr[j]->Txt()[k++]) ;
			utf = new wchar_t[k];
			if (!MultiByteToWideChar(1251, 0, arr[j]->Txt(), k, utf, k))
				throw new WithStr(66, arr[j]->Txt(), 1);
			k = 0;
			while (utf[k]) txt[l++] = utf[k++];
			delete [] utf;
			txt[l++] = ' ';
			++w;
		}
		txt[l - 1] = 0;
		if (!w || !(res = sol_MorphologyAnalysis(eng, txt,
			SOL_GREN_ALLOW_FUZZY, 0, 300000, RUSSIAN_LANGUAGE)) ||
			(1 < sol_CountGrafs(res)) || (w + 2 != sol_CountRoots(res, 0)))
			if (l) {
				lxm = new char[l];
				if (!WideCharToMultiByte(1251, 0, txt, l, lxm, l, 0, 0))
					throw new WithStr(65, (char *)txt, 1);
				fprintf(stdout, "\nНе получилось разпознать предложение \"%s\"\
, продолжаю...\n", lxm);
				delete [] lxm;
				delete [] txt;
			} else fprintf(stdout, "\nНе получилось разпознать предложение,\
 так как в нём не выделены синтаксические связи; продолжаю...\n");
		else {
			for (w = 0, j = 1; j < sol_CountRoots(res, 0) - 1; ++w, ++j) {
				while (-1 == arr[w]->I()) ++w;
				node = sol_GetRoot(res, 0, j);
			//	if (0 < sol_CountLeafs(node))
			//		throw new WithStrShort(60, j, (char *)txt, 1);
			//	if (1 < sol_GetNodeVersionCount(eng, node))
			//		throw new WithStrShort(61, j, (char *)txt, 1);
				fprintf(out, "%s%i%s%s", emsa[23], arr[w]->O(),
					emsa[0], Data::PSSP()[Class2Int(sol_GetEntryClass
					(eng, sol_GetNodeIEntry(eng, node)))]);
				for (k = 0; k < Data::DTAR(); ++k) if (-1 != (e =
					sol_GetNodeCoordState(node, I2C[k])))
						fprintf(out, " %s", Data::TDSA()[k][State2Int(k, e)]);
				utf = new wchar_t[sol_MaxLexemLen(eng)];
				sol_GetNodeContents(node, utf);
				for (k = 0; utf[k++]; ) ;
				lxm = new char[k];
				if (!WideCharToMultiByte(1251, 0, utf, k, lxm, k, 0, 0))
					throw new WithStr(65, (char *)utf, 1);
				delete [] utf;
				fprintf(out, "%s%i%s%s%s%s%s%s%s\n", emsa[2],
					arr[w]->S(), emsa[3], lxm, emsa[4],
					la[arr[w]->I()], emsa[5], arr[w]->Txt(), emsa[6]);
				delete [] lxm;
			}
			sol_DeleteResPack(res);
			delete [] txt;
		}
		arr.Reset();
		if (!str || !Entry::ReadStr(str, emsa[34], i = 0))
			throw new With2Str(17, str, emsa[34], 1, 0);
		fprintf(out, "%s\n", emsa[24]);
		while (str[i] != 0) {
			if (!Word::Space(str[i]))
				throw new WithCharStr(s, str[i], str, 0);
			fprintf(out, "%c", str[i++]);
		}
		delete [] str;
	}
	fprintf(stdout, "\n");
	Data::SetSLAR() = la.e() + 1;
	Data::SetSLSA() = new char *[Data::SLAR()];
	for (j = 0; j < Data::SLAR(); ++j) Data::SetSLSA()[j] = la[j];
	fprintf(out, "%s\n", emsa[26]);
}

void TempEntry::Train(GDecisionTree * model, const Array <Sentense> & sa) {
	int i, j, k, o, s;
	double * t;
	vector<size_t> feature_values, label_values;
	feature_values.push_back(63);
	for (i = 0; i <= Data::DTAR(); ++i) feature_values.push_back(2);
	feature_values.push_back(Data::PSPR());
	for (i = 0; i < Data::DTAR(); ++i) feature_values.push_back(Data::TDAR()[i]);
	feature_values.push_back(Data::PSPR());
	for (i = 0; i < Data::DTAR(); ++i) feature_values.push_back(Data::TDAR()[i]);
	label_values.push_back(2);
	label_values.push_back(Data::SLAR() + 1);
	GMatrix features(feature_values), labels(label_values);
	for (i = 0; i <= sa.e(); ++i) {
		for (j = 0; j <= sa[i]->EA()->e(); ++j) {
			o = (*sa[i]->EA())[j]->OD();
			s = (*sa[i]->EA())[j]->SD();
			t = features.newRow();
			if (s > o) t[0] = s - o - 1; else t[0] = o - s - 1;
			if (((*sa[i]->WA())[o - 1]->Prop().Data() & Data::PSMA()[0])
				== ((*sa[i]->WA())[s - 1]->Prop().Data() & Data::PSMA()[0])) t[1] = 0;
			else t[1] = 1;
			for (k = 0; k < Data::DTAR(); ++k) if (((*sa[i]->WA())[o - 1]->
				Prop().Data() & Data::TDMA()[k][0]) == ((*sa[i]->WA())[s - 1]->
				Prop().Data() & Data::TDMA()[k][0])) t[k + 2] = 0;
			else t[k + 2] = 1;
			t[Data::DTAR() + 2] = ((*sa[i]->WA())[o - 1]->Prop().Data() & Data::PSMA()[0])
				/ Data::PSMA()[1];
			for (k = 0; k < Data::DTAR(); ++k) t[k + Data::DTAR() + 3] = ((*sa[i]->
				WA())[o - 1]->Prop().Data() & Data::TDMA()[k][0]) / Data::TDMA()[k][1];
			t[2*Data::DTAR() + 3] = ((*sa[i]->WA())[s - 1]->
				Prop().Data() & Data::PSMA()[0]) / Data::PSMA()[1];
			for (k = 0; k < Data::DTAR(); ++k) t[k + 2*Data::DTAR() + 4] = ((*sa[i]->
				WA())[s - 1]->Prop().Data() & Data::TDMA()[k][0]) / Data::TDMA()[k][1];
			t = labels.newRow();
			t[0] = 1;
			t[1] = (*sa[i]->EA())[j]->ID();
		}
		for (j = 0; j <= sa[i]->UA()->e(); ++j) {
			o = (*sa[i]->UA())[j]->OD();
			s = (*sa[i]->UA())[j]->SD();
			t = features.newRow();
			if (s > o) t[0] = s - o - 1; else t[0] = o - s - 1;
			if (((*sa[i]->WA())[o - 1]->Prop().Data() & Data::PSMA()[0])
				== ((*sa[i]->WA())[s - 1]->Prop().Data() & Data::PSMA()[0])) t[1] = 0;
			else t[1] = 1;
			for (k = 0; k < Data::DTAR(); ++k) if (((*sa[i]->WA())[o - 1]->
				Prop().Data() & Data::TDMA()[k][0]) == ((*sa[i]->WA())[s - 1]->
				Prop().Data() & Data::TDMA()[k][0])) t[k + 2] = 0;
			else t[k + 2] = 1;
			t[Data::DTAR() + 2] = ((*sa[i]->WA())[o - 1]->Prop().Data() & Data::PSMA()[0])
				/ Data::PSMA()[1];
			for (k = 0; k < Data::DTAR(); ++k) t[k + Data::DTAR() + 3] = ((*sa[i]->
				WA())[o - 1]->Prop().Data() & Data::TDMA()[k][0]) / Data::TDMA()[k][1];
			t[2*Data::DTAR() + 3] = ((*sa[i]->WA())[s - 1]->
				Prop().Data() & Data::PSMA()[0]) / Data::PSMA()[2];
			for (k = 0; k < Data::DTAR(); ++k) t[k + 2*Data::DTAR() + 4] = ((*sa[i]->
				WA())[s - 1]->Prop().Data() & Data::TDMA()[k][0]) / Data::TDMA()[k][1];
			t = labels.newRow();
			t[0] = 0;
			t[1] = Data::SLAR();
		}
	}
	model->train(features, labels);
}

void TempEntry::Download() {
	FILE * in, * out;
	int i;
	char buf[BUF_SIZE];
	HGREN eng;
	try {
		if (!(in = fopen("data.txt", "r"))) throw new Exception(57);
		else Data::Read(in);
	} catch (Exception * exc) {
		exc->Print(stderr);
		delete exc;
		fprintf(stdout, "Составить данные автоматически по выборке примеров\
 связей?\n(введите \"no\" или укажите через '?' имена файла с выборкой и\
 выходного файла)\n");
		if (in) fclose(in);
		in = 0;
		for (;;) {
			fprintf(stdout, ">:");
			i = 0;
			buf[i] = fgetc(stdin);
			while ((buf[i] != '\n') && (buf[i] != '\r') && !feof(stdin))
				buf[++i] = fgetc(stdin);
			buf[i] = 0;
			i = 0;
			try {
				if (Entry::ReadStr(buf, "no", i)) exit(0);
				in = Computing::FileOpen(buf, i, "r, ccs=UTF-8");
				if (!buf[i++]) throw new WithStr(64, buf, 0);
				out = Computing::FileOpen(buf, i, "w");
				eng = sol_CreateGrammarEngine("C:\\Program\
 Files\\RussianGrammaticalDictionary\\bin-windows\\dictionary.xml");
				Translate(eng, in, out);
				sol_DeleteGrammarEngine(eng);
				fprintf(stdout, done);
			} catch (Exception * e) {
				e->Print(stderr);
				delete e;
			}
			if (0 != in) {
				fclose(in);
				in = 0;
			}
			if (0 != out) {
				fclose(out);
				out = 0;
			}
		}
	}
}
