#ifndef	_UTILS_H
#define	_UTILS_H

extern void delay ( unsigned long);
extern void put32 ( unsigned long, unsigned int );
extern unsigned int get32 ( unsigned long );
extern unsigned long get_el ( void );
extern void set_pgd(unsigned long pgd);
extern unsigned long get_pgd();
extern unsigned long get_pgd();
extern void* bl_init( void *(*entry_func) (void *), void *arg, ...);

#endif  /*_UTILS_H */
