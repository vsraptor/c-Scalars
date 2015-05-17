#include "perl_like.h"
using namespace pl;

int main() {

	Scalar s1 = "The quick brown fox...";

	cout << "First it is a string: " << s1 << nl;
	s1 = 15;
	cout << " and then a number : "<< s1 << nl;

	pnl;
	Scalar s2(10);
	cout << "Then we sum two scalars : s1(15) + s2(20) = " << s1 + s2 << nl;

	//string concatenation is done using |, instead of +
	Scalar $s3, $s4;
	$s3 = "conca"; $s4 = "tenated";
	cout << "Lets concatenate strings : ";
	Scalar $s5 = $s3 | $s4;
	cout << $s5 << nl;

	//regex match
	if ($s5 ^= "tena") {
		cout << "match regular expression, succesfull" << nl;
	}

	//refer to how it behaves in Perl, if in doubt
	Scalar $s6 = $s5 * s1;
	cout << "Multiplying a string * number yelds : " << $s6 << nl;

	Scalar $s7 = 0;
	if ($s7) {}
	else { cout << "perl idiom if($s) : 0 => false" << nl; }

	$s7 = 1;
	if ($s7) cout << "perl idiom if($s) : not 0 => true" << nl;

	//you can always dump a variable to see the internal
	cout << "Here is how dumping a variable works $s5.dump()" << nl;
	$s5.dump();

	Scalar $zero_but_true = "0E0";
	if ($zero_but_true) cout << "Scalar is zero but true" << nl;

	return 0;
}