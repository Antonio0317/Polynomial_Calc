#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

using namespace std;

class Polynomial;

class Term {

	friend class Polynomial;

	public:
	int coef;
	int exp;
	string str;

	void CalcExp();
	Term();
};

class Polynomial {

	public:
		Term termArray[100];
		int terms;
		string cutTermArray[100];

		Polynomial();
		Polynomial Add(Polynomial poly);
		void CutTerm(string);
		void CutCoef();
};

int main (int argc, char *argv[]) {

	string str;
	ifstream fin;
	fin.open(argv[1]);
	ofstream fout;
	fout.open(argv[2]);

	while(fin >> str) {
		Polynomial a, b, c;
		a.CutTerm(str);
		a.CutCoef();
		for(int j=0; j<a.terms; j++) {
			a.termArray[j].CalcExp();
		}

		fin >> str;
		b.CutTerm(str);
		b.CutCoef();
		for(int j=0; j<b.terms; j++) {
			b.termArray[j].CalcExp();
		}
		c = a.Add(b);
		if (c.terms == 0) {
			fout<<0;
		}
		else {
			if(c.termArray[0].coef == -1)
				fout<<'-';
			else if(c.termArray[0].coef != 1)
				fout<<c.termArray[0].coef;
			fout<<c.termArray[0].str;
			for(int j=1; j<c.terms; j++) {
				if(c.termArray[j].coef > 0)
					fout<<'+';
				if(c.termArray[j].coef == -1)
					fout<<"-";
				if(c.termArray[j].coef != 1 && c.termArray[j].coef != -1)
					fout<<c.termArray[j].coef;
				fout<<c.termArray[j].str;
			}
		}
		fout<<endl;
	}

	fin.close();
	fout.close();

	return 0;
}

void Polynomial::CutTerm (string str) {
	int len = str.length();
	int count = 0, st=0;
	string tmp[100];
	int size = 1;
	for (int i=0; i<100; i++) {
		tmp[i] = "";
	}
	for (int i=1; i<len; i++) {
		if(str[i] == '+' || str[i] == '-') {
			if(str[i] == '-') {
				tmp[count]=str.substr(st, i-st);
				count++;
				st=i;
				size++;
			}
			else {
				tmp[count]=str.substr(st, i-st);
				count++;
				st=i+1;
				size++;
			}
		}
	}
	tmp[count]=str.substr(st, len);

	for (int i=0; i<size; i++) {
		cutTermArray[i] =tmp[i];
	}
	terms = size;
}

void Polynomial::CutCoef() {
	int cnt=0;
	int j;
	for(int i=0; i<terms; i++) {
		bool is_Minus=0;
		j=0;
		cnt=0;
		if(cutTermArray[i][0] == '-') {
			is_Minus=1;
			j++;
		}
		for(j; j<cutTermArray[i].length(); j++) {
			if(isdigit(cutTermArray[i][j])) {
				if(cnt) {
					termArray[i].coef *= 10;
				}
				termArray[i].coef += cutTermArray[i][j]-'0';
				cnt++;
			}
			else {
				if(cnt ==0) {
					termArray[i].coef = 1;
				}
				break;
			}
		}
		if(is_Minus) {
			termArray[i].coef *= -1;
		}
		termArray[i].str=cutTermArray[i].substr(j, cutTermArray[i].length());
	}

}

Polynomial::Polynomial() {
	terms = 0;
	for (int i=0; i<100; i++) {
		cutTermArray[i] = "";
	}
}

void Term::CalcExp() {
	exp=0;
	if(str.length() == 0) {
		return;
	}
	int itmp=0;
	for(int i=0; i<str.length()-1; i++) {
		if(isdigit(str[i])) {
			itmp*=10;
			itmp+=str[i]-'0';
		}
		else if(str[i]>='a' && str[i]<='z') {
			if(str[i+1]>='a' && str[i+1]<='z') {
				exp++;
			}
			exp+=itmp;
			itmp=0;
		}
	}
	if(isdigit(str[str.length()-1])) {
		itmp*=10;
		itmp+=str[str.length()-1]-'0';
		exp+=itmp;
	}
	else {
		exp++;
		exp+=itmp;
	}
}

Term::Term() {
	coef=0;
	exp=0;
	str="";
}

Polynomial Polynomial::Add(Polynomial b) {
	Polynomial c;
	int aPos=0, bPos=0;
	int i,len, chk;

	while((aPos<terms) && (bPos<b.terms)) {
		if(termArray[aPos].exp == b.termArray[bPos].exp) {
			if(termArray[aPos].str.compare(b.termArray[bPos].str)==0) {
				int t = termArray[aPos].coef + b.termArray[bPos].coef;
				if(t) {
					c.termArray[c.terms].coef = t;
					c.termArray[c.terms++].str = termArray[aPos].str;
				}
				aPos++; bPos++;
			}
			else {
				len = termArray[aPos].str.length() < b.termArray[bPos].str.length() ? 
					termArray[aPos].str.length() : b.termArray[bPos].str.length();

				for(i=0;i<len;i++){
					if(termArray[aPos].str[i] != b.termArray[bPos].str[i])
						break;
				}
				if(isdigit(termArray[aPos].str[i])) {
					if(termArray[aPos].str.compare(b.termArray[bPos].str) < 0) {
						chk=2;
					}
					else {
						chk=1;
					}
				}
				else {
					int num1=0, num2=0, j;
					if(termArray[aPos].str[i] == '^'){
						chk=1;
					}
					else if(b.termArray[bPos].str[i] == '^'){
						chk=2;
					}
					else{
						if(termArray[aPos].str.length() <= i+1 || termArray[aPos].str[i+1] != '^') {
							num1 = 1;
						}
						else{
							for(j=i+2; j<len; j++) {
								if( !isdigit(termArray[aPos].str[j]) )
									break;
								else {
									num1*=10;
									num1+=termArray[aPos].str[j]-'0';
								}
							}
						}

						if(b.termArray[bPos].str.length() <= i+1 || b.termArray[bPos].str[i+1] != '^') {
							num2 = 1;
						}
						else{
							for(j=i+2; j<len; j++) {
								if( !isdigit(b.termArray[bPos].str[j]) )
									break;
								else {
									num2*=10;
									num2+=b.termArray[bPos].str[j]-'0';
								}
							}
						}
						if(num1 < num2) {
							chk=2;
						}
						else {
							chk=1;
						}
					}
				}
				if (chk == 1) {
					c.termArray[c.terms].coef = termArray[aPos].coef;
					c.termArray[c.terms++].str = termArray[aPos].str;
					aPos++;
				}
				else if(chk == 2) {
					c.termArray[c.terms].coef = b.termArray[bPos].coef;
					c.termArray[c.terms++].str = b.termArray[bPos].str;
					bPos++;
				}
			}
		}
		else if(termArray[aPos].exp < b.termArray[bPos].exp) {
			c.termArray[c.terms].coef = b.termArray[bPos].coef;
			c.termArray[c.terms++].str = b.termArray[bPos].str;
			bPos++;
		}
		else {
			c.termArray[c.terms].coef = termArray[aPos].coef;
			c.termArray[c.terms++].str = termArray[aPos].str;
			aPos++;
		}
	}
	for(;aPos<terms;aPos++) {
		c.termArray[c.terms].coef = termArray[aPos].coef;
		c.termArray[c.terms++].str = termArray[aPos].str;
	}
	for(;bPos<b.terms;bPos++) {
		c.termArray[c.terms].coef = b.termArray[bPos].coef;
		c.termArray[c.terms++].str = b.termArray[bPos].str;
	}
	return c;
}
