#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define NUM_ITEMS   6
#define VEC_DEFAULT     4

typedef struct Foo {
    size_t a[NUM_ITEMS];
} Foo;

typedef struct Vec1 {
    Foo *items;
    size_t l;
    size_t c;
} Vec1;

typedef struct Vec2 {
    Foo **items;
    size_t l;
    size_t c;
} Vec2;


#define IMAX_BITS(m) ((m)/((m)%255+1) / 255%255*8 + 7-86/((m)%255+12))
#define RAND_MAX_WIDTH IMAX_BITS(RAND_MAX)
_Static_assert((RAND_MAX & (RAND_MAX + 1u)) == 0, "RAND_MAX not a Mersenne number");

__attribute__((noinline, noclone))
uint64_t rand64(void) {
  uint64_t r = 0;
  for (int i = 0; i < 64; i += RAND_MAX_WIDTH) {
    r <<= RAND_MAX_WIDTH;
    r ^= (unsigned) rand();
  }
  return r;
}


__attribute__((noinline, noclone))
int foo_rand(Foo *foo)
{
    if(!foo) return -1;
    for(size_t i = 0; i < NUM_ITEMS; i++) {
        foo->a[i] = rand64();
    }
    return 0;
}

/**********************************************************/
/* VEC1 ***************************************************/
/**********************************************************/

__attribute__((noinline, noclone))
int vec1_append(Vec1 *vec, Foo *item)
{
    if(!vec || !item) return -1;
    size_t len = vec->l;
    size_t required = vec->c ? vec->c : VEC_DEFAULT;
    size_t cap = len + 1;
    while(required < cap) required *= 2;
    if(required > cap) {
        void *temp = realloc(vec->items, sizeof(*vec->items) * required);
        if(!temp) return -1;
        vec->items = temp;
        memset(&vec->items[len], 0, sizeof(*vec->items) * (required - len));
        vec->c = required;
    }
    memcpy(&vec->items[vec->l++], item, sizeof(*item));
    return 0;
}

__attribute__((noinline, noclone))
int vec1_get(Vec1 *vec, size_t index, Foo **item)
{
    if(!vec || !item) return -1;
    if(index >= vec->l) return -1;
    *item = &vec->items[index];
    return 0;
}

__attribute__((noinline, noclone))
int vec1_free(Vec1 *vec)
{
    if(!vec) return -1;
    free(vec->items);
    memset(vec, 0, sizeof(*vec));
    return 0;
}

/**********************************************************/
/* VEC2 ***************************************************/
/**********************************************************/

__attribute__((noinline, noclone))
int vec2_append(Vec2 *vec, Foo *item)
{
    if(!vec || !item) return -1;
    size_t len = vec->l;
    size_t required = vec->c ? vec->c : VEC_DEFAULT;
    size_t cap = len + 1;
    while(required < cap) required *= 2;
    if(required > cap) {
        void *temp = realloc(vec->items, sizeof(*vec->items) * required);
        if(!temp) return -1;
        vec->items = temp;
        memset(&vec->items[len], 0, sizeof(*vec->items) * (required - len));
        vec->c = required;
    }
    vec->items[len] = malloc(sizeof(**vec->items));
    if(!vec->items[len]) return -1;
    memcpy(vec->items[vec->l++], item, sizeof(*item));
    return 0;
}

__attribute__((noinline, noclone))
int vec2_get(Vec2 *vec, size_t index, Foo **item)
{
    if(!vec || !item) return -1;
    if(index >= vec->l) return -1;
    *item = vec->items[index];
    return 0;
}

__attribute__((noinline, noclone))
int vec2_free(Vec2 *vec)
{
    if(!vec) return -1;
    for(size_t i = 0; i < vec->c; i++) {
        free(vec->items[i]);
    }
    free(vec->items);
    memset(vec, 0, sizeof(*vec));
    return 0;
}

#define NUM_TESTS   10000 /* in ./prof/ this is the first number */
#define NUM_APPEND  10000 /* in ./prof/ this is the second number */

__attribute__((noinline, noclone))
void vec1_test(size_t n)
{
    Foo foo = {0};
    Foo *foo2 = 0;
    Vec1 vec1 = {0};
    for(size_t i = 0; i < n; i++) {
        foo_rand(&foo);
        vec1_append(&vec1, &foo);
    }
    for(size_t i = 0; i < n; i++) {
        vec1_get(&vec1, i, &foo2);
        foo_rand(foo2);
    }
    vec1_free(&vec1);
}

__attribute__((noinline, noclone))
void vec2_test(size_t n)
{
    Foo foo = {0};
    Foo *foo2 = 0;
    Vec2 vec2 = {0};
    for(size_t i = 0; i < n; i++) {
        foo_rand(&foo);
        vec2_append(&vec2, &foo);
    }
    for(size_t i = 0; i < n; i++) {
        vec2_get(&vec2, i, &foo2);
        foo_rand(foo2);
    }
    vec2_free(&vec2);
}

int main(void)
{
    printf("sizeof foo %zu\n", sizeof(Foo));
    printf("sizeof vec1 %zu\n", sizeof(Vec1));
    printf("sizeof vec2 %zu\n", sizeof(Vec2));
    for(size_t i = 0; i < NUM_TESTS; i++) {
        printf("iteration %zu\r", i);
        vec1_test(NUM_APPEND);
        vec2_test(NUM_APPEND);
    }
    printf("\ndone\n");
    return 0;
}

