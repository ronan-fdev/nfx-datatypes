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
 * @file Decimal.cpp
 * @brief Implementation of cross-platform 128-bit decimal arithmetic
 * @details Provides exact decimal arithmetic with portable 128-bit operations
 */

#include <cmath>
#include <iomanip>
#include <istream>
#include <ostream>
#include <sstream>

#include "nfx/datatypes/Decimal.h"

#include "nfx/datatypes/Int128.h"
#include "Constants.h"

namespace nfx::datatypes
{
	namespace internal
	{
		//=====================================================================
		// Internal helper functions
		//=====================================================================

		/**
		 * @brief Multiply a 96-bit mantissa by 10 and add a digit (in-place)
		 * @param mantissa Pointer to 3-element mantissa array
		 * @param digit Digit to add (0-9)
		 */
		static void multiplyMantissaBy10AndAdd( std::uint32_t* mantissa, std::uint32_t digit ) noexcept
		{
			// Multiply 96-bit number by 10 using: result = mantissa * 8 + mantissa * 2
			std::uint64_t carry = 0;

			// Multiply by 10 (shift left 3 bits + shift left 1 bit = multiply by 8 + 2 = 10)
			for ( int i = 0; i < 3; ++i )
			{
				std::uint64_t temp = static_cast<std::uint64_t>( mantissa[i] ) * 10ULL + carry;
				mantissa[i] = static_cast<std::uint32_t>( temp );
				carry = temp >> 32;
			}

			// Add the digit
			carry = digit;
			for ( int i = 0; i < 3 && carry > 0; ++i )
			{
				std::uint64_t temp = static_cast<std::uint64_t>( mantissa[i] ) + carry;
				mantissa[i] = static_cast<std::uint32_t>( temp );
				carry = temp >> 32;
			}
		}

		/**
		 * @brief Get power of 10 as Int128 for any scale 0-28
		 * @param power The power (0-28)
		 * @return Int128 representing 10^power
		 */
		static Int128 getPowerOf10( std::uint8_t power ) noexcept
		{
			if ( power < constants::DECIMAL_POWER_TABLE_SIZE && constants::DECIMAL_POWERS_OF_10[power] != 0 )
			{
				// Use 64-bit lookup table for powers 0-19
				return Int128{ constants::DECIMAL_POWERS_OF_10[power] };
			}
			else if ( power >= constants::DECIMAL_EXTENDED_POWER_MIN && power <= constants::DECIMAL_EXTENDED_POWER_MAX )
			{
				// Use pre-computed 128-bit values for powers 20-28
				const auto& extended{ constants::DECIMAL_EXTENDED_POWERS_OF_10[power - constants::DECIMAL_EXTENDED_POWER_MIN] };
				return Int128{ extended.first, extended.second };
			}
			else
			{
				// Fallback to iterative computation for invalid powers (shouldn't happen)
				Int128 result{ 1 };
				for ( std::uint8_t i{ 0 }; i < power; ++i )
				{
					result = result * Int128{ constants::DECIMAL_BASE };
				}
				return result;
			}
		}

		/**
		 * @brief Extract 128-bit mantissa value from Decimal
		 * @param decimal The decimal value to extract mantissa from
		 * @return Int128 representation of the mantissa
		 */
		static Int128 mantissaAsInt128( const Decimal& decimal ) noexcept
		{
#if NFX_DATATYPES_HAS_NATIVE_INT128
			const auto& mantissaArray{ decimal.mantissa() };
			NFX_DATATYPES_NATIVE_INT128 value{ static_cast<NFX_DATATYPES_NATIVE_INT128>( mantissaArray[2] ) << constants::BITS_PER_UINT64 |
											   static_cast<NFX_DATATYPES_NATIVE_INT128>( mantissaArray[1] ) << constants::BITS_PER_UINT32 |
											   static_cast<NFX_DATATYPES_NATIVE_INT128>( mantissaArray[0] ) };

			return Int128{ value };
#else
			const auto& mantissaArray{ decimal.mantissa() };
			std::uint64_t low{ static_cast<std::uint64_t>( mantissaArray[1] ) << constants::BITS_PER_UINT32 | mantissaArray[0] };
			std::uint64_t high{ mantissaArray[2] };

			return Int128{ low, high };
#endif
		}

		/**
		 * @brief Align scales of two decimals for arithmetic operations
		 * @param decimal First decimal value
		 * @param other Second decimal value
		 * @return Pair of Int128 mantissas with aligned scales
		 */
		static std::pair<Int128, Int128> alignScale( const Decimal& decimal, const Decimal& other )
		{
			Int128 left{ mantissaAsInt128( decimal ) };
			Int128 right{ mantissaAsInt128( other ) };

			std::uint8_t leftScale{ decimal.scale() };
			std::uint8_t rightScale{ other.scale() };

			// Optimized scaling using enhanced power-of-10 lookup with 128-bit support
			if ( leftScale < rightScale )
			{
				std::uint8_t scaleDiff{ static_cast<std::uint8_t>( rightScale - leftScale ) };
				left = left * getPowerOf10( scaleDiff );
			}
			else if ( rightScale < leftScale )
			{
				std::uint8_t scaleDiff{ static_cast<std::uint8_t>( leftScale - rightScale ) };
				right = right * getPowerOf10( scaleDiff );
			}

			return { std::move( left ), std::move( right ) };
		}

