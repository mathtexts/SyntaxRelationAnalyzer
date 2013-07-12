#include "link.h"

Node::Node() {
	int i;
	data = new int[Data::SUM()];
	for (i = 0; i < Data::SUM(); ++i) data[i] = 0;
	Start();
}

Node::~Node() {
	delete [] data;
	Terminate();
}

void Node::Start() {
	int i;
	s = new int[Data::DTAR() + 1];
	for (i = 0; i <= Data::DTAR(); ++i) s[i] = 0;
	dh = new float[Data::DTAR() + 1];
	for (i = 0; i <= Data::DTAR(); ++i) dh[i] = 0.0;
	li = new int[Data::SUM()];
	for (i = 0; i < Data::SUM(); ++i) li[i] = 0;
	mi = new int[Data::SUM()];
	for (i = 0; i < Data::SUM(); ++i) mi[i] = 0;
	lr = new int[Data::DTAR() + 1];
	for (i = 0; i <= Data::DTAR(); ++i) lr[i] = 0;
	mr = new int[Data::DTAR() + 1];
	for (i = 0; i <= Data::DTAR(); ++i) mr[i] = 0;
	ls = new float[Data::DTAR() + 1];
	for (i = 0; i <= Data::DTAR(); ++i) ls[i] = 0.0;
	ms = new float[Data::DTAR() + 1];
	for (i = 0; i <= Data::DTAR(); ++i) ms[i] = 0.0;
}

void Node::Terminate() {
	delete [] s;
	delete [] dh;
	delete [] li;
	delete [] mi;
	delete [] lr;
	delete [] mr;
	delete [] ls;
	delete [] ms;
}

void Node::Clear() {
	int i;
	delete [] data;
	data = new int[Data::SUM()];
	for (i = 0; i < Data::SUM(); ++i) data[i] = 0;
}

void Node::AddEntry(Word * x, Word * y, int z) {
	int i, j, k;
	if (!y) {
		j = (x->Prop().Data() & Data::PSMA()[0])/Data::PSMA()[1];
		data[j]++;
		for (i = 0; i < Data::DTAR(); ++i) if (Data::DTIA()[j][i])
			data[Data::TDAS()[i] + (x->Prop().Data() & Data::TDMA()[i][0])/Data::TDMA()[i][1]]++;
	} else if (!z) {
		j = (x->Prop().Data() & Data::PSMA()[0])/Data::PSMA()[1];
		k = (y->Prop().Data() & Data::PSMA()[0])/Data::PSMA()[1];
		data[j != k]++;
		for (i = 0; i < Data::DTAR(); ++i) if (Data::DTIA()[j][i] && Data::DTIA()[k][i])
			data[Data::TDAS()[i] + ((x->Prop().Data() & Data::TDMA()[i][0]) !=
				(y->Prop().Data() & Data::TDMA()[i][0]))]++;
	} else {
		if (x->ID() > y->ID()) data[x->ID() - y->ID() - 1]++;
		else if (y->ID() > x->ID()) data[y->ID() - x->ID() - 1]++;
		else throw new WithShort(31, x->ID());
	}
}

void Node::Finalize(int e) {
	int i, j, k;
	float * l, * m;
	if (2 == e) {
		for (i = 0; i < Data::SUM(); ++i) s[0] += data[i];
		return;
	}
	l = new float[Data::SUM()];
	m = new float[Data::SUM()];
	if (e) k = 2; else k = Data::PSPR();
	for (j = 0; j < k; ++j) s[0] += data[j];
	if (s[0]) {
		for (j = 0; j < k; ++j) {
			if (data[j]*k < s[0]) {
				ls[0] += l[lr[0]] = data[li[lr[0]] = j]*1.0/s[0];
				lr[0]++;
			} else {
				ms[0] += m[mr[0]] = data[mi[mr[0]] = j]*1.0/s[0];
				mr[0]++;
			}
			if (data[j]) dh[0] += data[j]*1.0/s[0]*log(data[j]*1.0/s[0]);
		}
		ls[0] /= lr[0];
		ms[0] /= mr[0];
	}
	for (i = 0; i < Data::DTAR(); ++i) {
		if (e) k = 2; else k = Data::TDAR()[i];
		for (j = Data::TDAS()[i]; j < Data::TDAS()[i] + k; ++j)
			s[i + 1] += data[j];
		if (s[i + 1]) {
			for (j = 0; j < k; ++j) {
				if (data[Data::TDAS()[i] + j]*k < s[i + 1]) {
					ls[i + 1] += l[Data::TDAS()[i] + lr[i + 1]] = data[Data::TDAS()[i]
						+ (li[Data::TDAS()[i] + lr[i + 1]] = j)]*1.0/s[i + 1];
					lr[i + 1]++;
				} else {
					ms[i + 1] += m[Data::TDAS()[i] + mr[i + 1]] = data[Data::TDAS()[i]
						+ (mi[Data::TDAS()[i] + mr[i + 1]] = j)]*1.0/s[i + 1];
					mr[i + 1]++;
				}
				if (data[Data::TDAS()[i] + j]) dh[i + 1] += data[Data::TDAS()[i] + j]*1.0
					/s[i + 1]*log(data[Data::TDAS()[i] + j]*1.0/s[i + 1]);
			}
			ls[i + 1] /= lr[i + 1];
			ms[i + 1] /= mr[i + 1];
		}
	}
	delete [] l;
	delete [] m;
}

