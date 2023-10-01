#pragma once
//#include <vcruntime.h>

class CommonUtil {
public:
	template <typename T, size_t N>
	constexpr static size_t GetArrayLength(const T(&array)[N]);

	template <typename T>
	constexpr static T CantorPairing(const T& a, const T& b);

	//template <typename T>
	//constexpr static T CantorPairing(const T& a, const T& b, const T& c);

	template <typename T, typename... Args>
	constexpr static T CantorPairing(const T& a, const T& b, Args... args);

private:
	template <typename T>
	constexpr static T makeNormalizedValue(const T& value);

	template <typename T>
	constexpr static T CantorPairingWithoutNormalization(const T& a, const T& b);
};