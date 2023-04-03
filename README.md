# test1
## compile
`make`

## prof test
- first go to `cd prof`
- prof test results `./a && gprof a gmon.out > prof.txt`

## what is this
Found in [`src/main.c`](src/main.c):
- `Vec1` is a non-pointer-persistent vector (`memcpy` each entry)
- `Vec2` is a pointer-persistent vector (`malloc` each entry, then `memcpy`)

## what were my expectations
- I expected `Vec1` to be faster than `Vec2` but surprisingly it's not all that bad.

## what are the results
- `Vec2` was faster in most test cases. I always thought that `malloc` takes a lot of time, but apparently not (in my test cases. Maybe I just picked wrong test numbers).

## am I surprised
- not really
- in fact, I'm actually relieved, considering the huge benefit of pointer persistence of `Vec2` over `Vec1` (_if I programmed it correctly_).of `Vec2` over `Vec1` (_if I programmed it correctly_).

