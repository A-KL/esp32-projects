#ifndef DRIVER_H
#define DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*update_callback) (void *data);

void hal_setup(void);
void hal_loop(void);

void delay(unsigned long);
void updates(update_callback fn);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*DRIVER_H*/
