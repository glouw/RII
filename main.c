#include <stdio.h>
#include <stdbool.h>

#define P
#define T char
#include <que.h>

#include <str.h>

int line = 0;

#define quit(...) printf("error: line %d: ", line), printf(__VA_ARGS__), exit(1)

typedef enum { OBJ, ARR, STR, NUM, BLN, NUL } Type;

typedef struct Val* Elem;

bool
Equal(str* a, const char* b)
{
    return str_compare(a, b) == 0;
}

void Elem_free(Elem*);
Elem Elem_copy(Elem*);
#define T Elem
#include <vec.h>

typedef struct
{
    str str;
    Elem elem;
}
Memb;

void Memb_free(Memb*);
Memb Memb_copy(Memb*);
#define T Memb
#include <set.h>

typedef union
{
    set_Memb obj;
    vec_Elem arr;
    str str;
    double num;
    bool bln;
}
Poly;

typedef struct Val
{
    Type type;
    Poly poly;
}
Val;

Elem
Elem_Init(Type type, Poly poly)
{
    Elem elem = malloc(sizeof(*elem));
    elem->type = type;
    elem->poly = poly;
    return elem;
}

void
Elem_free(Elem* e)
{
    Type type = (*e)->type;
    Poly poly = (*e)->poly;
    switch(type)
    {
    case OBJ:
        set_Memb_free(&poly.obj);
        break;
    case ARR:
        vec_Elem_free(&poly.arr);
        break;
    case STR:
        str_free(&poly.str);
        break;
    default:
        break;
    }
    free(*e);
}

Elem
Elem_copy(Elem* e)
{
    Type type = (*e)->type;
    Poly poly = (*e)->poly;
    Elem copy = malloc(sizeof(*copy));
    copy->type = type;
    switch(type)
    {
    case OBJ:
        copy->poly.obj = set_Memb_copy(&poly.obj);
        break;
    case ARR:
        copy->poly.arr = vec_Elem_copy(&poly.arr);
        break;
    case STR:
        copy->poly.str = str_copy(&poly.str);
        break;
    default:
        copy->poly = poly;
        break;
    }
    return copy;
}

void
Tabs(int tabs)
{
    int width = 4;
    while(tabs--)
        for(int i = 0; i < width; i++)
            putchar(' ');
}

void
Elem_write(Elem* e, int tabs)
{
    int indent = tabs + 1;
    Type type = (*e)->type;
    Poly poly = (*e)->poly;
    size_t index = 0;
    switch(type)
    {
    case OBJ:
        putchar('{');
        putchar('\n');
        foreach(set_Memb, &poly.obj, it)
        {
            Tabs(indent);
            printf("\"%s\" : ", it.ref->str.value);
            Elem_write(&it.ref->elem, indent);
            index += 1;
            if(index < poly.obj.size)
                putchar(',');
            putchar('\n');
        }
        Tabs(tabs);
        putchar('}');
        break;
    case ARR:
        putchar('[');
        putchar('\n');
        foreach(vec_Elem, &poly.arr, it)
        {
            Tabs(indent);
            Elem_write(it.ref, indent);
            index += 1;
            if(index < poly.arr.size)
                putchar(',');
            putchar('\n');
        }
        Tabs(tabs);
        putchar(']');
        break;
    case STR:
        printf("%s", poly.str.value);
        break;
    case BLN:
        printf("%s", poly.bln ? "true" : "false");
        break;
    case NUL:
        printf("null");
        break;
    case NUM:
        printf("%f", poly.num);
        break;
    default:
        break;
    }
}

void
Elem_print(Elem* e)
{
    Elem_write(e, 0);
    putchar('\n');
}

void
Memb_free(Memb* m)
{
    str_free(&m->str);
    Elem_free(&m->elem);
}

Memb
Memb_copy(Memb* m)
{
    return (Memb) { str_copy(&m->str), Elem_copy(&m->elem) };
}

int
Memb_Compare(Memb* a, Memb* b)
{
    return str_key_compare(&a->str, &b->str);
}

que_char
Queue(const char* code)
{
    que_char q = que_char_init();
    for(const char* c = code; *c; c++)
        que_char_push(&q, *c);
    return q;
}

bool IsSpace(char c) { return c == ' ' || c == '\n'; }
bool IsLower(char c) { return c >= 'a' && c <= 'z'; }
bool IsUpper(char c) { return c >= 'A' && c <= 'Z'; }

bool
IsString(char c)
{
    return IsLower(c) || IsUpper(c) || IsSpace(c);
}

bool
IsDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool
IsNum(char c)
{
    return IsDigit(c) || c == '.';
}

