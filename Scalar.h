#include <iostream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <regex.h>
#include <map>
#include <vector>
#include <typeinfo>
#include <stdexcept>
#include <assert.h>
using namespace std;

namespace pl {

#define  
//cout << __PRETTY_FUNCTION__ << endl;
#define nl	endl
#define sep  << "," << 
#define pnl	cout << endl
#define pBool std::pair<bool,number>
#define MAX_RE_MATCHES	10

typedef double number;
enum scalar_subtypes { NUMBER = 1, STRING = 2 };


//========================UTILS====================================

template<class T>
void say(T val) { cout << val << nl; }

pBool str2num(const string& str) {
	istringstream is(str);
	pBool rv(false,0);
	if (str == "0E0") {//Zero but true
		rv = std::make_pair(true,0);
	} else {
		is >> rv.second;//convert
		//logical XOR : !fail != !0?
		if ( !is.fail() != (rv.second == 0) ) rv.first = true;
	};
	return rv;
};

string num2str(const number n) {
	ostringstream os;
	os << n;
	return os.str();
};


//========================Scalar====================================

class Scalar {
 protected:
 	struct {	string s; number n; } value;
 	scalar_subtypes sub_type;

	scalar_subtypes get_type() const { return sub_type;}
	const number get_num() const { return value.n; }
	const string& get_str() const { return value.s; }
	void set_type(const scalar_subtypes t) { sub_type = t;}
	void set_num(const number& num) { value.n = num; set_type(NUMBER); }
	void set_str(const string& str) {value.s = str; set_type(STRING); };


 public:
	regmatch_t matches[MAX_RE_MATCHES];//list of the matches in re-match

	scalar_subtypes type() const { return get_type();}
 	bool is_num() const { return get_type() == NUMBER; }
 	bool is_str() const { return get_type() == STRING; }
	const number num() const { return get_num(); }
	const string& str() const { return get_str(); }
	number to_num() const { return is_num() ? get_num() : str2num(get_str()).second; }
   string to_str() const { return is_str() ? get_str() : num2str(get_num()); }
	void num(const number& n) { set_num(n); }
	void str(const string& s) { set_str(s);};


	Scalar() { set_num(0); };
	//copy constructor : Scalar $s = 55
	Scalar(const Scalar& c) {
		c.get_type() == NUMBER ? set_num(c.num()) : set_str(c.str());
	}
	Scalar(const string& x) { set_str(x); };
	Scalar(const char* x) {
		string s = x;//convert to string first
		set_str(s);
	};
	Scalar(const number& n) {  set_num(n); };//$s = 55
 	Scalar(const int& n) {  set_num(n); };//Scalar $s = 0; zero-ambiugity assignment conv


	friend ostream& operator << (ostream& out, const Scalar& s) {
		s.is_num() ? out << s.num() : out << s.str();
		return out;
	};

	operator number() const { return to_num(); }
	operator float() const { return to_num(); }
	operator int() const { return to_num(); }
	operator char() const { return to_num(); }
	operator string() const { return to_str(); }
	//mimic boolean
	operator bool() const { return is_num() ? num() != 0 : str2num(str()).first; }

	Scalar& operator = (const Scalar& rhs) { 
		if (this == &rhs) return *this;//self-assignment no,no..!
		assert(rhs.is_str() || rhs.is_num());
		rhs.is_num() ? num(rhs.num()) : str(rhs.str());
		return *this;
	}

	template<class T>
	Scalar& operator = (const T& rhs) {
		return *this = Scalar(rhs);
	}

	//++prefix: return ref
	Scalar& operator ++ () {
		if (is_num()) num(num()+1);
		//if string nothig to do, just return
		return *this;
	}

	//postfix++ : return copy
	Scalar operator ++ (int) {
		Scalar $rv(num());
		++(*this);
		return $rv;
	}

	Scalar& operator -- () {
		if (is_num()) num(num()-1);
		//if string nothig to do, just return
		return *this;
	}

	//postfix--
	Scalar operator -- (int) {
		Scalar $rv(num());
		--(*this);
		return $rv;
	}

	//================== +,-,*,/ operators =========================


	Scalar& operator += (const Scalar& rhs) {
		if (is_num() && rhs.is_num()) { num(num() + rhs.num()); return *this; }
		if (is_num() && rhs.is_str()) {
			number n1 = rhs.to_num(); num( n1 ? num() + n1 : num() + 0);
			return *this;
		}
		if (is_str() && rhs.is_num()) {
			number n1 = to_num(); num( n1 ? n1 + rhs.num() : 0 + rhs.num());
			return *this;
		}
		if (is_str() && rhs.is_str()) {
			number n1 = to_num();
			number n2 = rhs.to_num();
			//logical XOR : first case str+str OR num+num, else ....
			if (!n1 != !n2)	{ n1 && !n2 ? num(n1 + 0)	 : num(0 + n1); }
				else				{ n1 && n2  ? num(n1 + n2) : num(n1 + n2); }
		}
		return *this;
	}


	template<class T>
	Scalar operator + (const T& rhs) {
		//first make a copy then do shortcut-summation
		Scalar $rv = *this;
		$rv += Scalar(rhs);
		return $rv;
	}


	Scalar& operator -= (const Scalar& rhs) {
		if (is_num() && rhs.is_num()) { num(num() - rhs.num()); return *this; }
		if (is_num() && rhs.is_str()) {
			number n1 = rhs.to_num(); num( n1 ? num() - n1 : num() - 0);
			return *this;
		}
		if (is_str() && rhs.is_num()) {
			number n1 = to_num(); num( n1 ? n1 - rhs.num() : 0 - rhs.num());
			return *this;
		}
		if (is_str() && rhs.is_str()) {
			number n1 = to_num();
			number n2 = rhs.to_num();
			//logical XOR : first case str-str OR num-num, else ....
			if (!n1 != !n2)	{ n1 && !n2 ? num(n1 - 0)	 : num(0 - n1); }
				else				{ n1 && n2  ? num(n1 - n2) : num(n1 - n2); }
		}
		return *this;
	}


