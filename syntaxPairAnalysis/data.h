// S______T_____A___________R       A_________G_______________E__S
// Syntax Texts Analysis of Russian Automated Generator of rulEs Systems

#ifndef STAR_AGES_DATA_H
#define STAR_AGES_DATA_H

#include "array.h"

const int BUF_SIZE = 256;

class Data {
	static int pspr, * psla, pslm, dtar, * dtsl, dtlm, * tdar,
		** tdsl, * tdsm, ** tdla, * tdlm, slar, sum, * tdas, * tdpa;
	static char ** pssp, * tdua, * tdxa, ** dtsa,
		** dtma, *** tdsa, *** tdna, ** slsa;
	static long * psma, ** tdma;
	static bool ** dtia;
public:
	static int ReadStr(FILE * in, const char * s, char & c, int & i);
	static int Str2Int(FILE * in, char & c);
	static void Read(FILE * in);
	static const int PSPR() {	return pspr;	}
	static const int * const PSLA() {	return psla;	}
	static const int PSLM() {	return pslm;	}
	static const char * const * const PSSP() {	return pssp;	}
	static const char * const TDUA() {	return tdua;	}
	static const char * const TDXA() {	return tdxa;	}
	static const long * const PSMA() {	return psma;	}
	static const int DTAR() {	return dtar;	}
	static const int * const DTSL() {	return dtsl;	}
	static const int DTLM() {	return dtlm;	}
	static const char * const * const DTSA() {	return dtsa;	}
	static const char * const * const DTMA() {	return dtma;	}
	static const int * const TDAR() {	return tdar;	}
	static const int * const * const TDSL() {	return tdsl;	}
	static const int * const TDSM() {	return tdsm;	}
	static const char * const * const * const TDSA() {	return tdsa;	}
	static const int * const * const TDLA() {	return tdla;	}
	static const int * const TDLM() {	return tdlm;	}
	static const char * const * const * const TDNA() {	return tdna;	}
	static const long * const * const TDMA() {	return tdma;	}
	static const bool * const * const DTIA() {	return dtia;	}
	static const int SLAR() {	return slar;	}
	static const char * const * const SLSA() {	return slsa;	}
	static const int SUM() {	return sum;	}
	static const int * const TDAS() {	return tdas;	}
	static const int * const TDPA() {	return tdpa;	}
	static int & SetPSPR() {	return pspr;	}
	static int *& SetPSLA() {	return psla;	}
	static int & SetPSLM() {	return pslm;	}
	static char **& SetPSSP() {	return pssp;	}
	static char *& SetTDUA() {	return tdua;	}
	static char *& SetTDXA() {	return tdxa;	}
	static long *& SetPSMA() {	return psma;	}
	static int & SetDTAR() {	return dtar;	}
	static int *& SetDTSL() {	return dtsl;	}
	static int & SetDTLM() {	return dtlm;	}
	static char **& SetDTSA() {	return dtsa;	}
	static char **& SetDTMA() {	return dtma;	}
	static int *& SetTDAR() {	return tdar;	}
	static int **& SetTDSL() {	return tdsl;	}
	static int *& SetTDSM() {	return tdsm;	}
	static char ***& SetTDSA() {	return tdsa;	}
	static int **& SetTDLA() {	return tdla;	}
	static int *& SetTDLM() {	return tdlm;	}
	static char ***& SetTDNA() {	return tdna;	}
	static long **& SetTDMA() {	return tdma;	}
	static bool **& SetDTIA() {	return dtia;	}
	static int & SetSLAR() {	return slar;	}
	static char **& SetSLSA() {	return slsa;	}
	static int & SetSUM() {	return sum;	}
	static int *& SetTDAS() {	return tdas;	}
	static int *& SetTDPA() {	return tdpa;	}
private:
	static void MakeTDUXA(FILE * in);
	static void ReadBoolMatrix(FILE * in);
	static void MakePSTDMA();
	static void ReadStrArray(FILE * in, char **& arr,
		int & n, bool x, int *& len, int & m, bool z = false);
};

#endif
