#!/bin/zsh


assert(){
    expect="$1"

    input="$2"

    ./rvcc $input > tmp.s || exit

    riscv64-unknown-linux-gnu-gcc -static tmp.s -o tmp

    qemu-riscv64 -L $RISCV/sysroot ./tmp

    actual="$?"

    if [ "$actual" = "$expect" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expect expected, but got $actual"
        exit 1
    fi

}

assert 25 25
assert 42 42+6+9-5-10
assert 23 "22+      6 
 + 9 -5- 10 +1"


echo OK
