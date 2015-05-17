//Compile: g++ test.cpp -ltap++ -o test

#include <time.h>
#include <tap++/tap++.h>
using namespace TAP;
#include "../perl_like.h"
using namespace pl;

//Specify which specific Scalar we wanna test
#define ScalarTest	Scalar

void next_test(string s) {
	cout << nl << "---=============" << s << "=============---" << nl;
}

void sub_test(string s) {
	cout << nl << "   -------------" << s << "-------------   " << nl;
}


number r() { return rand() % 100 + 1; }
// generating random real's, instead of int's
//float r() { return rand() % 100 + 1 + rand()/100; }

// is(0.0,0.0, ...) fails, shouldnt!!! that's why we need to handle it manually
void myis (number got, number expected, string msg) {
	if (got == 0 && expected == 0) {
		is(0,0,msg);
	} else {
		is(got,expected,msg);
	}
}

void utility_tests() {
	next_test("utility functions");

	is("12345", num2str(12345), "num2str(12345)");
	is( 12345 , str2num("12345").second, "str2num('12345')");

	ScalarTest $s = 5; int i = 5;
	is($s.is_num(),true,"$s.is_num() => true");
	is($s.is_str(),false,"$s.is_str() => false");
	is($s.to_num(),i,"number => $s.to_num()");
	is($s.to_str(),"5","number => $s.to_str()");
	$s = "string"; string str = "string";
	is($s.is_str(),true,"$s.is_str() => true");
	is($s.is_num(),false,"$s.is_str() => false");
	is($s.to_str(),str,"string => $s.to_str()");
	is($s.to_num(),0,"string => $s.to_num()");
}


void create_scalar() {

	next_test("Creating scalars");
	number n = r();
	ScalarTest sn(n);
	is(sn.is_num(), true, "Creating Scalar-number("+ sn.to_str() +")");
	sn.dump("sn");
	is(num2str(n), sn.to_str(), "sn.to_str() conversion");

	string str = "a string";
	ScalarTest sss(str);
	bool is_sss = sss.is_str() && sss.str() == str ? true : false;
	is(is_sss, true, "Creating Scalar-string-string('"+ sss.to_str() +"')");
	sss.dump("sss");
	is(0 == 0, sss.to_num() == 0, "sss.to_num() conversion");//is(0.0, 0.0, ...) doesn't work in this ver of libtap++

	ScalarTest ssn(num2str(n));
	ssn.dump("ssn");//subtype is string, but on summation should be interpeted as number
	is(ssn.is_num(), false, "Creating Scalar-string-number('"+ ssn.to_str() +"')");
	is(n, ssn.to_num(), "ssn.to_num() conversion");//ensures it will be ok when +,-

	ScalarTest $sn1(55);
	is($sn1.num() == 55,true,"Scalar $s(55)");
	ScalarTest $ss1("a text");
	//havent tested comparison yet, thats why we use .str()
	is($ss1.str() == "a text",true,"Scalar $s('a text')");

}


void copy_init() {
	next_test("Create and assign");

	int ni = r();
	int i = ScalarTest(ni);
//ScalarTest(ni).dump(); cout << i << nl;
	is(i,ni,"int i = Scalar(" + num2str(ni) + ")");

	float nf = r() + r()/100;//generate real float
	ScalarTest $sn2 = 55;
	is($sn2.num() == 55, true, "Scalar $s = 55");
	ScalarTest $s_str = "another string";
	is($s_str.str() == "another string", true, "Scalar $s = 'a string'");

	float $f = ScalarTest(nf);
	is($f,nf,"float f = Scalar(" + num2str(nf) + ")");

	string str = "a string";
	string $s = ScalarTest(str);
	is($s,str,"string str = Scalar('" + str + "')");

	ScalarTest $sn = int(ni);
	is($sn.num(), ni, "Scalar $sn = int(20)");
	ScalarTest $ss = string("a_string");
	is($ss.str(), "a_string", "scalar $ss = string('a_string')");

	ScalarTest $s_zero = 0;//conversion ambiguity
	is($s_zero.num() == 0, true ,"Zero assignemnt: Scalar $zero = 0");
	$s_zero.dump("$s_zero");
}

void direct_init() {
	next_test("Direct assign");
	ScalarTest $s1, $s2;
	number n1 = r();
	string str = "a text";

	$s1 = n1;
	is($s1.num(), n1, "$s = 55");

	$s2 = str;
	is($s2.str(), str, "$s = 'a text'");

	$s1 = $s2;
	is($s1.str(),$s2.str(), "$Sn = $Ss");
	$s1.dump("$s1"); $s2.dump("$s2");

//	$s1(n1);//!FIXME no match call for ..
}

void comparison_ops() {
	next_test("comparison operators");
	number n = r();
	ScalarTest $s1(n), $s2(n);
	ScalarTest $s_str1("text");
	string str2 = "text";

	is($s1 == n, true, "scalar == number");
	is($s1 != n, false, "scalar != number");
	is($s_str1 == str2, true, "scalar == string");
	is($s_str1 != str2, false, "scalar != string");
	is($s1 == $s2, true, "scalar == scalar");
	is($s1 != $s2, false, "scalar != scalar");

	$s1 = n+1;
	is($s1 > n, true, "scalar > number");
	is($s1 < n, false, "scalar < number");
	is($s_str1 > str2, false, "scalar > string");
	is($s_str1 < str2, false, "scalar < string");
	is($s1 > $s2, true, "scalar > scalar");
	is($s1 < $s2, false, "scalar < scalar");

	is($s1 >= n, true, "scalar >= number");
	is($s1 <= n, false, "scalar <= number");
	is($s_str1 >= str2, true, "scalar >= string");
	is($s_str1 <= str2, true, "scalar <= string");
	is($s1 >= $s2, true, "scalar >= scalar");
	is($s1 <= $s2, false, "scalar <= scalar");

//	$s1 = "compare"; $s2 = "compare";
//	is($s1.cmp($s2), 0, "$s1.cmp($s2)");

	ScalarTest $zero(0);//ambiguity
	is($zero == 0, true ,"Zero comparison: $zero == 0");
	is($zero != 0, false,"Zero comparison: $zero != 0");

	next_test("booleans");
	is((bool) $zero,false, "if($zero) false?");
	is((bool) !$zero,true, "if(!$zero) true?");
	ScalarTest $s_zero("0");
	is((bool) $s_zero,false,"if($str_zero) false?");
	is((bool) !$s_zero,true,"if(!$str_zero) true?");
	is((bool) $s_str1,true,"if($Ss) true?");
	is((bool) !$s_str1,false,"if(!$Ss) false?");
	ScalarTest $szt("0E0");//zero but true
	is((bool) $szt, true, "if('0E0'), zero but true");
	$szt.dump("SZT");
}

