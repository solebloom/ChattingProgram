#pragma once
#include "CommonUtil.hpp"

template <typename T, size_t N>
constexpr static size_t CommonUtil::GetArrayLength(const T(&array)[N]) {
	return N;
}

template <typename T>
constexpr static T CommonUtil::CantorPairing(const T& a, const T& b) {
	// Cantor pairing function makes a unique value that represents the pair (a, b) in a one-to-one mapping.
	return CantorPairingWithoutNormalization(makeNormalizedValue(a), makeNormalizedValue(b));
}

//template <typename T>
//constexpr static T CommonUtil::CantorPairing(const T& a, const T& b, const T& c) {
//	// Cantor pairing function makes a unique value that represents the pair (a, b, c) in a one-to-one mapping.
//	return CantorPairingWithoutNormalization(CantorPairingWithoutNormalization(makeNormalizedValue(a), makeNormalizedValue(b)), makeNormalizedValue(c));
//}

template <typename T, typename... Args>
constexpr static T CommonUtil::CantorPairing(const T& a, const T& b, Args... args) {
	// Cantor pairing function makes a unique value that represents the pair (a, b, ...) in a one-to-one mapping.
	T ret = CantorPairingWithoutNormalization(makeNormalizedValue(a), makeNormalizedValue(b));
	for (auto argument : { args... }) {
		ret = CantorPairingWithoutNormalization(ret, makeNormalizedValue(argument));
	}

	return ret;
}

template <typename T>
constexpr static T CommonUtil::makeNormalizedValue(const T& value) {
	// Normalize values to support negative integers
	return value >= T(0) ? T(2) * value : T(-2) * value - T(1);
}

template <typename T>
constexpr static T CommonUtil::CantorPairingWithoutNormalization(const T& a, const T& b) {
	// Cantor pairing function makes a unique value that represents the pair (a, b) in a one-to-one mapping. (a > 0 and b > 0, because a, b aren't normalized)
	T sum_value = a + b;
	return sum_value * (sum_value + 1) / 2 + b;
}
