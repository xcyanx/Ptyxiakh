/*
 * RSA.h
 *
 *  Created on: 7 Νοε 2012
 *      Author: Σταμάτης
 */

#ifndef RSA_H_
#define RSA_H_

#include <MAUtil/String.h>
#include "../BigInt/BigIntegerLibrary.h"
#include <madmath.h>
#include <mavsprintf.h>


class RSA
{
	public:
		enum {Normal = 0, Chinese };

	public:
		RSA(bool useChineseTheorem);
		~RSA();
		MAUtil::String encrypt(MAUtil::String plaintext);
		MAUtil::String decrypt(MAUtil::String encrypted_text);
		void setN(BigInteger n);
		void setE(BigInteger e);
		void setD(BigInteger d);
		void setDP(BigInteger dP);
		void setDQ(BigInteger dQ);
		void setQInv(BigInteger qInv);
		void setEncryption(bool encryption);

	private:
		MAUtil::String decCRT(MAUtil::String encrypted_text);
		MAUtil::String decNormal(MAUtil::String encrypted_text);
		int howManyLetters(MAUtil::String text);
		BigInteger pow(BigInteger i, BigInteger j);

	private:
		BigInteger *n;
		BigInteger *e;
		BigInteger *d;
		BigInteger *dP;
		BigInteger *dQ;
		BigInteger *qInv;
		bool chinese;
};


#endif /* RSA_H_ */