void regex() {
	next_test("regular expressions");
	string str = "quick brown fox is 5 years old";
	ScalarTest $s(str);
	is($s ^= "brown", true, " if ($s ^= 'brown') match(ok) ");
	is($s ^= "blue", false, " if ($s ^= 'blue') fail(ok) ");
	is($s ^= "\\d", true, "if ($s ^= '\\\\5') match(ok) ");
}

void operations() {
	next_test("operation tests");
	number n1 = r(); number n2 = r();
	string str1 = "abc"; string str2 = "xyz";
	ScalarTest $sn1(n1); ScalarTest $sn2(n2);
	ScalarTest $ss1(str1); ScalarTest $ss2(str2);
	ScalarTest $ssn1(num2str(n1)); ScalarTest $ssn2(num2str(n2));

	number op_rv = n1 + n2;
	cout << "n1,n2 :" << n1 << "," << n2 << nl;
	$sn1.dump("$Sn1"); $sn2.dump("$Sn2");
	$ss1.dump("$Ss1"); $ss2.dump("$Ss2");
	$ssn1.dump("$Ssn1"); $ssn2.dump("$Ssn2");

	sub_test("shortcut summation");
	ScalarTest $srv(n1);
	$srv += n2;
	is($srv == op_rv, true, "$Sn1 += n2");
	$srv = n1;

	sub_test("summation");
	number rv = $sn1 + n2;	is(rv, op_rv, "$Sn1 + n2 = sum");
	rv = $ss1 + n2;	is(rv, n2, "$Ss1 + n2 = n2");
	rv = $sn1 + $sn2;	is(rv,op_rv, "$Sn1 + $Sn2 = sum");
	rv = $sn1 + $ss1;	is(rv,$sn1.num(), "$Sn1 + $Ss1 = n1");
	rv = $ssn1 + $ssn2; is(rv,op_rv, "$Ssn1 + $Ssn2 = sum");
	rv = $ssn1 + n2; is(rv,op_rv, "$Ssn1 + n2 = sum");

	string op_rvs = str1+str2;
	number rvs = $ss1 + str2;
	is(rvs, 0, "$Ss1 + str2 = 0, not concatenation");
	rvs = $ss1 + $ss2;
	is(rvs, 0, "$Ss1 + $Ss2 = 0, not concatenation");

	sub_test("Scalar + Scalar");
	ScalarTest $rv = $sn1 + $sn2;
	is($rv.num(),op_rv,"$Sn1 + $Sn2");
	$rv = $sn1 + $ss1;
	is($rv.num(),n1,"$Sn1 + $Ss1 = $Sn1.num()");

	sub_test("division");
}

void inc_dec() {
	next_test("increment/decrement");

	int i = r();
	ScalarTest $s1(i);
	int x = ++$s1;
	is(x,i+1,"++scalar");
	$s1 = i; x = $s1++;
	is(x,i,"scalar++");
	$s1 = i; x = --$s1;
	is(x,i-1,"--scalar");
	$s1 = i; x = $s1--;
	is(x,i,"scalar--");
}

void concatenation() {
	next_test("concatenation");
	ScalarTest $s1("abc");
	ScalarTest $s2("xyz");

	ScalarTest $ss = $s1 | $s2;
	is($ss,"abcxyz", "$Ss1 | $Ss2");
	$ss = $s1 | "zzz";
	is($ss,"abczzz", "$Ss1 | 'zzz'");
	$ss = $s1 | "a" | $s2 | "z";
	is($ss,"abcaxyzz", "$Ss1 | 'a' | $Ss2 | 'z'");
}

/* !FIXME
void hash_tests() {
	next_test("hash");
	hash $hash;

	$hash["elem1"] = ScalarTest(555);
	is($hash.exists("elem1"), true,  "$hash.exists('elem1')");
//bool b = ($hash["elem1"] == 555 );
//	is(($hash["elem"]).nump(), "elem1", "$hash['elem'] = 555");
}
*/

int main (int argc, const char* argv[]) {
	string arg  = "all";

	srand(time(NULL));
	if (argc > 1) arg = argv[1];
	if (arg == "all" || arg == "utility") utility_tests();
	if (arg == "all" || arg == "create" ) create_scalar();
	if (arg == "all" || arg == "direct" ) direct_init();//direct initialization: $s = X
	if (arg == "all" || arg == "copy"   ) copy_init();//copy initialization: Scalar $s = X;
	if (arg == "all" || arg == "compare") comparison_ops();
	if (arg == "all" || arg == "ops"    ) operations();
	if (arg == "all" || arg == "inc_dec") inc_dec();
	if (arg == "all" || arg == "concat" ) concatenation();
	if (arg == "all" || arg == "regex"  ) regex();

	done_testing();
	return 0;
}