bool
IsIdent(char c)
{
    return IsLower(c) || IsUpper(c) || IsDigit(c) || c == '_';
}

void
Pop(que_char* q)
{
    if(*que_char_front(q) == '\n')
        line += 1;
    que_char_pop(q);
}

void
Spin(que_char* q)
{
    while(que_char_empty(q) == false)
        if(IsSpace(*que_char_front(q)))
            Pop(q);
        else
            break;
}

char
Next(que_char* q)
{
    Spin(q);
    return *que_char_front(q);
}

void
Match(que_char* q, char c)
{
    char n = Next(q);
    if(n != c)
        quit("got '%c'; expected '%c'\n", n, c);
    Pop(q);
}

str
Read(que_char* q, bool clause(char))
{
    Spin(q);
    str s = str_init("");
    while(que_char_empty(q) == false)
    {
        char c = *que_char_front(q);
        if(clause(c))
        {
            str_push_back(&s, c);
            Pop(q);
        }
        else
            break;
    }
    return s;
}

str
String(que_char* q)
{
    Match(q, '"');
    str s = Read(q, IsString);
    Match(q, '"');
    return s;
}

Elem Element(que_char*, set_Memb*);

vec_Elem
Array(que_char* q, set_Memb* idents)
{
    vec_Elem es = vec_Elem_init();
    Match(q, '[');
    if(Next(q) != ']')
        while(true)
        {
            vec_Elem_push_back(&es, Element(q, idents));
            if(Next(q) == ',')
                Match(q, ',');
            else
                break;
        }
    Match(q, ']');
    return es;
}

double
Number(que_char* q)
{
    str s = Read(q, IsNum);
    double d = strtod(s.value, NULL);
    str_free(&s);
    return d;
}

Elem
Ident(que_char* q, set_Memb* idents)
{
    str s = Read(q, IsIdent);
    set_Memb_node* node = set_Memb_find(idents, (Memb) { .str = s });
    if(node == NULL)
        quit("identifier '%s' not defined\n", s.value);
    str_free(&s);
    return Elem_copy(&node->key.elem);
}

set_Memb Object(que_char*, set_Memb* idents);

Elem
Element(que_char* q, set_Memb* idents)
{
    char n = Next(q);
    return (n == '"') ? Elem_Init(STR, (Poly) {.str = String(q)})
         : (n == '{') ? Elem_Init(OBJ, (Poly) {.obj = Object(q, idents)})
         : (n == '[') ? Elem_Init(ARR, (Poly) {.arr = Array(q, idents)})
         : (IsNum(n)) ? Elem_Init(NUM, (Poly) {.num = Number(q)}) : Ident(q, idents);
}

void
Members(que_char* q, set_Memb* idents, set_Memb* m)
{
    while(true)
    {
        str s = String(q);
        Match(q, ':');
        set_Memb_insert(m, (Memb) { s, Element(q, idents) });
        if(Next(q) == ',')
            Match(q, ',');
        else
            break;
    }
}

set_Memb
Object(que_char* q, set_Memb* idents)
{
    set_Memb m = set_Memb_init(Memb_Compare);
    Match(q, '{');
    if(Next(q) != '}')
        Members(q, idents, &m);
    Match(q, '}');
    return m;
}

void
Let(que_char* q, set_Memb* idents)
{
    str s = Read(q, IsIdent);
    Match(q, '=');
    set_Memb_insert(idents, (Memb) { s, Element(q, idents) });
    Match(q, ';');
}

set_Memb
Setup(void)
{
    static Poly zero;
    set_Memb idents = set_Memb_init(Memb_Compare);
    Memb members[] = {
        { str_init("true"),  Elem_Init(BLN, (Poly) {.bln = true  }) },
        { str_init("false"), Elem_Init(BLN, (Poly) {.bln = false }) },
        { str_init("null"),  Elem_Init(NUL, zero) },
    };
    for(size_t i = 0; i < len(members); i++)
        set_Memb_insert(&idents, members[i]);
    return idents;
}

void
Execute(const char* code)
{
    set_Memb idents = Setup();
    que_char q = Queue(code);
    while(que_char_empty(&q) == false)
    {
        str action = Read(&q, IsIdent);
        if(Equal(&action, "let"))
            Let(&q, &idents);
        str_free(&action);
        Spin(&q);
    }
    foreach(set_Memb, &idents, it)
        Elem_print(&it.ref->elem);
    que_char_free(&q);
    set_Memb_free(&idents);
}

int
main(void)
{
    Execute(
        "let a = [1,2,3,4, [1,2,3,4], {\"a\": 1, \"b\": 2, \"c\": [1,2,3,4]}];\n"
        "let b = {\"copied\": a};\n"
        "let c = 3;\n"
    );
}
