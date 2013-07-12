// S______T_____A___________R       A_________G_______________E__S
// Syntax Texts Analysis of Russian Automated Generator of rulEs Systems

#ifndef STAR_AGES_FORMULA_H
#define STAR_AGES_FORMULA_H

#include "link.h"

const char emsg[] = "до ";

enum LitType { LEFT = 0, RGHT = 1, EQLS = 2, DIST = 3 };

void DoNothing(void * null);

class Disjunction;
class Conjunction;

class Formula {
protected:
	float value;
public:
	Formula(float v = 0.0) : value(v) {}
	virtual ~Formula() {}
	virtual void Print(FILE * out) const = 0;
	float Set(float v) {	return value = v;	}
	float Value() const {	return value;	}
};

class MultiFormula : public Formula {
protected:
	bool ott;
	Array <Formula> data;
public:
	MultiFormula() : ott(false) {}
	virtual ~MultiFormula() {}
	virtual void Print(FILE * out) const = 0;
	virtual MultiFormula & operator += (Formula * f) = 0;
	virtual Formula * operator -- () = 0;
	static float P1(const Array <Formula> & f, int l,
		const Array <Sentense> & sa, const Array <Link> & la);
	static int P3(Conjunction ** fcon, Disjunction *** odis,
		Disjunction *** sdis, Disjunction *** edis, Disjunction ** ddis,
		int * l, const Array <Sentense> & sa, const Array <Link> & la);
	void Terminate(bool OTT = true) {	ott = OTT;	}
	bool OTT() const {	return ott;	}
	Array <Formula> & Data() {	return data;	}
	const Array <Formula> & Data() const {	return data;	}
};

class Conjunction : public MultiFormula {
public:
	Conjunction() {}
	virtual ~Conjunction() {};
	virtual void Print(FILE * out) const;
	static Conjunction * Scan(FILE * in, char & c);
	MultiFormula & operator += (Formula * f);
	Formula * operator -- ();
	bool Test(Word * ow, Word * sw);
	bool Filter(Conjunction ** fcon, const Array <Link> & la,
		const Array <Sentense> & sa, Link & l);
};

class Disjunction : public MultiFormula {
	int sum;
	Disjunction * u;
public:
	Disjunction(int x) : MultiFormula(), sum(x), u(0) {}
	virtual ~Disjunction() {	if (u) delete u;	}
	virtual void Print(FILE * out) const;
	static Disjunction * Scan(FILE * in, char & c, int x);
	MultiFormula & operator += (Formula * f);
	Formula * operator -- ();
	float P2(Link & l, const Array <Sentense> & sa,
		float * xs = 0, float * zs = 0);
	float Py(Link & l, Disjunction * con, const Array <Sentense> & sa,
		float * xs = 0, float * zs = 0);
	bool Test(Word * ow, Word * sw, bool x = false);
	int SUM() const {	return sum;	}
	void Un(Disjunction * f) {	u = f;	}
	Disjunction * U() {	return u;	}
};

class Literal : public Formula {
	LitType t;
	int n, m, data;
public:
	Literal(LitType t, int n, int m, int d, float v)
		: Formula(v), t(t), n(n), m(m), data(d) {}
	virtual ~Literal() {}
	virtual void Print(FILE * out) const;
	static Literal * Scan(FILE * in, char & c);
	bool Test(Word * ow, Word * sw);
	LitType T() const {	return t;	}
	int N() const {	return n;	}
	int M() const {	return m;	}
	int Data() const {	return data;	}
};

class Computing {
	float P1C, P2C;
	Array <Link> la;
public:
	Computing(float p1c, float p2c) : P1C(p1c), P2C(p2c), la() {}
	Computing(const Array <Sentense> & s, const Array <Link> & l,
		Array <Formula> & f, float p1c, float p2c);
	~Computing();
	void PrintEntry(Array <Sentense> & sa, Array <Formula> & fa,
		FILE * out, int j, int i);
	static FILE * FileOpen(const char * s, int & i, const char * p);
	float & P1() {	return P1C;	}
	float & P2() {	return P2C;	}
	Array <Link> & LA() {	return la;	}
	int SubCycle(int & lmax, int *& imax, float *& p3, bool & b,
		bool & d, bool *& c, Array <Formula> & f, Conjunction **& fcon, const
		Array <Sentense> & sa, Disjunction ***& odis, Disjunction ***& sdis,
		Disjunction ***& edis, Disjunction **& ddis);
	void ODis(int l, Disjunction ** odis);
	void SDis(int l, Disjunction ** sdis);
	void EDis(int l, Disjunction ** edis);
	void DDis(int l, Disjunction *& ddis);
	void Max(int *& imax, bool * c, Conjunction ** fcon, const Array <Sentense>
		& sa, Disjunction *** odis, Disjunction *** sdis, Disjunction *** edis,
		Disjunction ** ddis, float * xs = 0, float * zs = 0);
private:
	void BuildLink(Array <Formula> & f, const Array <Sentense> & sa);
	bool Cycle(int& lmax, int*& imax, float*& p3, bool& b, bool& d, bool*& c,
		Array<Formula>& f, Conjunction**& fcon, const Array<Sentense>& sa,
		Disjunction***& odis, Disjunction***& sdis, Disjunction***& edis,
		Disjunction**& ddis, void* p=0, void(*signal)(void*)=DoNothing);
	void AddConjunction(Disjunction ***& odis, Disjunction ***& sdis,
		Disjunction ***& edis, Disjunction **& ddis, Conjunction **& fcon,
		const Array <Sentense> & sa, Array <Formula> & f, float *& p3,
		int & lmax, bool & b, bool & d);
	void AddNegative(int lmax, int * imax, const Array <Sentense> &
		sa, Array <Formula> & f, Conjunction **& fcon, Disjunction ***& odis,
		Disjunction ***& sdis, Disjunction ***& edis, Disjunction **& ddis,
		float *& p3, bool & b, bool & d);
	void ODis0(Disjunction *& udis, int lmax, Conjunction ** fcon, Link *
		tmp, const Array <Sentense> & sa, float & fmax, Disjunction *& umax);
	void SDis0(Disjunction *& udis, int lmax, Conjunction ** fcon, Link *
		tmp, const Array <Sentense> & sa, float & fmax, Disjunction *& umax);
	void EDis0(Disjunction *& udis, int lmax, Conjunction ** fcon, Link *
		tmp, const Array <Sentense> & sa, float & fmax, Disjunction *& umax);
	void DDis0(Disjunction *& udis, int lmax, Conjunction ** fcon, Link *
		tmp, const Array <Sentense> & sa, float & fmax, Disjunction *& umax);
	void ODisL(int l, Disjunction *& udis, int lmax,
		Conjunction ** fcon, Link * tmp, const Array <Sentense> & sa,
		float & fmax, Disjunction *& umax);
	void SDisL(int l, Disjunction *& udis, int lmax,
		Conjunction ** fcon, Link * tmp, const Array <Sentense> & sa,
		float & fmax, Disjunction *& umax);
	void EDisL(int l, Disjunction *& udis, int lmax,
		Conjunction ** fcon, Link * tmp, const Array <Sentense> & sa,
		float & fmax, Disjunction *& umax);
};

#endif
