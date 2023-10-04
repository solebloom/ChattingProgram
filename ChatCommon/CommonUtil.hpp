#pragma once
#include <atomic>
#include <random>
#if defined _HAS_CXX17 || defined _WIN32 || defined __APPLE__
#include <thread>
#else
#include <sched.h>	// Supports sched_yield()
#include <time.h>	// Supports nanosleep(struct timespec)
#endif

class CommonUtil {
public:
	constexpr inline static int GetCacheLineSize() {
		return sizeof(std::atomic_uint64_t) * 2;
	}

	const static void yieldThread() {
#if defined _HAS_CXX17 || defined _WIN32 || defined __APPLE__
		std::this_thread::yield();
#else
		sched_yield();
#endif
	}

#if defined _HAS_CXX17 || defined _WIN32 || defined __APPLE__
	const static void sleepThread(std::chrono::milliseconds ms) {
		std::this_thread::sleep_for(ms);
#else
	const static void sleepThread(struct timespec ts) {
		nanosleep(&ts, nullptr);
#endif
	}

	template <typename T>
	constexpr static T getRandomNumber(T start, T end) {
		static std::mt19937 randomNumberGenerator{ std::random_device{}() };
		std::uniform_int_distribution<T> distribution{ start, end };
		return distribution(randomNumberGenerator);
	}

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