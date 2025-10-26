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
 * @file Int128.cpp
 * @brief Implementation of cross-platform 128-bit integer arithmetic
 * @details Provides exact 128-bit integer arithmetic with portable operations
 */

#include <istream>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <limits>

#include "nfx/datatypes/Int128.h"

#include "nfx/datatypes/Decimal.h"
#include "Constants.h"

namespace nfx::datatypes
{
	//=====================================================================
	// Int128 class
	//=====================================================================

	//----------------------------------------------
	// Construction
	//----------------------------------------------

#if NFX_DATATYPES_HAS_NATIVE_INT128

	Int128::Int128( std::uint64_t low, std::uint64_t high ) noexcept
		: m_value{ static_cast<NFX_DATATYPES_NATIVE_INT128>( high ) << constants::BITS_PER_UINT64 | low }
	{
	}

#else

	Int128::Int128( std::uint64_t low, std::uint64_t high ) noexcept
		: m_layout{ low, high }
	{
	}

#endif

	Int128::Int128( float val )
	{
		// Convert float to Int128, truncating fractional part (like static_cast<int>(float))
		if ( std::isnan( val ) || std::isinf( val ) )
		{
			// NaN or infinity - set to zero (matches some C++ implementations)
			*this = Int128{ 0 };
			return;
		}

		// Truncate toward zero (standard C++ behavior)
		double truncated = std::trunc( static_cast<double>( val ) );

		// Check for overflow
		if ( truncated > static_cast<double>( std::numeric_limits<std::int64_t>::max() ) ||
			 truncated < static_cast<double>( std::numeric_limits<std::int64_t>::min() ) )
		{
			// For values outside int64 range, we need more complex handling
			// For now, clamp to int64 range (could also throw overflow_error)
			if ( truncated > 0 )
			{
				*this = Int128{ std::numeric_limits<std::int64_t>::max() };
			}
			else
			{
				*this = Int128{ std::numeric_limits<std::int64_t>::min() };
			}
			return;
		}

		// Safe to convert via int64
		*this = Int128{ static_cast<std::int64_t>( truncated ) };
	}

	Int128::Int128( double val )
	{
		// Convert double to Int128, truncating fractional part (like static_cast<int>(double))
		if ( std::isnan( val ) || std::isinf( val ) )
		{
			// NaN or infinity - set to zero (matches some C++ implementations)
			*this = Int128{ 0 };
			return;
		}

		// Truncate toward zero (standard C++ behavior)
		double truncated = std::trunc( val );

		// Check for overflow - double can represent much larger values than Int128
		// Int128 max is approximately 1.7e38, double can go up to ~1.8e308
		if ( truncated > constants::INT_128_MAX_AS_DOUBLE || truncated < constants::INT_128_MIN_AS_DOUBLE )
		{
			// Overflow - clamp to Int128 range
			if ( truncated > 0 )
			{
				// Set to maximum positive Int128
				*this = Int128{ constants::INT_128_MAX_POSITIVE_LOW, constants::INT_128_MAX_POSITIVE_HIGH };
			}
			else
			{
				// Set to minimum negative Int128
				*this = Int128{ constants::INT_128_MIN_NEGATIVE_LOW, constants::INT_128_MIN_NEGATIVE_HIGH };
			}
			return;
		}

		// For values within reasonable range, convert via string parsing for accuracy
		// This avoids floating-point precision issues
		std::ostringstream oss;
		oss << std::fixed << std::setprecision( 0 ) << truncated;
		*this = Int128::parse( oss.str() );
	}

	Int128::Int128( const Decimal& decimal )
	{
		// Note: Following C++ standard behavior - truncate fractional parts
		// (similar to static_cast<int>(double) which truncates toward zero)

		// Handle zero case
		if ( decimal.isZero() )
		{
			*this = Int128{ 0 };
			return;
		}

		// Extract integer part from Decimal (truncate fractional part)
		// If Decimal has scale, we need to divide by 10^scale to get integer part
		Int128 integerPart;

		// Get the mantissa first
		const auto& mantissaArray = decimal.mantissa();
		std::uint64_t low = static_cast<std::uint64_t>( mantissaArray[1] ) << constants::BITS_PER_UINT32 | mantissaArray[0];
		std::uint64_t high = mantissaArray[2];

		// Build the full mantissa value using constructor that works on both platforms
		integerPart = Int128{ low, high };

		// If there's a scale, divide by 10^scale to get integer part (truncating)
		if ( decimal.scale() > 0 )
		{
			Int128 divisor{ 1 };
			for ( std::uint8_t i = 0; i < decimal.scale(); ++i )
			{
				divisor = divisor * Int128{ constants::INT_128_BASE };
			}
			integerPart = integerPart / divisor; // Integer division truncates
		}

		bool isNegative = decimal.isNegative();

		// Apply sign to the integer part
		if ( isNegative )
		{
			integerPart = -integerPart;
		}

		// Copy the result to this object
		*this = integerPart;
	}

