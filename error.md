Function | UseErr | ErrType | FlushStdout | Terminate
---|:---:|:---:|:---:|:---:
err_msg     | • | errno | • |
err_exit    | • | errno | • | • 
_err_exit   | • | errno |   | • 
err_exit_en | • | errnum| • | • 
fatal       |   |       | • | • 

### TODO
1. The difference between exit() and _exit() - Chapter 25
2. Where to use err_exit_en() and _err_exit()?
3. Dump core?
4. vsnprintf / snprintf / vfprintf / fprintf / ...
5. Generate emane.c.inc
6. fflush? / line-buffered?
7. Why save errno in err_msg()?