void Node::Load(FILE * in, char & c, int e) {
	int i, j, k;
	if (2 == e) {
		while (c == ' ') c = fgetc(in);
		data[0] += Data::Str2Int(in, c);
		for (i = 1; i < Data::SUM(); ++i) {
			if (c != ' ') throw new WithChar(27, c); else c = fgetc(in);
			while (c == ' ') c = fgetc(in);
			data[i] += Data::Str2Int(in, c);
		}
		return;
	}
	if (c != '[') throw new WithChar(27, c); else c = fgetc(in);
	while (c == ' ') c = fgetc(in);
	data[0] += Data::Str2Int(in, c);
	if (e) k = 2; else k = Data::PSPR();
	for (i = 1; i < k; ++i) {
		if (c != ' ') throw new WithChar(27, c); else c = fgetc(in);
		while (c == ' ') c = fgetc(in);
		data[i] += Data::Str2Int(in, c);
	}
	for (i = 0; i < Data::DTAR(); ++i) {
		if (c != ']') throw new WithChar(27, c); else c = fgetc(in);
		if (c != ' ') throw new WithChar(27, c); else c = fgetc(in);
		if (c != '[') throw new WithChar(27, c); else c = fgetc(in);
		while (c == ' ') c = fgetc(in);
		data[Data::TDAS()[i]] += Data::Str2Int(in, c);
		if (e) k = 2; else k = Data::TDAR()[i];
		for (j = 1; j < k; ++j) {
			if (c != ' ') throw new WithChar(27, c); else c = fgetc(in);
			while (c == ' ') c = fgetc(in);
			data[Data::TDAS()[i] + j] += Data::Str2Int(in, c);
		}
	}
	if (c != ']') throw new WithChar(27, c); else c = fgetc(in);
	Finalize();
}

void Node::Print(FILE * out, int x, int e) {
	int i, j, k;
	if (!x) {
		if (2 == e) {
			for (i = 0; i < Data::SUM(); ++i) fprintf(out, "%7i", data[i]);
			return;
		}
		fprintf(out, "[");
		if (e) k = 2; else k = Data::PSPR();
		for (i = 0; i < k; ++i) fprintf(out, "%7i", data[i]);
		for (i = 0; i < Data::DTAR(); ++i) {
			fprintf(out, "] [");
			if (e) k = 2; else k = Data::TDAR()[i];
			for (j = 0; j < k; ++j)
				fprintf(out, "%7i", data[Data::TDAS()[i] + j]);
		}
		fprintf(out, "]");
	} else {
		if (2 == e) {
			for (i = 0; i < Data::SUM(); ++i) fprintf(out, "%7i;", data[i]);
			return;
		}
		if (e) k = 2; else k = Data::PSPR();
		for (i = 0; i < k; ++i) fprintf(out, "%7i;", data[i]);
		for (i = 0; i < Data::DTAR(); ++i) {
			if (e) k = 2; else k = Data::TDAR()[i];
			for (j = 0; j < k; ++j)
				fprintf(out, "%7i;", data[Data::TDAS()[i] + j]);
		}
	}
}

void Node::Max(int i, int * mid, int & mxp, int & mxs,
	float & mnd, float & mxd, int n, int & mn)
{
	int k, l;
	for (k = 0; k <= Data::DTAR(); ++k) if (s[k] != 0) {
		for (l = 0; l < i; ++l) if (k == mid[l]) break;
		if (l < i) continue;
		if (Data::TDPA()[k] > mxp) {
			mxp = Data::TDPA()[k];
			mxs = s[k];
			mnd = dh[k];
			mxd = ms[k] - ls[k];
			mid[i] = k;
			mn = n;
		} else if (Data::TDPA()[k] == mxp) if (s[k] > mxs) {
			mxs = s[k];
			mnd = dh[k];
			mxd = ms[k] - ls[k];
			mid[i] = k;
			mn = n;
		} else if (s[k] == mxs) if (dh[k] < mnd) {
			mnd = dh[k];
			mxd = ms[k] - ls[k];
			mid[i] = k;
			mn = n;
		} else if (dh[k] == mnd) if (ms[k] - ls[k] > mxd) {
			mxd = ms[k] - ls[k];
			mid[i] = k;
			mn = n;
		}
	}
}