		/**
		 * @brief Set mantissa value in Decimal from Int128
		 * @param decimal The decimal to modify
		 * @param value The Int128 mantissa value to set
		 */
		static void setMantissa( Decimal& decimal, const Int128& value ) noexcept
		{
#if NFX_DATATYPES_HAS_NATIVE_INT128
			auto nativeValue{ value.toNative() };
			auto& mantissa{ decimal.mantissa() };
			mantissa[0] = static_cast<std::uint32_t>( nativeValue );
			mantissa[1] = static_cast<std::uint32_t>( nativeValue >> constants::BITS_PER_UINT32 );
			mantissa[2] = static_cast<std::uint32_t>( nativeValue >> constants::BITS_PER_UINT64 );
#else
			auto& mantissa{ decimal.mantissa() };
			std::uint64_t low{ value.toLow() };
			std::uint64_t high{ value.toHigh() };

			mantissa[0] = static_cast<std::uint32_t>( low );
			mantissa[1] = static_cast<std::uint32_t>( low >> constants::BITS_PER_UINT32 );
			mantissa[2] = static_cast<std::uint32_t>( high );
#endif
		}

		/**
		 * @brief Divide decimal mantissa by power of 10
		 * @param decimal The decimal to modify
		 * @param power The power of 10 to divide by (0-28)
		 */
		static void divideByPowerOf10( Decimal& decimal, std::uint8_t power )
		{
			Int128 mantissa{ mantissaAsInt128( decimal ) };

			// Use enhanced power-of-10 lookup with full 128-bit support
			mantissa = mantissa / getPowerOf10( power );

			setMantissa( decimal, mantissa );
		}

		/**
		 * @brief Normalize decimal by removing trailing zeros and reducing scale
		 * @param decimal The decimal to normalize
		 */
		static void normalize( Decimal& decimal ) noexcept
		{
			// Remove trailing zeros and reduce scale
			while ( decimal.scale() > 0 && ( mantissaAsInt128( decimal ) % Int128{ constants::DECIMAL_BASE } ) == Int128{ 0 } )
			{
				divideByPowerOf10( decimal, 1U );
				std::uint8_t currentScale{ decimal.scale() };
				decimal.flags() = ( decimal.flags() & ~constants::DECIMAL_SCALE_MASK ) |
								  ( static_cast<std::uint32_t>( currentScale - 1U )
									  << constants::DECIMAL_SCALE_SHIFT );
			}
		}

		/**
		 * @brief Determine if rounding up is needed for ToNearest mode (Banker's rounding)
		 */
		static bool shouldRoundUpToNearest( const Int128& roundingDigit, const Int128& mantissa,
			const Int128& divisor, std::uint8_t digitsToRemove,
			const Decimal& result ) noexcept
		{
			if ( roundingDigit.toLow() > constants::DECIMAL_ROUNDING_THRESHOLD )
			{
				return true; // > 5: always round away from zero
			}
			else if ( roundingDigit.toLow() == constants::DECIMAL_ROUNDING_THRESHOLD )
			{
				// == 5: check if there are any non-zero digits after this one
				bool hasRemainingFraction{ false };
				if ( digitsToRemove > 1U )
				{
					Int128 remainderDivisor{ divisor };
					Int128 remainder{ mantissa % remainderDivisor };
					Int128 roundingDigitContribution{ roundingDigit * ( divisor / Int128{ constants::DECIMAL_BASE } ) };
					hasRemainingFraction = ( remainder != roundingDigitContribution );
				}

				if ( hasRemainingFraction )
				{
					return true; // Ties away from zero when there's additional fractional part
				}
				else
				{
					// Exact tie: round to even
					Int128 resultMantissa{ mantissaAsInt128( result ) };
					bool isEven{ ( resultMantissa % Int128{ 2 } ) == Int128{ 0 } };
					return !isEven; // Round up if currently odd
				}
			}
			return false;
		}

		/**
		 * @brief Determine if rounding up is needed for ToNearestTiesAway mode
		 */
		static bool shouldRoundUpToNearestTiesAway( const Int128& roundingDigit ) noexcept
		{
			return ( roundingDigit.toLow() >= constants::DECIMAL_ROUNDING_THRESHOLD );
		}

		/**
		 * @brief Determine if rounding up is needed for ToPositiveInfinity mode (Ceiling)
		 */
		static bool shouldRoundUpToPositiveInfinity( const Int128& mantissa, std::uint8_t digitsToRemove,
			bool isNegative ) noexcept
		{
			if ( isNegative )
			{
				return false; // Negative numbers round toward zero for ceiling
			}

			// Check if ANY fractional digits exist
			if ( digitsToRemove > 0 )
			{
				Int128 fractionalDivisor{ getPowerOf10( digitsToRemove ) };
				Int128 fractionalPart{ mantissa % fractionalDivisor };
				return !fractionalPart.isZero();
			}
			return false;
		}

		/**
		 * @brief Determine if rounding up is needed for ToNegativeInfinity mode (Floor)
		 */
		static bool shouldRoundUpToNegativeInfinity( const Int128& mantissa, std::uint8_t digitsToRemove,
			bool isNegative ) noexcept
		{
			if ( !isNegative )
			{
				return false; // Positive numbers round toward zero for floor
			}

			// Check if ANY fractional digits exist
			if ( digitsToRemove > 0 )
			{
				Int128 fractionalDivisor{ getPowerOf10( digitsToRemove ) };
				Int128 fractionalPart{ mantissa % fractionalDivisor };
				return !fractionalPart.isZero();
			}
			return false;
		}
	} // namespace internal

	//=====================================================================
	// Decimal class
	//=====================================================================

	//----------------------------------------------
	// Construction
	//----------------------------------------------