	//----------------------------------------------
	// Comparison with nfx Decimal
	//----------------------------------------------

	bool Int128::operator==( const Decimal& val ) const noexcept
	{
		// If Decimal has fractional part, it can't equal an integer
		if ( val.scale() > 0 )
		{
			return false;
		}

		// Handle signs
		if ( isNegative() != val.isNegative() )
		{
			// Different signs can only be equal if both are zero
			return isZero() && val.isZero();
		}

		// Convert Decimal's mantissa to Int128 for comparison
		const auto& mantissaArray = val.mantissa();
		std::uint64_t low = static_cast<std::uint64_t>( mantissaArray[1] ) << constants::BITS_PER_UINT32 | mantissaArray[0];
		std::uint64_t high = mantissaArray[2];
		Int128 decimalMantissa{ low, high };

		// Compare absolute values
		Int128 thisAbs = abs();
		return thisAbs == decimalMantissa;
	}

	bool Int128::operator<( const Decimal& val ) const noexcept
	{
		// Handle different signs
		if ( isNegative() && !val.isNegative() )
		{
			return true; // Negative < Non-negative
		}
		if ( !isNegative() && val.isNegative() )
		{
			return false; // Non-negative > Negative
		}

		// Same signs - need to handle Decimal's scale
		const auto& mantissaArray = val.mantissa();
		std::uint64_t low = static_cast<std::uint64_t>( mantissaArray[1] ) << constants::BITS_PER_UINT32 | mantissaArray[0];
		std::uint64_t high = mantissaArray[2];
		Int128 decimalMantissa{ low, high };

		if ( val.scale() > 0 )
		{
			// Decimal has fractional part - scale up this integer for comparison
			Int128 scaledThis = *this;

			// Apply power of 10 scaling
			for ( std::uint8_t i = 0; i < val.scale(); ++i )
			{
				scaledThis = scaledThis * Int128{ constants::INT_128_BASE };
			}

			if ( isNegative() )
			{
				// Both negative - compare absolute values with flipped result
				return scaledThis.abs() > decimalMantissa;
			}
			else
			{
				return scaledThis < decimalMantissa;
			}
		}
		else
		{
			// No fractional part - direct comparison of absolute values
			if ( isNegative() )
			{
				// Both negative - flip comparison
				return abs() > decimalMantissa;
			}
			else
			{
				return abs() < decimalMantissa;
			}
		}
	}

	//----------------------------------------------
	// Arithmetic operations
	//----------------------------------------------

#if NFX_DATATYPES_HAS_NATIVE_INT128

	Int128 Int128::operator+( const Int128& other ) const noexcept
	{
		return Int128{ m_value + other.m_value };
	}

	Int128 Int128::operator-( const Int128& other ) const noexcept
	{
		return Int128{ m_value - other.m_value };
	}

	Int128 Int128::operator*( const Int128& other ) const noexcept
	{
		return Int128{ m_value * other.m_value };
	}

	Int128 Int128::operator/( const Int128& other ) const
	{
		if ( other.m_value == 0 )
		{
			throw std::overflow_error{ "Division by zero" };
		}

		return Int128{ m_value / other.m_value };
	}

#else
	Int128 Int128::operator+( const Int128& other ) const noexcept
	{
		// 128-bit addition with carry propagation
		std::uint64_t result_low{ m_layout.lower64bits + other.m_layout.lower64bits };
		std::uint64_t carry{ ( result_low < m_layout.lower64bits ) ? constants::BIT_MASK_ONE : constants::BIT_MASK_ZERO };
		std::uint64_t result_high{ m_layout.upper64bits + other.m_layout.upper64bits + carry };
		return Int128{ result_low, result_high };
	}

