# Roman II

Roman II is a simple to use programming language supporting no more than dictionaries,
lists, basic control flow, looping, and the conventional built in CPU data-types.

## Entry

Roman II requires the conventional entry point:

```
main()
{
}
```

## Basic Types

Roman II supports the conventional CPU types:

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

* Variables without a specified type, if a string, default as a list of u8 characters.
* Variables without a specified type, if a number, default to type `f64` (see variable `l`).
* Variables without a specified type, if a character, default to type `u8` (see variable `m`).
* Variables - when hexidecimal - and without a specified type, obtain the smallest amount of available
unsigned memory. Variable `o` types as `u8`, variable `p` as `u16`, variable `q`
as `u32`, and variable `r` as `u64`.

## Data structures

Roman II supports lists and dictionaries.

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
Lists are not contiguous, and thus support the mixing of types (see variable `d`).
Strings are lists of characters (see variables `e`, `f`, or `g`, are the same).
Dictionary keys can only be direct strings.

## Roman Script Object Notation (RSON)

Roman II introduces RSON, which merely extends the JavaScript Object Notation (JSON) with
the aforementioned basic Roman II types.

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

RSON objects are backwards compatible with JSON objects. JSON numbers will default to type `f64`
when imported into Roman II.

## Functions

Roman II functions pass values by reference. Functions are not terms and do not return values.

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

Roman II manages memory similar to that of C++'s RAII system. Resources are allocated and freed by brace.

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

Roman II assignment will free a previously declared value before assigning:

```
main()
{
    a = {};
    # a is freed here and then assigned.
    a = {};
}
```

Roman II assignment works similarly via function references:

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

Roman II assignment can also be done by value copy, both immediately or by
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
    b = a; # A copy of a is create dand assigned to b.
    c = {};
    d = {};
    modify(c, b);
}
```

* References are function arguments, and function arguments are references.
* References may not be defined anywhere elsewhere - Roman II creates copies otherwise.
* References may be protected with `const` for programmer safety:

```
modify(a, const b)
{
    a = b;
}
```

A Private Reference occurs when a function argument is a direct value.

```
main()
{
    a = {};
    modify(a, { "value" : 1 });
}
```

In such, the lifetime of the private reference is offloaded to the lifetime of the calling function.

# Control Flow

Roman II control flow is limited to `if` and `else` statements.

```
main()
{
    a = 1;
    b = 2;
    if(a + b > 2)
    {
        print("here is a: ");
        print(a);
        print('\n');
    }
    else if(a + b == 3)
    {
        print("here is b: ");
        print(b);
        print('\n');
    }
    else
    {
        print("Here is neither\n");
    }
}
```

# Looping

Roman II loops are limited to `while` and `for` loops.


The conventional `while` loop aids in standard expression looping:
```
main()
{
    a = (i32) 42;
    while(a)
    {
        print(a);
        print('\n');
        a -= 1;
    }
}
```

The conventional `for` loop aids in traversing either lists or dictionaries:

```
main()
{
    sum = 0;
    a = [ 1, 2, 3 ];
    for(x : a)
    {
        sum += x;
    }
    sum = 0;
    b = {
        'a' : 1,
        'b' : 2,
        'c' : 3,
    };
    for(x, y : b)
    {
        sum += y;
    }
}
```
