#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "error.h"

vee_conf_t *vee_read_conf(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
        err_exit("[vee_read_conf] open file error.");

    vee_conf_t *conf = (vee_conf_t *)malloc(sizeof(vee_conf_t));
    if (!conf)
        err_exit("[vee_read_conf] malloc vee_conf_t error.");

    char buf[BUF_LEN];
    char *cur_pos, *delim_pos;
    int line_no = 0;

    while (fgets(buf, BUF_LEN, fp)) {
        delim_pos = strstr(buf, DELIM);
        if (!delim_pos++)
            err_exit("[vee_read_conf] format error of line %d.", line_no);

        if (strncmp("root", buf, 4) == 0) {
            int i = 0;
            while (*delim_pos != '\n') {
                conf->root[i++] = *delim_pos++;
            }
        }

        if (strncmp("port", buf, 4) == 0)
            conf->port = atoi(delim_pos);

        if (strncmp("thread_num", buf, 10) == 0)
            conf->thread_num = atoi(delim_pos);

        line_no++;
    }

    fclose(fp);

    return conf;
}
