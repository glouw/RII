# Specification

Roman II (RII) is a simple to use programming language supporting no more than dictionaries,
lists, basic control flow, looping, and the conventional built in CPU data-types.

## Entry

RII requires the conventional entry point:

```
main()
{
}
```

## Basic Types

RII supports the conventional CPU types:

```
main()
{
    # Unsigned integers
    a = (u8) 42;
    b = (u16) 42;
    c = (u32) 42;
    d = (u64) 42;

    # Signed integers
    e = (i8) 42;
    f = (i16) 42;
    g = (i32) 42;
    h = (i64) 42;

    # Floating point
    i = (f32) 42.0;
    j = (f64) 42.0;

    # Strings
    k = "The quick brown fox jumps over the lazy dog";

    # No type number.
    l = 43;

    # No type character.
    m = 'a';

    # No type hexadecimal values.
    o = 0xAA;
    p = 0xFFFF;
    q = 0xAAAAAAAA;
    r = 0xFFFFFFFFF;
}
```

Variables without a specified type, if a string, default as a list of u8 characters.

Variables without a specified type, if a number, default to type `f64` (see variable `l`).

Variables without a specified type, if a character, default to type `u8` (see variable `m`).

Variables - when hexidecimal - and without a specified type, obtain the smallest amount of available
unsigned memory. Variable `o` types as type `u8`, variable `p` types as type `u16`, variable `q`
as `u32`, and variable `r` as type `u64`.

## Data structures

RII supports lists and dictionaries.

```
main()
{
    a = {}; 
    b = [];
    c = {
        "key": (u8) 42;
    };
    d = [
        (u8) 42,
        (u8) 43,
        (u8) 44,
        (u8) 45,
        (u16) 88,
        (u32) 99,
        (f32) 199.0,
    ];
    e = "ABCD"; # A direct string.
    f = [ 'A', 'B', 'C', 'D' ];
    g = [ (u8) 65, (u8) 66, (u8) 67, (u8) 68 ];
}
```
Lists are not contiguous in memory, and thus support the mixing of types (see variable `d`).
Strings are lists of characters (see variables `e`, `f`, or `g`, are the same).
Dictionary keys can only be direct strings.

## Roman Script Object Notation (RSON)

RII introduces RSON, which merely extends the JavaScript Object Notation (JSON) with
the aforementioned basic RII types.

```
main()
{
    a = {
        "a": (u8) 42,
        "b": [ (i32) 1, (i32) 2, (i32) 3 ],
        "c": {
            "a": (u8) 32,
            "b": 'c'
        }
    };
}
```

RSON objects are backwards compatible with JSON objects. Numbers will default to type `f64`.

## Functions

RII functions pass values by reference. Functions are not terms and do not return values.

```
add(a, b)
{
    a += b;
}

main()
{
    a = [1];
    b = [2];
    c = { "a" : 1 };
    d = { "b" : 1 };
    e = 32;
    f = 32;
    add(a, b); # a == [1, 2]
    add(c, d); # c == { "a": 1, "b": 1 }
    add(e, f); # e == 64;
}
```

## Memory management

RII manages memory similar to that of C++'s RAII system. Resources are allocated and freed by brace.

```
main()
{
    a = 1;
    {
        b = 2;
        c = {};
        # Both `b` and `c` are freed here.
    }
    c = {};
    # Both `a` and `c` are freed here.
}
```

RII assignment will free a previously declared value before assigning:

```
main()
{
    a = {};
    # a is freed here and then assigned.
    a = {};
}
```

RII assignment works similarly via function references:

```
modify(a)
{
    # a is freed here and then assigned.
    a = {};
}

main()
{
    a = { "data" : [] };
    assign(a)
}
```

RII assignment can also be done by value copy, both immediately or by
reference through a function:

```
modify(a, b)
{
    # a is freed.
    # b is copied and assigned to a.
    a = b;
}

main()
{
    a = {};
    b = a;
    c = {};
    d = {};
    modify(c, b);
}
```

References are function arguments, and function arguments are references.
References may not be defined anywhere elsewhere - RII creates copies otherwise.
References may be protected with `const` as a matter of safety.

```
modify(a, const b)
{
    a = b;
}
```
