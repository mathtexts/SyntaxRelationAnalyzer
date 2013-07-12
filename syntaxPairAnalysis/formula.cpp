#include "formula.h"

void DoNothing(void * null) {}

FILE * Computing::FileOpen(const char * s, int & i, const char * p) {
	Array<char> ca;
	char * str;
	FILE * res;
	while ((s[i] != 0) && (s[i] != '?')) ca += new char(s[i++]);
	ca += new char(0);
	str = Array<char>::Arr2Str(ca);
	res = fopen(str, p);
	if (!res) throw new WithStr(11, str, 1);
	delete str;
	return res;
}

Computing::Computing(const Array <Sentense> & s, const Array <Link> & l,
	Array <Formula> & f, float p1c, float p2c) : P1C(p1c), P2C(p2c), la()
{
	int i;
	f.Reset();
	for (i = 0; i <= l.e(); ++i) {
		la += new Link(*l[i], s);
	}
	BuildLink(f, s);
}

Computing::~Computing() {}

void Computing::BuildLink(Array <Formula> & f, const Array <Sentense> & sa) {
	Disjunction *** odis = new Disjunction **[la.e() + 1],
				*** sdis = new Disjunction **[la.e() + 1],
				*** edis = new Disjunction **[la.e() + 1],
				** ddis = new Disjunction *[la.e() + 1];
	Conjunction ** fcon = new Conjunction *[la.e() + 1];
	int i, * imax = new int[Data::SLAR()], lmax;
	bool b = false, * c = new bool[Data::SLAR()], d = false;
	float * p3 = new float[la.e() + 1];
	for (i = 0; i <= la.e(); ++i) {
		f += new Disjunction(-1);
		ODis(i, odis[i] = new Disjunction *[Data::DTAR() + 1]);
		SDis(i, sdis[i] = new Disjunction *[Data::DTAR() + 1]);
		EDis(i, edis[i] = new Disjunction *[Data::DTAR() + 1]);
		DDis(i, ddis[i]);
		fcon[i] = new Conjunction;
		p3[i] = -1.0;
		c[i] = false;
	}
	for (;;) if (Cycle(lmax, imax, p3, b, d, c, f,
		fcon, sa, odis, sdis, edis, ddis)) break;
	for (i = 0; i <= f.e(); ++i) if (P1C > f[i]->Value())
		((Disjunction *)f[i])->Terminate();
	delete [] imax;
	delete [] c;
	delete [] p3;
	for (i = 0; i <= la.e(); ++i) {
		delete [] odis[i];
		delete [] sdis[i];
		delete [] edis[i];
	}
	delete [] odis;
	delete [] sdis;
	delete [] edis;
	delete [] ddis;
	delete [] fcon;
}

bool Computing::Cycle(int& lmax, int*& imax, float*& p3, bool& b, bool& d,
	bool*& c, Array<Formula>& f, Conjunction**& fcon, const Array<Sentense>&
	sa, Disjunction***& odis, Disjunction***& sdis, Disjunction***& edis,
	Disjunction**& ddis, void* p, void(*signal)(void*))
{
	int i, j;
	for (;;) if (1 == (i = SubCycle(lmax, imax, p3, b, d, c, f,
		fcon, sa, odis, sdis, edis, ddis))) break; else if (!i) signal(p);
	if (!b) return true;
	for (i = 0; i <= la.e(); ++i) {
		if (fcon[i]) {
			for (j = 0; j <= Data::DTAR(); ++j) {
				delete odis[i][j];
				delete sdis[i][j];
				delete edis[i][j];
				odis[i][j] = 0;
				sdis[i][j] = 0;
				edis[i][j] = 0;
			}
			delete ddis[i];
			ddis[i] = 0;
			for (j = 0; j <= fcon[i]->Data().e(); ++j)
				if (((Disjunction *)fcon[i]->Data()[j])->OTT())
					delete (fcon[i]->Data() - j);
			fcon[i]->Data().Compact();
			if (-1 != fcon[i]->Data().e()) {
				if (!(d = (p3[i] > P2C))) fcon[i]->Terminate();
				c[i] = false;
				*((Disjunction *)f[i]) += fcon[i];
				if ((-1 != P1C) && (P1C < f[i]->Set
					(MultiFormula::P1(f, i, sa, la))))
				{
					fcon[i] = 0;
					fprintf(stdout, ".");
					continue;
				}
				if (d) if (!(d = fcon[i]->Filter(fcon, la, sa, *la[i])))
					((Conjunction *)((Disjunction *)f[i])->Data()
					[((Disjunction *)f[i])->Data().e()])->Terminate();
				fcon[i] = 0;
				if (d) {
					fprintf(stdout, ".");
					ODis(i, odis[i]);
					SDis(i, sdis[i]);
					EDis(i, edis[i]);
					DDis(i, ddis[i]);
					fcon[i] = new Conjunction;
					p3[i] = -1.0;
					b = false;
				}
			} else {
				delete fcon[i];
				fcon[i] = 0;
			}
		}
		if (!fcon[i] && c[i] && (P1C >= f[i]->Set
			(MultiFormula::P1(f, i, sa, la))))
		{
			ODis(i, odis[i]);
			SDis(i, sdis[i]);
			EDis(i, edis[i]);
			DDis(i, ddis[i]);
			fcon[i] = new Conjunction;
			p3[i] = -1.0;
			c[i] = false;
			b = false;
		}
	}
	fprintf(stdout, "\n");
	return b;
}

int Computing::SubCycle(int & lmax, int *& imax, float *& p3, bool & b,
	bool & d, bool *& c, Array <Formula> & f, Conjunction **& fcon,
	const Array <Sentense> & sa, Disjunction ***& odis, Disjunction ***& sdis,
	Disjunction ***& edis, Disjunction **& ddis)
{
	int r = 0;
	Max(imax, c, fcon, sa, odis, sdis, edis, ddis);
	lmax = MultiFormula::P3(fcon, odis, sdis, edis, ddis, imax, sa, la);
	if (-1 == lmax) return 1;
	if (imax[lmax] == 3*Data::DTAR() + 3) {
		*fcon[lmax] += ddis[lmax];
		if (c[lmax] = (-1 != P2C) && (p3[lmax] > fcon[lmax]->Set
			(ddis[lmax]->Value()))) r = 2;
		if (c[lmax]) ddis[lmax]->Terminate();
		else p3[lmax] = fcon[lmax]->Value();
		ddis[lmax] = 0;
	} else if (imax[lmax] <= Data::DTAR()) {
		*fcon[lmax] += odis[lmax][imax[lmax]];
		if (c[lmax] = (-1 != P2C) && (p3[lmax] > fcon[lmax]->Set
			(odis[lmax][imax[lmax]]->Value()))) r = 2;
		if (c[lmax]) odis[lmax][imax[lmax]]->Terminate();
		else p3[lmax] = fcon[lmax]->Value();
		odis[lmax][imax[lmax]] = 0;
	} else if (imax[lmax] <= 2*Data::DTAR() + 1) {
		*fcon[lmax] += sdis[lmax][imax[lmax] - Data::DTAR() - 1];
		if (c[lmax] = (-1 != P2C) && (p3[lmax] > fcon[lmax]->Set
			(sdis[lmax][imax[lmax] - Data::DTAR() - 1]->Value()))) r = 2;
		if (c[lmax]) sdis[lmax][imax[lmax] - Data::DTAR() - 1]->Terminate();
		else p3[lmax] = fcon[lmax]->Value();
		sdis[lmax][imax[lmax] - Data::DTAR() - 1] = 0;
	} else {
		*fcon[lmax] += edis[lmax][imax[lmax] - 2*Data::DTAR() - 2];
		if (c[lmax] = (-1 != P2C) && (p3[lmax] > fcon[lmax]->Set
			(edis[lmax][imax[lmax] - 2*Data::DTAR() - 2]->Value()))) r = 2;
		if (c[lmax]) edis[lmax][imax[lmax] - 2*Data::DTAR() - 2]->Terminate();
		else p3[lmax] = fcon[lmax]->Value();
		edis[lmax][imax[lmax] - 2*Data::DTAR() - 2] = 0;
	}
	fcon[lmax]->Set(p3[lmax]);
	if (p3[lmax] > P2C) if (-1 == P2C) {
		b = true;
		fprintf(stdout, ".");
	} else AddConjunction(odis, sdis, edis, ddis, fcon, sa, f, p3, lmax, b, d);
	else if (!c[lmax]) AddNegative(lmax, imax, sa,
		f, fcon, odis, sdis, edis, ddis, p3, b, d);
	c[lmax] = false;
	return r;
}