void Node::Top(Node * ob, Node * sb,
	Node * en, FILE * out, int j, int x)
{
	int i, k, * mid, mxp, mxs, mn;
	float mnd = 1.0, mxd = 0.0;
	mid = new int[j];
	for (i = 0; i < j; ++i) {
		mid[i] = -1;
		mnd = 1.0;
		mxd = 0.0;
		mxp = -1;
		mxs = -1;
		mn = 0;
		ob->Max(i, mid, mxp, mxs, mnd, mxd, 1, mn);
		sb->Max(i, mid, mxp, mxs, mnd, mxd, 2, mn);
		en->Max(i, mid, mxp, mxs, mnd, mxd, 3, mn);
		if (mid[i] == -1) {
			if (!i) fprintf(out, "[(<вхождений правила нет>");
			else fprintf(out, ")] [(<больше признаков нет>");
			break;
		} else if (mid[i] == 0) if (mn == 3) {
				if (x) if (!i) fprintf(out, "[<%i %i %f %f> Части речи %s",
					mxp, mxs, mnd, mxd, psse[en->mi[0]]);
				else fprintf(out, "] [<%i %i %f %f> Части речи %s",
					mxp, mxs, mnd, mxd, psse[en->mi[0]]);
				else if (!i) fprintf(out, "[Части речи %s", psse[en->mi[0]]);
				else fprintf(out, "] [Части речи %s", psse[en->mi[0]]);
			} else if (mn == 1) {
				if (x) if (!i) fprintf(out,
					"[<%i %i %f %f> Часть речи объекта=(%s",
					mxp, mxs, mnd, mxd, Data::PSSP()[ob->mi[0]]);
				else fprintf(out, ")] [<%i %i %f %f> Часть речи объекта=(%s",
					mxp, mxs, mnd, mxd, Data::PSSP()[ob->mi[0]]);
				else if (!i) fprintf(out, "[Часть речи объекта=(%s",
					Data::PSSP()[ob->mi[0]]);
				else fprintf(out, ")] [Часть речи объекта=(%s",
					Data::PSSP()[ob->mi[0]]);
				for (k = 1; k < ob->mr[0]; ++k)
					fprintf(out, ", %s", Data::PSSP()[ob->mi[k]]);
			} else {
				if (x) if (!i) fprintf(out,
					"[<%i %i %f %f> Часть речи субъекта=(%s",
					mxp, mxs, mnd, mxd, Data::PSSP()[sb->mi[0]]);
				else fprintf(out, ")] [<%i %i %f %f> Часть речи субъекта=(%s",
					mxp, mxs, mnd, mxd, Data::PSSP()[sb->mi[0]]);
				else if (!i) fprintf(out, "[Часть речи субъекта=(%s",
					Data::PSSP()[sb->mi[0]]);
				else fprintf(out, ")] [Часть речи субъекта=(%s",
					Data::PSSP()[sb->mi[0]]);
				for (k = 1; k < sb->mr[0]; ++k)
					fprintf(out, ", %s", Data::PSSP()[sb->mi[k]]);
		} else if (mn == 3) {
				if (x) if (!i) fprintf(out, "[<%i %i %f %f> %s %s", mxp, mxs,
					mnd, mxd, Data::DTMA()[mid[i]-1], psse[en->mi[Data::TDAS()[mid[i]-1]]]);
				else fprintf(out, "] [<%i %i %f %f> %s %s", mxp, mxs, mnd,
					mxd, Data::DTMA()[mid[i] - 1], psse[en->mi[Data::TDAS()[mid[i] - 1]]]);
				else if (!i) fprintf(out, "[%s %s", Data::DTMA()[mid[i] - 1],
					psse[en->mi[Data::TDAS()[mid[i] - 1]]]);
				else fprintf(out, "] [%s %s", Data::DTMA()[mid[i] - 1],
					psse[en->mi[Data::TDAS()[mid[i] - 1]]]);
			} else if (mn == 1) {
				if (x) if (!i) fprintf(out, "[<%i %i %f %f> %s объекта=(%s",
					mxp, mxs, mnd, mxd, Data::DTSA()[mid[i] - 1],
					Data::TDSA()[mid[i] - 1][ob->mi[Data::TDAS()[mid[i] - 1]]]);
				else fprintf(out, ")] [<%i %i %f %f> %s объекта=(%s",
					mxp, mxs, mnd, mxd, Data::DTSA()[mid[i] - 1],
					Data::TDSA()[mid[i] - 1][ob->mi[Data::TDAS()[mid[i] - 1]]]);
				else if (!i) fprintf(out, "[%s объекта=(%s", Data::DTSA()[mid[i] - 1],
					Data::TDSA()[mid[i] - 1][ob->mi[Data::TDAS()[mid[i] - 1]]]);
				else fprintf(out, ")] [%s объекта=(%s", Data::DTSA()[mid[i] - 1],
					Data::TDSA()[mid[i] - 1][ob->mi[Data::TDAS()[mid[i] - 1]]]);
				for (k = 1; k < ob->mr[mid[i]]; ++k) fprintf(out,
					", %s", Data::TDSA()[mid[i] - 1][ob->mi[Data::TDAS()[mid[i] - 1] + k]]);
			} else {
				if (x) if (!i) fprintf(out, "[<%i %i %f %f> %s субъекта=(%s",
					mxp, mxs, mnd, mxd, Data::DTSA()[mid[i] - 1],
					Data::TDSA()[mid[i] - 1][sb->mi[Data::TDAS()[mid[i] - 1]]]);
				else fprintf(out, ")] [<%i %i %f %f> %s субъекта=(%s",
					mxp, mxs, mnd, mxd, Data::DTSA()[mid[i] - 1],
					Data::TDSA()[mid[i] - 1][sb->mi[Data::TDAS()[mid[i] - 1]]]);
				else if (!i) fprintf(out, "[%s субъекта=(%s", Data::DTSA()[mid[i] - 1],
					Data::TDSA()[mid[i] - 1][sb->mi[Data::TDAS()[mid[i] - 1]]]);
				else fprintf(out, ")] [%s субъекта=(%s", Data::DTSA()[mid[i] - 1],
					Data::TDSA()[mid[i] - 1][sb->mi[Data::TDAS()[mid[i] - 1]]]);
				for (k = 1; k < sb->mr[mid[i]]; ++k) fprintf(out,
					", %s", Data::TDSA()[mid[i] - 1][sb->mi[Data::TDAS()[mid[i] - 1] + k]]);
		}
	}
	if (mn == 3) fprintf(out, "]"); else fprintf(out, ")]");
	delete [] mid;
}