	template<class T>
	Scalar operator - (const T& rhs) {
		Scalar $rv = *this;
		$rv -= Scalar(rhs);
		return $rv;
	}


	Scalar& operator *= (const Scalar& rhs) {
		if (is_num() && rhs.is_num()) { num(num() * rhs.num()); return *this; }
		if (is_num() && rhs.is_str()) {
			number n1 = rhs.to_num(); num( n1 ? num() * n1 : num() * 0);
			return *this;
		}
		if (is_str() && rhs.is_num()) {
			number n1 = to_num(); num( n1 ? n1 * rhs.num() : 0 * rhs.num());
			return *this;
		}
		if (is_str() && rhs.is_str()) {
			number n1 = to_num();
			number n2 = rhs.to_num();
			//logical XOR : first case str*str OR num*num, else ....
			if (!n1 != !n2)	{ n1 && !n2 ? num(n1 * 0)	 : num(0 * n1); }
				else				{ n1 && n2  ? num(n1 * n2) : num(n1 * n2); }
		}
		return *this;
	}


	template<class T>
	Scalar operator * (const T& rhs) {
		Scalar $rv = *this;
		$rv *= Scalar(rhs);
		return $rv;
	}


	Scalar& operator /= (const Scalar& rhs) {
		if (is_num() && rhs.is_num()) { num(num() / rhs.num()); return *this; }
		if (is_num() && rhs.is_str()) {
			number n1 = rhs.to_num(); num( n1 ? num() / n1 : throw std::overflow_error("Divide by zero exception"));
			return *this;
		}
		if (is_str() && rhs.is_num()) {
			number n1 = to_num(); num( n1 ? n1 / rhs.num() : 0 / rhs.num());
			return *this;
		}
		if (is_str() && rhs.is_str()) {
			number n1 = to_num();
			number n2 = rhs.to_num();
			//logical XOR : first case str/str OR num/num, else ....
			if (!n1 != !n2)	{ n1 && !n2 ? throw std::overflow_error("Divide by zero exception") : num(0 / n1); }
				else				{ n1 && n2  ? num(n1 / n2) : num(n1 / n2); }
		}
		return *this;
	}


	template<class T>
	Scalar operator / (const T& rhs) {
		Scalar $rv = *this;
		$rv /= Scalar(rhs);
		return $rv;
	}




	//================== comparison operators =========================


	bool operator == (const Scalar& rhs) const {
		if (is_num() && rhs.is_num()) return num() == rhs.num();
		if (is_num() && rhs.is_str()) return num() == rhs.to_num();
		if (is_str() && rhs.is_num()) return to_num() == rhs.num();
		if (is_str() && rhs.is_str()) return str() == rhs.str();
		return false;
	}

	template<class T>//T = number,string,Scalar
	bool operator == (const T& rhs) const {
		return *this == Scalar(rhs);
	}

	bool operator != (const Scalar& rhs) const {
		return !(*this == rhs);
	}

	template<class T>
	bool operator != (const T& rhs) const {
		return *this != Scalar(rhs);
	}

	bool operator > (const Scalar& rhs) const {
		if (is_num() && rhs.is_num()) return num() > rhs.num();
		if (is_num() && rhs.is_str()) return true;//num is always bigger
		if (is_str() && rhs.is_num()) return false;
		if (is_str() && rhs.is_str()) return str() > rhs.str();//!FIXME
		return false;
	}

	template<class T>
	bool operator > (const T& rhs) const {
		return *this > Scalar(rhs);
	}

	bool operator >= (const Scalar& rhs) const {
		return (*this > rhs || *this == rhs);
	}

	template<class T>
	bool operator >= (const T& rhs) const {
		return *this >= Scalar(rhs);
	}


	bool operator <= (const Scalar& rhs) const {
		return (*this < rhs || *this == rhs);
	}

	template<class T>
	bool operator <= (const T& rhs) const {
		return *this <= Scalar(rhs);
	}

	bool operator < (const Scalar& rhs) const {
		return !(*this >= rhs);
	}

	template<class T>//T = number,string,Scalar
	bool operator < (const T& rhs) const {
		return (*this < Scalar(rhs));
	}

	//--- bitwise OR | - is used for concatenation i.e. Scalar1 | Scalar2 | Scalar3 | ....
	Scalar operator | (const Scalar& rhs) {
		return to_str() + rhs.to_str();
	}

	//=== regex match operator Ex.: if ( scalar_var ^= "lookfor") .....
	bool operator ^=(const string& re_str) {
		if (is_num()) return false;//nothing to match in a number
		int rvc;//return value of compilation
		regex_t exp;//compiled regex

		rvc = regcomp(&exp,re_str.c_str(),REG_EXTENDED);//compile the re
		if (rvc != 0) return false;//couldn't compile
		int rv = regexec(&exp, str().c_str(), MAX_RE_MATCHES, matches, 0);
		regfree(&exp);
		return  rv == 0 ? true : false;
	};


	void dump() const {
		cout << "  .type:" << type();
		cout << ", .num:" << num();
		cout << ", .str:" << str() << nl;
	}
	void dump(const char* var_name) const {
		cout << "VAR:" << var_name;
		dump();
	}


};


};//end namespace



