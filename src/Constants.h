/*
 * MIT License
 *
 * Copyright (c) 2025 nfx
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file Constants.h
 * @brief Internal constants for Decimal and Int128 implementations
 * @details Defines precision limits, scale constraints, and internal masks
 */

#pragma once

#include <array>
#include <cstdint>

namespace nfx::datatypes::constants
{
	//=====================================================================
	// Common constants
	//=====================================================================

	//----------------------------------------------
	// Bit manipulation
	//----------------------------------------------

	/** @brief Bit shift amount for 32-bit operations (used for extracting/combining uint32_t from uint64_t) */
	inline constexpr int BITS_PER_UINT32{ 32 };

	/** @brief Bit shift amount for 64-bit operations (used for extracting/combining uint64_t from uint128_t) */
	inline constexpr int BITS_PER_UINT64{ 64 };

	/** @brief Maximum value for 32-bit unsigned integer (2^32 - 1). */
	inline constexpr std::uint64_t UINT32_MAX_VALUE{ 0xFFFFFFFFULL };

	/** @brief Bit mask value 1 as unsigned 64-bit (used for bit operations). */
	inline constexpr std::uint64_t BIT_MASK_ONE{ 1ULL };

	/** @brief Bit mask value 0 as unsigned 64-bit (used for bit operations). */
	inline constexpr std::uint64_t BIT_MASK_ZERO{ 0ULL };

	//----------------------------------------------
	// Maximum values
	//----------------------------------------------

	/** @brief Maximum 96-bit mantissa value (2^96 - 1) as low 64 bits. */
	inline constexpr std::uint64_t DECIMAL_96BIT_MAX_LOW{ 0xFFFFFFFFFFFFFFFFULL };

	/** @brief Maximum 96-bit mantissa value (2^96 - 1) as high 32 bits. */
	inline constexpr std::uint64_t DECIMAL_96BIT_MAX_HIGH{ 0x00000000FFFFFFFFULL };

	//----------------------------------------------
	// Precision limits
	//----------------------------------------------

	/** @brief Maximum decimal precision that IEEE 754 double can accurately represent. */
	inline constexpr std::uint8_t DOUBLE_DECIMAL_PRECISION{ 15U };

	//=====================================================================
	// Int128 data type constants
	//=====================================================================

	//----------------------------------------------
	// Numeric base
	//----------------------------------------------

	/** @brief Base for decimal digit conversion. */
	inline constexpr int INT_128_BASE{ 10 };

	//----------------------------------------------
	// Bit indexing
	//----------------------------------------------

	/** @brief Maximum bit index for 128-bit integers (0-127). */
	inline constexpr int INT_128_MAX_BIT_INDEX{ 127 };

	//----------------------------------------------
	// Overflow detection
	//----------------------------------------------

	/** @brief Maximum high 64-bit value before multiplying by 10 causes overflow (0xFFFFFFFFFFFFFFFF / 10). */
	inline constexpr std::uint64_t INT128_MUL10_OVERFLOW_THRESHOLD{ 0x1999999999999999ULL };

	/** @brief Double approximation of maximum positive Int128 value (2^127 - 1) for overflow checks. */
	inline constexpr double INT_128_MAX_AS_DOUBLE{ 1.7014118346046923e38 };

	/** @brief Double approximation of minimum negative Int128 value (-2^127) for overflow checks. */
	inline constexpr double INT_128_MIN_AS_DOUBLE{ -1.7014118346046924e38 };

	//----------------------------------------------
	// Arithmetic
	//----------------------------------------------

	/** @brief High 64 bits of maximum positive 128-bit signed integer (2^127 - 1) */
	inline constexpr std::uint64_t INT_128_MAX_POSITIVE_HIGH{ 0x7FFFFFFFFFFFFFFFULL };

	/** @brief Low 64 bits of maximum positive 128-bit signed integer (2^127 - 1) */
	inline constexpr std::uint64_t INT_128_MAX_POSITIVE_LOW{ 0xFFFFFFFFFFFFFFFFULL };

	/** @brief High 64 bits of minimum negative 128-bit signed integer (-2^127) */
	inline constexpr std::uint64_t INT_128_MIN_NEGATIVE_HIGH{ 0x8000000000000000ULL };

	/** @brief Low 64 bits of minimum negative 128-bit signed integer (-2^127) */
	inline constexpr std::uint64_t INT_128_MIN_NEGATIVE_LOW{ 0x0000000000000000ULL };

	//----------------------------------------------
	// String conversion
	//----------------------------------------------

	/** @brief Maximum number of decimal digits in a 128-bit signed integer (2^127 - 1 has 39 digits). */
	inline constexpr size_t INT_128_MAX_DIGIT_COUNT{ 39UL };

	/** @brief Maximum expected length for Int128 string representation. */
	inline constexpr size_t INT_128_MAX_STRING_LENGTH{ 64UL };

	/** @brief Maximum positive value as decimal string (2^127 - 1). */
	inline constexpr std::string_view INT_128_MAX_POSITIVE_STRING{ "170141183460469231731687303715884105727" };

	/** @brief Maximum negative value as decimal string (absolute value of -2^127). */
	inline constexpr std::string_view INT_128_MAX_NEGATIVE_STRING{ "170141183460469231731687303715884105728" };

	//=====================================================================
	// Decimal data type constants
	//=====================================================================

	//----------------------------------------------
	// Numeric base
	//----------------------------------------------

	/** @brief Base for decimal digit conversion. */
	inline constexpr std::uint64_t DECIMAL_BASE{ 10ULL };

	//----------------------------------------------
	// Arithmetic
	//----------------------------------------------