void Entry::Output(FILE * out, Array<Sentense> & sa, int x) {
	int i, j;
	if (!x) {
		fprintf(out, "%6i: [%6i:\"%20s\",\"%20s\"(%4s", pd + 1,
			sd, (*sa[pd]->WA())[sd - 1]->Txt(),
			(*sa[pd]->WA())[sd - 1]->Prop().Lxm(),
			Data::PSSP()[j = ((*sa[pd]->WA())[sd - 1]->Prop().Data()
				& Data::PSMA()[0]) / Data::PSMA()[1]]);
		for (i = 0; i < Data::DTAR(); ++i) if (Data::DTIA()[j][i]) fprintf(out, " %9s",
			Data::TDSA()[i][((*sa[pd]->WA())[sd - 1]->Prop().Data()
				& Data::TDMA()[i][0]) / Data::TDMA()[i][1]]);
			else fprintf(out, "          ");
		if (-1 == id) fprintf(out, ")]<-%20s-[", NLNK);
		else fprintf(out, ")]<-%20s-[", Data::SLSA()[id]);
		fprintf(out, "%6i:\"%20s\",\"%20s\"(%4s",
			od, (*sa[pd]->WA())[od - 1]->Txt(),
			(*sa[pd]->WA())[od - 1]->Prop().Lxm(),
			Data::PSSP()[j = ((*sa[pd]->WA())[od - 1]->Prop().Data()
				& Data::PSMA()[0]) / Data::PSMA()[1]]);
		for (i = 0; i < Data::DTAR(); ++i) if (Data::DTIA()[j][i]) fprintf(out, " %9s",
			Data::TDSA()[i][((*sa[pd]->WA())[od - 1]->Prop().Data()
				& Data::TDMA()[i][0]) / Data::TDMA()[i][1]]);
			else fprintf(out, "          ");
		fprintf(out, ")]\n");
	} else {
		fprintf(out, "%20s;%20s;%4s;",
			(*sa[pd]->WA())[sd - 1]->Txt(),
			(*sa[pd]->WA())[sd - 1]->Prop().Lxm(),
			Data::PSSP()[j = ((*sa[pd]->WA())[sd - 1]->Prop().Data()
				& Data::PSMA()[0]) / Data::PSMA()[1]]);
		for (i = 0; i < Data::DTAR(); ++i) if (Data::DTIA()[j][i]) fprintf(out, "%9s;",
			Data::TDSA()[i][((*sa[pd]->WA())[sd - 1]->Prop().Data()
				& Data::TDMA()[i][0]) / Data::TDMA()[i][1]]);
			else fprintf(out, "         ;");
		if (-1 == id) fprintf(out, "%20s;", NLNK);
		else fprintf(out, "%20s;", Data::SLSA()[id]);
		fprintf(out, "%20s;%20s;%4s;",
			(*sa[pd]->WA())[od - 1]->Txt(),
			(*sa[pd]->WA())[od - 1]->Prop().Lxm(),
			Data::PSSP()[j = ((*sa[pd]->WA())[od - 1]->Prop().Data()
				& Data::PSMA()[0]) / Data::PSMA()[1]]);
		for (i = 0; i < Data::DTAR(); ++i) if (Data::DTIA()[j][i]) fprintf(out, "%9s;",
			Data::TDSA()[i][((*sa[pd]->WA())[od - 1]->Prop().Data()
				& Data::TDMA()[i][0]) / Data::TDMA()[i][1]]);
			else fprintf(out, "         ;");
		fprintf(out, "\n");
	}
}

void Entry::PrintHeader(FILE * out, int x) {
	int i;
	if (!x) {
		fprintf(out, "Предл.: [суб-кт:\"        Слово       \",\
\"       Лексема      \"( POS");
		for (i = 0; i < Data::DTAR(); ++i) fprintf(out, " %9s", Data::DTSA()[i]);
		fprintf(out, ")]<-      Тип связи     -[обьект:\
\"        Слово       \",\"       Лексема      \"( POS");
		for (i = 0; i < Data::DTAR(); ++i) fprintf(out, " %9s", Data::DTSA()[i]);
		fprintf(out, ")]\n");
	} else {
		fprintf(out, "        Слово       ;       Лексема      ; POS;");
		for (i = 0; i < Data::DTAR(); ++i) fprintf(out, "%9s;", Data::DTSA()[i]);
		fprintf(out, "      Тип связи     ;        Слово       ;\
       Лексема      ; POS;");
		for (i = 0; i < Data::DTAR(); ++i) fprintf(out, "%9s;", Data::DTSA()[i]);
		fprintf(out, "\n");
	}
}

Word * Entry::Input(FILE * in, Array<Entry> & la, char & c, int p) {
	Array<char> buf;
	char * str;
	int i = 0, j, k, o, s;
	Word * sb;
	if (feof(in)) return 0;
	if (c != '$') buf += new char(c);
	else buf += new char(fgetc(in));
	if (feof(in)) throw new WithStr(20, emsa[23], 0);
	buf += new char(fgetc(in));
	while ((*buf[buf.e()] != '<') && !feof(in))
		buf += new char(fgetc(in));
	if (*buf[buf.e()] == '<') c = '<';
	*buf[buf.e()] = 0;
	str = Array<char>::Arr2Str(buf);
	buf.Reset();
	if (!ReadStr(str, emsa[23], i)) throw new WithStr(16, str, 0);
	if (ReadStr(str, "_root", i)) o = 0;
	else o = Str2Int(str, i);
	if (!ReadStr(str, emsa[0], i)) throw new With2Str(17, str, emsa[0], 1, 0);
	sb = new Word;
	for (j = 0; j < Data::PSPR(); ++j) if (ReadStr(str, Data::PSSP()[j], i)) {
		sb->Prop().SetSP(j);
		break;
	}
	if (j == Data::PSPR()) throw new WithStr(18, str, 1);
	for (;;) {
		if (str[i++] != ' ') if (str[--i] == '"') break;
		else throw new With2Str(17, str, emsa[1], 1, 0);
		if (str[i] == '"') break;
		for (j = 0; j < Data::DTAR(); ++j) {
			for (k = 0; k < Data::TDAR()[j]; ++k)
				if (ReadStr(str, Data::TDSA()[j][k], i)) {
					sb->Prop().SetData(j, k);
					break;
				}
			if (k < Data::TDAR()[j]) break;
		}
		if (j == Data::DTAR()) throw new WithStr(25, str, 1);
	}
	if (!ReadStr(str, emsa[2], i)) throw new With2Str(17, str, emsa[2], 1, 0);
	s = Str2Int(str, i);
	sb->Set(s);
	if (!ReadStr(str, emsa[3], i)) throw new With2Str(17, str, emsa[3], 1, 0);
	while (str[i] != '"') buf += new char(str[i++]);
	buf += new char(0);
	sb->Prop().SetLxm(Array<char>::Arr2Str(buf));
	buf.Reset();
	if (o) {
		if (!ReadStr(str, emsa[4], i)) throw new With2Str(17, str, emsa[4], 1, 0);
		for (j = 0; j < Data::SLAR(); ++j) if (ReadStr(str, Data::SLSA()[j], i)) {
			la += new Entry(j, o, s, p);
			break;
		}
		if (j == Data::SLAR()) throw new WithStr(19, str, 1);
	}
	if (ReadStr(str, "\" NODETYPE=\"FANTOM\"/>", i)) sb->Prop().SetData(2, 1);
	else {
		if (!ReadStr(str, emsa[5], i)) throw new With2Str(17, str, emsa[5], 1, 0);
		while (str[i] != 0) buf += new char(str[i++]);
		buf += new char(0);
		sb->Txt() = Array<char>::Arr2Str(buf);
		buf.Reset();
		delete str;
		buf += new char(c);
		if (feof(in)) throw new WithStr(20, emsa[6], 0);
		buf += new char(fgetc(in));
		while ((*buf[buf.e()] != '<') && !feof(in))
			buf += new char(fgetc(in));
		if (*buf[buf.e()] == '<') c = '<';
		*buf[buf.e()] = 0;
		str = Array<char>::Arr2Str(buf);
		i = 0;
		buf.Reset();
		if (!ReadStr(str, emsa[6], i)) throw new With2Str(17, str, emsa[6], 1, 0);
	}
	while (str[i] != 0) {
		if ((str[i] != '\n') && (str[i] != '\r')) sb->Pnct() += new char(str[i]);
		++i;
	}
	delete str;
	return sb;
}

