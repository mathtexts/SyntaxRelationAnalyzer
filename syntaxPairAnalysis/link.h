// S______T_____A___________R       A_________G_______________E__S
// Syntax Texts Analysis of Russian Automated Generator of rulEs Systems

#ifndef STAR_AGES_LINK_H
#define STAR_AGES_LINK_H

#include <stdlib.h>
#include <math.h>
#include "word.h"

const char NLNK[] = "без_связи";
const char * const emsa[] = { "\" FEAT=\"", " ", "\" ID=\"", "\" LEMMA=\"",
	"\" LINK=\"", "\">", "</W>", "<?xml", "<text", "<inf>", "<annot>",
	"</annot>", "<author>", "</author>", "<date>", "</date>", "<editor>",
	"</editor>", "<title>", "</title>", "</inf>", "<body>", "<S ID=\"",
	"<W DOM=\"", "</S>", "</body>", "</text>", "<source>", "</source>",
	"<comment>", "</comment>", "<sentences>", "</p></sentences>",
	"<se id=\"", "</se>", "<w id=\"", "<content>", "</content>",
	"<rel id_head=\"", "\" type=\"", "\"/>", "</w>", "<p>" };
const char * const psse[2] = { "равны", "не равны" };

class Sentense;
class Link;

class Node {
	int * data, * s;
	int * lr, * mr, * li, * mi;
	float * dh, * ls, * ms;
public:
	Node();
	~Node();
	void Start();
	void Terminate();
	void Clear();
	void Finalize(int e = 0);
	void AddEntry(Word * x, Word * y = 0, int z = 0);
	void Print(FILE * out, int x, int e = 0);
	void Load(FILE * in, char & c, int e = 0);
	static void Top(Node * ob, Node * sb,
		Node * en, FILE * out, int j, int x);
	const int * Data() const {	return data;	}
	const int * S() const {	return s;	}
	int * Data() {	return data;	}
	int * S() {	return s;	}
private:
	void Max(int i, int * mid, int & mxp, int & mxs,
		float & mnd, float & mxd, int n, int & mn);
};

class Entry {
	int id, od, sd, pd;
public:
	Entry(int i, int o, int s, int p) : id(i), od(o), sd(s), pd(p) {}
	void Output(FILE * out, Array<Sentense> & sa, int x);
	int ID() const {	return id;	}
	int OD() const {	return od;	}
	int SD() const {	return sd;	}
	int PD() const {	return pd;	}
	void IncPD(int i) {	pd += i;	}
	void DecPD(int i) {	pd -= i;	}
	static void PrintHeader(FILE * out, int x);
	static Word * Input(FILE * in, Array<Entry> & la, char & c, int p);
	static int ReadStr(const char * s, const char * c, int & j, bool x = true);
	static int Str2Int(const char * s, int & j);
	static float Str2Float(const char * s, int & j);
	static void ReadFile(FILE * in, int NTPE,
		Array <Sentense> & sa, Array <Link> & la, int c);
};

class Sentense {
	int NTPE;
	int id;
	Array<char> * pa;
	Array<Word> * wa;
	Array<Entry> * ea, * ua;
public:
	Sentense(int ntpe, int l, Array<char> * p,
		Array<Word> * w, Array<Entry> * e);
	~Sentense();
	void Output(FILE * out);
	void Cat(FILE * out);
	const Array <char> * PA() const {	return pa;	}
	Array <char> *& PA() {	return pa;	}
	const Array <Word> * WA() const {	return wa;	}
	Array <Word> *& WA() {	return wa;	}
	const Array <Entry> * EA() const {	return ea;	}
	Array <Entry> *& EA() {	return ea;	}
	const Array <Entry> * UA() const {	return ua;	}
	Array <Entry> *& UA() {	return ua;	}
	int ID() const {	return id;	}
	void PrintEntry(FILE * out, Array<Sentense> & sa, int x);
	void AddEntries(int m);
};

class Link {
	int id;
	Array<Entry> arr;
	Node * ob, * sb, * en, * dn;
	bool acc;
public:
	Link(const Link & l, const Array <Sentense> & sa);
	Link(int i, int a = 0);
	~Link();
	static int ReadXML(FILE * in, int i, char & c, int x, int & d);
	static void Input(FILE * in, int NTPE,
		Array<Sentense> & sa, Array<Link> & la);
	static void PrintHeader(FILE * out, int x);
	void Load(FILE * in, char & c);
	int GetID() const {	return id;	}
	void AddEntry(Entry * e, Array<Word> & a, int x = 1);
	void ReEnter(const Array <Sentense> & sa);
	void Finalize();
	void Reset();
	void Output(FILE * out, int x);
	void Top(FILE * out, int j, int x);
	void PrintEntry(FILE * out, Array<Sentense> & sa, int x);
	const int * OData() const {	return ob->Data();	}
	const int * SData() const {	return sb->Data();	}
	const int * EData() const {	return en->Data();	}
	const int * DData() const {	return dn->Data();	}
	const int * OS() const {	return ob->S();	}
	const int * SS() const {	return sb->S();	}
	const int * ES() const {	return en->S();	}
	int DS() const {	return dn->S()[0];	}
	int * OData() {	return ob->Data();	}
	int * SData() {	return sb->Data();	}
	int * EData() {	return en->Data();	}
	int * DData() {	return dn->Data();	}
	int * OS() {	return ob->S();	}
	int * SS() {	return sb->S();	}
	int * ES() {	return en->S();	}
	int & DS() {	return dn->S()[0];	}
	Array <Entry> & Arr() {	return arr;	}
	int ID() const {	return id;	}
};

#endif
