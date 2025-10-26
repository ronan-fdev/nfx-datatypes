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
 * @file Int128.inl
 * @brief Inline implementations for cross-platform Int128 class
 */

#include <cmath>
#include <stdexcept>
#include <string_view>

namespace nfx::datatypes
{
	//=====================================================================
	// Int128 class
	//=====================================================================

#if NFX_DATATYPES_HAS_NATIVE_INT128

	//----------------------------------------------
	// Construction
	//----------------------------------------------

	inline constexpr Int128::Int128() noexcept
		: m_value{ 0 }
	{
	}

	inline constexpr Int128::Int128( int val ) noexcept
		: m_value{ val }
	{
	}

	inline constexpr Int128::Int128( std::int64_t val ) noexcept
		: m_value{ val }
	{
	}

	inline constexpr Int128::Int128( std::uint32_t val ) noexcept
		: m_value{ val }
	{
	}

	inline constexpr Int128::Int128( std::uint64_t val ) noexcept
		: m_value{ val }
	{
	}

	inline constexpr Int128::Int128( NFX_DATATYPES_NATIVE_INT128 val ) noexcept
		: m_value{ val }
	{
	}

	inline Int128::Int128( std::string_view str )
	{
		if ( !tryParse( str, *this ) )
		{
			throw std::invalid_argument{ "Invalid Int128 string format" };
		}
	}

	//----------------------------------------------
	// Arithmetic operations
	//----------------------------------------------

	inline Int128 Int128::operator%( const Int128& other ) const
	{
		if ( other.m_value == 0 )
		{
			throw std::overflow_error{ "Division by zero" };
		}

		return Int128{ m_value % other.m_value };
	}

	inline Int128 Int128::operator-() const noexcept
	{
		return Int128{ -m_value };
	}

	//----------------------------------------------
	// Comparison operations
	//----------------------------------------------

	inline bool Int128::operator==( const Int128& other ) const noexcept
	{
		return m_value == other.m_value;
	}

	inline bool Int128::operator!=( const Int128& other ) const noexcept
	{
		return m_value != other.m_value;
	}

	inline bool Int128::operator<( const Int128& other ) const noexcept
	{
		return m_value < other.m_value;
	}

	inline bool Int128::operator<=( const Int128& other ) const noexcept
	{
		return m_value <= other.m_value;
	}

	inline bool Int128::operator>( const Int128& other ) const noexcept
	{
		return m_value > other.m_value;
	}

	inline bool Int128::operator>=( const Int128& other ) const noexcept
	{
		return m_value >= other.m_value;
	}

	//----------------------------------------------
	// Comparison with built-in integer types
	//----------------------------------------------

	inline bool Int128::operator==( int val ) const noexcept
	{
		return m_value == val;
	}

	inline bool Int128::operator!=( int val ) const noexcept
	{
		return m_value != val;
	}

	inline bool Int128::operator<( int val ) const noexcept
	{
		return m_value < val;
	}

	inline bool Int128::operator<=( int val ) const noexcept
	{
		return m_value <= val;
	}

	inline bool Int128::operator>( int val ) const noexcept
	{
		return m_value > val;
	}

	inline bool Int128::operator>=( int val ) const noexcept
	{
		return m_value >= val;
	}

	inline bool Int128::operator==( std::int64_t val ) const noexcept
	{
		return m_value == val;
	}

	inline bool Int128::operator!=( std::int64_t val ) const noexcept
	{
		return m_value != val;
	}

	inline bool Int128::operator<( std::int64_t val ) const noexcept
	{
		return m_value < val;
	}

	inline bool Int128::operator<=( std::int64_t val ) const noexcept
	{
		return m_value <= val;
	}

	inline bool Int128::operator>( std::int64_t val ) const noexcept
	{
		return m_value > val;
	}

	inline bool Int128::operator>=( std::int64_t val ) const noexcept
	{
		return m_value >= val;
	}

	inline bool Int128::operator==( std::uint64_t val ) const noexcept
	{
		return m_value >= 0 && static_cast<std::uint64_t>( m_value ) == val;
	}

	inline bool Int128::operator!=( std::uint64_t val ) const noexcept
	{
		return m_value < 0 || static_cast<std::uint64_t>( m_value ) != val;
	}

	inline bool Int128::operator<( std::uint64_t val ) const noexcept
	{
		return m_value < 0 || static_cast<std::uint64_t>( m_value ) < val;
	}

	inline bool Int128::operator<=( std::uint64_t val ) const noexcept
	{
		return m_value < 0 || static_cast<std::uint64_t>( m_value ) <= val;
	}

