#pragma once

#include <limits>

// Determines if two floats are within epsilon of each other
bool AreEqual(float a, float b);

// Determines if two floats are _not_ within epsilon of each other, and `a` is greater than `b`
bool IsGreaterThan(float a, float b);

// Determines if two floats are _not_ within epsilon of each other, and `a` is less than `b`
bool IsLessThan(float a, float b);

// Determines whether `num` is either within epsilon of rangeStart or rangeEnd, or is greater than rangeStart and less than rangeEnd
bool IsBetweenInclusive(float num, float rangeStart, float rangeEnd);