	Int128 Int128::operator-( const Int128& other ) const noexcept
	{
		// 128-bit subtraction with borrow propagation
		std::uint64_t result_low{ m_layout.lower64bits - other.m_layout.lower64bits };
		std::uint64_t borrow{ ( m_layout.lower64bits < other.m_layout.lower64bits ) ? constants::BIT_MASK_ONE : constants::BIT_MASK_ZERO };
		std::uint64_t result_high{ m_layout.upper64bits - other.m_layout.upper64bits - borrow };
		return Int128{ result_low, result_high };
	}

	Int128 Int128::operator*( const Int128& other ) const noexcept
	{
		// 128-bit multiplication using Karatsuba-style algorithm (https://en.wikipedia.org/wiki/Karatsuba_algorithm)
		// Performance: Breaks 64x64 multiplication into 32x32 operations
		// to leverage hardware multipliers efficiently on all platforms
		std::uint64_t a_low{ m_layout.lower64bits & constants::UINT32_MAX_VALUE };
		std::uint64_t a_high{ m_layout.lower64bits >> constants::BITS_PER_UINT32 };
		std::uint64_t b_low{ other.m_layout.lower64bits & constants::UINT32_MAX_VALUE };
		std::uint64_t b_high{ other.m_layout.lower64bits >> constants::BITS_PER_UINT32 }; // Four 32x32->64 multiplications
		std::uint64_t p0{ a_low * b_low };
		std::uint64_t p1{ a_low * b_high };
		std::uint64_t p2{ a_high * b_low };
		std::uint64_t p3{ a_high * b_high };

		// Carry computation for intermediate sum
		std::uint64_t carry{
			( ( p0 >> constants::BITS_PER_UINT32 ) +
				( p1 & constants::UINT32_MAX_VALUE ) +
				( p2 & constants::UINT32_MAX_VALUE ) ) >>
			constants::BITS_PER_UINT32 }; // Final result assembly

		std::uint64_t result_low{
			p0 +
			( p1 << constants::BITS_PER_UINT32 ) +
			( p2 << constants::BITS_PER_UINT32 ) };

		std::uint64_t result_high{
			p3 +
			( p1 >> constants::BITS_PER_UINT32 ) +
			( p2 >> constants::BITS_PER_UINT32 ) +
			carry +
			m_layout.upper64bits * other.m_layout.lower64bits +
			m_layout.lower64bits * other.m_layout.upper64bits };

		return Int128{ result_low, result_high };
	}

