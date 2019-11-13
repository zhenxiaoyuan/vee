# vee

### Contents

#### Socket -- done

1 socket
1.1 declaration
1.2 listen-svr_sock
1.2.1 init
1.2.2 bind
1.2.3 listen
1.2.4 set_nonblock
1.3 accept-clt_sock
1.3.1 accept
1.3.2 set_nonblock # put in io or socket?
1.4 implement of set_nonblock
1.4.1 fcntl

#### Thread -- done

3 threadpool
3.1 init
3.2 add
3.3 destroy
3.4 task
3.4.1 task_queue
3.5 worker
3.5.1 mutex
3.5.2 cond

#### Timer -- done

4 timer
4.1 init
4.2 add
4.3 del
4.4 update
4.5 find
4.6 expire
*relate to priority_queue*

#### Priority Queue -- done

5 priority_queue
5.1 init
5.2 algorithm-min_heap
5.3 add
5.4 del

#### RIO -- done

6 rio
6.1 buffered
6.1.1 read
6.1.2 readline
6.2 unbuffered
6.2.1 read
6.2.2 write
*ref-CSAPP*

#### HTTP -- todo

7 http
7.1 request
7.2 parse
7.2.1 header
7.2.2 body
7.2.3 list

#### Config File -- todo

8 config
8.1 read the cfg file
8.2 TODO: sig when remote close

#### Log -- done

9 log and dbg
9.2.1 log_err
9.2.2 log_info
9.3 debug

#### Other -- todo

10. other 
10.1 why use #ifndef XXX_H ... in the forefront of header files?
10.2 what's close-on-exec? tlpi p1356
10.3 signal

致谢
