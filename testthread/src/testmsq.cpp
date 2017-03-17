/*
 * testmsq.cpp
 *
 *  Created on: Mar 16, 2017
 *      Author: josco
 */

#include <fcntl.h>
#include <unistd.h>
#include <mqueue.h>
#include <cstdio>
#include <cstdlib>

char buf[160*120];

int main(void)
{
    mq_unlink("/depthThread");

    struct mq_attr attr;
    attr.mq_flags = 1;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 2000;
    mqd_t mfd = mq_open("/depthThread", O_RDONLY|O_CREAT, 0777, &attr);

    if (-1 == mfd) {
        perror("mq_open error");
        exit(-1);
    }
    if (mq_receive(mfd, buf, 1000,NULL) == -1) {
        perror("mq_receive error");
        exit(-2);
    }
}
