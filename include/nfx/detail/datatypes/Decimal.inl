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
 * @file Decimal.inl
 * @brief Inline implementations for cross-platform Decimal class
 */

namespace nfx::datatypes
{
	//=====================================================================
	// Decimal class
	//=====================================================================

	//----------------------------------------------
	// Construction
	//----------------------------------------------

	inline constexpr Decimal::Decimal() noexcept
		: m_layout{ 0, { { 0, 0, 0 } } }
	{
	}

	inline Decimal::Decimal( float value ) noexcept
		: Decimal( static_cast<double>( value ) )
	{
	}

	//----------------------------------------------
	// Decimal constants
	//----------------------------------------------

	inline constexpr Decimal Decimal::zero() noexcept
	{
		return Decimal{};
	}

	inline constexpr Decimal Decimal::one() noexcept
	{
		Decimal result{};
		result.m_layout.mantissa[0] = 1;

		return result;
	}

	//----------------------------------------------
	// Static mathematical operations
	//----------------------------------------------

	inline Decimal Decimal::truncate( const Decimal& value ) noexcept
	{
		return value.truncate();
	}

	inline Decimal Decimal::floor( Decimal& value ) noexcept
	{
		return value.floor();
	}

	inline Decimal Decimal::ceiling( Decimal& value ) noexcept
	{
		return value.ceiling();
	}

	inline Decimal Decimal::round( Decimal& value, std::int32_t decimalsPlacesCount, RoundingMode mode ) noexcept
	{
		return value.round( decimalsPlacesCount, mode );
	}

	inline Decimal Decimal::abs( const Decimal& value ) noexcept
	{
		return value.abs();
	}

	//----------------------------------------------
	// Arithmetic operators
	//----------------------------------------------

	inline Decimal& Decimal::operator+=( const Decimal& other )
	{
		*this = *this + other;
		return *this;
	}

	inline Decimal& Decimal::operator-=( const Decimal& other )
	{
		*this = *this - other;
		return *this;
	}

	inline Decimal& Decimal::operator*=( const Decimal& other )
	{
		*this = *this * other;
		return *this;
	}

	inline Decimal& Decimal::operator/=( const Decimal& other )
	{
		*this = *this / other;
		return *this;
	}

	//----------------------------------------------
	// Comparison operators
	//----------------------------------------------

	inline bool Decimal::operator!=( const Decimal& other ) const noexcept
	{
		return !( *this == other );
	}

	inline bool Decimal::operator<=( const Decimal& other ) const noexcept
	{
		return *this < other || *this == other;
	}

	inline bool Decimal::operator>( const Decimal& other ) const noexcept
	{
		return !( *this <= other );
	}

	inline bool Decimal::operator>=( const Decimal& other ) const noexcept
	{
		return !( *this < other );
	}

	//----------------------------------------------
	// Comparison with built-in floating point types
	//----------------------------------------------

	inline bool Decimal::operator==( float val ) const noexcept
	{
		return *this == static_cast<double>( val );
	}

	inline bool Decimal::operator!=( float val ) const noexcept
	{
		return *this != static_cast<double>( val );
	}

	inline bool Decimal::operator<( float val ) const noexcept
	{
		return *this < static_cast<double>( val );
	}

	inline bool Decimal::operator<=( float val ) const noexcept
	{
		return *this <= static_cast<double>( val );
	}

	inline bool Decimal::operator>( float val ) const noexcept
	{
		return *this > static_cast<double>( val );
	}

	inline bool Decimal::operator>=( float val ) const noexcept
	{
		return *this >= static_cast<double>( val );
	}

	inline bool Decimal::operator==( double val ) const noexcept
	{
		if ( std::isnan( val ) || std::isinf( val ) )
		{
			return false; // Decimal has no NaN/Infinity representation
		}

		// Convert double to Decimal for comparison
		Decimal temp{ val };
		return *this == temp;
	}

	inline bool Decimal::operator!=( double val ) const noexcept
	{
		return !( *this == val );
	}

	inline bool Decimal::operator<( double val ) const noexcept
	{
		if ( std::isnan( val ) )
		{
			return false; // No ordering with NaN
		}
		if ( std::isinf( val ) )
		{
			return val > 0.0; // Any finite value < +infinity, any finite value > -infinity
		}

		Decimal temp{ val };
		return *this < temp;
	}

	inline bool Decimal::operator<=( double val ) const noexcept
	{
		return *this < val || *this == val;
	}

	inline bool Decimal::operator>( double val ) const noexcept
	{
		if ( std::isnan( val ) )
		{
			return false; // No ordering with NaN
		}
		if ( std::isinf( val ) )
		{
			return val < 0.0; // Any finite value > -infinity, any finite value < +infinity
		}

		Decimal temp{ val };
		return *this > temp;
	}

	inline bool Decimal::operator>=( double val ) const noexcept
	{
		return *this > val || *this == val;
	}