	inline bool Int128::operator>( std::uint64_t val ) const noexcept
	{
		return m_value >= 0 && static_cast<std::uint64_t>( m_value ) > val;
	}

	inline bool Int128::operator>=( std::uint64_t val ) const noexcept
	{
		return m_value >= 0 && static_cast<std::uint64_t>( m_value ) >= val;
	}

	//----------------------------------------------
	// Comparison with built-in floating point types
	//----------------------------------------------

	inline bool Int128::operator==( float val ) const noexcept
	{
		return *this == static_cast<double>( val );
	}

	inline bool Int128::operator!=( float val ) const noexcept
	{
		return *this != static_cast<double>( val );
	}

	inline bool Int128::operator<( float val ) const noexcept
	{
		return *this < static_cast<double>( val );
	}

	inline bool Int128::operator<=( float val ) const noexcept
	{
		return *this <= static_cast<double>( val );
	}

	inline bool Int128::operator>( float val ) const noexcept
	{
		return *this > static_cast<double>( val );
	}

	inline bool Int128::operator>=( float val ) const noexcept
	{
		return *this >= static_cast<double>( val );
	}

	inline bool Int128::operator!=( double val ) const noexcept
	{
		return !( *this == val );
	}

	inline bool Int128::operator<=( double val ) const noexcept
	{
		return *this < val || *this == val;
	}

	inline bool Int128::operator>=( double val ) const noexcept
	{
		return *this > val || *this == val;
	}

	//----------------------------------------------
	// State checking
	//----------------------------------------------

	inline bool Int128::isZero() const noexcept
	{
		return m_value == 0;
	}

	inline bool Int128::isNegative() const noexcept
	{
		return m_value < 0;
	}

	//----------------------------------------------
	// Mathematical operations
	//----------------------------------------------

	inline Int128 Int128::abs() const noexcept
	{
		return Int128{ m_value < 0
						   ? -m_value
						   : m_value };
	}

#else

	//----------------------------------------------
	// Construction
	//----------------------------------------------

	inline constexpr Int128::Int128() noexcept
		: m_layout{ 0, 0 }
	{
	}

	inline constexpr Int128::Int128( int val ) noexcept
		: m_layout{ static_cast<std::uint64_t>( val ),
			  ( val < 0 ) ? static_cast<std::uint64_t>( -1 )
						  : 0 }
	{
	}

	inline constexpr Int128::Int128( std::int64_t val ) noexcept
		: m_layout{ static_cast<std::uint64_t>( val ),
			  ( val < 0 ) ? static_cast<std::uint64_t>( -1 )
						  : 0 }
	{
	}

	inline constexpr Int128::Int128( std::uint32_t val ) noexcept
		: m_layout{ val, 0 }
	{
	}

	inline constexpr Int128::Int128( std::uint64_t val ) noexcept
		: m_layout{ val, 0 }
	{
	}

	inline Int128::Int128( std::string_view str )
	{
		if ( !tryParse( str, *this ) )
		{
			throw std::invalid_argument{ "Invalid Int128 string format" };
		}
	}

#endif

	//----------------------------------------------
	// Comparison with nfx Decimal
	//----------------------------------------------

	inline bool Int128::operator!=( const Decimal& val ) const noexcept
	{
		return !( *this == val );
	}

	inline bool Int128::operator<=( const Decimal& val ) const noexcept
	{
		return *this < val || *this == val;
	}

	inline bool Int128::operator>( const Decimal& val ) const noexcept
	{
		return !( *this <= val );
	}

	inline bool Int128::operator>=( const Decimal& val ) const noexcept
	{
		return !( *this < val );
	}

	//----------------------------------------------
	// Arithmetic operations
	//----------------------------------------------

#if NFX_DATATYPES_HAS_NATIVE_INT128

	inline Int128& Int128::operator+=( const Int128& other ) noexcept
	{
		m_value += other.m_value;
		return *this;
	}

	inline Int128& Int128::operator-=( const Int128& other ) noexcept
	{
		m_value -= other.m_value;
		return *this;
	}

	inline Int128& Int128::operator*=( const Int128& other ) noexcept
	{
		m_value *= other.m_value;
		return *this;
	}

	inline Int128& Int128::operator/=( const Int128& other )
	{
		if ( other.m_value == 0 )
		{
			throw std::overflow_error{ "Division by zero" };
		}
		m_value /= other.m_value;
		return *this;
	}

	inline Int128& Int128::operator%=( const Int128& other )
	{
		if ( other.m_value == 0 )
		{
			throw std::overflow_error{ "Division by zero" };
		}
		m_value %= other.m_value;
		return *this;
	}
#else
	inline Int128& Int128::operator+=( const Int128& other ) noexcept
	{
		*this = *this + other;
		return *this;
	}

