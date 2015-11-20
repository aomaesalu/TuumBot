/*
 * macros.h
 *
 */ 


#ifndef MACROS_H_
#define MACROS_H_

#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))
#define bit_flip(p,m) ((p) ^= (m))
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m))
#define BIT(x) (0x01 << (x))
#define BITS(b, x) ((b) << (x))



#endif /* MACROS_H_ */