int Entry::ReadStr(const char * s, const char * c, int & j, bool x) {
	int i = 0;
	while ((s[j] == c[i]) && (c[i] != 0)) {	++j; ++i;	}
	if ((c[i] == 0) && (x || (s[j] == '"'))) return 1;
	j -= i;
	return 0;
}

int Entry::Str2Int(const char * s, int & j) {
	int r = 0;
	while ((s[j] >= '0') && (s[j] <= '9')) (r *= 10) += s[j++] - '0';
	while (s[j] == ' ') ++j;
	return r;
}

float Entry::Str2Float(const char * s, int & j) {
	float r = 0.0, m = 1.0;
	while ((s[j] >= '0') && (s[j] <= '9')) (r *= 10) += s[j++] - '0';
	if (s[j] != '.') {
		while (s[j] == ' ') ++j;
		return r;
	} else ++j;
	while ((s[j] >= '0') && (s[j] <= '9')) r += (s[j++] - '0')*(m /= 10);
	while (s[j] == ' ') ++j;
	return r;
}

void Entry::ReadFile(FILE * in, int NTPE,
	Array <Sentense> & sa, Array <Link> & la, int c)
{
	int sr = sa.e(), i, j, k, ep, es, eo, ei, * ser = new int[sr + 1];
	char * s;
	Array<char> buf;
	Sentense * ns;
	Word * w;
	Entry * e;
	for (j = 0; j < sr; ++j) if (sa.OK(j)) ser[j] = sa[j]->EA()->e(); else {
		sa + j = new Sentense(NTPE, j, new Array <char>,
			new Array <Word>, new Array<Entry>);
		ser[j] = -1;
	}
	while (!feof(in)) {
		while ((c != '\n') && !feof(in)) {
			buf += new char(c);
			c = fgetc(in);
		}
		buf += new char(0);
		s = Array<char>::Arr2Str(buf);
		buf.Reset();
		c = fgetc(in);
		i = 0;
		while (s[i] == ' ') ++i;
		ep = Str2Int(s, i);
		if (!sa.OK(sr + ep)) ns = (sa + (sr + ep)) =
			new Sentense(NTPE, sr + ep, new Array<char>(),
				new Array<Word>(), new Array<Entry>());
		else ns = sa[sr + ep];
		if (s[i++] != ':') throw new WithChar(27, s[--i]);
		if (s[i++] != ' ') throw new WithChar(27, s[--i]);
		if (s[i++] != '[') throw new WithChar(27, s[--i]);
		while (s[i] == ' ') ++i;
		es = Str2Int(s, i);
		if (!ns->WA()->OK(es - 1)) w = (*ns->WA() + (es - 1)) = new Word(es);
		else w = (*ns->WA())[es - 1];
		if (s[i++] != ':') throw new WithChar(27, s[--i]);
		if (s[i++] != '"') throw new WithChar(27, s[--i]);
		while (s[i] == ' ') ++i;
		while ((s[i] != '"') && s[i]) buf += new char(s[i++]);
		buf += new char(0);
		if (!s[i]) throw new WithStr(30, s, 1);
		w->Txt() = Array<char>::Arr2Str(buf);
		buf.Reset();
		if (s[i++] != '"') throw new WithChar(27, s[--i]);
		if (s[i++] != ',') throw new WithChar(27, s[--i]);
		if (s[i++] != '"') throw new WithChar(27, s[--i]);
		while (s[i] == ' ') ++i;
		while ((s[i] != '"') && s[i]) buf += new char(s[i++]);
		buf += new char(0);
		if (!s[i]) throw new WithStr(30, s, 1);
		w->Prop().SetLxm(Array<char>::Arr2Str(buf));
		buf.Reset();
		if (s[i++] != '"') throw new WithChar(27, s[--i]);
		if (s[i++] != '(') throw new WithChar(27, s[--i]);
		while (s[i] == ' ') ++i;
		for (j = 0; j < Data::PSPR(); ++j) if (ReadStr(s, Data::PSSP()[j], i)) break;
		if (j == Data::PSPR()) throw new WithStr(18, s, 1);
		w->Prop().SetSP(j);
		for (j = 0; j < Data::DTAR(); ++j) {
			while (s[i] == ' ') ++i;
			for (k = 0; k < Data::TDAR()[j]; ++k)
				if (ReadStr(s, Data::TDSA()[j][k], i)) break;
			if (k == Data::TDAR()[j]) continue;
			w->Prop().SetData(j, k);
		}
		if (s[i++] != ')') throw new WithChar(27, s[--i]);
		if (s[i++] != ']') throw new WithChar(27, s[--i]);
		if (s[i++] != '<') throw new WithChar(27, s[--i]);
		if (s[i++] != '-') throw new WithChar(27, s[--i]);
		while (s[i] == ' ') ++i;
		for (ei = 0; ei < Data::SLAR(); ++ei) if (ReadStr(s, Data::SLSA()[ei], i)) break;
		if (ei == Data::SLAR()) throw new WithStr(19, s, 1);
		if (s[i++] != '-') throw new WithChar(27, s[--i]);
		if (s[i++] != '[') throw new WithChar(27, s[--i]);
		while (s[i] == ' ') ++i;
		eo = Str2Int(s, i);
		if (!ns->WA()->OK(eo - 1)) w = (*ns->WA() + (eo - 1)) = new Word(eo);
		else w = (*ns->WA())[eo - 1];
		if (s[i++] != ':') throw new WithChar(27, s[--i]);
		if (s[i++] != '"') throw new WithChar(27, s[--i]);
		while (s[i] == ' ') ++i;
		while ((s[i] != '"') && s[i]) buf += new char(s[i++]);
		buf += new char(0);
		if (!s[i]) throw new WithStr(30, s, 1);
		w->Txt() = Array<char>::Arr2Str(buf);
		buf.Reset();
		if (s[i++] != '"') throw new WithChar(27, s[--i]);
		if (s[i++] != ',') throw new WithChar(27, s[--i]);
		if (s[i++] != '"') throw new WithChar(27, s[--i]);
		while (s[i] == ' ') ++i;
		while ((s[i] != '"') && s[i]) buf += new char(s[i++]);
		buf += new char(0);
		if (!s[i]) throw new WithStr(30, s, 1);
		w->Prop().SetLxm(Array<char>::Arr2Str(buf));
		buf.Reset();
		if (s[i++] != '"') throw new WithChar(27, s[--i]);
		if (s[i++] != '(') throw new WithChar(27, s[--i]);
		while (s[i] == ' ') ++i;
		for (j = 0; j < Data::PSPR(); ++j) if (ReadStr(s, Data::PSSP()[j], i)) break;
		if (j == Data::PSPR()) throw new WithStr(18, s, 1);
		w->Prop().SetSP(j);
		for (j = 0; j < Data::DTAR(); ++j) {
			while (s[i] == ' ') ++i;
			for (k = 0; k < Data::TDAR()[j]; ++k)
				if (ReadStr(s, Data::TDSA()[j][k], i)) break;
			if (k == Data::TDAR()[j]) continue;
			w->Prop().SetData(j, k);
		}
		if (s[i++] != ')') throw new WithChar(27, s[--i]);
		if (s[i++] != ']') throw new WithChar(27, s[--i]);
		if (s[i++] != 0) throw new WithChar(27, s[--i]);
		e = new Entry(ei, eo, es, sr + ep);
		*ns->EA() += e;
		la[ei]->AddEntry(e, *ns->WA(), 0);
	}
	for (j = 0; j <= sr; ++j) sa[j]->AddEntries(ser[j]);
	for (j = sr + 1; j <= sa.e(); ++j) {
		if (!sa.OK(j)) sa + j = new Sentense(NTPE, j,
			new Array <char>, new Array <Word>, new Array <Entry>);
		sa[j]->AddEntries(-1);
	}
	delete [] ser;
}