void Computing::AddConjunction(Disjunction ***& odis, Disjunction ***& sdis,
	Disjunction ***& edis, Disjunction **& ddis, Conjunction **& fcon,
	const Array <Sentense> & sa, Array <Formula> & f, float *& p3,
	int & lmax, bool & b, bool & d)
{
	int i;
	for (i = 0; i <= Data::DTAR(); ++i) {
		delete odis[lmax][i];
		delete sdis[lmax][i];
		delete edis[lmax][i];
		odis[lmax][i] = 0;
		sdis[lmax][i] = 0;
		edis[lmax][i] = 0;
	}
	delete ddis[lmax];
	ddis[lmax] = 0;
	for (i = 0; i <= fcon[lmax]->Data().e(); ++i)
		if (((Disjunction *)fcon[lmax]->Data()[i])->OTT())
			delete (fcon[lmax]->Data() - i);
	fcon[lmax]->Data().Compact();
	*((Disjunction *)f[lmax]) += fcon[lmax];
	if ((-1 != P1C) && (P1C < f[lmax]->Set
		(MultiFormula::P1(f, lmax, sa, la))))
	{
		fcon[lmax] = 0;
		b = true;
		fprintf(stdout, ".");
		return;
	}
	if (!(d = fcon[lmax]->Filter(fcon, la, sa, *la[lmax])))
		((Conjunction *)((Disjunction *)f[lmax])->Data()
		[((Disjunction *)f[lmax])->Data().e()])->Terminate();
	fcon[lmax] = 0;
	if (d) {
		b = true;
		fprintf(stdout, ".");
		ODis(lmax, odis[lmax]);
		SDis(lmax, sdis[lmax]);
		EDis(lmax, edis[lmax]);
		DDis(lmax, ddis[lmax]);
		fcon[lmax] = new Conjunction;
		p3[lmax] = -1.0;
	}
}

void Computing::AddNegative(int lmax, int * imax, const Array <Sentense> &
	sa, Array <Formula> & f, Conjunction **& fcon, Disjunction ***& odis,
	Disjunction ***& sdis, Disjunction ***& edis, Disjunction **& ddis,
	float *& p3, bool & b, bool & d)
{
	float fmax;
	int i, j, l;
	Word * ow, * sw;
	Disjunction * umax, * udis;
	Link * tmp = new Link(lmax);
	for (i = 0; i < Data::SLAR(); ++i) if (i != lmax)
		for (j = 0; j <= la[i]->Arr().e(); ++j) {
			if (!la[i]->Arr().OK(j)) continue;
			ow = (*sa[la[i]->Arr()[j]->PD()]->WA())
				[la[i]->Arr()[j]->OD() - 1];
			sw = (*sa[la[i]->Arr()[j]->PD()]->WA())
				[la[i]->Arr()[j]->SD() - 1];
			if (fcon[lmax]->Test(ow, sw))
				tmp->AddEntry(la[i]->Arr()[j],
				*sa[la[i]->Arr()[j]->PD()]->WA());
		}
	tmp->Finalize();
	if (!tmp->OS()[0]) return;
	fmax = -1.0;
	umax = 0;
	ODis0(udis, lmax, fcon, tmp, sa, fmax, umax);
	SDis0(udis, lmax, fcon, tmp, sa, fmax, umax);
	EDis0(udis, lmax, fcon, tmp, sa, fmax, umax);
	DDis0(udis, lmax, fcon, tmp, sa, fmax, umax);
	for (l = 1; l <= Data::DTAR(); ++l) {
		ODisL(l, udis, lmax, fcon, tmp, sa, fmax, umax);
		SDisL(l, udis, lmax, fcon, tmp, sa, fmax, umax);
		EDisL(l, udis, lmax, fcon, tmp, sa, fmax, umax);
	}
	((Disjunction *)fcon[lmax]->Data()[fcon[lmax]->Data().e()])->Un(umax);
	delete tmp;
	udis = new Disjunction(0);
	*udis += new Literal(EQLS, 0, 0, 1, 1);
	*udis += new Literal(EQLS, 0, 1, 1, 1);
	if (imax[lmax] == 3*Data::DTAR() + 3) ddis[lmax] = udis;
	else if (imax[lmax] <= Data::DTAR()) odis[lmax][imax[lmax]] = udis;
	else if (imax[lmax] <= 2*Data::DTAR() + 1)
		sdis[lmax][imax[lmax] - Data::DTAR() - 1] = udis;
	else edis[lmax][imax[lmax] - 2*Data::DTAR() - 2] = udis;
	if (lmax != MultiFormula::P3(fcon,
		odis, sdis, edis, ddis, imax, sa, la))
	{
		delete ((Disjunction *)fcon[lmax]->
			Data()[fcon[lmax]->Data().e()])->U();
		((Disjunction *)fcon[lmax]->Data()[fcon[lmax]->Data().e()])->Un(0);
		if (imax[lmax] == 3*Data::DTAR() + 3) ddis[lmax] = 0;
		else if (imax[lmax] <= Data::DTAR()) odis[lmax][imax[lmax]] = 0;
		else if (imax[lmax] <= 2*Data::DTAR() + 1)
			sdis[lmax][imax[lmax] - Data::DTAR() - 1] = 0;
		else edis[lmax][imax[lmax] - 2*Data::DTAR() - 2] = 0;
		delete udis;
	} else {
		if (p3[lmax] > fcon[lmax]->Set(udis->Value())) ((Disjunction *)
			fcon[lmax]->Data()[fcon[lmax]->Data().e()])->Terminate();
		else p3[lmax] = fcon[lmax]->Value();
		if (imax[lmax] == 3*Data::DTAR() + 3) ddis[lmax] = 0;
		else if (imax[lmax] <= Data::DTAR()) odis[lmax][imax[lmax]] = 0;
		else if (imax[lmax] <= 2*Data::DTAR() + 1)
			sdis[lmax][imax[lmax] - Data::DTAR() - 1] = 0;
		else edis[lmax][imax[lmax] - 2*Data::DTAR() - 2] = 0;
		delete udis;
		if (p3[lmax] > P2C) AddConjunction(odis, sdis,
			edis, ddis, fcon, sa, f, p3, lmax, b, d);
	}
}

void Computing::ODis0(Disjunction *& udis, int lmax, Conjunction ** fcon,
	Link * tmp, const Array <Sentense> & sa, float & fmax, Disjunction *& umax)
{
	Disjunction * tdis;
	int i, j, k, m, jmax;
	int max;
	udis = new Disjunction(0);
	for (i = 0; i < Data::PSPR(); ++i) {
		max = 0;
		jmax = -1;
		for (j = 0; j < Data::PSPR(); ++j) {
			if (la[lmax]->OData()[j] || !tmp->OData()[j]) continue;
			for (k = 0; k <= udis->Data().e(); ++k)
				if (((Literal *)udis->Data()[k])->M() == j) break;
			if (k <= udis->Data().e()) continue;
			for (k = 0; k <= fcon[lmax]->Data().e(); ++k) {
				tdis = (Disjunction *)fcon[lmax]->Data()[k];
				if ((-1 == tdis->Data().e()) ||
					(((Literal *)tdis->Data()[0])->T() != LEFT) ||
					(((Literal *)tdis->Data()[0])->N() != 0)) continue;
				for (m = 0; m <= tdis->Data().e(); ++m)
					if (((Literal *)tdis->Data()[m])->M() == j) break;
				if (m <= tdis->Data().e()) break;
			}
			if ((k > fcon[lmax]->Data().e()) && (max < tmp->OData()[j]))
				max = tmp->OData()[jmax = j];
		}
		if (-1 == jmax) break;
		if (0.0 == tmp->OS()[0]) *udis += new Literal(LEFT,
			0, jmax, max, 1.0); else *udis += new Literal(LEFT,
			0, jmax, max, max*1.0/tmp->OS()[0]);
		if ((-1 != P1C) && (udis->SUM() >= P1C*tmp->OS()[0])) break;
	}
	if (fmax < udis->Set(udis->Py(*tmp,
		(Disjunction *)fcon[lmax]->Data()[fcon[lmax]->Data().e()], sa)))
	{
		fmax = udis->Value();
		delete umax;
		umax = udis;
	} else delete udis;
}

void Computing::SDis0(Disjunction *& udis, int lmax, Conjunction ** fcon,
	Link * tmp, const Array <Sentense> & sa, float & fmax, Disjunction *& umax)
{
	Disjunction * tdis;
	int i, j, k, m, jmax;
	int max;
	udis = new Disjunction(0);
	for (i = 0; i < Data::PSPR(); ++i) {
		max = 0;
		jmax = -1;
		for (j = 0; j < Data::PSPR(); ++j) {
			if (la[lmax]->SData()[j] || !tmp->SData()[j]) continue;
			for (k = 0; k <= udis->Data().e(); ++k)
				if (((Literal *)udis->Data()[k])->M() == j) break;
			if (k <= udis->Data().e()) continue;
			for (k = 0; k <= fcon[lmax]->Data().e(); ++k) {
				tdis = (Disjunction *)fcon[lmax]->Data()[k];
				if ((-1 == tdis->Data().e()) ||
					(((Literal *)tdis->Data()[0])->T() != RGHT) ||
					(((Literal *)tdis->Data()[0])->N() != 0)) continue;
				for (m = 0; m <= tdis->Data().e(); ++m)
					if (((Literal *)tdis->Data()[m])->M() == j) break;
				if (m <= tdis->Data().e()) break;
			}
			if ((k > fcon[lmax]->Data().e()) && (max < tmp->SData()[j]))
				max = tmp->SData()[jmax = j];
		}
		if (-1 == jmax) break;
		if (0.0 == tmp->SS()[0]) *udis += new Literal(RGHT,
			0, jmax, max, 1.0); else *udis += new Literal(RGHT,
			0, jmax, max, max*1.0/tmp->SS()[0]);
		if ((-1 != P1C) && (udis->SUM() >= P1C*tmp->SS()[0])) break;
	}
	if (fmax < udis->Set(udis->Py(*tmp,
		(Disjunction *)fcon[lmax]->Data()[fcon[lmax]->Data().e()], sa)))
	{
		fmax = udis->Value();
		delete umax;
		umax = udis;
	} else delete udis;
}