	Int128 Int128::operator/( const Int128& other ) const
	{
		if ( other.isZero() )
		{
			throw std::overflow_error{ "Division by zero" };
		}

		// Performance optimization: Fast path for 64-bit values
		// Avoids expensive 128-bit division when possible
		if ( m_layout.upper64bits == 0 && other.m_layout.upper64bits == 0 )
		{
			// Both fit in 64-bit - use native division
			return Int128{ m_layout.lower64bits / other.m_layout.lower64bits, 0 };
		}

		// Optimized path: dividend is 128-bit but divisor fits in 64-bit
		// Use precise 128/64 division algorithm
		if ( other.m_layout.upper64bits == 0 )
		{
			std::uint64_t divisor{ other.m_layout.lower64bits };

			// Divide high part first
			std::uint64_t high_quotient{ m_layout.upper64bits / divisor };
			std::uint64_t high_remainder{ m_layout.upper64bits % divisor };

			// Now divide (high_remainder << 64 + m_layout.lower64bits) by divisor
			// This is equivalent to dividing a 128-bit number by a 64-bit number
			if ( high_remainder == 0 )
			{
				// Simple case: high part divides evenly
				std::uint64_t low_quotient{ m_layout.lower64bits / divisor };
				return Int128{ low_quotient, high_quotient };
			}
			else
			{
				// Complex case: use long division for the remainder
				// We need to compute (high_remainder * 2^64 + m_layout.lower64bits) / divisor

				// Split the lower 64 bits into two 32-bit parts for easier handling
				std::uint64_t low_high{ m_layout.lower64bits >> constants::BITS_PER_UINT32 };
				std::uint64_t low_low{ m_layout.lower64bits & constants::UINT32_MAX_VALUE }; // Divide (high_remainder << 32 + low_high) by divisor
				std::uint64_t temp_dividend{ ( high_remainder << constants::BITS_PER_UINT32 ) + low_high };
				std::uint64_t temp_quotient{ temp_dividend / divisor };
				std::uint64_t temp_remainder{ temp_dividend % divisor };

				// Divide (temp_remainder << 32 + low_low) by divisor
				std::uint64_t final_dividend{ ( temp_remainder << constants::BITS_PER_UINT32 ) + low_low };
				std::uint64_t final_quotient{ final_dividend / divisor };

				// Combine the quotients */
				std::uint64_t low_quotient{ ( temp_quotient << constants::BITS_PER_UINT32 ) + final_quotient };
				return Int128{ low_quotient, high_quotient };
			}
		}

		// General case: 128-bit / 128-bit division using binary long division
		// This handles all cases where both operands require the full 128-bit range

		// Handle sign for signed division
		bool result_negative{ false };
		Int128 abs_dividend{ *this };
		Int128 abs_divisor{ other };

		if ( abs_dividend.isNegative() )
		{
			result_negative = !result_negative;
			abs_dividend = -abs_dividend;
		}

		if ( abs_divisor.isNegative() )
		{
			result_negative = !result_negative;
			abs_divisor = -abs_divisor;
		}

		// Early exit for simple cases
		if ( abs_dividend < abs_divisor )
		{
			return Int128{ 0, 0 };
		}

		if ( abs_dividend == abs_divisor )
		{
			return result_negative ? Int128{ 0, 0 } - Int128{ 1, 0 } : Int128{ 1, 0 };
		}

		// Binary long division algorithm
		Int128 quotient{ 0, 0 };
		Int128 remainder{ 0, 0 };

		// Process bits from most significant to least significant
		for ( int i{ constants::INT_128_MAX_BIT_INDEX }; i >= 0; --i )
		{
			// Shift remainder left by 1
			remainder = remainder + remainder; // equivalent to << 1

			// Set the least significant bit of remainder to the i-th bit of dividend
			if ( ( i >= constants::BITS_PER_UINT64 &&
					 ( ( abs_dividend.m_layout.upper64bits >> ( i - constants::BITS_PER_UINT64 ) ) & 1 ) ) ||
				 ( i < constants::BITS_PER_UINT64 &&
					 ( ( abs_dividend.m_layout.lower64bits >> i ) & 1 ) ) )
			{
				remainder = remainder + Int128{ 1, 0 };
			}

			// If remainder >= divisor, subtract divisor and set quotient bit
			if ( !( remainder < abs_divisor ) )
			{
				remainder = remainder - abs_divisor;

				// Set the i-th bit of quotient
				if ( i >= constants::BITS_PER_UINT64 )
				{
					quotient.m_layout.upper64bits |= ( constants::BIT_MASK_ONE << ( i - constants::BITS_PER_UINT64 ) );
				}
				else
				{
					quotient.m_layout.lower64bits |= ( constants::BIT_MASK_ONE << i );
				}
			}
		}
		return result_negative ? Int128{ 0, 0 } - quotient : quotient;
	}
#endif

	//----------------------------------------------
	// String parsing and conversion
	//----------------------------------------------

	Int128 Int128::parse( std::string_view str )
	{
		Int128 result;
		if ( !tryParse( str, result ) )
		{
			throw std::invalid_argument{ "Invalid Int128 string format" };
		}

		return result;
	}

	bool Int128::tryParse( std::string_view str, Int128& result ) noexcept
	{
		try
		{
			if ( str.empty() )
			{
				return false;
			}

			// Handle sign
			bool isNegative = false;
			size_t pos = 0;

			if ( str[0] == '-' )
			{
				isNegative = true;
				pos = 1;
			}
			else if ( str[0] == '+' )
			{
				pos = 1;
			}

			// Check if we have digits after sign
			if ( pos >= str.length() )
			{
				return false;
			}

			// Parse digits and build the number
			result = Int128{ 0 };

			// Quick overflow check: if string is too long, it's definitely overflow
			size_t digitCount = str.length() - pos;
			if ( digitCount > constants::INT_128_MAX_DIGIT_COUNT )
			{
				return false;
			}

			// For exactly 39 digits, we need to check against max values
			if ( digitCount == constants::INT_128_MAX_DIGIT_COUNT )
			{
				std::string_view digits = str.substr( pos );

				if ( !isNegative )
				{
					// Check against max positive
					if ( digits > constants::INT_128_MAX_POSITIVE_STRING )
					{
						return false;
					}
				}
				else
				{
					// Check against max negative (absolute value)
					if ( digits > constants::INT_128_MAX_NEGATIVE_STRING )
					{
						return false;
					}
				}
			}

			for ( size_t i = pos; i < str.length(); ++i )
			{
				char c{ str[i] };
				if ( c < '0' || c > '9' )
				{
					return false; // Invalid character
				}

				int digit = c - '0';
				result = result * Int128{ constants::INT_128_BASE } + Int128{ digit };
			}

			// Apply sign
			if ( isNegative )
			{
				result = -result;
			}

			return true;
		}
		catch ( ... )
		{
			return false;
		}
	}

