#include "testlib.h"
#include "parseint.h"

int main()
{
    test_start("parseint.c");

    test_equals_int(parseDecimalChar('.'), -1);
    test_equals_int(parseDecimalChar('/'), -1);
    test_equals_int(parseDecimalChar('0'), 0);
    test_equals_int(parseDecimalChar('4'), 4);
    test_equals_int(parseDecimalChar('8'), 8);
    test_equals_int(parseDecimalChar('9'), 9);
    test_equals_int(parseDecimalChar(':'), -1);
    test_equals_int(parseInt("010"), 8);
    test_equals_int(parseInt("1234"), 1234);
    test_equals_int(parseInt("?"), -1);
    test_equals_int(parseInt("01234"), 668);
    test_equals_int(parseInt("1234"), 1234);
    test_equals_int(parseInt("0234df3"), -1);
    test_equals_int(parseInt("033f"), -1);
    test_equals_int(parseInt("1345g"), -1);
    test_equals_int(parseInt("0"), -0);
    test_equals_int(parseInt("a1"), -1);
    test_equals_int(parseInt("00000"), 0);
    test_equals_int(parseInt("09"), -1);
    test_equals_int(parseInt("0x0"), -1);
    test_equals_int(parseInt("a"), -1);
    test_equals_int(parseInt(""), -1);
    test_equals_int(parseInt("100a"), -1);
    test_equals_int(parseInt("010?"), -1);
    test_equals_int(parseInt("08"), -1);
    test_equals_int(parseInt("03729"), -1);
    return test_end();
}