void Computing::EDis0(Disjunction *& udis, int lmax, Conjunction ** fcon,
	Link * tmp, const Array <Sentense> & sa, float & fmax, Disjunction *& umax)
{
	Disjunction * tdis;
	int k, m;
	udis = new Disjunction(0);
	for (k = 0; k <= fcon[lmax]->Data().e(); ++k) {
		tdis = (Disjunction *)fcon[lmax]->Data()[k];
		if ((-1 == tdis->Data().e()) ||
			(((Literal *)tdis->Data()[0])->T() != EQLS) ||
			(((Literal *)tdis->Data()[0])->N() != 0)) continue;
		for (m = 0; m <= tdis->Data().e(); ++m)
			if (((Literal *)tdis->Data()[m])->M() == 0) break;
		if (m <= tdis->Data().e()) break;
	}
	if (la[lmax]->EData()[0] || (k <= fcon[lmax]->Data().e())
		|| !tmp->EData()[0])
	{
		for (k = 0; k <= fcon[lmax]->Data().e(); ++k) {
			tdis = (Disjunction *)fcon[lmax]->Data()[k];
			if ((-1 == tdis->Data().e()) ||
				(((Literal *)tdis->Data()[0])->T() != EQLS) ||
				(((Literal *)tdis->Data()[0])->N() != 0)) continue;
			for (m = 0; m <= tdis->Data().e(); ++m)
				if (((Literal *)tdis->Data()[m])->M() == 1) break;
			if (m <= tdis->Data().e()) break;
		}
		if (!la[lmax]->EData()[1] && (k > fcon[lmax]->Data().e()) &&
			tmp->EData()[1]) *udis += new Literal(EQLS, 0, 1,
			tmp->EData()[1], tmp->EData()[1]*1.0/tmp->ES()[0]);
	} else {
		for (k = 0; k <= fcon[lmax]->Data().e(); ++k) {
			tdis = (Disjunction *)fcon[lmax]->Data()[k];
			if ((-1 == tdis->Data().e()) ||
				(((Literal *)tdis->Data()[0])->T() != EQLS) ||
				(((Literal *)tdis->Data()[0])->N() != 0)) continue;
			for (m = 0; m <= tdis->Data().e(); ++m)
				if (((Literal *)tdis->Data()[m])->M() == 1) break;
			if (m <= tdis->Data().e()) break;
		}
		if (la[lmax]->EData()[1] || (k <= fcon[lmax]->Data().e()) ||
			!tmp->EData()[1]) *udis += new Literal(EQLS, 0, 0,
			tmp->EData()[0], tmp->EData()[0]*1.0/tmp->ES()[0]);
		else if (tmp->EData()[0] > tmp->EData()[1]) {
			*udis += new Literal(EQLS, 0, 0,
				tmp->EData()[0], tmp->EData()[0]*1.0/tmp->ES()[0]);
			if ((-1 == P1C) || (udis->SUM() < P1C*tmp->ES()[0]))
				*udis += new Literal(EQLS, 0, 1,
					tmp->EData()[1], tmp->EData()[1]*1.0/tmp->ES()[0]);
		} else if (tmp->EData()[1] > 0) {
			*udis += new Literal(EQLS, 0, 1,
				tmp->EData()[1], tmp->EData()[1]*1.0/tmp->ES()[0]);
			if ((-1 == P1C) || (udis->SUM() < P1C*tmp->ES()[0]))
				*udis += new Literal(EQLS, 0, 0,
					tmp->EData()[0], tmp->EData()[0]*1.0/tmp->ES()[0]);
		}
	}
	if (fmax < udis->Set(udis->Py(*tmp,
		(Disjunction *)fcon[lmax]->Data()[fcon[lmax]->Data().e()], sa)))
	{
		fmax = udis->Value();
		delete umax;
		umax = udis;
	} else delete udis;
}

void Computing::DDis0(Disjunction *& udis, int lmax, Conjunction ** fcon,
	Link * tmp, const Array <Sentense> & sa, float & fmax, Disjunction *& umax)
{
	Disjunction * tdis;
	int i, j, k, m, jmax;
	int max;
	udis = new Disjunction(0);
	for (i = 0; i < Data::SUM(); ++i) {
		max = 0;
		jmax = -1;
		for (j = 0; j < Data::SUM(); ++j) {
			if (la[lmax]->DData()[j] || !tmp->DData()[j]) continue;
			for (k = 0; k < i; ++k)
				if (((Literal *)udis->Data()[k])->M() == j) break;
			if (k < i) continue;
			for (k = 0; k <= fcon[lmax]->Data().e(); ++k) {
				tdis = (Disjunction *)fcon[lmax]->Data()[k];
				if ((-1 == tdis->Data().e()) ||
					(((Literal *)tdis->Data()[0])->T() != DIST)) continue;
				for (m = 0; m <= tdis->Data().e(); ++m)
					if ((((Literal *)tdis->Data()[m])->N() <= j) &&
						(((Literal *)tdis->Data()[m])->M() >= j)) break;
				if (m <= tdis->Data().e()) break;
			}
			if ((k > fcon[lmax]->Data().e()) && (tmp->DData()[j] > max))
				max = tmp->DData()[jmax = j];
		}
		if (-1 == jmax) break;
		if (0 == tmp->DS()) *udis += new Literal(DIST,
			jmax, jmax, max, 1.0); else *udis += new
			Literal(DIST, jmax, jmax, max, max*1.0/tmp->DS());
		if ((-1 != P1C) && (udis->SUM() >= P1C*tmp->DS())) break;
	}
	if (-1 == jmax) --i;
	max = udis->SUM();
	jmax = 0;
	j = Data::SUM();
	for (k = i; k >= 0; --k) {
		if (((Literal *)udis->Data()[k])->M() > jmax)
			jmax = ((Literal *)udis->Data()[k])->M();
		if (((Literal *)udis->Data()[k])->M() < j)
			j = ((Literal *)udis->Data()[k])->M();
		delete --*udis;
	}
	*udis += new Literal(DIST, jmax, j, max, max*1.0/tmp->DS());
	if (fmax < udis->Set(udis->Py(*tmp,
		(Disjunction *)fcon[lmax]->Data()[fcon[lmax]->Data().e()], sa)))
	{
		fmax = udis->Value();
		delete umax;
		umax = udis;
	} else delete udis;
}

void Computing::ODisL(int l, Disjunction *& udis, int lmax,
	Conjunction ** fcon, Link * tmp, const Array <Sentense> & sa,
	float & fmax, Disjunction *& umax)
{
	Disjunction * tdis;
	int i, j, k, m, jmax;
	int max;
	udis = new Disjunction(0);
	for (i = 0; i < Data::TDAR()[l - 1]; ++i) {
		max = 0;
		jmax = -1;
		for (j = 0; j < Data::TDAR()[l - 1]; ++j) {
			if (la[lmax]->OData()[Data::TDAS()[l - 1] + j] ||
				!tmp->OData()[Data::TDAS()[l - 1] + j]) continue;
			for (k = 0; k <= udis->Data().e(); ++k)
				if (((Literal *)udis->Data()[k])->M() == j) break;
			if (k <= udis->Data().e()) continue;
			for (k = 0; k <= fcon[lmax]->Data().e(); ++k) {
				tdis = (Disjunction *)fcon[lmax]->Data()[k];
				if ((-1 == tdis->Data().e()) ||
					(((Literal *)tdis->Data()[0])->T() != LEFT) ||
					(((Literal *)tdis->Data()[0])->N() != l)) continue;
				for (m = 0; m <= tdis->Data().e(); ++m)
					if (((Literal *)tdis->Data()[m])->M() == j) break;
				if (m <= tdis->Data().e()) break;
			}
			if ((k > fcon[lmax]->Data().e()) && (max <
				tmp->OData()[Data::TDAS()[l - 1] + j])) max =
				tmp->OData()[Data::TDAS()[l - 1] + (jmax = j)];
		}
		if (-1 == jmax) break;
		if (0.0 == tmp->OS()[l]) *udis += new Literal(LEFT,
			l, jmax, max, 1.0); else *udis += new Literal(LEFT,
			l, jmax, max, max*1.0/tmp->OS()[l]);
		if ((-1 != P1C) && (udis->SUM() >= P1C*tmp->OS()[l])) break;
	}
	if (fmax < udis->Set(udis->Py(*tmp,
		(Disjunction *)fcon[lmax]->Data()[fcon[lmax]->Data().e()], sa)))
	{
		fmax = udis->Value();
		delete umax;
		umax = udis;
	} else delete udis;
}