void Sentense::AddEntries(int m) {
	int i, j, k, l, r;
	for (i = m + 1; i <= ea->e(); ++i) {
		l = 0;
		for (j = 0; j <= ea->e(); ++j)
			if ((*ea)[i]->SD() == (*ea)[j]->OD()) ++l;
		if (wa->e() - l >= NTPE) l = NTPE; else l = wa->e() - l;
		for (k = 0; k < l; ++k) {
			for (;;) {
				r = rand()%(wa->e() + 1);
				if (r + 1 == (*ea)[i]->SD()) continue;
				for (j = 0; j <= ea->e(); ++j) if (((*ea)[j]->OD() == (*ea)[i]->
					SD()) && ((*ea)[j]->SD() == r + 1)) break;
				if (j <= ea->e()) continue;
				for (j = 0; j <= ua->e(); ++j) if (((*ua)[j]->OD() == (*ea)[i]->
					SD()) && ((*ua)[j]->SD() == r + 1)) break;
				if (j > ua->e()) break;
			}
			*ua += new Entry(-1, (*ea)[i]->SD(), r + 1, id);
		}
	}
}

Sentense::Sentense(int ntpe, int l, Array <char> * p, Array <Word> * w, Array <
	Entry> * e) : NTPE(ntpe), id(l), pa(p), wa(w), ea(e), ua(new Array <Entry>)
{
	AddEntries(-1);
}

Sentense::~Sentense() {
	delete pa;
	delete wa;
	delete ea;
	delete ua;
}

void Sentense::Output(FILE * out) {
	int i;
	for (i = 0; i <= pa->e(); ++i) fprintf(out, "%c", *(*pa)[i]);
	for (i = 0; i <= wa->e(); ++i) (*wa)[i]->Print(out);
}