	Decimal::Decimal( double value ) noexcept
		: m_layout{ 0, { { 0, 0, 0 } } }
	{
		// Handle special cases (same as before)
		if ( std::isnan( value ) || std::isinf( value ) || value == 0.0 )
			return;

		// Extract sign
		bool negative = value < 0.0;
		if ( negative )
			value = -value;

		// Decompose double into integer and fractional parts
		// Using direct FPU operations instead of string conversion

		// 1. Extract integer part (fast)
		double intPart;
		double fracPart = std::modf( value, &intPart );

		// 2. Convert integer part directly to mantissa (no string!)
		std::uint64_t intValue = static_cast<std::uint64_t>( intPart );
		m_layout.mantissa[0] = static_cast<std::uint32_t>( intValue );
		m_layout.mantissa[1] = static_cast<std::uint32_t>( intValue >> 32 );

		// 3. Convert fractional part by multiplying by 10^scale
		std::uint8_t scale = 0;
		if ( fracPart > 0.0 )
		{
			// Determine optimal scale (up to 15 digits precision)
			while ( fracPart > 0.0 && scale < constants::DOUBLE_DECIMAL_PRECISION )
			{
				fracPart *= 10.0;
				double digit;
				fracPart = std::modf( fracPart, &digit );

				// Multiply mantissa by 10 and add digit
				internal::multiplyMantissaBy10AndAdd( m_layout.mantissa.data(), static_cast<std::uint32_t>( digit ) );
				scale++;

				if ( fracPart < 1e-15 ) // Stop when remaining is negligible
				{
					break;
				}
			}
		}

		// 4. Set scale and sign
		m_layout.flags = ( scale << constants::DECIMAL_SCALE_SHIFT );
		if ( negative )
			m_layout.flags |= constants::DECIMAL_SIGN_MASK;
	}

	Decimal::Decimal( std::int32_t value ) noexcept
		: m_layout{ 0, { { 0, 0, 0 } } }
	{
		if ( value < 0 )
		{
			m_layout.flags |= constants::DECIMAL_SIGN_MASK;
			value = -value;
		}

		m_layout.mantissa[0] = static_cast<std::uint32_t>( value );
	}

	Decimal::Decimal( std::int64_t value ) noexcept
		: m_layout{ 0, { { 0, 0, 0 } } }
	{
		if ( value < 0 )
		{
			m_layout.flags |= constants::DECIMAL_SIGN_MASK;
			value = -value;
		}

		m_layout.mantissa[0] = static_cast<std::uint32_t>( value );
		m_layout.mantissa[1] = static_cast<std::uint32_t>( value >> constants::BITS_PER_UINT32 );
	}

	Decimal::Decimal( std::uint32_t value ) noexcept
		: m_layout{ 0, { { 0, 0, 0 } } }
	{
		m_layout.mantissa[0] = value;
	}

	Decimal::Decimal( std::uint64_t value ) noexcept
		: m_layout{ 0, { { 0, 0, 0 } } }
	{
		m_layout.mantissa[0] = static_cast<std::uint32_t>( value );
		m_layout.mantissa[1] = static_cast<std::uint32_t>( value >> constants::BITS_PER_UINT32 );
	}

	Decimal::Decimal( std::string_view str )
		: m_layout{ 0, { { 0, 0, 0 } } }
	{
		if ( !tryParse( str, *this ) )
		{
			throw std::invalid_argument{ "Invalid decimal string format" };
		}
	}

	Decimal::Decimal( const Int128& val )
		: m_layout{ 0, { { 0, 0, 0 } } }
	{
		// Handle zero case
		if ( val.isZero() )
		{
			return;
		}

		// Extract sign and get absolute value
		bool isNegative = val.isNegative();

		// Handle the special case of minimum Int128 value (-2^127)
		// This value cannot be represented positively in 128-bit signed integer
		Int128 absoluteValue;
#if NFX_DATATYPES_HAS_NATIVE_INT128
		if ( val.toNative() == static_cast<NFX_DATATYPES_NATIVE_INT128>( constants::INT_128_MIN_NEGATIVE_HIGH ) << constants::BITS_PER_UINT64 )
#else
		if ( val.toHigh() == constants::INT_128_MIN_NEGATIVE_HIGH && val.toLow() == constants::INT_128_MIN_NEGATIVE_LOW )
#endif
		{
			// For the minimum value, we manually construct the absolute value
			// Since -2^127 cannot be represented as a positive Int128, we clamp to Decimal max
			m_layout.mantissa[0] = constants::DECIMAL_MAX_MANTISSA_0; // Lower 32 bits: all 1s
			m_layout.mantissa[1] = constants::DECIMAL_MAX_MANTISSA_1; // Middle 32 bits: all 1s
			m_layout.mantissa[2] = constants::DECIMAL_MAX_MANTISSA_2; // Upper 32 bits: all 1s

			// Set sign flag if negative
			if ( isNegative )
			{
				m_layout.flags |= constants::DECIMAL_SIGN_MASK;
			}
			return;
		}
		else
		{
			absoluteValue = val.abs();
		}

		// Set sign flag if negative
		if ( isNegative )
		{
			m_layout.flags |= constants::DECIMAL_SIGN_MASK;
		}

		// Check if the absolute value fits in Decimal's 96-bit mantissa capacity
		// Maximum 96-bit unsigned value: 2^96 - 1 = 79,228,162,514,264,337,593,543,950,335
		// This is much smaller than Int128 max value: 2^127 - 1 = 170,141,183,460,469,231,731,687,303,715,884,105,727

		// Check if the high 64 bits contain anything beyond what fits in 32 bits (mantissa[2])
		std::uint64_t high64 = absoluteValue.toHigh();
		if ( high64 > constants::UINT32_MAX_VALUE )
		{
			// Value exceeds Decimal's capacity - clamp to maximum representable value
			// Use the maximum 96-bit value: 2^96 - 1
			m_layout.mantissa[0] = constants::DECIMAL_MAX_MANTISSA_0; // Lower 32 bits: all 1s
			m_layout.mantissa[1] = constants::DECIMAL_MAX_MANTISSA_1; // Middle 32 bits: all 1s
			m_layout.mantissa[2] = constants::DECIMAL_MAX_MANTISSA_2; // Upper 32 bits: all 1s
		}
		else
		{
			// Value fits in 96 bits - store it directly
			internal::setMantissa( *this, absoluteValue );
		}
	}