void Computing::SDisL(int l, Disjunction *& udis, int lmax,
	Conjunction ** fcon, Link * tmp, const Array <Sentense> & sa,
	float & fmax, Disjunction *& umax)
{
	Disjunction * tdis;
	int i, j, k, m, jmax;
	int max;
	udis = new Disjunction(0);
	for (i = 0; i < Data::TDAR()[l - 1]; ++i) {
		max = 0;
		jmax = -1;
		for (j = 0; j < Data::TDAR()[l - 1]; ++j) {
			if (la[lmax]->SData()[Data::TDAS()[l - 1] + j] ||
				!tmp->SData()[Data::TDAS()[l - 1] + j]) continue;
			for (k = 0; k <= udis->Data().e(); ++k)
				if (((Literal *)udis->Data()[k])->M() == j) break;
			if (k <= udis->Data().e()) continue;
			for (k = 0; k <= fcon[lmax]->Data().e(); ++k) {
				tdis = (Disjunction *)fcon[lmax]->Data()[k];
				if ((-1 == tdis->Data().e()) ||
					(((Literal *)tdis->Data()[0])->T() != RGHT) ||
					(((Literal *)tdis->Data()[0])->N() != l)) continue;
				for (m = 0; m <= tdis->Data().e(); ++m)
					if (((Literal *)tdis->Data()[m])->M() == j) break;
				if (m <= tdis->Data().e()) break;
			}
			if ((k > fcon[lmax]->Data().e()) && (max <
				tmp->SData()[Data::TDAS()[l - 1] + j])) max =
				tmp->SData()[Data::TDAS()[l - 1] + (jmax = j)];
		}
		if (-1 == jmax) break;
		if (0.0 == tmp->SS()[l]) *udis += new Literal(RGHT,
			l, jmax, max, 1.0); else *udis += new Literal(RGHT,
			l, jmax, max, max*1.0/tmp->SS()[l]);
		if ((-1 != P1C) && (udis->SUM() >= P1C*tmp->SS()[l])) break;
	}
	if (fmax < udis->Set(udis->Py(*tmp,
		(Disjunction *)fcon[lmax]->Data()[fcon[lmax]->Data().e()], sa)))
	{
		fmax = udis->Value();
		delete umax;
		umax = udis;
	} else delete udis;
}

void Computing::EDisL(int l, Disjunction *& udis, int lmax,
	Conjunction ** fcon, Link * tmp, const Array <Sentense> & sa,
	float & fmax, Disjunction *& umax)
{
	Disjunction * tdis;
	int k, m;
	udis = new Disjunction(0);
	for (k = 0; k <= fcon[lmax]->Data().e(); ++k) {
		tdis = (Disjunction *)fcon[lmax]->Data()[k];
		if ((-1 == tdis->Data().e()) ||
			(((Literal *)tdis->Data()[0])->T() != EQLS) ||
			(((Literal *)tdis->Data()[0])->N() != l)) continue;
		for (m = 0; m <= tdis->Data().e(); ++m)
			if (((Literal *)tdis->Data()[m])->M() == 0) break;
		if (m <= tdis->Data().e()) break;
	}
	if (la[lmax]->EData()[Data::TDAS()[l - 1]] || (k <= fcon[lmax]->Data().e())
		|| !tmp->EData()[Data::TDAS()[l - 1]])
	{
		for (k = 0; k <= fcon[lmax]->Data().e(); ++k) {
			tdis = (Disjunction *)fcon[lmax]->Data()[k];
			if ((-1 == tdis->Data().e()) ||
				(((Literal *)tdis->Data()[0])->T() != EQLS) ||
				(((Literal *)tdis->Data()[0])->N() != l)) continue;
			for (m = 0; m <= tdis->Data().e(); ++m)
				if (((Literal *)tdis->Data()[m])->M() == 1) break;
			if (m <= tdis->Data().e()) break;
		}
		if (!la[lmax]->EData()[Data::TDAS()[l - 1] + 1] && (k > fcon[lmax]->Data().e())
			&& tmp->EData()[Data::TDAS()[l - 1] + 1])
				*udis += new Literal(EQLS, l, 1, tmp->EData()[Data::TDAS()[l - 1] + 1],
					tmp->EData()[Data::TDAS()[l - 1] + 1]*1.0/tmp->ES()[l]);
	} else {
		for (k = 0; k <= fcon[lmax]->Data().e(); ++k) {
			tdis = (Disjunction *)fcon[lmax]->Data()[k];
			if ((-1 == tdis->Data().e()) ||
				(((Literal *)tdis->Data()[0])->T() != EQLS) ||
				(((Literal *)tdis->Data()[0])->N() != l)) continue;
			for (m = 0; m <= tdis->Data().e(); ++m)
				if (((Literal *)tdis->Data()[m])->M() == 1) break;
			if (m <= tdis->Data().e()) break;
		}
		if (la[lmax]->EData()[Data::TDAS()[l - 1] + 1] || (k <= fcon[lmax]->Data().e())
			|| !tmp->EData()[Data::TDAS()[l - 1] + 1])
				*udis += new Literal(EQLS, l, 0, tmp->EData()[Data::TDAS()[l - 1]],
					tmp->EData()[Data::TDAS()[l - 1]]*1.0/tmp->ES()[l]);
		else if (tmp->EData()[Data::TDAS()[l - 1]] > tmp->EData()[Data::TDAS()[l - 1] + 1]) {
			*udis += new Literal(EQLS, l, 0, tmp->EData()[Data::TDAS()[l - 1]],
				tmp->EData()[Data::TDAS()[l - 1]]*1.0/tmp->ES()[l]);
			if ((-1 == P1C) || (udis->SUM() < P1C*tmp->ES()[l]))
				*udis += new Literal(EQLS, l, 1,
					tmp->EData()[Data::TDAS()[l - 1] + 1],
					tmp->EData()[Data::TDAS()[l - 1] + 1]*1.0/tmp->ES()[l]);
		} else {
			*udis += new Literal(EQLS, l, 1, tmp->EData()[Data::TDAS()[l - 1] +
				1], tmp->EData()[Data::TDAS()[l - 1] + 1]*1.0/tmp->ES()[l]);
			if ((-1 == P1C) || (udis->SUM() < P1C*tmp->ES()[l]))
				*udis += new Literal(EQLS, l, 0, tmp->EData()[Data::TDAS()[l -
					1]], tmp->EData()[Data::TDAS()[l - 1]]*1.0/tmp->ES()[l]);
		}
	}
	if (fmax < udis->Set(udis->Py(*tmp,
		(Disjunction *)fcon[lmax]->Data()[fcon[lmax]->Data().e()], sa)))
	{
		fmax = udis->Value();
		delete umax;
		umax = udis;
	} else delete udis;
}

void Computing::ODis(int l, Disjunction ** odis) {
	int i, j, k, m, jmax;
	int max;
	odis[0] = new Disjunction(0);
	for (j = 0; j < Data::PSPR(); ++j) {
		max = 0;
		jmax = -1;
		for (m = 0; m < Data::PSPR(); ++m) {
			for (k = 0; k <= odis[0]->Data().e(); ++k)
				if (((Literal *)odis[0]->Data()[k])->M() == m) break;
			if ((k > odis[0]->Data().e()) &&
				(la[l]->OData()[m] > max)) max = la[l]->OData()[jmax = m];
		}
		if (-1 == jmax) break;
		if (0.0 == la[l]->OS()[0]) *odis[0] += new Literal(LEFT,
			0, jmax, max, 1.0); else *odis[0] += new Literal(LEFT,
			0, jmax, max, max*1.0/la[l]->OS()[0]);
		if ((-1 != P1C) && (odis[0]->SUM() >= P1C*la[l]->OS()[0])) break;
	}
	for (i = 0; i < Data::DTAR(); ++i) {
		odis[i + 1] = new Disjunction(0);
		for (j = 0; j < Data::TDAR()[i]; ++j) {
			max = 0;
			jmax = -1;
			for (m = 0; m < Data::TDAR()[i]; ++m) {
				for (k = 0; k <= odis[i + 1]->Data().e(); ++k)
					if (((Literal *)odis[i + 1]->Data()[k])->M() == m) break;
				if ((k > odis[i + 1]->Data().e()) &&
					(la[l]->OData()[Data::TDAS()[i] + m] > max))
						max = la[l]->OData()[Data::TDAS()[i] + (jmax = m)];
			}
			if (-1 == jmax) break;
			if (0.0 == la[l]->OS()[i + 1]) *odis[i + 1] += new Literal(LEFT,
				i + 1, jmax, max, 1.0); else *odis[i + 1] += new Literal(LEFT,
				i + 1, jmax, max, max*1.0/la[l]->OS()[i + 1]);
			if ((-1 != P1C) && (odis[i + 1]->SUM() >= P1C*la[l]->OS()[i + 1]))
				break;
		}
	}
}