void Sentense::Cat(FILE * out) {
	int i;
	fprintf(out, "Предложение №%i:\n", id);
	for (i = 0; i <= wa->e(); ++i) (*wa)[i]->Prop().Print(out);
}

void Sentense::PrintEntry(FILE * out, Array<Sentense> & sa, int x) {
	int i;
	for (i = 0; i <= ua->e(); ++i) if (ua->OK(i)) (*ua)[i]->Output(out, sa, x);
}

Link::Link(const Link & l, const Array <Sentense> & sa) : id(l.id), arr(l.arr),
	ob(new Node()), sb(new Node()), en(new Node()), dn(new Node()), acc(1)
{
	ReEnter(sa);
}

Link::Link(int i, int a) : id(i), arr(a), ob(new Node()),
	sb(new Node()), en(new Node()), dn(new Node()), acc(1) {}

Link::~Link() {
	delete ob;
	delete sb;
	delete en;
	delete dn;
}

int Link::ReadXML(FILE * in, int i, char & c, int x, int & d) {
	if (!Data::ReadStr(in, emsa[i], c, d))
		if (x) throw new WithStr(20, emsa[i], 0);
	else return 0;
	d = 0;
	while ((c != '<') && !feof(in)) c = fgetc(in);
	if (c != '<') if (x) throw new WithStr(20, emsa[20], 0);
	else throw new WithStr(20, emsa[i + 1], 0);
	return 1;
}

void Link::Input(FILE * in, int NTPE, Array<Sentense> & sa, Array<Link> & la) {
	char c;
	int sr = sa.e(), i, j, o = 0, s, d = 0;
	const char * str;
	Word * w;
	Array<Word> * wa;
	Array<Entry> * ea;
	Array<char> * ca;
	c = fgetc(in);
	ReadXML(in, 7, c, 1, d);
	ReadXML(in, 8, c, 1, d);
	if (ReadXML(in, 9, c, 0, d)) {
		if (ReadXML(in, 10, c, 0, d)) ReadXML(in, 11, c, 1, d);
		if (ReadXML(in, 12, c, 0, d)) ReadXML(in, 13, c, 1, d);
		if (ReadXML(in, 29, c, 0, d)) ReadXML(in, 30, c, 1, d);
		if (ReadXML(in, 14, c, 0, d)) ReadXML(in, 15, c, 1, d);
		if (ReadXML(in, 16, c, 0, d)) ReadXML(in, 17, c, 1, d);
		if (ReadXML(in, 27, c, 0, d)) ReadXML(in, 28, c, 1, d);
		if (ReadXML(in, 18, c, 0, d)) ReadXML(in, 19, c, 1, d);
		ReadXML(in, 20, c, 1, d);
	}
	ReadXML(in, 21, c, 1, d);
	for (;;) {
		ca = new Array<char>();
		do {	*ca += new char(c);
		} while ((c = fgetc(in)) != '<');
		*ca += new char(0);
		str = Array<char>::Arr2Str(*ca);
		ca->Reset();
		i = 0;
		if (Entry::ReadStr(str, emsa[25], i)) {
			(*wa)[wa->e()]->Pnct() += new char('\n');
			(*wa)[wa->e()]->Pnct() += new char('\n');
			break;
		}
		if (!Entry::ReadStr(str, emsa[22], i))
			throw new With2Str(17, str, emsa[22], 1, 0);
		s = Entry::Str2Int(str, i);
		if (s != o + 1) throw new WithStrShort(21, o + 1, str, 1);
		if (!Entry::ReadStr(str, emsa[5], i))
			throw new With2Str(17, str, emsa[5], 1, 0);
		while (str[i] != 0) {
			if (!Word::Space(str[i])) *ca += new char(str[i]);
			++i;
		}
		delete str;
		wa = new Array<Word>();
		ea = new Array<Entry>();
		try {	while (0 != (w = Entry::Input(in, *ea, c, sr + s))) *wa += w;
		} catch (WithStr * e) {
			if (e->ID() != 16) throw;
			str = e->Data();
			delete e;
		}
		i = 0;
		if (!Entry::ReadStr(str, emsa[24], i))
			throw new With2Str(17, str, emsa[24], 1, 0);
		while (str[i] != 0) {
			if (!Word::Space(str[i]))
				throw new WithCharStr(s, str[i], str, 0);
			++i;
		}
		delete str;
		sa += new Sentense(NTPE, sr + s, ca, wa, ea);
		o = s;
	}
	delete str;
	delete ca;
	if (!Data::ReadStr(in, emsa[26], c, d)) throw new WithStr(20, emsa[26], 0);
	for (j = sr + 1; j <= sa.e(); ++j) for (i = 0; i <= sa[j]->EA()->e(); ++i)
		la[(*sa[j]->EA())[i]->ID()]->AddEntry((*sa[j]->EA())[i], *sa[j]->WA());
	for (i = 0; i < Data::SLAR(); ++i) la[i]->Finalize();
}

