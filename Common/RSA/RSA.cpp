/*
 * RSA.cpp
 *
 *  Created on: 7 Νοε 2012
 *      Author: Σταμάτης
 */




#include "RSA.h"


RSA::RSA(bool useChineseTheorem): chinese(useChineseTheorem)
{
	n = NULL;
	e = NULL;
	d = NULL;
	dP = NULL;
	dQ = NULL;
	qInv = NULL;
}

RSA::~RSA()
{
	if(n != NULL)
	{
		delete n;
	}

	if(e != NULL)
	{
		delete e;
	}

	if(d != NULL)
	{
		delete d;
	}

	if(dP != NULL)
	{
		delete dP;
	}

	if(dQ != NULL)
	{
		delete dQ;
	}

	if(qInv != NULL)
	{
		delete qInv;
	}
}

void RSA::setD(BigInteger d)
{
	RSA::d = new BigInteger(d);
}

void RSA::setE(BigInteger e)
{
	RSA::e = new BigInteger(e);
}

void RSA::setN(BigInteger n)
{
	RSA::n = new BigInteger(n);
}

void RSA::setDP(BigInteger dP)
{
	RSA::dP = new BigInteger(dP);
}

void RSA::setDQ(BigInteger dQ)
{
	RSA::dQ = new BigInteger(dQ);
}

void RSA::setQInv(BigInteger qInv)
{
	RSA::qInv = new BigInteger(qInv);
}

void RSA::setEncryption(bool encryption)
{
	chinese = encryption;
}

MAUtil::String RSA::decCRT(MAUtil::String encrypted_text)
{
	return MAUtil::String("Not implemented.");
}

MAUtil::String RSA::decrypt(MAUtil::String encrypted_text)
{
	MAUtil::String result, ret_value("");
	int letters;

	if(chinese)
	{
		result = decCRT(encrypted_text);
	}
	else
	{
		result = decNormal(encrypted_text);
	}

	BigInteger temp = stringToBigInteger(result);

	letters = howManyLetters(result);

	for(int i = letters-1; i <= 0; i++)
	{
		BigInteger temp_inner(temp);

		temp /= RSA::pow(256, i);

		char c = (char)temp.toInt();
		ret_value.append(&c, 1);

		temp = temp_inner-temp;
	}

	return ret_value;
}

MAUtil::String RSA::decNormal(MAUtil::String encrypted_text)
{
	if(!d || !n)
	{
		return MAUtil::String("Error");
	}

	BigInteger c = stringToBigInteger(encrypted_text);

	//c = pow(c, d) % n;
	for(int i = 0; (BigInteger)i < *d; i++)
	{
		c *= c;
	}

	c = c % *n;

	return bigIntegerToString(c);
}

int RSA::howManyLetters(MAUtil::String text)
{
	int i = 0;
	BigInteger temp;

	temp = stringToBigInteger(text);

	while(!temp.isZero())
	{
		temp /= 256;
		i++;
	}

	return i;
}

BigInteger RSA::pow(BigInteger i, BigInteger j)
{
	for(BigInteger k = 0; k < j; j++)
	{
		i *= i;
	}

	return i;
}


MAUtil::String RSA::encrypt(MAUtil::String plaintext)
{
	if(!n || !e)
	{
		return MAUtil::String("Error");
	}

	BigInteger m(0);
	int ptsize = plaintext.length();

	for(int i = ptsize-1; i >= 0; i--)
	{
		lprintfln("i: %d", i);
		m += (BigInteger)( (int)(plaintext[i]) )*RSA::pow(256, ptsize-1-i);
	}

	//m = pow(m, e) % n;
	/*for(BigInteger i = 0; i < *e; i++)
	{
		m *= m;
	}

	m = m % *n;*/

	return bigIntegerToString(m);
}
