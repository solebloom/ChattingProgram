#pragma once
#include <iostream>
#include <iomanip>
#include <regex>
#include <boost/asio.hpp>
#include "ChatClient.cpp"
#include "../ChatCommon/RoomInfoBuilder.cpp"

using namespace boost::asio;

void RoomInfoParsingTest();

template <size_t data_size, size_t first_delimiter_size, size_t second_delimiter_size>
void StrtokParsing(char(&data)[data_size], const int MAX_COUNT,
					const char(&first_delimiter)[first_delimiter_size], const char(&second_delimiter)[second_delimiter_size]);
template <size_t size>
void RegexParsing(const char(&data)[size], const int MAX_COUNT, std::regex first_regex, std::regex second_regex);

int main(int argc, char* argv[]) {
	//RoomInfoParsingTest();
	try {
		io_context io_context;
		// TODO: Additional handling with taking arguments or GUI form is required.
		ChatClient client{ io_context, std::move(std::string("localhost")), std::move(std::string("8080")) };
		io_context.run();
	} catch (std::exception& e) {
		std::cerr << "¿¡·¯ : " << e.what() << std::endl;
		return -1;
	}

	return 0;
}

void RoomInfoParsingTest() {
	char data[]{ "123:456,43:12345,222:4356,12342:34343" };
	char first_delimiter[]{ '[', '^', Message::DATA_SEPARATOR, ']', '+', '\0' };
	char second_delimiter[]{ '[', '^', Message::KEY_VALUE_SEPARATOR, ']', '+', '\0' };
	//char firstDelimiter[]{ "[^,]+" };
	//char secondDelimiter[]{ "[^:]+" };

	std::regex first_regex{ first_delimiter };
	std::regex second_regex{ second_delimiter };

	char strtok_data[sizeof(data)];
#ifdef _WIN32
#define _STRNCPY strncpy_s
#else
#define _STRNCPY strncpy
#endif
	_STRNCPY(strtok_data, data, sizeof(data));

	//constexpr int MAX_COUNT{ 1000000 };
	constexpr int MAX_COUNT{ 10 };

#define _PARSING_VIEW_ENABLE false

	StrtokParsing(strtok_data, MAX_COUNT, first_delimiter, second_delimiter);
	RegexParsing(data, MAX_COUNT, first_regex, second_regex);
	StrtokParsing(strtok_data, MAX_COUNT, first_delimiter, second_delimiter);
	RegexParsing(data, MAX_COUNT, first_regex, second_regex);
	StrtokParsing(strtok_data, MAX_COUNT, first_delimiter, second_delimiter);
	RegexParsing(data, MAX_COUNT, first_regex, second_regex);
}

template <size_t data_size, size_t first_delimiter_size, size_t second_delimiter_size>
void StrtokParsing(char(&data)[data_size], const int MAX_COUNT,
					const char(&firstDelimiter)[first_delimiter_size], const char(&secondDelimiter)[second_delimiter_size]) {
	// Measure strtok performance
	char* firstRemainder{ nullptr };
	char* secondRemainder{ nullptr };

#ifdef _WIN32
#define _STRTOK strtok_s
#else
#define _STRTOK strtok_r
#endif

	char tempData[data_size]{ 0 };
	_STRNCPY(tempData, data, sizeof(data));

	double totalTime{ 0.0L };
	for (int i = 0; i < MAX_COUNT; ++i) {
		auto startTime{ std::chrono::high_resolution_clock::now() };
		char* firstQuotient{ _STRTOK(data, firstDelimiter, &firstRemainder) };
		while (firstQuotient != nullptr) {
			//std::cout << firstQuotient << std::endl;
			RoomInfoBuilder builder{};
			char* secondQuotient{ _STRTOK(firstQuotient, secondDelimiter, &secondRemainder) };
			while (secondQuotient != nullptr) {
#if _PARSING_VIEW_ENABLE
				std::cout << secondQuotient << Message::KEY_VALUE_SEPARATOR;
#endif
				builder.SetVariable(*reinterpret_cast<int*>(secondQuotient));
				secondQuotient = _STRTOK(nullptr, secondDelimiter, &secondRemainder);
			}
			std::unique_ptr<RoomInfo> roomInfo{ builder.Build() };
#if _PARSING_VIEW_ENABLE
			std::cout << "\b \n";
#endif
			firstQuotient =  _STRTOK(nullptr, firstDelimiter, &firstRemainder);
		}
		auto endTime{ std::chrono::high_resolution_clock::now() };
		totalTime += static_cast<std::chrono::duration<double>>(endTime - startTime).count();

		_STRNCPY(data, tempData, data_size);
	}
	std::cout << std::fixed << std::setprecision(15) << "strtok_s parsing took " << totalTime / (double)MAX_COUNT << " seconds." << std::endl;
}

template <size_t size>
void RegexParsing(const char(&data)[size], const int MAX_COUNT, std::regex firstRegex, std::regex secondRegex) {
	// Measure regex performance
	double totalTime{ 0.0L };
	for (int i = 0; i < MAX_COUNT; ++i) {
		auto startTime{ std::chrono::high_resolution_clock::now() };
		std::regex_iterator<const char*> firstQuotient{ std::begin(data), std::end(data), firstRegex}, emptyFirstQuotient{};
		while (firstQuotient != emptyFirstQuotient) {
			std::string firstQuotientStr{ firstQuotient->str() };
			//std::cout << firstQuotientStr << std::endl;
			RoomInfoBuilder builder{};
			std::regex_iterator<std::string::iterator> secondQuotient{ firstQuotientStr.begin(), firstQuotientStr.end(), secondRegex }, emptySecondQuotient{};
			while (secondQuotient != emptySecondQuotient) {
				auto quotient{ secondQuotient->str() };
#if _PARSING_VIEW_ENABLE
				std::cout << quotient << Message::KEY_VALUE_SEPARATOR;
#endif
				builder.SetVariable(*reinterpret_cast<const int*>(quotient.c_str()));
				++secondQuotient;
			}
			std::unique_ptr<RoomInfo> roomInfo{ builder.Build() };
#if _PARSING_VIEW_ENABLE
			std::cout << "\b \n";
#endif
			++firstQuotient;
		}
		auto endTime{ std::chrono::high_resolution_clock::now() };
		totalTime += static_cast<std::chrono::duration<double>>(endTime - startTime).count();
	}
	std::cout << std::fixed << std::setprecision(15) << "Regex parsing took " << totalTime / (double)MAX_COUNT << " seconds." << std::endl;
}

#ifdef _STRTOK
#undef _STRTOK
#endif

#ifdef _STRNCPY
#undef _STRNCPY
#endif

#ifdef _PARSING_VIEW_ENABLE
#undef _PARSING_VIEW_ENABLE
#endif