	//----------------------------------------------
	// Decimal constants
	//----------------------------------------------

	Decimal Decimal::minValue() noexcept
	{
		Decimal result{};
		result.m_layout.mantissa[0] = constants::DECIMAL_MIN_MANTISSA_0;
		result.m_layout.mantissa[1] = constants::DECIMAL_MIN_MANTISSA_1;
		result.m_layout.mantissa[2] = constants::DECIMAL_MIN_MANTISSA_2;
		result.m_layout.flags = ( constants::DECIMAL_MAXIMUM_PLACES << constants::DECIMAL_SCALE_SHIFT );

		return result;
	}

	Decimal Decimal::maxValue() noexcept
	{
		Decimal result{};
		result.m_layout.mantissa[0] = constants::DECIMAL_MAX_MANTISSA_0;
		result.m_layout.mantissa[1] = constants::DECIMAL_MAX_MANTISSA_1;
		result.m_layout.mantissa[2] = constants::DECIMAL_MAX_MANTISSA_2;

		return result;
	}

	//----------------------------------------------
	// Property accessors
	//----------------------------------------------

	std::uint8_t Decimal::scale() const noexcept
	{
		return static_cast<std::uint8_t>( ( m_layout.flags & constants::DECIMAL_SCALE_MASK ) >> constants::DECIMAL_SCALE_SHIFT );
	}

	//----------------------------------------------
	// State checking
	//----------------------------------------------

	bool Decimal::isNegative() const noexcept
	{
		return ( m_layout.flags & constants::DECIMAL_SIGN_MASK ) != 0;
	}

	//----------------------------------------------
	// Arithmetic operators
	//----------------------------------------------

	Decimal Decimal::operator+( const Decimal& other )
	{
		if ( isZero() )
		{
			return other;
		}
		if ( other.isZero() )
		{
			return *this;
		}

		Decimal result;
		auto [left, right]{ internal::alignScale( *this, other ) };

		internal::setMantissa( result, left + right );
		result.m_layout.flags = ( m_layout.flags & ~constants::DECIMAL_SCALE_MASK ) |
								( std::max( scale(), other.scale() ) << constants::DECIMAL_SCALE_SHIFT );

		// Handle sign
		if ( isNegative() == other.isNegative() )
		{
			if ( isNegative() )
			{
				result.m_layout.flags |= constants::DECIMAL_SIGN_MASK;
			}
		}
		else
		{
			// Different signs - need subtraction logic
			if ( left > right )
			{
				internal::setMantissa( result, left - right );
				if ( isNegative() )
				{
					result.m_layout.flags |= constants::DECIMAL_SIGN_MASK;
				}
			}
			else
			{
				internal::setMantissa( result, right - left );
				if ( other.isNegative() )
				{
					result.m_layout.flags |= constants::DECIMAL_SIGN_MASK;
				}
			}
		}

		internal::normalize( result );

		return result;
	}

	Decimal Decimal::operator-( const Decimal& other )
	{
		Decimal negatedOther{ other };

		negatedOther.m_layout.flags ^= constants::DECIMAL_SIGN_MASK;

		return *this + negatedOther;
	}

	Decimal Decimal::operator*( const Decimal& other ) const
	{
		if ( isZero() || other.isZero() )
		{
			return Decimal{};
		}

		Decimal result;
		Int128 left{ internal::mantissaAsInt128( *this ) };
		Int128 right{ internal::mantissaAsInt128( other ) };

		// Calculate the product mantissa without storing it yet
		Int128 productMantissa{ left * right };

		// Combine scales
		std::uint8_t newScale{ static_cast<std::uint8_t>( scale() + other.scale() ) };

		// Check if the mantissa fits in 96 bits (max value: 2^96 - 1)
		const Int128 max96bit{ constants::DECIMAL_96BIT_MAX_LOW, constants::DECIMAL_96BIT_MAX_HIGH };

		// If mantissa exceeds 96 bits OR scale exceeds maximum, we need to truncate precision
		while ( ( productMantissa > max96bit ) || ( newScale > constants::DECIMAL_MAXIMUM_PLACES ) )
		{
			// Divide mantissa by 10 to reduce precision
			productMantissa = productMantissa / Int128{ constants::DECIMAL_BASE };
			newScale--;

			// Safety check to prevent infinite loop
			if ( newScale == 0 && productMantissa > max96bit )
			{
				// If we still can't fit in 96 bits even with scale 0,
				// the number is too large for Decimal representation
				break;
			}
		}

		// Now store the properly scaled mantissa
		internal::setMantissa( result, productMantissa );

		result.m_layout.flags = ( static_cast<std::uint32_t>( newScale ) << constants::DECIMAL_SCALE_SHIFT );

		// Combine signs
		if ( isNegative() != other.isNegative() )
		{
			result.m_layout.flags |= constants::DECIMAL_SIGN_MASK;
		}

		internal::normalize( result );

		return result;
	}