	inline Int128& Int128::operator-=( const Int128& other ) noexcept
	{
		*this = *this - other;
		return *this;
	}

	inline Int128& Int128::operator*=( const Int128& other ) noexcept
	{
		*this = *this * other;
		return *this;
	}

	inline Int128& Int128::operator/=( const Int128& other )
	{
		*this = *this / other; // Division operator handles zero check
		return *this;
	}

	inline Int128& Int128::operator%=( const Int128& other )
	{
		if ( other.isZero() )
		{
			throw std::overflow_error{ "Division by zero" };
		}
		*this = *this % other;
		return *this;
	}

	inline Int128 Int128::operator%( const Int128& other ) const
	{
		if ( other.isZero() )
		{
			throw std::overflow_error{ "Division by zero" };
		}

		// Performance optimization: Fast modulo for 64-bit values
		if ( m_layout.upper64bits == 0 && other.m_layout.upper64bits == 0 )
		{
			// Both fit in 64-bit - use native modulo
			return Int128{ m_layout.lower64bits % other.m_layout.lower64bits, 0 };
		}

		Int128 quotient{ *this / other };

		return *this - ( quotient * other );
	}

	inline Int128 Int128::operator-() const noexcept
	{
		// Two's complement negation
		Int128 result{ Int128{ ~m_layout.lower64bits, ~m_layout.upper64bits } };

		return Int128{ result + Int128{ 1 } };
	}

	//----------------------------------------------
	// Comparison operations
	//----------------------------------------------

	inline bool Int128::operator==( const Int128& other ) const noexcept
	{
		return m_layout.lower64bits == other.m_layout.lower64bits && m_layout.upper64bits == other.m_layout.upper64bits;
	}

	inline bool Int128::operator!=( const Int128& other ) const noexcept
	{
		return m_layout.lower64bits != other.m_layout.lower64bits || m_layout.upper64bits != other.m_layout.upper64bits;
	}

	inline bool Int128::operator<( const Int128& other ) const noexcept
	{
		if ( m_layout.upper64bits != other.m_layout.upper64bits )
		{
			return static_cast<std::int64_t>( m_layout.upper64bits ) < static_cast<std::int64_t>( other.m_layout.upper64bits );
		}
		return m_layout.lower64bits < other.m_layout.lower64bits;
	}

	inline bool Int128::operator<=( const Int128& other ) const noexcept
	{
		if ( m_layout.upper64bits != other.m_layout.upper64bits )
		{
			return static_cast<std::int64_t>( m_layout.upper64bits ) < static_cast<std::int64_t>( other.m_layout.upper64bits );
		}
		return m_layout.lower64bits <= other.m_layout.lower64bits;
	}

	inline bool Int128::operator>( const Int128& other ) const noexcept
	{
		if ( m_layout.upper64bits != other.m_layout.upper64bits )
		{
			return static_cast<std::int64_t>( m_layout.upper64bits ) > static_cast<std::int64_t>( other.m_layout.upper64bits );
		}
		return m_layout.lower64bits > other.m_layout.lower64bits;
	}

	inline bool Int128::operator>=( const Int128& other ) const noexcept
	{
		if ( m_layout.upper64bits != other.m_layout.upper64bits )
		{
			return static_cast<std::int64_t>( m_layout.upper64bits ) > static_cast<std::int64_t>( other.m_layout.upper64bits );
		}
		return m_layout.lower64bits >= other.m_layout.lower64bits;
	}

	//----------------------------------------------
	// Comparison with built-in integer types
	//----------------------------------------------

	inline bool Int128::operator==( int val ) const noexcept
	{
		return *this == static_cast<std::int64_t>( val );
	}

	inline bool Int128::operator!=( int val ) const noexcept
	{
		return *this != static_cast<std::int64_t>( val );
	}

	inline bool Int128::operator<( int val ) const noexcept
	{
		return *this < static_cast<std::int64_t>( val );
	}

	inline bool Int128::operator<=( int val ) const noexcept
	{
		return *this <= static_cast<std::int64_t>( val );
	}

	inline bool Int128::operator>( int val ) const noexcept
	{
		return *this > static_cast<std::int64_t>( val );
	}

	inline bool Int128::operator>=( int val ) const noexcept
	{
		return *this >= static_cast<std::int64_t>( val );
	}

	inline bool Int128::operator==( std::int64_t val ) const noexcept
	{
		// For negative values, upper64bits should be all 1s (sign extension)
		// For positive values, upper64bits should be 0
		std::uint64_t expected_upper = ( val < 0 ) ? static_cast<std::uint64_t>( -1 ) : 0;

		return m_layout.upper64bits == expected_upper &&
			   m_layout.lower64bits == static_cast<std::uint64_t>( val );
	}