void Computing::SDis(int l, Disjunction ** sdis) {
	int i, j, k, m, jmax;
	int max;
	sdis[0] = new Disjunction(0);
	for (j = 0; j < Data::PSPR(); ++j) {
		max = 0;
		jmax = -1;
		for (m = 0; m < Data::PSPR(); ++m) {
			for (k = 0; k <= sdis[0]->Data().e(); ++k)
				if (((Literal *)sdis[0]->Data()[k])->M() == m) break;
			if ((k > sdis[0]->Data().e()) &&
				(la[l]->SData()[m] > max)) max = la[l]->SData()[jmax = m];
		}
		if (-1 == jmax) break;
		if (0.0 == la[l]->SS()[0]) *sdis[0] += new Literal(RGHT,
			0, jmax, max, 1.0); else *sdis[0] += new Literal(RGHT,
			0, jmax, max, max*1.0/la[l]->SS()[0]);
		if ((-1 != P1C) && (sdis[0]->SUM() >= P1C*la[l]->SS()[0])) break;
	}
	for (i = 0; i < Data::DTAR(); ++i) {
		sdis[i + 1] = new Disjunction(0);
		for (j = 0; j < Data::TDAR()[i]; ++j) {
			max = 0;
			jmax = -1;
			for (m = 0; m < Data::TDAR()[i]; ++m) {
				for (k = 0; k <= sdis[i + 1]->Data().e(); ++k)
					if (((Literal *)sdis[i + 1]->Data()[k])->M() == m) break;
				if ((k > sdis[i + 1]->Data().e()) &&
					(la[l]->SData()[Data::TDAS()[i] + m] > max))
						max = la[l]->SData()[Data::TDAS()[i] + (jmax = m)];
			}
			if (-1 == jmax) break;
			if (0.0 == la[l]->SS()[i + 1]) *sdis[i + 1] += new Literal(RGHT,
				i + 1, jmax, max, 1.0); else *sdis[i + 1] += new Literal(RGHT,
				i + 1, jmax, max, max*1.0/la[l]->SS()[i + 1]);
			if ((-1 != P1C) && (sdis[i + 1]->SUM() >= P1C*la[l]->SS()[i + 1]))
				break;
		}
	}
}

void Computing::EDis(int l, Disjunction ** edis) {
	int i;
	edis[0] = new Disjunction(0);
	if (la[l]->EData()[0] > la[l]->EData()[1]) {
		*edis[0] += new Literal(EQLS, 0, 0,
			la[l]->EData()[0], la[l]->EData()[0]*1.0/la[l]->ES()[0]);
		if ((-1 == P1C) || (edis[0]->SUM() < P1C*la[l]->ES()[0]))
			*edis[0] += new Literal(EQLS, 0, 1, la[l]->EData()[1],
				la[l]->EData()[1]*1.0/la[l]->ES()[0]);
	} else if (la[l]->EData()[1] > 0) {
		*edis[0] += new Literal(EQLS, 0, 1,
			la[l]->EData()[1], la[l]->EData()[1]*1.0/la[l]->ES()[0]);
		if ((-1 == P1C) || (edis[0]->SUM() < P1C*la[l]->ES()[0]))
			*edis[0] += new Literal(EQLS, 0, 0, la[l]->EData()[0],
				la[l]->EData()[0]*1.0/la[l]->ES()[0]);
	}
	for (i = 0; i < Data::DTAR(); ++i) {
		edis[i + 1] = new Disjunction(0);
		if (la[l]->EData()[Data::TDAS()[i]] > la[l]->EData()[Data::TDAS()[i] + 1]) {
			*edis[i + 1] += new Literal(EQLS, i + 1, 0, la[l]->EData()
				[Data::TDAS()[i]], la[l]->EData()[Data::TDAS()[i]]*1.0/la[l]->ES()[i + 1]);
			if ((-1 == P1C) || (edis[i + 1]->SUM() < P1C*la[l]->ES()[i + 1]))
				*edis[i + 1] += new Literal(EQLS,
					i + 1, 1, la[l]->EData()[Data::TDAS()[i] + 1],
						la[l]->EData()[Data::TDAS()[i] + 1]*1.0/la[l]->ES()[i + 1]);
		} else if (la[l]->EData()[Data::TDAS()[i] + 1] > 0) {
			*edis[i + 1] += new Literal(EQLS, i + 1, 1, la[l]->EData()[Data::TDAS()[i]
				+ 1], la[l]->EData()[Data::TDAS()[i] + 1]*1.0/la[l]->ES()[i + 1]);
			if ((-1 == P1C) || (edis[i + 1]->SUM() < P1C*la[l]->ES()[i + 1]))
				*edis[i + 1] += new Literal(EQLS,
					i + 1, 0, la[l]->EData()[Data::TDAS()[i]],
						la[l]->EData()[Data::TDAS()[i]]*1.0/la[l]->ES()[i + 1]);
		}
	}
}

void Computing::DDis(int l, Disjunction *& ddis) {
	int i, j, k, jmax;
	int max;
	ddis = new Disjunction(0);
	for (i = 0; i < Data::SUM(); ++i) {
		max = 0;
		jmax = -1;
		for (j = 0; j < Data::SUM(); ++j) {
			for (k = 0; k < i; ++k)
				if (((Literal *)ddis->Data()[k])->M() == j) break;
			if ((k >= i) && (la[l]->DData()[j] > max))
				max = la[l]->DData()[jmax = j];
		}
		if (-1 == jmax) break;
		if (0 == la[l]->DS()) *ddis += new Literal(DIST, jmax, jmax, max, 1.0);
		else *ddis += new Literal(DIST, jmax, jmax, max, max*1.0/la[l]->DS());
		if ((-1 != P1C) && (ddis->SUM() >= P1C*la[l]->DS())) break;
	}
	if (-1 == jmax) --i;
	max = ddis->SUM();
	jmax = 0;
	j = Data::SUM();
	for (k = i; k >= 0; --k) {
		if (((Literal *)ddis->Data()[k])->M() > jmax)
			jmax = ((Literal *)ddis->Data()[k])->M();
		if (((Literal *)ddis->Data()[k])->M() < j)
			j = ((Literal *)ddis->Data()[k])->M();
		delete --*ddis;
	}
	*ddis += new Literal(DIST, j, jmax, max, max*1.0/la[l]->DS());
}

void Computing::Max(int *& imax, bool * c, Conjunction ** fcon,
	const Array <Sentense> & sa, Disjunction *** odis, Disjunction *** sdis,
	Disjunction *** edis, Disjunction ** ddis, float * xs, float * zs)
{
	Disjunction * tmp;
	int i, j, l;
	float max, * xt = 0, * zt = 0;
	if (xs) xt = new float;
	if (zs) zt = new float;
	for (l = 0; l <= la.e(); ++l) {
		if (!c[l]) {
			imax[l] = -1;
			if (fcon[l]) {
				max = 0.0;
				for (i = 0; i <= Data::DTAR(); ++i) {
					for (j = 0; j <= fcon[l]->Data().e(); ++j) {
						tmp = (Disjunction *)fcon[l]->Data()[j];
						if ((((Literal *)tmp->Data()[0])->T() == LEFT) &&
							(((Literal *)tmp->Data()[0])->N() == i)) break;
					}
					if ((j > fcon[l]->Data().e()) && odis[l][i] && (max <
						odis[l][i]->Set(odis[l][i]->P2(*la[l], sa, xt, zt))))
					{
						max = odis[l][i]->Value();
						imax[l] = i;
						if (xs) *xs = *xt;
						if (zs) *zs = *zt;
					}
				}
				for (i = 0; i <= Data::DTAR(); ++i) {
					for (j = 0; j <= fcon[l]->Data().e(); ++j) {
						tmp = (Disjunction *)fcon[l]->Data()[j];
						if ((((Literal *)tmp->Data()[0])->T() == RGHT) &&
							(((Literal *)tmp->Data()[0])->N() == i)) break;
					}
					if ((j > fcon[l]->Data().e()) && sdis[l][i] && (max <
						sdis[l][i]->Set(sdis[l][i]->P2(*la[l], sa, xt, zt))))
					{
						max = sdis[l][i]->Value();
						imax[l] = i + Data::DTAR() + 1;
						if (xs) *xs = *xt;
						if (zs) *zs = *zt;
					}
				}
				for (i = 0; i <= Data::DTAR(); ++i) {
					for (j = 0; j <= fcon[l]->Data().e(); ++j) {
						tmp = (Disjunction *)fcon[l]->Data()[j];
						if ((((Literal *)tmp->Data()[0])->T() == EQLS) &&
							(((Literal *)tmp->Data()[0])->N() == i)) break;
					}
					if ((j > fcon[l]->Data().e()) && edis[l][i] && (max <
						edis[l][i]->Set(edis[l][i]->P2(*la[l], sa, xt, zt))))
					{
						max = edis[l][i]->Value();
						imax[l] = i + 2*Data::DTAR() + 2;
						if (xs) *xs = *xt;
						if (zs) *zs = *zt;
					}
				}
				for (j = 0; j <= fcon[l]->Data().e(); ++j)
					if (((Literal *)((Disjunction *)fcon[l]->
						Data()[j])->Data()[0])->T() == DIST) break;
				if ((j > fcon[l]->Data().e()) && ddis[l] &&
					(max < ddis[l]->Set(ddis[l]->P2(*la[l], sa, xt, zt))))
				{
					max = ddis[l]->Value();
					imax[l] = 3*Data::DTAR() + 3;
					if (xs) *xs = *xt;
					if (zs) *zs = *zt;
				}
			}
			c[l] = true;
		}
	}
	delete xt;
	delete zt;
}