	//----------------------------------------------
	// Comparison with built-in integer types
	//----------------------------------------------

	inline bool Decimal::operator==( std::int64_t val ) const noexcept
	{
		// For integer comparison, we need exact equality
		if ( scale() > 0 )
		{
			// If this has fractional part, it can't equal an integer
			return false;
		}

		Decimal temp{ val };
		return *this == temp;
	}

	inline bool Decimal::operator!=( std::int64_t val ) const noexcept
	{
		return !( *this == val );
	}

	inline bool Decimal::operator<( std::int64_t val ) const noexcept
	{
		Decimal temp{ val };
		return *this < temp;
	}

	inline bool Decimal::operator<=( std::int64_t val ) const noexcept
	{
		return *this < val || *this == val;
	}

	inline bool Decimal::operator>( std::int64_t val ) const noexcept
	{
		Decimal temp{ val };
		return *this > temp;
	}

	inline bool Decimal::operator>=( std::int64_t val ) const noexcept
	{
		return *this > val || *this == val;
	}

	inline bool Decimal::operator==( std::uint64_t val ) const noexcept
	{
		// For integer comparison, we need exact equality
		if ( scale() > 0 )
		{
			// If this has fractional part, it can't equal an integer
			return false;
		}

		// Also check if this is negative (can't equal positive uint64_t)
		if ( isNegative() )
		{
			return false;
		}

		Decimal temp{ val };
		return *this == temp;
	}

	inline bool Decimal::operator!=( std::uint64_t val ) const noexcept
	{
		return !( *this == val );
	}

	inline bool Decimal::operator<( std::uint64_t val ) const noexcept
	{
		// Negative Decimal is always less than positive uint64_t
		if ( isNegative() )
		{
			return true;
		}

		Decimal temp{ val };
		return *this < temp;
	}

	inline bool Decimal::operator<=( std::uint64_t val ) const noexcept
	{
		return *this < val || *this == val;
	}

	inline bool Decimal::operator>( std::uint64_t val ) const noexcept
	{
		// Negative Decimal is never greater than positive uint64_t
		if ( isNegative() )
		{
			return false;
		}

		Decimal temp{ val };
		return *this > temp;
	}

	inline bool Decimal::operator>=( std::uint64_t val ) const noexcept
	{
		return *this > val || *this == val;
	}

	inline bool Decimal::operator==( std::int32_t val ) const noexcept
	{
		return *this == static_cast<std::int64_t>( val );
	}

	inline bool Decimal::operator!=( std::int32_t val ) const noexcept
	{
		return *this != static_cast<std::int64_t>( val );
	}

	inline bool Decimal::operator<( std::int32_t val ) const noexcept
	{
		return *this < static_cast<std::int64_t>( val );
	}

	inline bool Decimal::operator<=( std::int32_t val ) const noexcept
	{
		return *this <= static_cast<std::int64_t>( val );
	}

	inline bool Decimal::operator>( std::int32_t val ) const noexcept
	{
		return *this > static_cast<std::int64_t>( val );
	}

	inline bool Decimal::operator>=( std::int32_t val ) const noexcept
	{
		return *this >= static_cast<std::int64_t>( val );
	}

	//----------------------------------------------
	// Comparison with nfx Int128
	//----------------------------------------------

	inline bool Decimal::operator!=( const Int128& val ) const noexcept
	{
		return !( *this == val );
	}

	inline bool Decimal::operator<=( const Int128& val ) const noexcept
	{
		return *this < val || *this == val;
	}

	inline bool Decimal::operator>( const Int128& val ) const noexcept
	{
		return !( *this <= val );
	}

	inline bool Decimal::operator>=( const Int128& val ) const noexcept
	{
		return !( *this < val );
	}

	//----------------------------------------------
	// Property accessors
	//----------------------------------------------

	inline const std::uint32_t& Decimal::flags() const noexcept
	{
		return m_layout.flags;
	}

	inline std::uint32_t& Decimal::flags() noexcept
	{
		return m_layout.flags;
	}

	inline const std::array<std::uint32_t, 3>& Decimal::mantissa() const noexcept
	{
		return m_layout.mantissa;
	}

	inline std::array<std::uint32_t, 3>& Decimal::mantissa() noexcept
	{
		return m_layout.mantissa;
	}

	//----------------------------------------------
	// State checking
	//----------------------------------------------

	inline bool Decimal::isZero() const noexcept
	{
		return m_layout.mantissa[0] == 0 && m_layout.mantissa[1] == 0 && m_layout.mantissa[2] == 0;
	}

	//----------------------------------------------
	// Mathematical operations
	//----------------------------------------------

	inline Decimal Decimal::abs() const noexcept
	{
		if ( isNegative() )
		{
			return -*this;
		}

		return *this;
	}
} // namespace nfx::datatypes
