# vec_test
## compile
`make`

## prof test
- first go to `cd prof`
- prof test results `./a && gprof a gmon.out > prof.txt`
- I dumped some of my result in [`prof`](./prof). the filename suggests, that:
    - the first number (before the `x`) is `NUM_TESTS`
    - the second number (after the `x`) is `NUM_APPEND`

## what is this
Found in [`src/main.c`](src/main.c):
- `Vec1` is a non-pointer-persistent vector (`memcpy` each entry)
- `Vec2` is a pointer-persistent vector (`malloc` each entry, then `memcpy`)
- both hold `Foo` as their item

## what were my expectations
- I expected `Vec1` to be faster than `Vec2`, since calls less `malloc` / `realloc`.

## what are the results
- `Vec2` was faster in most test cases. I always thought that `malloc` takes a lot of time, but apparently not (in my test cases. Maybe I just picked wrong test numbers).

## am I surprised
- a bit, but when thinking about it, it does make sense.
- in fact, I'm actually relieved, considering the huge benefit of pointer persistence of `Vec2` over `Vec1` (_if I programmed it correctly_).

## further thought
- if the `sizeof(Foo)` were to increase, `Vec2` would likely be even faster than `Vec1`. _This is untested._