	//----------------------------------------------
	// Type conversion
	//----------------------------------------------

	std::string Int128::toString() const
	{
		if ( isZero() )
		{
			return "0";
		}

		// Handle the special case of minimum value (-2^127)
		// This value cannot be represented positively in 128-bit signed integer
		if ( toHigh() == constants::INT_128_MIN_NEGATIVE_HIGH && toLow() == constants::INT_128_MIN_NEGATIVE_LOW )
		{
			return "-" + std::string{ constants::INT_128_MAX_NEGATIVE_STRING };
		}
		std::string result;
		result.reserve( constants::INT_128_MAX_DIGIT_COUNT + 1 ); // Reserve space for efficiency (39 digits + sign)

		Int128 temp = abs();

		// Extract digits by repeated division by 10
		while ( !temp.isZero() )
		{
			Int128 quotient = temp / Int128{ constants::INT_128_BASE };
			Int128 remainder = temp % Int128{ constants::INT_128_BASE };

			// remainder should be 0-9, extract as single digit
			char digit{ static_cast<char>( '0' + remainder.toLow() ) };
			result = digit + result;

			temp = quotient;
		}

		if ( isNegative() )
		{
			result = '-' + result;
		}

		return result;
	}

	std::array<std::int32_t, 4> Int128::toBits() const noexcept
	{
		std::array<std::int32_t, 4> bits{};

		// Extract from low/high 64-bit words (works on both platforms via toLow/toHigh)
		std::uint64_t low = toLow();
		std::uint64_t high = toHigh();

		bits[0] = static_cast<std::int32_t>( low );
		bits[1] = static_cast<std::int32_t>( low >> constants::BITS_PER_UINT32 );
		bits[2] = static_cast<std::int32_t>( high );
		bits[3] = static_cast<std::int32_t>( high >> constants::BITS_PER_UINT32 );
		return bits;
	}

	//----------------------------------------------
	// Comparison with built-in floating point types
	//----------------------------------------------

#if NFX_DATATYPES_HAS_NATIVE_INT128
	bool Int128::operator==( double val ) const noexcept
	{
		// Convert to long double for better precision comparison
		constexpr long double EPSILON = std::numeric_limits<long double>::epsilon();

		return std::fabs( static_cast<long double>( m_value ) - static_cast<long double>( val ) ) <= EPSILON;
	}

	bool Int128::operator<( double val ) const noexcept
	{
		return static_cast<long double>( m_value ) < static_cast<long double>( val );
	}

	bool Int128::operator>( double val ) const noexcept
	{
		return static_cast<long double>( m_value ) > static_cast<long double>( val );
	}
#else
	bool Int128::operator==( double val ) const noexcept
	{
		if ( std::isnan( val ) || std::isinf( val ) )
		{
			return false; // Int128 has no NaN/Infinity representation
		}

		// Convert this Int128 to long double for comparison
		// For manual implementation, we need to carefully construct the value
		long double thisValue;
		if ( isNegative() )
		{
			// Handle negative values using two's complement
			Int128 abs_this = this->abs();
			thisValue = -( static_cast<long double>( abs_this.m_layout.upper64bits ) *
							   static_cast<long double>( constants::BIT_MASK_ONE << constants::BITS_PER_UINT32 ) *
							   static_cast<long double>( constants::BIT_MASK_ONE << constants::BITS_PER_UINT32 ) +
						   static_cast<long double>( abs_this.m_layout.lower64bits ) );
		}
		else
		{
			thisValue = static_cast<long double>( m_layout.upper64bits ) *
							static_cast<long double>( constants::BIT_MASK_ONE << constants::BITS_PER_UINT32 ) *
							static_cast<long double>( constants::BIT_MASK_ONE << constants::BITS_PER_UINT32 ) +
						static_cast<long double>( m_layout.lower64bits );
		}
		return thisValue == static_cast<long double>( val );
	}

