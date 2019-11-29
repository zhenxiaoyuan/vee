# Vee
Vee is a HTTP WebServer based on multithreading, under continuous improvement...
```
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C                               15            347            123           1375
Markdown                         7             80              0            334
C/C++ Header                    14            120            141            307
make                             2             25              2             47
-------------------------------------------------------------------------------
SUM:                            38            572            266           2063
-------------------------------------------------------------------------------
```

## Usage
### Build
```bash
git clone https://github.com/zhenxiaoyuan/vee.git
cd vee/src
make
```
### Run
```bash
./vee               # default port - 7777
```

## Features
- Epoll, non-block, edged trigger
- Thread pool, task queue
- Timer, priority queue
- Rubost I/O
- HTTP
- Logger
- Error handler

## Todo
- [x] Port reuse
- [x] Config file
- [ ] Handle SIGPIPE
- [ ] Memory pool
- [ ] Memory mapping
- [ ] Error handler and logger station
- [ ] Domain logger
- [ ] Makefile, maybe cmake later
- [ ] Unit test
- [ ] Benchmark
- [ ] Graphviz ang Codeviz
- [ ] HTTP features

## Test

## Benchmark

## Reference
[zaver](https://github.com/zyearn/zaver)  
[TKeed](https://github.com/linw7/TKeed)  
[vino](https://github.com/tinylcy/vino)  
