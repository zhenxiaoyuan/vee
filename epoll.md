## Epoll - event poll
**event** will notificate when the registered file descriptor monitored.

### 1. Header file
```
    #include <sys/epoll.h>
```

### 2. Functions

#### create -> epoll instance

```
    int epoll_create(int size);
    int epoll_create1();            // starting with kernel 2.6.27
            // return epfd on success, or -1 on error.
```

epfd - epoll file descriptor, refer to the new **epoll instance**

#### ctl -> interest list
 
```
    int epoll_ctl(int epfd, int op, int fd, struct epoll_event *ev);
            // return 0 on success, or -1 on error.
```

Operation|Error|File descriptor in the interest list?
---|:---|---:
EPOLL_CTL_ADD|EEXIST|yes
EPOLL_CTL_MOD|ENOENT|no
EPOLL_CTL_DEL|ENOENT|no

#### wait -> ready list

```
    int epoll_wait(int epfd, struct epoll_event *evlist, int maxevents, int timeout);
            // return number of ready fd, 0 on timeout, -1 on error.
```

Timeout | Description
---     | ---
-1      | block until occur
0       | nonblock check
>0      | block for up to timeout

### 3. Structure - epoll_event

```
struct epoll_event {
    uint32_t events;    /* epoll events (bit mask) */ 
    epoll_data_t data;  /* User data */
};

typedef union epoll_data {
    void        *ptr;   /* Pointer to user-defined data */
    int          fd;    /* File descriptor */
    uint32_t     u32;   /* 32-bit integer */
    uint64_t     u64;   /* 64-bit integer */
} epoll_data_t;
```

**epoll_ctl 函数中的第二个参数 fd 与 epoll_event.epoll_data_t.fd 有什么区别？**  
一样的，对应同一个 fd，只是用的位置不同。
- epoll_ctl 将 fd 注册到 interest list 中
- epoll 对这个 fd 进行监听
- fd 被触发，进入ready list
- epoll_wait 将 ready list 中的事件存储在 epoll_event 数组中并返回
- epoll_event.epoll_data_t.fd 进行 IO 操作（也可以将 epoll_event.epoll_data_t.fd 存储在 epoll_data_t.ptr 指向的结构体中）


Bit|Input to epoll_ctl()?|Returned by epoll_wait()?|Description
---|:---:|:---:|:---
EPOLLIN     | • | • |Data other than high-priority data can be read
EPOLLPRI    | • | • |High-priority data can be read
EPOLLRDHUP  | • | • |Shutdown on peer socket (since Linux 2.6.17)
EPOLLOUT    | • | • |Normal data can be written
EPOLLET     | • |   |Employ edge-triggered event notification
EPOLLONESHOT| • |   |Disable monitoring after event notification
EPOLLERR    |   | • |An error has occurred
EPOLLHUP    |   | • |A hangup has occurred

**为什么使用 EPOLLONESHOT?**  
每次请求处理完后，使用 ctl_mod, 刷新 epoll 对这个 fd 的监听，同时新增timer。这样可以继续保持连接，如果在timeout之前有新的请求进来，可以立刻处理。

### 4. Edge-Triggered Notification and starvation

- 当我们使用 Edge-Triggered 时，对应的 fd 只会被 epoll_wait() 触发一次，因此需要在被触发时完成所有I/O操作；相对应的，当我们使用Level-Triggered 时，对应的 fd 每次都会被 epoll_wait() 触发，因此不必一次性完成所有的 I/O 操作。
- 因为要一次性完成所有操作，为了不长时间阻塞线程/进程，一般需要和nonblock捆绑销售。
- 当要处理大数据块时，可能会长时间占用线程/进程，从而造成 fd starvation(怎么翻译？)，使用 timer 来防止饥饿问题的发生。


### 5. Epoll Semantics

关于语义，首先要弄清楚 *file descriptor | open file descriptor | i-node* 三者的关系。（见 TLPI-5.4节）  
因为 dup() 和 fork() 的存在，可能会出现多 file descriptor 和多 epoll file descriptor 的情况。

- file descriptor will close when request end or timeout.
- open file descriptor will removed only after all fd closed.
- epoll interest list will remove the file descriptor after reference open file descriptor removed.

因此，当 file descriptor 关闭后，epoll 会自动将其从 interest list中移出。  

### 6. Tips

- About multithread?  
In a multithreaded program, it is possible for one thread to use epoll_ctl() to add file descriptors to the interest list of an epoll instance that is already being monitored by epoll_wait() in another thread. These changes to the interest list will be taken into account immediately, and the epoll_wait() call will return readiness information about the newly added file descriptors.

### 7. TODO

- epoll_wait() 的 timeout 参数是使用 -1 还是 fine_timer()？
- Deep in nonblock and block
- Deep in kernel, how to connected epoll instance, interest list and ready list?
- What's the advantages of edge-triggered?
- EAGAIN and EWOULDBLOCK?