	bool Int128::operator<( double val ) const noexcept
	{
		if ( std::isnan( val ) )
		{
			return false; // No ordering with NaN
		}
		if ( std::isinf( val ) )
		{
			return val > 0.0; // Any finite value < +infinity, any finite value > -infinity
		}

		// Convert this Int128 to long double for comparison
		long double thisValue;
		if ( isNegative() )
		{
			Int128 abs_this = this->abs();
			thisValue = -( static_cast<long double>( abs_this.m_layout.upper64bits ) *
							   static_cast<long double>( constants::BIT_MASK_ONE << constants::BITS_PER_UINT32 ) *
							   static_cast<long double>( constants::BIT_MASK_ONE << constants::BITS_PER_UINT32 ) +
						   static_cast<long double>( abs_this.m_layout.lower64bits ) );
		}
		else
		{
			thisValue = static_cast<long double>( m_layout.upper64bits ) *
							static_cast<long double>( constants::BIT_MASK_ONE << constants::BITS_PER_UINT32 ) * static_cast<long double>( constants::BIT_MASK_ONE << constants::BITS_PER_UINT32 ) +
						static_cast<long double>( m_layout.lower64bits );
		}

		return thisValue < static_cast<long double>( val );
	}

	bool Int128::operator>( double val ) const noexcept
	{
		if ( std::isnan( val ) )
		{
			return false; // No ordering with NaN
		}
		if ( std::isinf( val ) )
		{
			return val < 0.0; // Any finite value > -infinity, any finite value < +infinity
		}

		// Convert this Int128 to long double for comparison
		long double thisValue;
		if ( isNegative() )
		{
			Int128 abs_this = this->abs();
			thisValue = -( static_cast<long double>( abs_this.m_layout.upper64bits ) *
							   static_cast<long double>( constants::BIT_MASK_ONE << constants::BITS_PER_UINT32 ) *
							   static_cast<long double>( constants::BIT_MASK_ONE << constants::BITS_PER_UINT32 ) +
						   static_cast<long double>( abs_this.m_layout.lower64bits ) );
		}
		else
		{
			thisValue = static_cast<long double>( m_layout.upper64bits ) *
							static_cast<long double>( constants::BIT_MASK_ONE << constants::BITS_PER_UINT32 ) *
							static_cast<long double>( constants::BIT_MASK_ONE << constants::BITS_PER_UINT32 ) +
						static_cast<long double>( m_layout.lower64bits );
		}

		return thisValue > static_cast<long double>( val );
	}
#endif

	//----------------------------------------------
	// Access operations
	//----------------------------------------------

#if NFX_DATATYPES_HAS_NATIVE_INT128

	std::uint64_t Int128::toLow() const noexcept
	{
		return static_cast<std::uint64_t>( m_value );
	}

	std::uint64_t Int128::toHigh() const noexcept
	{
		return static_cast<std::uint64_t>( m_value >> constants::BITS_PER_UINT64 );
	}

	NFX_DATATYPES_NATIVE_INT128 Int128::toNative() const noexcept
	{
		return m_value;
	}
#else
	std::uint64_t Int128::toLow() const noexcept
	{
		return m_layout.lower64bits;
	}

	std::uint64_t Int128::toHigh() const noexcept
	{
		return m_layout.upper64bits;
	}
#endif

	//=====================================================================
	// Stream operators
	//=====================================================================

	std::ostream& operator<<( std::ostream& os, const Int128& value )
	{
		// Simple string conversion for display purposes
		if ( value.isZero() )
		{
			return os << '0';
		}

		Int128 temp = value.abs();
		std::string result;

		// Extract digits by repeated division by 10
		while ( !temp.isZero() )
		{
			Int128 quotient = temp / Int128{ constants::INT_128_BASE };
			Int128 remainder = temp % Int128{ constants::INT_128_BASE };

			// remainder should be 0-9, extract as single digit
			char digit{ static_cast<char>( '0' + remainder.toLow() ) };
			result = digit + result;

			temp = quotient;
		}

		if ( value.isNegative() )
		{
			result = '-' + result;
		}

		return os << result;
	}

	std::istream& operator>>( std::istream& is, Int128& value )
	{
		std::string str;
		is >> str;

		if ( !Int128::tryParse( str, value ) )
		{
			is.setstate( std::ios::failbit );
		}

		return is;
	}
} // namespace nfx::datatypes