	Decimal Decimal::operator/( const Decimal& other ) const
	{
		if ( other.isZero() )
		{
			throw std::overflow_error{ "Division by zero" };
		}

		if ( isZero() )
		{
			return Decimal{};
		}

		Decimal result;
		Int128 dividend{ internal::mantissaAsInt128( *this ) };
		Int128 divisor{ internal::mantissaAsInt128( other ) };

		// Scale adjustment for division:
		// If dividend has scale d and divisor has scale s,
		// result should have scale (d - s)
		// To maintain precision, we ALWAYS scale up the dividend
		std::int32_t targetScale{ static_cast<std::int32_t>( scale() ) - static_cast<std::int32_t>( other.scale() ) };

		// Scale up dividend to maintain precision
		std::uint8_t extraPrecision{ constants::DECIMAL_DIVISION_EXTRA_PRECISION };
		for ( std::uint8_t i{ 0U }; i < extraPrecision; ++i )
		{
			// Check if scaling would cause overflow
			if ( dividend.toHigh() > constants::INT128_MUL10_OVERFLOW_THRESHOLD )
			{
				break; // Stop scaling to prevent overflow
			}
			dividend = dividend * Int128{ constants::DECIMAL_BASE };
			targetScale++;
		}

		// If target scale would still be negative, scale up more
		if ( targetScale < 0 )
		{
			std::uint8_t scaleUp{ static_cast<std::uint8_t>( -targetScale ) };
			for ( std::uint8_t i{ 0U }; i < scaleUp && i < constants::DECIMAL_MAXIMUM_PLACES; ++i )
			{
				if ( dividend.toHigh() > constants::INT128_MUL10_OVERFLOW_THRESHOLD )
				{
					break; // Stop scaling to prevent overflow
				}
				dividend = dividend * Int128{ constants::DECIMAL_BASE };
				targetScale++;
			}
		}

		internal::setMantissa( result, dividend / divisor );
		result.m_layout.flags = ( static_cast<std::uint32_t>( targetScale ) << constants::DECIMAL_SCALE_SHIFT );

		// Combine signs
		if ( isNegative() != other.isNegative() )
		{
			result.m_layout.flags |= constants::DECIMAL_SIGN_MASK;
		}

		internal::normalize( result );

		return result;
	}

	Decimal Decimal::operator-() const noexcept
	{
		Decimal result{ *this };

		result.m_layout.flags ^= constants::DECIMAL_SIGN_MASK;
		return result;
	}

	//----------------------------------------------
	// Comparison operators
	//----------------------------------------------

	bool Decimal::operator==( const Decimal& other ) const noexcept
	{
		if ( isZero() && other.isZero() )
		{
			return true;
		}

		if ( isNegative() != other.isNegative() )
		{
			return false;
		}

		auto [left, right] = internal::alignScale( *this, other );

		return left == right;
	}

	bool Decimal::operator<( const Decimal& other ) const noexcept
	{
		if ( isNegative() != other.isNegative() )
		{
			return isNegative();
		}

		auto [left, right] = internal::alignScale( *this, other );

		if ( isNegative() )
		{
			return left > right;
		}
		else
		{
			return left < right;
		}
	}

	//----------------------------------------------
	// Comparison with nfx Int128
	//----------------------------------------------

	bool Decimal::operator==( const Int128& val ) const noexcept
	{
		// For integer comparison, we need exact equality
		if ( scale() > 0 )
		{
			// If this has fractional part, it can't equal an integer
			return false;
		}

		// Convert this decimal's mantissa to Int128 and compare directly
		Int128 mantissa{ internal::mantissaAsInt128( *this ) };

		// Handle signs
		if ( isNegative() )
		{
			if ( val >= Int128{ 0 } )
			{
				return false; // Different signs
			}
			// Both negative - compare absolute values (negate mantissa for comparison)
			return mantissa == -val;
		}
		else
		{
			if ( val < Int128{ 0 } )
			{
				return false; // Different signs
			}
			// Both positive
			return mantissa == val;
		}
	}

	bool Decimal::operator<( const Int128& val ) const noexcept
	{
		// Handle different signs
		if ( isNegative() && val >= Int128{ 0 } )
		{
			return true; // Negative < Non-negative
		}
		if ( !isNegative() && val < Int128{ 0 } )
		{
			return false; // Non-negative > Negative
		}

		// Same signs - convert decimal to comparable form
		Int128 mantissa{ internal::mantissaAsInt128( *this ) };

		if ( scale() > 0 )
		{
			// This decimal has fractional part - scale up the integer for comparison
			Int128 scaledVal{ val * internal::getPowerOf10( scale() ) };

			if ( isNegative() )
			{
				// Both negative - compare absolute values with flipped result
				return mantissa > scaledVal.abs();
			}
			else
			{
				return mantissa < scaledVal;
			}
		}
		else
		{
			// No fractional part - direct comparison
			if ( isNegative() )
			{
				// Both negative - compare absolute values with flipped result
				return mantissa > val.abs();
			}
			else
			{
				return mantissa < val;
			}
		}
	}

	//----------------------------------------------
	// String parsing and conversion
	//----------------------------------------------

	Decimal Decimal::parse( std::string_view str )
	{
		Decimal result;
		if ( !tryParse( str, result ) )
		{
			throw std::invalid_argument{ "Invalid decimal string format" };
		}
		return result;
	}