void Computing::PrintEntry(Array <Sentense> & sa, Array <Formula> & fa,
	FILE * out, int j, int i)
{
	int k, l, m, n, o, p, r;
	Word * ow, * sw;
	if (-1 != ((Disjunction *)fa[i])->Data().e()) {
		k = 0;
		n = 0;
		p = 0;
		r = 0;
		for (m = 0; m <= sa.e(); ++m) {
			for (l = 0; l <= sa[m]->EA()->e(); ++l) {
				ow = (*sa[m]->WA())[(*sa[m]->EA())[l]->OD() - 1];
				sw = (*sa[m]->WA())[(*sa[m]->EA())[l]->SD() - 1];
				for (o = 0; o <= ((Disjunction *)fa[i])->Data().e(); ++o)
					if (((Conjunction *)((Disjunction *)
						fa[i])->Data()[o])->Test(ow, sw)) break;
				if (o > ((Disjunction *)fa[i])->Data().e()) {
					if (i == (*sa[m]->EA())[l]->ID()) {
						++p;
						if (k < j) {
							fprintf(out, "TF\t%s %s;\n", sw->Txt(), ow->Txt());
							++k;
						}
					}
				} else {
					++n;
					if (i != (*sa[m]->EA())[l]->ID()) {
						++r;
						if (k < j) {
							fprintf(out, "FT\t%s %s;\n", sw->Txt(), ow->Txt());
							++k;
						}
					}
				}
			}
			for (l = 0; l <= sa[m]->UA()->e(); ++l) {
				ow = (*sa[m]->WA())[(*sa[m]->UA())[l]->OD() - 1];
				sw = (*sa[m]->WA())[(*sa[m]->UA())[l]->SD() - 1];
				for (o = 0; o <= ((Disjunction *)fa[i])->Data().e(); ++o)
					if (((Conjunction *)((Disjunction *)
						fa[i])->Data()[o])->Test(ow, sw)) break;
				if (o > ((Disjunction *)fa[i])->Data().e()) {
					if (i == -1) {
						++p;
						if (k < j) {
							fprintf(out, "TF\t%s %s;\n", sw->Txt(), ow->Txt());
							++k;
						}
					}
				} else {
					++n;
					if (i != -1) {
						++r;
						if (k < j) {
							fprintf(out, "FT\t%s %s;\n", sw->Txt(), ow->Txt());
							++k;
						}
					}
				}
			}
		}
		fprintf(out, "\tЧисло позитивных примеров %i.\n", n - r);
		if (0 == n + p) fprintf(out, "\tЧисло ошибок типа TF: 0,\
 полнота правила 1.0.\n");
		else fprintf(out, "\tЧисло ошибок типа TF: %i, полнота правила %f.\n",
			p, (n - r)*1.0/(n - r + p));
		if (0 == n) fprintf(out, "\tЧисло ошибок типа FT: 0,\
 точность правила 1.0.\n");
		else fprintf(out, "\tЧисло ошибок типа FT: %i, точность правила %f.\n",
			r, (n - r)*1.0/n);
		for (o = 0; o <= ((Disjunction *)fa[i])->Data().e(); ++o) {
			fprintf(out, "\tКонъюнкция № %i:\n", o + 1);
			k = 0;
			n = 0;
			p = 0;
			r = 0;
			for (m = 0; m <= sa.e(); ++m) {
				for (l = 0; l <= sa[m]->EA()->e(); ++l) {
					ow = (*sa[m]->WA())[(*sa[m]->EA())[l]->OD() - 1];
					sw = (*sa[m]->WA())[(*sa[m]->EA())[l]->SD() - 1];
					if (!((Conjunction *)((Disjunction *)
						fa[i])->Data()[o])->Test(ow, sw))
					{
						if (i == (*sa[m]->EA())[l]->ID()) ++p;
					} else {
						++n;
						if (i != (*sa[m]->EA())[l]->ID()) ++r;
						else if (k < j) {
							fprintf(out, "TT\t%s %s;\n", sw->Txt(), ow->Txt());
							++k;
						}
					}
				}
				for (l = 0; l <= sa[m]->UA()->e(); ++l) {
					ow = (*sa[m]->WA())[(*sa[m]->UA())[l]->OD() - 1];
					sw = (*sa[m]->WA())[(*sa[m]->UA())[l]->SD() - 1];
					if (!((Conjunction *)((Disjunction *)
						fa[i])->Data()[o])->Test(ow, sw))
					{
						if (i == -1) ++p;
					} else {
						++n;
						if (i != -1) ++r;
						else if (k < j) {
							fprintf(out, "TT\t%s %s;\n", sw->Txt(), ow->Txt());
							++k;
						}
					}
				}
			}
			fprintf(out, "\tЧисло позитивных примеров %i.\n", n - r);
			if (0 == n + p) fprintf(out, "\tЧисло ошибок типа TF: 0,\
 полнота правила 1.0.\n");
			else fprintf(out, "\tЧисло ошибок типа TF: %i,\
 полнота правила %f.\n", p, (n - r)*1.0/(n - r + p));
			if (0 == n) fprintf(out, "\tЧисло ошибок типа FT: 0,\
 точность правила 1.0.\n");
			else fprintf(out, "\tЧисло ошибок типа FT: %i,\
 точность правила %f.\n", r, (n - r)*1.0/n);
		}
	}
}

void Conjunction::Print(FILE * out) const {
	int i = 0;
	if (ott || (data.e() == -1)) {	fprintf(out, "()");	return;	}
	fprintf(out, "(");
	while (!data.OK(i) && (i <= data.e())) ++i;
	if (i <= data.e()) {
		data[i]->Print(out);
		for (++i; i <= data.e(); ++i)
			if (data.OK(i) && !((Disjunction *)data[i])->OTT()) {
				fprintf(out, "/\\");
				data[i]->Print(out);
			}
	}
	fprintf(out, ")");
}

Conjunction * Conjunction::Scan(FILE * in, char & c) {
	Conjunction * tmp = new Conjunction;
	if ('(' == c) c = fgetc(in); else throw new WithChar(27, c);
	if (')' == c) {	c = fgetc(in);	return tmp;	}
	tmp->data += Disjunction::Scan(in, c, 0);
	while (')' != c) {
		if ('/' == c) c = fgetc(in); else throw new WithChar(27, c);
		if ('\\' == c) c = fgetc(in); else throw new WithChar(27, c);
		tmp->data += Disjunction::Scan(in, c, 0);
	}
	if (')' == c) c = fgetc(in); else throw new WithChar(27, c);
	return tmp;
}

void Disjunction::Print(FILE * out) const {
	int i;
	if (ott || (data.e() == -1)) {	fprintf(out, "()");	return;	}
	fprintf(out, "(");
	data[0]->Print(out);
	for (i = 1; i <= data.e(); ++i) {
		fprintf(out, "\\/");
		data[i]->Print(out);
	}
	fprintf(out, ")");
	if (u) {
		fprintf(out, " - ");
		u->Print(out);
	}
}

Disjunction * Disjunction::Scan(FILE * in, char & c, int x) {
	Disjunction * tmp = new Disjunction(x);
	if ('(' == c) c = fgetc(in); else throw new WithChar(27, c);
	if (')' == c) {	c = fgetc(in);	return tmp;	}
	if (x) tmp->data += Conjunction::Scan(in, c);
	else tmp->data += Literal::Scan(in, c);
	while (')' != c) {
		if ('\\' == c) c = fgetc(in); else throw new WithChar(27, c);
		if ('/' == c) c = fgetc(in); else throw new WithChar(27, c);
		if (x) tmp->data += Conjunction::Scan(in, c);
		else tmp->data += Literal::Scan(in, c);
	}
	if (')' == c) c = fgetc(in); else throw new WithChar(27, c);
	if (' ' == c) if (!x) {	c = fgetc(in);
		if ('-' == c) c = fgetc(in); else throw new WithChar(27, c);
		if (' ' == c) c = fgetc(in); else throw new WithChar(27, c);
		tmp->Un(Disjunction::Scan(in, c, 0));
	} else throw new WithChar(27, c);
	return tmp;
}

void Literal::Print(FILE * out) const {
	if (t == DIST) fprintf(out, "Расстояние между словами от %i до %i",
		n + 1, m + 1);
	else if (n == 0) if (t == LEFT) fprintf(out,
		"Часть речи объекта = %s", Data::PSSP()[m]);
	else if (t == RGHT) fprintf(out,
		"Часть речи субъекта = %s", Data::PSSP()[m]);
	else fprintf(out, "Части речи %s", psse[m]);
	else if (t == LEFT) fprintf(out,
		"%s объекта = %s", Data::DTSA()[n - 1], Data::TDSA()[n - 1][m]);
	else if (t == RGHT) fprintf(out,
		"%s субъекта = %s", Data::DTSA()[n - 1], Data::TDSA()[n - 1][m]);
	else fprintf(out, "%s %s", Data::DTMA()[n - 1], psse[m]);
}

