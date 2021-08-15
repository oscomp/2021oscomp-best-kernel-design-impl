#include <os/sched.h>
#include <os/time.h>

#define FD_ISSET(num, fds) \
    ( ( ( *( (uint32_t *)(fds) + ((num) / 32) ) ) >> (num) ) & (1lu))

enum{
    SELECT_READ,
#define SELECT_READ SELECT_READ
    SELECT_WRITE,
#define SELECT_WRITE SELECT_WRITE
    SELECT_EXCEPTION,
#define SELECT_EXCEPTION SELECT_EXCEPTION
};

static inline void FD_CLR(fd_num_t fd, fd_set *fds)
{
    fds = ( fds + fd / 32 );
    *fds &= ~(1lu << (fd % 32) ) ;
}

static inline void FD_SET(fd_num_t fd, fd_set *fds)
{
    fds = ( fds + fd / 32 );
    *fds |= (1lu << (fd % 32) ) ;
}
