#include "temp.h"

const int NTPE = 3;

int main() {
	Array<Sentense> sa;
	Array<Link> la;
	Array<Formula> fa;
	Computing * cmp;
	GDecisionTree * model;
	char buf[BUF_SIZE], c;
	FILE * file = 0;
	std::fstream out;
	int i, j, k;
	float l, m;
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	GRand rand(0);
	TempEntry::Download();
	for (i = 0; i < Data::SLAR(); ++i) la += new Link(i);
	for (;;) {
		fprintf(stdout, ">:");
		i = 0;
		buf[i] = fgetc(stdin);
		while ((buf[i] != '\n') && (buf[i] != '\r') && !feof(stdin))
			buf[++i] = fgetc(stdin);
		buf[i] = 0;
		i = 0;
		try {
			if (Entry::ReadStr(buf, "exit", i)) break;
			else if (Entry::ReadStr(buf, "help", i)) throw new Exception(24);
			else if (Entry::ReadStr(buf, "stat ", i)) {
				k = Entry::ReadStr(buf, "-csv ", i);
				file = Computing::FileOpen(buf, i, "w");
				Link::PrintHeader(file, k);
				for (i = 0; i <= la.e(); ++i) la[i]->Output(file, k);
				fprintf(stdout, done);
			} else if (Entry::ReadStr(buf, "top ", i)) {
				k = Entry::ReadStr(buf, "-x ", i);
				j = Entry::Str2Int(buf, i);
				file = Computing::FileOpen(buf, i, "w");
				for (i = 0; i <= la.e(); ++i) la[i]->Top(file, j, k);
				fprintf(stdout, done);
			} else if (Entry::ReadStr(buf, "tree ", i)) {
				out.open(buf + i, std::ios_base::out);
				model = new GDecisionTree(rand);
				TempEntry::Train(model, sa);
				model->print(out);
				delete model;
				out.close();
				fprintf(stdout, done);
			} else if (Entry::ReadStr(buf, "frml ", i)) {
				k = Entry::ReadStr(buf, "-x ", i);
				if (k) j = Entry::Str2Int(buf, i);
				if (Entry::ReadStr(buf, "-no ", i)) l = -1.0;
				else l = Entry::Str2Float(buf, i);
				if (Entry::ReadStr(buf, "-no ", i)) m = -1.0;
				else m = Entry::Str2Float(buf, i);
				cmp = new Computing(sa, la, fa, l, m);
				file = Computing::FileOpen(buf, i, "w");
				if (k) fprintf(file, "Тип примера, субъект и объект связи\n");
				for (i = 0; i <= fa.e(); ++i) {
					fprintf(file, "%s: ", Data::SLSA()[i]);
					fa[i]->Print(file);
					fprintf(file, "\n");
					if (k) cmp->PrintEntry(sa, fa, file, j, i);
				}
				delete cmp;
				fprintf(stdout, done);
			} else if (Entry::ReadStr(buf, "text ", i)) {
				file = Computing::FileOpen(buf, i, "w");
				for (i = 0; i <= sa.e(); ++i) sa[i]->Output(file);
				fprintf(stdout, done);
			} else if (Entry::ReadStr(buf, "cat ", i)) {
				file = Computing::FileOpen(buf, i, "w");
				for (i = 0; i <= sa.e(); ++i) sa[i]->Cat(file);
				fprintf(stdout, done);
			} else if (Entry::ReadStr(buf, "entry ", i)) {
				k = Entry::ReadStr(buf, "-csv ", i);
				file = Computing::FileOpen(buf, i, "w");
				Entry::PrintHeader(file, k);
				for (i = 0; i <= la.e(); ++i) la[i]->PrintEntry(file, sa, k);
				fprintf(stdout, done);
			} else if (Entry::ReadStr(buf, "ngtv ", i)) {
				k = Entry::ReadStr(buf, "-csv ", i);
				file = Computing::FileOpen(buf, i, "w");
				Entry::PrintHeader(file, k);
				for (i = 0; i <= sa.e(); ++i) sa[i]->PrintEntry(file, sa, k);
				fprintf(stdout, done);
			} else if (Entry::ReadStr(buf, "load ", i)) {
				file = Computing::FileOpen(buf, i, "r");
				for (i = 0; i <= la.e(); ++i) la[i]->Reset();
				Link::Input(file, NTPE, sa, la);
				fprintf(stdout, done);
			} else if (Entry::ReadStr(buf, "data ", i)) {
				file = Computing::FileOpen(buf, i, "r");
				c = fgetc(file);
				while ((c != '\n') && !feof(file)) c = fgetc(file);
				if (feof(file)) throw new Exception(29);
				c = fgetc(file);
				for (j = 0; j <= la.e(); ++j) {
					la[j]->Reset();
					la[j]->Load(file, c);
				}
				if (buf[i++]) {
					fclose(file);
					file = Computing::FileOpen(buf, i, "r");
					c = fgetc(file);
					while ((c != '\n') && !feof(file)) c = fgetc(file);
					if (feof(file)) throw new Exception(29);
					c = fgetc(file);
					Entry::ReadFile(file, NTPE, sa, la, c);
				}
				fprintf(stdout, done);
			} else if (!Entry::ReadStr(buf, "reset", i))
				throw new WithStr(23, buf, 0);
			else {
				sa.Reset();
				la.Reset();
				for (i = 0; i < Data::SLAR(); ++i) la += new Link(i);
				fprintf(stdout, done);
			}
		} catch (Exception * e) {
			e->Print(stderr);
			delete e;
		}
		if (0 != file) {
			fclose(file);
			file = 0;
		}
	}
	return 0;
}