Literal * Literal::Scan(FILE * in, char & c) {
	Literal * tmp;
	char * buf = new char[37];
	int l = 0, m, n;
	buf[l++] = c;
	while (!feof(in) && (')' != c) && ('\\' != c)) buf[l++] = c = fgetc(in);
	buf[--l] = 0;
	l = 0;
	if (Entry::ReadStr(buf, "Расстояние между словами от ", l)) {
		if (-1 == (n = Entry::Str2Int(buf, l) - 1)) throw new WithStr(46, buf);
		if (!Entry::ReadStr(buf, emsg, l))
			throw new With2Str(17, buf, emsg, 1, 0);
		if (-1 == (m = Entry::Str2Int(buf, l) - 1)) throw new WithStr(46, buf);
		tmp = new Literal(DIST, n, m, -1, -1.0);
	} else if (Entry::ReadStr(buf, "Часть речи ", l)) {
		if (Entry::ReadStr(buf, "объекта = ", l)) {
			for (m = 0; m < Data::PSPR(); ++m)
				if (Entry::ReadStr(buf, Data::PSSP()[m], l)) break;
			if (m < Data::PSPR()) tmp = new Literal(LEFT, 0, m, -1, -1.0);
			else throw new WithStr(47, buf);
		} else if (Entry::ReadStr(buf, "субъекта = ", l)) {
			for (m = 0; m < Data::PSPR(); ++m)
				if (Entry::ReadStr(buf, Data::PSSP()[m], l)) break;
			if (m < Data::PSPR()) tmp = new Literal(RGHT, 0, m, -1, -1.0);
			else throw new WithStr(48, buf);
		} else throw new WithStr(18, buf);
	} else if (Entry::ReadStr(buf, "Части речи ", l)) {
		if (Entry::ReadStr(buf, psse[0], l))
			tmp = new Literal(EQLS, 0, 0, -1, -1.0);
		else if (Entry::ReadStr(buf, psse[1], l))
			tmp = new Literal(EQLS, 0, 1, -1, -1.0);
		else throw new WithStr(49, buf);
	} else {
		for (n = 0; n < Data::DTAR(); ++n) if (Entry::ReadStr(buf, Data::DTMA()[n], l)) break;
		if (n < Data::DTAR()) {	++n;
			if (' ' != buf[l++]) throw new WithChar(27, buf[l - 1]);
			if (Entry::ReadStr(buf, psse[0], l))
				tmp = new Literal(EQLS, n, 0, -1, -1.0);
			else if (Entry::ReadStr(buf, psse[1], l))
				tmp = new Literal(EQLS, n, 1, -1, -1.0);
			else throw new With2Str(52, Data::DTSA()[n - 1], buf, 0, 1);
		} else {
			for (n = 0; n < Data::DTAR(); ++n)
				if (Entry::ReadStr(buf, Data::DTSA()[n], l)) break;
			if (n >= Data::DTAR()) throw new WithStr(25, buf); else ++n;
			if (Entry::ReadStr(buf, " объекта = ", l)) {
				for (m = 0; m < Data::TDAR()[n - 1]; ++m)
					if (Entry::ReadStr(buf, Data::TDSA()[n - 1][m], l)) break;
				if (m < Data::PSPR()) tmp = new Literal(LEFT, n, m, -1, -1.0);
				else throw new With2Str(50, Data::DTSA()[n - 1], buf, 0, 1);
			} else if (Entry::ReadStr(buf, " субъекта = ", l)) {
				for (m = 0; m < Data::TDAR()[n - 1]; ++m)
					if (Entry::ReadStr(buf, Data::TDSA()[n - 1][m], l)) break;
				if (m < Data::PSPR()) tmp = new Literal(RGHT, n, m, -1, -1.0);
				else throw new With2Str(51, Data::DTSA()[n - 1], buf, 0, 1);
			} else throw new With2Str(53, Data::DTSA()[n - 1], buf, 0, 1);
		}
	}
	delete buf;
	return tmp;
}

MultiFormula & Conjunction::operator += (Formula * F) {
	data += F;
	return *this;
}

MultiFormula & Disjunction::operator += (Formula * F) {
	data += F;
	if (sum != -1) sum += ((Literal *)F)->Data();
	return *this;
}

Formula * Conjunction::operator -- () { return --data; }

Formula * Disjunction::operator -- () {
	if (-1 != sum) sum -= ((Literal *)data[data.e()])->Data();
	return --data;
}

bool Conjunction::Filter(Conjunction ** fcon,
	const Array <Link> & la, const Array <Sentense> & sa, Link & l)
{
	int j;
	bool b = false;
	Sentense * p;
	Word * ow, * sw;
	for (j = 0; j <= l.Arr().e(); ++j) if (l.Arr().OK(j)) {
		p = sa[l.Arr()[j]->PD()];
		ow = (*p->WA())[l.Arr()[j]->OD() - 1];
		sw = (*p->WA())[l.Arr()[j]->SD() - 1];
		if (fcon[l.ID()]->Test(ow, sw))
		{ l.Arr() - j; b = true; }
	}
	l.Reset();
	l.ReEnter(sa);
	return b;
}

bool Conjunction::Test(Word * ow, Word * sw) {
	int i;
	if (ott || (-1 == data.e())) return false;
	for (i = 0; i <= data.e(); ++i)
		if (data.OK(i) && !((Disjunction *)data[i])->OTT() &&
			(-1 != ((Disjunction *)data[i])->Data().e()) &&
			!((Disjunction *)data[i])->Test(ow, sw)) return false;
	return true;
}

bool Disjunction::Test(Word * ow, Word * sw, bool x) {
	int i;
	if (!x && u && u->Test(ow, sw)) return false;
	for (i = 0; i <= data.e(); ++i)
		if (((Literal *)data[i])->Test(ow, sw)) return true;
	return false;
}

bool Literal::Test(Word * ow, Word * sw) {
	if (t == LEFT) if (n == 0) return
		((ow->Prop().Data() & Data::PSMA()[0]) == m*Data::PSMA()[1]);
	else return (Data::DTIA()[(ow->Prop().Data() & Data::PSMA()[0]) / Data::PSMA()[1]][n - 1] &&
		((ow->Prop().Data() & Data::TDMA()[n - 1][0]) == m*Data::TDMA()[n - 1][1]));
	else if (t == RGHT) if (n == 0) return
		((sw->Prop().Data() & Data::PSMA()[0]) == m*Data::PSMA()[1]);
	else return (Data::DTIA()[(sw->Prop().Data() & Data::PSMA()[0]) / Data::PSMA()[1]][n - 1] &&
		((sw->Prop().Data() & Data::TDMA()[n - 1][0]) == m*Data::TDMA()[n - 1][1]));
	else if (t == EQLS) if (n == 0) return ((m == 0) ==
		((ow->Prop().Data() & Data::PSMA()[0]) == (sw->Prop().Data() & Data::PSMA()[0])));
	else return (((m == 0) == ((ow->Prop().Data() & Data::TDMA()[n - 1][0]) ==
		(sw->Prop().Data() & Data::TDMA()[n - 1][0])))
		&& Data::DTIA()[(ow->Prop().Data() & Data::PSMA()[0]) / Data::PSMA()[1]][n - 1]
		&& Data::DTIA()[(sw->Prop().Data() & Data::PSMA()[0]) / Data::PSMA()[2]][n - 1]);
	else if (ow->ID() > sw->ID()) return (n <= ow->ID() - sw->ID() - 1) &&
		(ow->ID() - sw->ID() - 1 <= m);
	else if (sw->ID() > ow->ID()) return (n <= sw->ID() - ow->ID() - 1) &&
		(sw->ID() - ow->ID() - 1 <= m);
	else throw new WithShort(31, ow->ID());
}

float MultiFormula::P1(const Array <Formula> & f, int l,
	const Array <Sentense> & sa, const Array <Link> & la)
{
	int i, j, k, q, r = 0, n = 0;
	float max, tmp;
	Word * ow, * sw;
	for (i = 0; i <= sa.e(); ++i) for (j = 0; j <= sa[i]->EA()->e(); ++j) {
		if ((*sa[i]->EA())[j]->ID() != l) continue; else ++n;
		ow = (*sa[i]->WA())[(*sa[i]->EA())[j]->OD() - 1];
		sw = (*sa[i]->WA())[(*sa[i]->EA())[j]->SD() - 1];
		max = -1.0;
		for (q = 0; q <= ((Disjunction *)f[l])->Data().e(); ++q) if
			(((Conjunction *)((Disjunction *)f[l])->Data()[q])->Test(ow, sw))
		{
			tmp = ((Conjunction *)((Disjunction *)f[l])->Data()[q])->Value();
			if (max < tmp) max = tmp;
		}
		if (-1.0 == max) continue;
		for (k = 0; k < Data::SLAR(); ++k) if ((k != l)
			&& !((Disjunction *)f[k])->OTT())
		{
			for (q = 0; q <= ((Disjunction *)f[k])->Data().e(); ++q)
				if (((Conjunction *)((Disjunction *)f[k])->Data()[q])->
					Test(ow, sw) && (max < ((Conjunction *)
					((Disjunction *)f[k])->Data()[q])->Value())) break;
			if (q <= ((Disjunction *)f[k])->Data().e()) break;
		}
		if (k >= Data::SLAR()) ++r;
	}
	if (0 == n) return 0.0;
	return r*1.0/n;
}