	bool Decimal::tryParse( std::string_view str, Decimal& result ) noexcept
	{
		try
		{
			result = Decimal{};

			if ( str.empty() )
			{
				return false;
			}

			// Handle sign
			bool negative{ false };
			size_t pos{ 0 };
			if ( str[0] == '-' )
			{
				negative = true;
				pos = 1;
			}
			else if ( str[0] == '+' )
			{
				pos = 1;
			}

			// Check if we have at least one character after sign
			if ( pos >= str.length() )
			{
				return false;
			}

			// Find decimal point and validate there's only one
			size_t decimalPos{ std::string_view::npos };
			std::uint8_t currentScale{ 0 };
			size_t decimalCount{ 0 };

			for ( size_t i{ pos }; i < str.length(); ++i )
			{
				if ( str[i] == '.' )
				{
					decimalCount++;
					if ( decimalCount > 1 )
					{
						return false;
					}

					decimalPos = i;
				}
			}

			if ( decimalPos != std::string_view::npos )
			{
				currentScale = static_cast<std::uint8_t>( str.length() - decimalPos - 1 );
				if ( currentScale > constants::DECIMAL_MAXIMUM_PLACES )
				{
					currentScale = constants::DECIMAL_MAXIMUM_PLACES;
				}
			}

			// Optimized digit accumulation
			Int128 mantissaValue;
			const Int128 ten{ constants::DECIMAL_BASE };
			bool hasDigits{ false };
			std::uint8_t significantDigits{ 0 };
			std::uint8_t decimalDigitsProcessed{ 0 };

			for ( size_t i{ pos }; i < str.length(); ++i )
			{
				if ( str[i] == '.' )
				{
					continue;
				}

				if ( str[i] < '0' || str[i] > '9' )
				{
					// Invalid character
					return false;
				}

				hasDigits = true;
				std::uint64_t digit{ static_cast<std::uint64_t>( str[i] - '0' ) };

				// Decimal specification: maximum 28 significant digits
				if ( significantDigits >= constants::DECIMAL_MAXIMUM_PLACES )
				{
					// Truncate excess digits - adjust scale based on actual decimal digits processed
					if ( decimalPos != std::string_view::npos )
					{
						currentScale = decimalDigitsProcessed;
					}
					break;
				}

				// Count significant digits (skip leading zeros only before decimal point)
				if ( digit != 0 || mantissaValue != Int128{ 0 } || ( decimalPos != std::string_view::npos && i > decimalPos ) )
				{
					significantDigits++;
				}

				// Count decimal digits processed
				if ( decimalPos != std::string_view::npos && i > decimalPos )
				{
					decimalDigitsProcessed++;
				}

				// Safe to accumulate this digit
				mantissaValue = mantissaValue * ten + Int128{ digit };
			}

			// Ensure we have at least one digit (prevents parsing ".", "+", "-", etc.)
			if ( !hasDigits )
			{
				return false;
			}

			// Check if mantissa fits in our 96-bit storage
			if ( mantissaValue.toHigh() > constants::UINT32_MAX_VALUE )
			{
				// Value too large - truncate excess precision to fit
				while ( mantissaValue.toHigh() > constants::UINT32_MAX_VALUE && currentScale > 0 )
				{
					mantissaValue = mantissaValue / Int128{ constants::DECIMAL_BASE };
					--currentScale;
				}

				// If still too large after removing all decimal places, truncate the integer part to fit
				while ( mantissaValue.toHigh() > constants::UINT32_MAX_VALUE )
				{
					mantissaValue = mantissaValue / Int128{ constants::DECIMAL_BASE };
				}
			}

			// Set result
			if ( negative )
			{
				result.m_layout.flags |= constants::DECIMAL_SIGN_MASK;
			}

			result.m_layout.flags |= ( static_cast<std::uint32_t>( currentScale ) << constants::DECIMAL_SCALE_SHIFT );

			// Store the 96-bit mantissa
			std::uint64_t low{ mantissaValue.toLow() };
			std::uint64_t high{ mantissaValue.toHigh() };

			result.m_layout.mantissa[0] = static_cast<std::uint32_t>( low );
			result.m_layout.mantissa[1] = static_cast<std::uint32_t>( low >> constants::BITS_PER_UINT32 );
			result.m_layout.mantissa[2] = static_cast<std::uint32_t>( high );

			// Normalize to remove trailing zeros
			internal::normalize( result );

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

	double Decimal::toDouble() const noexcept
	{
		Int128 mantissa{ internal::mantissaAsInt128( *this ) };

		double result;
#if NFX_DATATYPES_HAS_NATIVE_INT128
		result = static_cast<double>( mantissa.toNative() );
#else
		// Convert 128-bit to double (approximate)
		result = static_cast<double>(
					 mantissa.toHigh() ) *
					 ( constants::BIT_MASK_ONE << constants::BITS_PER_UINT32 ) *
					 ( constants::BIT_MASK_ONE << constants::BITS_PER_UINT32 ) +
				 static_cast<double>( mantissa.toLow() );
#endif
		// Apply scale
		std::uint8_t currentScale = scale();
		for ( std::uint8_t i = 0; i < currentScale; ++i )
		{
			result /= static_cast<double>( constants::DECIMAL_BASE );
		}

		// Apply sign
		if ( isNegative() )
		{
			result = -result;
		}

		return result;
	}

	std::string Decimal::toString() const
	{
		if ( isZero() )
		{
			return "0";
		}

		std::string result;
		result.reserve( constants::DECIMAL_STRING_BUFFER_SIZE );

		Int128 mantissa{ internal::mantissaAsInt128( *this ).abs() };
		std::uint8_t currentScale{ scale() };

		// Optimized digit extraction with fast division
		std::array<char, constants::DECIMAL_MAX_STRING_LENGTH> digits;
		size_t digitCount{ 0 };

#if NFX_DATATYPES_HAS_NATIVE_INT128
		// Fast path for values that fit in 64-bit
		if ( mantissa.toNative() <= UINT64_MAX )
		{
			std::uint64_t value{ static_cast<std::uint64_t>( mantissa.toNative() ) };
			while ( value > 0 && digitCount < digits.size() )
			{
				digits[digitCount++] = static_cast<char>( '0' + ( value % constants::DECIMAL_BASE ) );
				value /= constants::DECIMAL_BASE;
			}
		}
		else
		{
			// Full 128-bit extraction
			while ( !mantissa.isZero() && digitCount < digits.size() )
			{
				digits[digitCount++] = static_cast<char>( '0' +
														  ( mantissa.toNative() % constants::DECIMAL_BASE ) );
				mantissa = Int128{ mantissa.toNative() / constants::DECIMAL_BASE };
			}
		}
#else
		if ( mantissa.toHigh() == 0 )
		{
			// Fast 64-bit path
			std::uint64_t value{ mantissa.toLow() };
			while ( value > 0 && digitCount < digits.size() )
			{
				digits[digitCount++] = static_cast<char>( '0' + ( value % constants::DECIMAL_BASE ) );
				value /= constants::DECIMAL_BASE;
			}
		}
		else
		{
			// Manual 128-bit extraction
			while ( !mantissa.isZero() && digitCount < digits.size() )
			{
				if ( mantissa.toHigh() == 0 )
				{
					// Switched to 64-bit range
					std::uint64_t value{ mantissa.toLow() };
					while ( value > 0 && digitCount < digits.size() )
					{
						digits[digitCount++] = static_cast<char>( '0' + ( value % constants::DECIMAL_BASE ) );
						value /= constants::DECIMAL_BASE;
					}
					break;
				}
				// Use full 128-bit value for modulo operation, not just low 64 bits
				Int128 remainder{ mantissa % Int128{ constants::DECIMAL_BASE } };
				digits[digitCount++] = static_cast<char>( '0' + remainder.toLow() );

				mantissa = mantissa / Int128{ constants::DECIMAL_BASE };
			}
		}
#endif

		if ( digitCount == 0 )
		{
			digitCount = 1;
			digits[0] = '0';
		}

		// Handle sign
		if ( isNegative() )
		{
			result.push_back( '-' );
		}

		// Apply decimal point formatting
		if ( currentScale > 0 )
		{
			if ( currentScale >= digitCount )
			{
				// Need leading zeros: "0.00123"
				result.push_back( '0' );
				result.push_back( '.' );

				// Add leading zeros
				for ( size_t i = 0; i < currentScale - digitCount; ++i )
				{
					result.push_back( '0' );
				}

				// Add digits in reverse order
				for ( size_t i = digitCount; i > 0; --i )
				{
					result.push_back( digits[i - 1] );
				}
			}
			else
			{
				// Add integer part (reverse order)
				for ( size_t i = digitCount; i > currentScale; --i )
				{
					result.push_back( digits[i - 1] );
				}

				result.push_back( '.' );

				// Add fractional part (reverse order)
				for ( size_t i = currentScale; i > 0; --i )
				{
					result.push_back( digits[i - 1] );
				}
			}
		}
		else
		{
			// No decimal point, just add digits in reverse order
			for ( size_t i = digitCount; i > 0; --i )
			{
				result.push_back( digits[i - 1] );
			}
		}

		return result;
	}

	std::array<std::int32_t, 4> Decimal::toBits() const noexcept
	{
		std::array<std::int32_t, 4> bits{};

		// First three elements are the 96-bit mantissa
		bits[0] = static_cast<std::int32_t>( m_layout.mantissa[0] );
		bits[1] = static_cast<std::int32_t>( m_layout.mantissa[1] );
		bits[2] = static_cast<std::int32_t>( m_layout.mantissa[2] );

		// Fourth element contains scale and sign information
		bits[3] = static_cast<std::int32_t>( m_layout.flags );

		return bits;
	}

	//----------------------------------------------
	// Mathematical operations
	//----------------------------------------------

	Decimal Decimal::truncate() const noexcept
	{
		return round( 0, RoundingMode::ToZero );
	}

	Decimal Decimal::floor() const noexcept
	{
		return round( 0, RoundingMode::ToNegativeInfinity );
	}

	Decimal Decimal::ceiling() const noexcept
	{
		return round( 0, RoundingMode::ToPositiveInfinity );
	}

	Decimal Decimal::round( std::int32_t decimalsPlacesCount, RoundingMode mode ) const noexcept
	{
		if ( decimalsPlacesCount < 0 )
		{
			decimalsPlacesCount = 0;
		}

		if ( decimalsPlacesCount >= static_cast<std::int32_t>( scale() ) || isZero() )
		{
			return *this;
		}

		Decimal result{ *this };
		std::uint8_t currentScale{ scale() };
		std::uint8_t targetScale{ static_cast<std::uint8_t>( decimalsPlacesCount ) };
		std::uint8_t digitsToRemove{ static_cast<std::uint8_t>( currentScale - targetScale ) };

		// Get the digit that determines rounding direction
		Int128 mantissa{ internal::mantissaAsInt128( *this ) };
		Int128 divisor{ 1 };
		if ( digitsToRemove > 1U )
		{
			std::uint8_t divisorPowers = static_cast<std::uint8_t>( digitsToRemove - 1U );
			for ( std::uint8_t i{ 0 }; i < divisorPowers; ++i )
			{
				divisor = divisor * Int128{ constants::DECIMAL_BASE };
			}
		}

		Int128 roundingDigit{ ( mantissa / divisor ) % Int128{ constants::DECIMAL_BASE } };

		// Perform truncation to target scale
		for ( std::uint8_t i = 0; i < digitsToRemove; ++i )
		{
			internal::divideByPowerOf10( result, 1U );
		}

		result.m_layout.flags =
			( result.m_layout.flags & ~constants::DECIMAL_SCALE_MASK ) |
			( static_cast<std::uint32_t>( targetScale ) << constants::DECIMAL_SCALE_SHIFT );

		// Determine if we should round up based on the rounding mode
		bool shouldRoundUp{ false };

		switch ( mode )
		{
			case RoundingMode::ToNearest:
			{
				shouldRoundUp = internal::shouldRoundUpToNearest( roundingDigit, mantissa, divisor, digitsToRemove, result );
				break;
			}
			case RoundingMode::ToNearestTiesAway:
			{
				shouldRoundUp = internal::shouldRoundUpToNearestTiesAway( roundingDigit );
				break;
			}
			case RoundingMode::ToZero:
			{
				shouldRoundUp = false; // Truncate (never round up)
				break;
			}
			case RoundingMode::ToPositiveInfinity:
			{
				shouldRoundUp = internal::shouldRoundUpToPositiveInfinity( mantissa, digitsToRemove, isNegative() );
				break;
			}
			case RoundingMode::ToNegativeInfinity:
			{
				shouldRoundUp = internal::shouldRoundUpToNegativeInfinity( mantissa, digitsToRemove, isNegative() );
				break;
			}
		}

		// Apply rounding adjustment
		if ( shouldRoundUp )
		{
			Int128 resultMantissa{ internal::mantissaAsInt128( result ) };
			if ( isNegative() )
			{
				// For negative numbers, "rounding up" means increasing the absolute value (magnitude)
				// Since mantissa is unsigned, we ADD to make the number more negative
				// Example: -123 → -124 means mantissa goes from 123 to 124
				resultMantissa = resultMantissa + Int128{ 1 };
			}
			else
			{
				// For positive numbers, rounding up means adding to the mantissa
				resultMantissa = resultMantissa + Int128{ 1 };
			}
			internal::setMantissa( result, resultMantissa );
		}

		return result;
	}

	//----------------------------------------------
	// Utilities
	//----------------------------------------------

	std::uint8_t Decimal::decimalPlacesCount() const noexcept
	{
		// If the value is zero, it has 0 decimal places
		if ( isZero() )
		{
			return 0;
		}

		// Get the current scale
		std::uint8_t currentScale = scale();

		// If scale is 0, it's an integer - no decimal places
		if ( currentScale == 0 )
		{
			return 0;
		}

		// Convert mantissa to Int128 for proper arithmetic
		const auto& mantissaArray = mantissa();
#if NFX_DATATYPES_HAS_NATIVE_INT128
		NFX_DATATYPES_NATIVE_INT128 mantissaValue{ static_cast<NFX_DATATYPES_NATIVE_INT128>( mantissaArray[2] ) << constants::BITS_PER_UINT64 |
												   static_cast<NFX_DATATYPES_NATIVE_INT128>( mantissaArray[1] ) << constants::BITS_PER_UINT32 |
												   static_cast<NFX_DATATYPES_NATIVE_INT128>( mantissaArray[0] ) };
		Int128 mantissa128{ mantissaValue };
#else
		std::uint64_t low{ static_cast<std::uint64_t>( mantissaArray[1] ) << constants::BITS_PER_UINT32 | mantissaArray[0] };
		std::uint64_t high{ mantissaArray[2] };
		Int128 mantissa128{ low, high };
#endif

		std::uint8_t trailingZeros = 0;
		Int128 ten{ constants::DECIMAL_BASE };

		// Count trailing zeros by testing divisibility by 10 iteratively
		while ( trailingZeros < currentScale )
		{
			// If there's a remainder, we can't divide evenly by 10
			if ( mantissa128 % ten != Int128{ 0 } )
			{
				break;
			}

			// Continue testing with the next power of 10
			mantissa128 = mantissa128 / ten;
			trailingZeros++;
		}

		return currentScale - trailingZeros;
	}

	//=====================================================================
	// Stream operators
	//=====================================================================

	std::ostream& operator<<( std::ostream& os, const Decimal& decimal )
	{
		// Check if std::fixed is set with specific precision
		if ( ( os.flags() & std::ios_base::fixed ) && os.precision() >= 0 )
		{
			// Format with minimum decimal places based on stream precision
			std::string str{ decimal.toString() };

			// Find decimal point
			size_t dot_pos{ str.find( '.' ) };

			if ( dot_pos == std::string::npos )
			{
				// No decimal point - add it with required precision
				str += '.';
				for ( std::streamsize i = 0; i < os.precision(); ++i )
				{
					str += '0';
				}
			}
			else
			{
				// Has decimal point - pad to required precision
				size_t current_decimals{ str.length() - dot_pos - 1 };
				if ( static_cast<std::streamsize>( current_decimals ) < os.precision() )
				{
					for ( std::streamsize i = static_cast<std::streamsize>( current_decimals ); i < os.precision(); ++i )
					{
						str += '0';
					}
				}
			}

			return os << str;
		}

		// Default behavior - use toString() as-is
		return os << decimal.toString();
	}

	std::istream& operator>>( std::istream& is, Decimal& decimal )
	{
		std::string str;
		is >> str;

		if ( !Decimal::tryParse( str, decimal ) )
		{
			is.setstate( std::ios::failbit );
		}

		return is;
	}
} // namespace nfx::datatypes
