/** Program to convert logic equations
 * of 16 inputs and 8 outputs
 * to a 64-kilobyte truth table.
 * @author Marko Mäkelä (Marko.Makela@nic.funet.fi)
 * @date 2nd July 2002
 *
 * Compilation:
 *	cc -o pla pla.c
 * Example usage:
 *	./pla | diff - pla-dump.bin
 * or
 *	./pla | od -Ax -t x1
 * or
 *	./pla | cksum
 * or
 *	./pla > pla-dump.bin
 *
 * The following Perl 5 code extracts output F7 from a truth table dump:
 * perl -e 'undef $/;$_=<>;s/(.)/chr(ord($1)&128)/ge;print' < pla-dump.bin
 *
 * To generate only selected outputs with this program,
 * you may as well comment out some lines in the main function.
 */

#include <stdio.h>

/** Extract an input bit
 * @param b	the bit to be extracted
 * @return	nonzero if the input bit b is set
 */
#define I(b) (!!((i) & (1 << b)))

/** @name The input signals of the plus/4 PLA.
 * Based on information at
 * http://www.funet.fi/pub/cbm/firmware/computers/plus4/pla.txt
 * composed by William Levak. */
/*@{*/
#define	I0_F7	I(0)
#define PHI0	I(1)
#define A15	I(2)
#define A4	I(3)
#define A5	I(4)
#define A6	I(5)
#define A7	I(6)
#define A12	I(7)
#define MUX	I(8)
#define A14	I(9)
#define A8	I(10)
#define A9	I(11)
#define A13	I(12)
#define A11	I(13)
#define A10	I(14)
#define RAS_	I(15)
/*@}*/

/** @name The output signals of the plus/4 PLA.
 * Reverse engineered from the scratch
 * by Marko Mäkelä on 2nd July 2002
 * from a dump provided by William Levak.
 * The equations are in disjunctive normal form (DNF).
 * (All others except I0_F7 are also in conjunctive normal form (CNF).)
 * Operators:	!x	negation of x: not x
 * 		x || y	disjunction: x or b
 * 		x && y	conjunction: x and b
 */
/*@{*/
/* unused_	0 when 0111 011x 1001 011x */
#define F0	RAS_ || !A10 || !A11 || !A13 || A9 || !A8 || !A14 ||	\
		!A12 || A7 || A6 || !A5 || A4 || !A15 || !PHI0
/* PHI2		1 when 0xxx xxxx xxxx xx11 */
#define F1	!RAS_ && PHI0 && I0_F7
/* USER_	0 when 0111 011x 1000 1111 */
#define F2	RAS_ || !A10 || !A11 || !A13 || A9 || !A8 || !A14 ||	 \
		!A12 || A7 || A6 || A5 || !A4 || !A15 || !PHI0 || !I0_F7
/* 6551_	0 when x111 011x 1000 011x */
#define F3	!A10 || !A11 || !A13 || A9 || !A8 || !A14 ||	\
		!A12 || A7 || A6 || A5 || A4 || !A15 || !PHI0
/* ADDR_CLK	0 when 1111 011x 1110 1111 */
#define F4	RAS_ || !A10 || !A11 || !A13 || A9 || !A8 || !A14 ||	\
		!A12 || !A7 || !A6 || A5 || !A4 || !A15 || !PHI0 || !I0_F7
/* KEYPORT_	0 when 0111 011x 1001 1111 */
#define F5	RAS_ || !A10 || !A11 || !A13 || A9 || !A8 || !A14 ||	\
		!A12 || A7 || A6 || !A5 || !A4 || !A15 || !PHI0 || !I0_F7
/* KERNAL_	1 when x111 001x 1xxx x1xx */
#define F6	A10 && A11 && A13 && !A9 && !A8 && A14 &&	\
		A12 && A15
/* I0_F7	1 when xxxx xxx1 xxxx xxxx or
		  when 0xxx xxxx xxxx xx11 */
#define F7	MUX || (F1)
/*@}*/

/** The main program
 * @param argc	command line argument count
 * @param argv	command line argument vector
 * @return	zero on successfull termination
 */
int
main (int argc, char** argv)
{
  /** The input combination, at least 16 bits */
  register unsigned int i = 0;
  do {
    /** The output combination, 8 bits */
    register unsigned char o = 0;
    if (F0) o |= 1 << 0;
    if (F1) o |= 1 << 1;
    if (F2) o |= 1 << 2;
    if (F3) o |= 1 << 3;
    if (F4) o |= 1 << 4;
    if (F5) o |= 1 << 5;
    if (F6) o |= 1 << 6;
    if (F7) o |= 1 << 7;
    putchar (o);
  }
  while (++i & 0xffff);
  return 0;
}