float Disjunction::P2(Link & l,
	const Array <Sentense> & sa, float * xs, float * zs)
{
	bool b;
	float x, z;
	Word * ow, * sw;
	int i, j, n = 0, r = 0, p = 0;
	if (-1 == data.e()) {
		if (xs) *xs = 0.0;
		if (zs) *zs = 0.0;
		return 0.0;
	}
	for (i = 0; i <= sa.e(); ++i) {
		for (j = 0; j <= sa[i]->EA()->e(); ++j) {
			ow = (*sa[i]->WA())[(*sa[i]->EA())[j]->OD() - 1];
			sw = (*sa[i]->WA())[(*sa[i]->EA())[j]->SD() - 1];
			if (b = Test(ow, sw)) ++n;
			if ((*sa[i]->EA())[j]->ID() == l.ID()) {
				++p;
				if (b) ++r;
			}
		}
		for (j = 0; j <= sa[i]->UA()->e(); ++j) {
			ow = (*sa[i]->WA())[(*sa[i]->UA())[j]->OD() - 1];
			sw = (*sa[i]->WA())[(*sa[i]->UA())[j]->SD() - 1];
			if (Test(ow, sw)) ++n;
		}
	}
	if (0 == p) x = 0.0;
	else x = r*1.0/p;
	if (0 == n) z = 0.0;
	else z = r*1.0/n;
	if (xs) *xs = x;
	if (zs) *zs = z;
	if (DIST == ((Literal *)data[0])->T()) return x*z/(x + z);
	if (EQLS == ((Literal *)data[0])->T()) return x*z/(x + z)*(1 - data.e());
	if (0 == ((Literal *)data[0])->N())
		return x*z/(x + z)*(Data::PSPR() - 1 - data.e())/(Data::PSPR() - 1);
	return x*z/(x + z)*(Data::TDAR()[((Literal *)data[0])->N() - 1] - 1 - data.e())
		/(Data::TDAR()[((Literal *)data[0])->N() - 1] - 1);
}

float Disjunction::Py(Link & l, Disjunction * con,
	const Array <Sentense> & sa, float * xs, float * zs)
{
	bool b;
	float x, z;
	Word * ow, * sw;
	int i, j, n = 0, r = 0, p = 0;
	if (-1 == data.e()) {
		if (xs) *xs = 0.0;
		if (zs) *zs = 0.0;
		return 0.0;
	}
	for (i = 0; i <= sa.e(); ++i) {
		for (j = 0; j <= sa[i]->EA()->e(); ++j) {
			ow = (*sa[i]->WA())[(*sa[i]->EA())[j]->OD() - 1];
			sw = (*sa[i]->WA())[(*sa[i]->EA())[j]->SD() - 1];
			if (b = Test(ow, sw)) ++n;
			if (con->Test(ow, sw, true) && (l.ID() != (*sa[i]->EA())[j]->ID()))
			{
				++p;
				if (b) ++r;
			}
		}
		for (j = 0; j <= sa[i]->UA()->e(); ++j) {
			ow = (*sa[i]->WA())[(*sa[i]->UA())[j]->OD() - 1];
			sw = (*sa[i]->WA())[(*sa[i]->UA())[j]->SD() - 1];
			if (b = Test(ow, sw)) ++n;
			if (con->Test(ow, sw, true)) {
				++p;
				if (b) ++r;
			}
		}
	}
	if (0 == p) x = 0.0;
	else x = r*1.0/p;
	if (0 == n) z = 0.0;
	else z = r*1.0/n;
	if (xs) *xs = x;
	if (zs) *zs = z;
	if (EQLS == ((Literal *)data[0])->T()) return x*z/(x + z)*(1 - data.e());
	if (0 == ((Literal *)data[0])->N())
		return x*z/(x + z)*(Data::PSPR() - 1 - data.e())/(Data::PSPR() - 1);
	return x*z/(x + z)*(Data::TDAR()[((Literal *)data[0])->N() - 1] - 1 - data.e())
		/(Data::TDAR()[((Literal *)data[0])->N() - 1] - 1);
}

int MultiFormula::P3(Conjunction ** fcon, Disjunction *** odis,
	Disjunction *** sdis, Disjunction *** edis, Disjunction ** ddis,
	int * l, const Array <Sentense> & sa, const Array <Link> & la)
{
	int i, j, k, m, * r = new int[Data::SLAR()], * n = new int[Data::SLAR()], lmax;
	float max;
	Word * ow, * sw;
	for (i = 0; i < Data::SLAR(); ++i) {
		n[i] = 0;
		r[i] = 0;
	}
	for (m = 0; m < Data::SLAR(); ++m) if (fcon[m] && (-1 != l[m]))
		for (i = 0; i <= sa.e(); ++i) {
			for (j = 0; j <= sa[i]->EA()->e(); ++j) {
				ow = (*sa[i]->WA())[(*sa[i]->EA())[j]->OD() - 1];
				sw = (*sa[i]->WA())[(*sa[i]->EA())[j]->SD() - 1];
				if (l[m] == 3*Data::DTAR() + 3) {
					if ((-1 != fcon[m]->Data().e()) && !fcon[m]->Test(ow, sw)
						|| !ddis[m]->Test(ow, sw)) continue;
				} else if (l[m] <= Data::DTAR()) {
					if ((-1 != fcon[m]->Data().e()) && !fcon[m]->Test(ow, sw)
						|| !odis[m][l[m]]->Test(ow, sw)) continue;
				} else if (l[m] <= 2*Data::DTAR() + 1) {
					if ((-1 != fcon[m]->Data().e()) && !fcon[m]->Test(ow, sw)
						|| !sdis[m][l[m] - Data::DTAR() - 1]->Test(ow, sw)) continue;
				} else {
					if ((-1 != fcon[m]->Data().e()) && !fcon[m]->Test(ow, sw)
						|| !edis[m][l[m] - 2*Data::DTAR() - 2]->Test(ow, sw)) continue;
				}
				for (k = 0; k < Data::SLAR(); ++k) if ((k != m) && fcon[k] &&
					(-1 != l[k]) && fcon[k]->Test(ow, sw) &&
					(fcon[m]->Value() < fcon[k]->Value())) break;
				if (k < Data::SLAR()) continue; else ++n[m];
				if (m == (*sa[i]->EA())[j]->ID()) ++r[m];
			}
			for (j = 0; j <= sa[i]->UA()->e(); ++j) {
				ow = (*sa[i]->WA())[(*sa[i]->UA())[j]->OD() - 1];
				sw = (*sa[i]->WA())[(*sa[i]->UA())[j]->SD() - 1];
				if (l[m] == 3*Data::DTAR() + 3) {
					if ((-1 != fcon[m]->Data().e()) && !fcon[m]->Test(ow, sw)
						|| !ddis[m]->Test(ow, sw)) continue;
				} else if (l[m] <= Data::DTAR()) {
					if ((-1 != fcon[m]->Data().e()) && !fcon[m]->Test(ow, sw)
						|| !odis[m][l[m]]->Test(ow, sw)) continue;
				} else if (l[m] <= 2*Data::DTAR() + 1) {
					if ((-1 != fcon[m]->Data().e()) && !fcon[m]->Test(ow, sw)
						|| !sdis[m][l[m] - Data::DTAR() - 1]->Test(ow, sw)) continue;
				} else {
					if ((-1 != fcon[m]->Data().e()) && !fcon[m]->Test(ow, sw)
						|| !edis[m][l[m] - 2*Data::DTAR() - 2]->Test(ow, sw)) continue;
				}
				for (k = 0; k < Data::SLAR(); ++k) if ((k != m) && fcon[k] &&
					(-1 != l[k]) && fcon[k]->Test(ow, sw) &&
					(fcon[m]->Value() < fcon[k]->Value())) break;
				if (k >= Data::SLAR()) ++n[m];
			}
		}
	max = -1.0;
	lmax = -1;
	for (i = 0; i < Data::SLAR(); ++i) {
		if (-1 == l[i]) continue;
		else if (l[i] == 3*Data::DTAR() + 3) if (n[i]) ddis[i]->Set(r[i]*1.0/n[i]);
		else ddis[i]->Set(0.0);
		else if (l[i] <= Data::DTAR()) if (n[i]) odis[i][l[i]]->Set(r[i]*1.0/n[i]);
		else odis[i][l[i]]->Set(0.0);
		else if (l[i] <= 2*Data::DTAR() + 1)
			if (n[i]) sdis[i][l[i] - Data::DTAR() - 1]->Set(r[i]*1.0/n[i]);
		else sdis[i][l[i] - Data::DTAR() - 1]->Set(0.0);
		else if (n[i]) edis[i][l[i] - 2*Data::DTAR() - 2]->Set(r[i]*1.0/n[i]);
		else edis[i][l[i] - 2*Data::DTAR() - 2]->Set(0.0);
		if (r[i] > max*n[i]) max = r[lmax = i]*1.0/n[i];
	}
	return lmax;
}
