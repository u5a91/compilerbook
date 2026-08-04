#!/bin/bash

assert() {
    expected="$1"
    input="$2"

    ./9cc "$input" > tmp.s
    cc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}

assert 0 0
assert 42 42
assert 0 "1+2-3"
assert 6 "1 + 2 + 3"
assert 6 "(1+2)+3"
assert 6 "1*2*3"
assert 88 "(12/3+4)*(5+6)"
assert 2 "8/2/2"
assert 2 "-1+3"

assert 1 "2026>804"
assert 0 "2026<804"
assert 1 "804<2026"
assert 1 "2026>=804"
assert 0 "2026<=804"
assert 1 "804<=2026"
assert 1 "20==20"
assert 0 "20==26"
assert 1 "20!=26"
assert 0 "20!=20"