#include "AMath.h"

bool AreEqual(float a, float b) {
	return abs(a - b) < std::numeric_limits<double>::epsilon();
}

bool IsGreaterThan(float a, float b) {
	return !AreEqual(a, b) && a > b;
}

bool IsLessThan(float a, float b) {
	return !AreEqual(a, b) && a < b;
}

bool IsBetweenInclusive(float num, float rangeStart, float rangeEnd) {
	return AreEqual(num, rangeStart) || AreEqual(num, rangeEnd) || (IsGreaterThan(num, rangeStart) && IsLessThan(num, rangeEnd));
}