	/** @brief Lower 32 bits of maximum Decimal mantissa (2^96 - 1) */
	inline constexpr std::uint32_t DECIMAL_MAX_MANTISSA_0{ 0xFFFFFFFFU };

	/** @brief Middle 32 bits of maximum Decimal mantissa (2^96 - 1) */
	inline constexpr std::uint32_t DECIMAL_MAX_MANTISSA_1{ 0xFFFFFFFFU };

	/** @brief Upper 32 bits of maximum Decimal mantissa (2^96 - 1) */
	inline constexpr std::uint32_t DECIMAL_MAX_MANTISSA_2{ 0xFFFFFFFFU };

	/** @brief Lower 32 bits of minimum positive Decimal mantissa (1 × 10^-28) */
	inline constexpr std::uint32_t DECIMAL_MIN_MANTISSA_0{ 1U };

	/** @brief Middle 32 bits of minimum positive Decimal mantissa (1 × 10^-28) */
	inline constexpr std::uint32_t DECIMAL_MIN_MANTISSA_1{ 0U };

	/** @brief Upper 32 bits of minimum positive Decimal mantissa (1 × 10^-28) */
	inline constexpr std::uint32_t DECIMAL_MIN_MANTISSA_2{ 0U };

	/** @brief Maximum number of decimal places supported. */
	inline constexpr std::uint8_t DECIMAL_MAXIMUM_PLACES{ 28U };

	/** @brief Extra precision digits added during division to maintain accuracy. */
	inline constexpr std::uint8_t DECIMAL_DIVISION_EXTRA_PRECISION{ 18U };

	/** @brief Rounding threshold value (round up if digit >= this value). */
	inline constexpr std::uint8_t DECIMAL_ROUNDING_THRESHOLD{ 5U };

	/** @brief Bit mask for extracting scale from flags field (bits 16-23). */
	inline constexpr std::uint32_t DECIMAL_SCALE_MASK{ 0x00FF0000U };

	/** @brief Bit mask for sign detection (bit 31). */
	inline constexpr std::uint32_t DECIMAL_SIGN_MASK{ 0x80000000U };

	/** @brief Bit position for scale field in flags. */
	inline constexpr std::uint8_t DECIMAL_SCALE_SHIFT{ 16U };

	//----------------------------------------------
	// String conversion
	//----------------------------------------------

	/** @brief Maximum expected length for decimal string representation. */
	inline constexpr size_t DECIMAL_MAX_STRING_LENGTH{ 64UL };

	/** @brief Reserved buffer size for string conversion to avoid reallocation. */
	inline constexpr size_t DECIMAL_STRING_BUFFER_SIZE{ 64UL };

	//----------------------------------------------
	// Performance optimization
	//----------------------------------------------

	/** @brief Power of 10 lookup table size for fast scaling operations (64-bit range). */
	inline constexpr std::uint8_t DECIMAL_POWER_TABLE_SIZE{ 20U };

	/** @brief Minimum power for extended 128-bit power-of-10 table (10^20 and above). */
	inline constexpr std::uint8_t DECIMAL_EXTENDED_POWER_MIN{ 20U };

	/** @brief Maximum power for extended 128-bit power-of-10 table (10^28). */
	inline constexpr std::uint8_t DECIMAL_EXTENDED_POWER_MAX{ 28U };

	/** @brief Powers of 10 lookup table for efficient scaling operations (64-bit range: 10^0 to 10^19). */
	inline constexpr std::array<std::uint64_t, DECIMAL_POWER_TABLE_SIZE> DECIMAL_POWERS_OF_10{ {
		1ULL,					 // 10^0
		10ULL,					 // 10^1
		100ULL,					 // 10^2
		1000ULL,				 // 10^3
		10000ULL,				 // 10^4
		100000ULL,				 // 10^5
		1000000ULL,				 // 10^6
		10000000ULL,			 // 10^7
		100000000ULL,			 // 10^8
		1000000000ULL,			 // 10^9
		10000000000ULL,			 // 10^10
		100000000000ULL,		 // 10^11
		1000000000000ULL,		 // 10^12
		10000000000000ULL,		 // 10^13
		100000000000000ULL,		 // 10^14
		1000000000000000ULL,	 // 10^15
		10000000000000000ULL,	 // 10^16
		100000000000000000ULL,	 // 10^17
		1000000000000000000ULL,	 // 10^18
		10000000000000000000ULL, // 10^19 (max uint64_t support)
	} };

	/**
	 * @brief Power-of-10 calculation constants for 128-bit arithmetic
	 * @details Pre-computed constants for powers that exceed 64-bit range.
	 *          These are the low and high 64-bit components of 10^n where n > 19.
	 */
	inline constexpr std::array<std::pair<std::uint64_t, std::uint64_t>, 9> DECIMAL_EXTENDED_POWERS_OF_10{ {
		{ 0x6BC75E2D63100000ULL, 0x0000000000000005ULL }, // 10^20
		{ 0x35C9ADC5DEA00000ULL, 0x0000000000000036ULL }, // 10^21
		{ 0x19E0C9BAB2400000ULL, 0x000000000000021EULL }, // 10^22
		{ 0x02C7E14AF6800000ULL, 0x000000000000152DULL }, // 10^23
		{ 0x1BCECCEDA1000000ULL, 0x000000000000D3C2ULL }, // 10^24
		{ 0x161401484A000000ULL, 0x0000000000084595ULL }, // 10^25
		{ 0xDCC80CD2E4000000ULL, 0x000000000052B7D2ULL }, // 10^26
		{ 0x9FD0803CE8000000ULL, 0x00000000033B2E3CULL }, // 10^27
		{ 0x3E25026110000000ULL, 0x00000000204FCE5EULL }  // 10^28
	} };
} // namespace nfx::datatypes::constants