void Link::PrintHeader(FILE * out, int x) {
	int i, j, k;
	if (!x) fprintf(out, "%s", Data::TDUA());
	else fprintf(out, "%s", Data::TDXA());
	for (i = 0; i < Data::SUM(); ++i) {
		if (!x) fprintf(out, " Dist%2i", i);
		else fprintf(out, " Dist%2i;", i);
	}
	if (!x) fprintf(out, "}:{[");
	for (i = 0; i < Data::PSPR(); ++i) {
		for (j = 0; j < Data::PSLM() - Data::PSLA()[i]; ++j) fprintf(out, " ");
		if (!x) fprintf(out, "1Ps%s", Data::PSSP()[i]);
		else fprintf(out, "1Ps%s;", Data::PSSP()[i]);
	}
	for (i = 0; i < Data::DTAR(); ++i) {
		if (!x) fprintf(out, "] [");
		for (j = 0; j < Data::TDAR()[i]; ++j) {
			for (k = 0; k < Data::TDLM()[i] - Data::TDLA()[i][j]; ++k)
				fprintf(out, " ");
			if (!x) fprintf(out, "1%s", Data::TDNA()[i][j]);
			else fprintf(out, "1%s;", Data::TDNA()[i][j]);
		}
	}
	if (!x) fprintf(out, "]}<-   Тип связи   -{[");
	else fprintf(out, "      Тип связи;");
	for (i = 0; i < Data::PSPR(); ++i) {
		for (j = 0; j < Data::PSLM() - Data::PSLA()[i]; ++j) fprintf(out, " ");
		if (!x) fprintf(out, "2Ps%s", Data::PSSP()[i]);
		else fprintf(out, "2Ps%s;", Data::PSSP()[i]);
	}
	for (i = 0; i < Data::DTAR(); ++i) {
		if (!x) fprintf(out, "] [");
		for (j = 0; j < Data::TDAR()[i]; ++j) {
			for (k = 0; k < Data::TDLM()[i] - Data::TDLA()[i][j]; ++k)
				fprintf(out, " ");
			if (!x) fprintf(out, "2%s", Data::TDNA()[i][j]);
			else fprintf(out, "2%s;", Data::TDNA()[i][j]);
		}
	}
	if (!x) fprintf(out, "]}");
	fprintf(out, "\n");
}

void Link::Load(FILE * in, char & c) {
	int i = 0;
	if (c == '$') c = fgetc(in);
	if (c != '{') throw new WithChar(27, c); else c = fgetc(in);
	en->Load(in, c, 1);
	if (c != '}') throw new WithChar(27, c); else c = fgetc(in);
	if (c != '~') throw new WithChar(27, c); else c = fgetc(in);
	if (c != '{') throw new WithChar(27, c); else c = fgetc(in);
	dn->Load(in, c, 2);
	if (c != '}') throw new WithChar(27, c); else c = fgetc(in);
	if (c != ':') throw new WithChar(27, c); else c = fgetc(in);
	if (c != '{') throw new WithChar(27, c); else c = fgetc(in);
	sb->Load(in, c);
	if (c != '}') throw new WithChar(27, c); else c = fgetc(in);
	if (c != '<') throw new WithChar(27, c); else c = fgetc(in);
	if (c != '-') throw new WithChar(27, c); else c = fgetc(in);
	while (c == ' ') c = fgetc(in);
	if (!Data::ReadStr(in, Data::SLSA()[id], c, i)) throw new WithStr(28, Data::SLSA()[id], 0);
	if (c != '-') throw new WithChar(27, c); else c = fgetc(in);
	if (c != '{') throw new WithChar(27, c); else c = fgetc(in);
	ob->Load(in, c);
	if (c != '}') throw new WithChar(27, c); else c = fgetc(in);
	if (c != '\n') throw new WithChar(27, c); else c = fgetc(in);
}

void Link::AddEntry(Entry * e, Array<Word> & a, int x) {
	if (!acc) throw new Exception(15);
	arr += e;
	if (x) {
		ob->AddEntry(a[e->OD() - 1]);
		sb->AddEntry(a[e->SD() - 1]);
		en->AddEntry(a[e->OD() - 1], a[e->SD() - 1]);
		dn->AddEntry(a[e->OD() - 1], a[e->SD() - 1], 1);
	}
}

void Link::ReEnter(const Array <Sentense> & sa) {
	int i;
	if (!acc) throw new Exception(15);
	ob->Clear();
	sb->Clear();
	en->Clear();
	dn->Clear();
	for (i = 0; i <= arr.e(); ++i) if (arr.OK(i)) {
		ob->AddEntry((*sa[arr[i]->PD()]->WA())[arr[i]->OD() - 1]);
		sb->AddEntry((*sa[arr[i]->PD()]->WA())[arr[i]->SD() - 1]);
		en->AddEntry((*sa[arr[i]->PD()]->WA())[arr[i]->OD() - 1],
			(*sa[arr[i]->PD()]->WA())[arr[i]->SD() - 1]);
		dn->AddEntry((*sa[arr[i]->PD()]->WA())[arr[i]->OD() - 1],
			(*sa[arr[i]->PD()]->WA())[arr[i]->SD() - 1], 1);
	}
	Finalize();
}

void Link::Finalize() {
	ob->Finalize();
	sb->Finalize();
	en->Finalize(1);
	dn->Finalize(2);
	acc = 0;
}

void Link::Reset() {
	ob->Terminate();
	ob->Start();
	sb->Terminate();
	sb->Start();
	en->Terminate();
	en->Start();
	dn->Terminate();
	dn->Start();
	acc = 1;
}

void Link::Output(FILE * out, int x) {
	if (!x) fprintf(out, "{");
	en->Print(out, x, 1);
	if (!x) fprintf(out, "}~{");
	dn->Print(out, x, 2);
	if (!x) fprintf(out, "}:{");
	sb->Print(out, x);
	if (!x) fprintf(out, "}<-%15s-{", Data::SLSA()[id]);
	else fprintf(out, "%15s;", Data::SLSA()[id]);
	ob->Print(out, x);
	if (!x) fprintf(out, "}\n"); else fprintf(out, "\n");
}

void Link::Top(FILE * out, int j, int x) {
	fprintf(out, "%s:{", Data::SLSA()[id]);
	Node::Top(ob, sb, en, out, j, x);
	fprintf(out, "}\n");
}

void Link::PrintEntry(FILE * out, Array<Sentense> & sa, int x) {
	int i;
	for (i = 0; i <= arr.e(); ++i) if (arr.OK(i)) arr[i]->Output(out, sa, x);
}
