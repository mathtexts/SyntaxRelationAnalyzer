#include "except.h"

int Exception::Print(FILE * out) {
	fprintf(out, msg[id]);
	return id + 1;
}

int WithChar::Print(FILE * out) {
	fprintf(out, msg[id], char1);
	return id + 1;
}

int WithShort::Print(FILE * out) {
	fprintf(out, msg[id], num1);
	return id + 1;
}

WithStr::~WithStr() {
	if (OTT1) delete data1;
}

int WithStr::Print(FILE * out) {
	fprintf(out, msg[id], data1);
	return id + 1;
}

int WithCharStr::Print(FILE * out) {
	fprintf(out, msg[id], char1, data1);
	return id + 1;
}

int WithStrShort::Print(FILE * out) {
	fprintf(out, msg[id], data1, num1);
	return id + 1;
}

With2Str::~With2Str() {
	if (OTT2) delete data2;
}

int With2Str::Print(FILE * out) {
	fprintf(out, msg[id], data1, data2);
	return id + 1;
}

With3Str::~With3Str() {
	if (OTT3) delete data3;
}

int With3Str::Print(FILE * out) {
	fprintf(out, msg[id], data1, data2, data3);
	return id + 1;
}
