#include "BigIntegerUtils.h"
#include "BigUnsignedInABase.h"

MAUtil::String bigUnsignedToString(const BigUnsigned &x) {
	return MAUtil::String(BigUnsignedInABase(x, 10));
}

MAUtil::String bigIntegerToString(const BigInteger &x) {
	return (x.getSign() == BigInteger::negative)
		? (MAUtil::String("-") + bigUnsignedToString(x.getMagnitude()))
		: (bigUnsignedToString(x.getMagnitude()));
}

BigUnsigned stringToBigUnsigned(const MAUtil::String &s) {
	return BigUnsigned(BigUnsignedInABase(s, 10));
}

BigInteger stringToBigInteger(const MAUtil::String &s) {
	// Recognize a sign followed by a BigUnsigned.
	return (s[0] == '-') ? BigInteger(stringToBigUnsigned(s.substr(1, s.length() - 1)), BigInteger::negative)
		: (s[0] == '+') ? BigInteger(stringToBigUnsigned(s.substr(1, s.length() - 1)))
		: BigInteger(stringToBigUnsigned(s));
}
