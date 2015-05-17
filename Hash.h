#include <iostream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <regex.h>
#include <map>
#include <vector>
#include <typeinfo>
#include <stdexcept>
using namespace std;

namespace pl {

#define JOIN2(a,b) a##b
#define JOIN(a,b) JOIN2(a,b)
#define foreach(a) for(bool JOIN(flag,__LINE__)=a.first();JOIN(flag,__LINE__);JOIN(flag,__LINE__)=a.next())

class kv_list : public vector<string> {
	protected:
		kv_list::iterator it;
	public:
		//foreach helpers
		bool first() { it = begin(); return it != end(); }
		bool next()  { ++it; return it != end(); }
		string current(){ return *it; }
};



class Hash : public map<string,scalar> {
	private:
		kv_list ary;//used in keys, values
	protected:
		Hash::iterator it;

	public:

		//foreach helpers
		bool first() { it = begin(); return it != end(); }
		bool next()  { ++it; return it != end(); }
		pair<string,scalar> current(){ return *it; }
		string key() { return it->first; }
		scalar value() { return it->second; }
		void value(const scalar& s) { it->second = s; }


		bool exists(const string& key) {
			if (this->find(key) != this->end()) return true;
			return false;
		}

		const kv_list& keys() {
			ary.clear();
			foreach((*this)) {
				ary.push_back( this->current().first );
			};
			return ary;
		}

		void dump() {
			//we use (*this) instead of just *this, so that unpacking foreach macro generates
			// (*this).begin(), (*this).end(), etc...
			foreach((*this)) {
				cout << this->key() << " => " << this->value() << nl;
			}
		}

//		void dump() { cout << dump_as_str() << nl; }

		bool match(const string& str) {
			foreach((*this)) {
				if (this->value() ^=  str) return true;
			}
			return false;
		}

		bool equal(const scalar& s) {
			foreach((*this)) {
				if (this->value() == s) return true;
			}
			return false;
		}


		//hyper operators
		bool operator ^= (const string& str) {
			return match(str);
		}
		bool operator == (const scalar& s) {
			return equal(s);
		}
		Hash& operator += (const scalar& rhs) {
			foreach((*this)) it->second += rhs;
			return *this;
		}
		Hash& operator -= (const scalar& rhs) {
			foreach((*this)) it->second -= rhs;
			return *this;
		}
		Hash& operator *= (const scalar& rhs) {
			foreach((*this)) it->second *= rhs;
			return *this;
		}
		Hash& operator /= (const scalar& rhs) {
			foreach((*this)) it->second /= rhs;
			return *this;
		}
		Hash& operator ++ () {
			foreach((*this)) ++it->second;
			return *this;
		}
		Hash& operator ++ (int) {
			foreach((*this)) it->second++;
			return *this;
		}
		Hash& operator -- () {
			foreach((*this)) --it->second;
			return *this;
		}
		Hash& operator -- (int) {
			foreach((*this)) it->second--;
			return *this;
		}

};


};//end namespace