	inline bool Int128::operator!=( std::int64_t val ) const noexcept
	{
		return !( *this == val );
	}

	inline bool Int128::operator<( std::int64_t val ) const noexcept
	{
		return *this < Int128{ val };
	}

	inline bool Int128::operator<=( std::int64_t val ) const noexcept
	{
		return *this <= Int128{ val };
	}

	inline bool Int128::operator>( std::int64_t val ) const noexcept
	{
		return *this > Int128{ val };
	}

	inline bool Int128::operator>=( std::int64_t val ) const noexcept
	{
		return *this >= Int128{ val };
	}

	inline bool Int128::operator==( std::uint64_t val ) const noexcept
	{
		// For unsigned comparison, this Int128 must be non-negative
		return m_layout.upper64bits == 0 && m_layout.lower64bits == val;
	}

	inline bool Int128::operator!=( std::uint64_t val ) const noexcept
	{
		return !( *this == val );
	}

	inline bool Int128::operator<( std::uint64_t val ) const noexcept
	{
		// If this is negative, it's always less than any positive uint64_t
		if ( isNegative() )
		{
			return true;
		}

		// If upper bits are non-zero, this is definitely >= 2^64, so greater than any uint64_t
		if ( m_layout.upper64bits != 0 )
		{
			return false;
		}

		// Compare lower bits
		return m_layout.lower64bits < val;
	}

	inline bool Int128::operator<=( std::uint64_t val ) const noexcept
	{
		// If this is negative, it's always less than any positive uint64_t
		if ( isNegative() )
		{
			return true;
		}

		// If upper bits are non-zero, this is definitely >= 2^64, so greater than any uint64_t
		if ( m_layout.upper64bits != 0 )
		{
			return false;
		}

		// Compare lower bits
		return m_layout.lower64bits <= val;
	}

	inline bool Int128::operator>( std::uint64_t val ) const noexcept
	{
		// If this is negative, it's never greater than any positive uint64_t
		if ( isNegative() )
		{
			return false;
		}

		// If upper bits are non-zero, this is definitely >= 2^64, so greater than any uint64_t
		if ( m_layout.upper64bits != 0 )
		{
			return true;
		}

		// Compare lower bits
		return m_layout.lower64bits > val;
	}

	inline bool Int128::operator>=( std::uint64_t val ) const noexcept
	{
		// If this is negative, it's never greater than or equal to any positive uint64_t
		if ( isNegative() )
		{
			return false;
		}

		// If upper bits are non-zero, this is definitely >= 2^64, so greater than any uint64_t
		if ( m_layout.upper64bits != 0 )
		{
			return true;
		}
		// Compare lower bits
		return m_layout.lower64bits >= val;
	}

	//----------------------------------------------
	// Comparison with built-in floating point types
	//----------------------------------------------

	inline bool Int128::operator==( float val ) const noexcept
	{
		return *this == static_cast<double>( val );
	}

	inline bool Int128::operator!=( float val ) const noexcept
	{
		return *this != static_cast<double>( val );
	}

	inline bool Int128::operator<( float val ) const noexcept
	{
		return *this < static_cast<double>( val );
	}

	inline bool Int128::operator<=( float val ) const noexcept
	{
		return *this <= static_cast<double>( val );
	}

	inline bool Int128::operator>( float val ) const noexcept
	{
		return *this > static_cast<double>( val );
	}

	inline bool Int128::operator>=( float val ) const noexcept
	{
		return *this >= static_cast<double>( val );
	}

	inline bool Int128::operator!=( double val ) const noexcept
	{
		return !( *this == val );
	}

	inline bool Int128::operator<=( double val ) const noexcept
	{
		return *this < val || *this == val;
	}

	inline bool Int128::operator>=( double val ) const noexcept
	{
		return *this > val || *this == val;
	}

	//----------------------------------------------
	// State checking
	//----------------------------------------------

	inline bool Int128::isZero() const noexcept
	{
		return m_layout.lower64bits == 0 && m_layout.upper64bits == 0;
	}

	inline bool Int128::isNegative() const noexcept
	{
		return static_cast<std::int64_t>( m_layout.upper64bits ) < 0;
	}

	//----------------------------------------------
	// Mathematical operations
	//----------------------------------------------

	inline Int128 Int128::abs() const noexcept
	{
		if ( !isNegative() )
		{
			return *this;
		}
		return -*this;
	}
#endif
} // namespace nfx::datatypes
