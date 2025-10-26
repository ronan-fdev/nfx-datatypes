/**
 * @file BM_Decimal.cpp
 * @brief Benchmark Decimal construction, arithmetic, parsing, formatting, and comparison operations
 */

#include <benchmark/benchmark.h>

#include <nfx/datatypes/Decimal.h>
#include <nfx/datatypes/Int128.h>

namespace nfx::datatypes::benchmark
{
	//=====================================================================
	// Decimal benchmark suite
	//=====================================================================

	//----------------------------------------------
	// Construction
	//----------------------------------------------

	static void BM_DecimalConstructDefault( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			Decimal value{};
			::benchmark::DoNotOptimize( value );
		}
	}

	static void BM_DecimalConstructFromInt32( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			Decimal value{ static_cast<std::int32_t>( 42 ) };
			::benchmark::DoNotOptimize( value );
		}
	}

	static void BM_DecimalConstructFromInt64( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			Decimal value{ static_cast<std::int64_t>( 1234567890123456789LL ) };
			::benchmark::DoNotOptimize( value );
		}
	}

	static void BM_DecimalConstructFromUint32( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			Decimal value{ static_cast<std::uint32_t>( 4294967295U ) };
			::benchmark::DoNotOptimize( value );
		}
	}

	static void BM_DecimalConstructFromUint64( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			Decimal value{ static_cast<std::uint64_t>( 9876543210987654321ULL ) };
			::benchmark::DoNotOptimize( value );
		}
	}

	static void BM_DecimalConstructFromFloat( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			Decimal value{ 123.456f };
			::benchmark::DoNotOptimize( value );
		}
	}

	static void BM_DecimalConstructFromDouble( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			Decimal value{ 123456.789012345 };
			::benchmark::DoNotOptimize( value );
		}
	}

	static void BM_DecimalConstructFromInt128( ::benchmark::State& state )
	{
		Int128 int128Value{ static_cast<std::int64_t>( 1234567890123456789LL ) };
		for ( auto _ : state )
		{
			Decimal value{ int128Value };
			::benchmark::DoNotOptimize( value );
		}
	}

	static void BM_DecimalCopyConstruct( ::benchmark::State& state )
	{
		Decimal original{ 123456.789 };
		for ( auto _ : state )
		{
			Decimal value{ original };
			::benchmark::DoNotOptimize( value );
		}
	}

	//----------------------------------------------
	// Arithmetic operators
	//----------------------------------------------

	static void BM_DecimalAddition( ::benchmark::State& state )
	{
		Decimal a{ 123456.789 };
		Decimal b{ 987654.321 };
		for ( auto _ : state )
		{
			Decimal result{ a + b };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalSubtraction( ::benchmark::State& state )
	{
		Decimal a{ 987654.321 };
		Decimal b{ 123456.789 };
		for ( auto _ : state )
		{
			Decimal result{ a - b };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalMultiplication( ::benchmark::State& state )
	{
		Decimal a{ 123.456 };
		Decimal b{ 789.012 };
		for ( auto _ : state )
		{
			Decimal result{ a * b };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalMultiplicationLarge( ::benchmark::State& state )
	{
		Decimal a{ 123456789012345.678 };
		Decimal b{ 987654321098765.432 };
		for ( auto _ : state )
		{
			Decimal result{ a * b };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalDivision( ::benchmark::State& state )
	{
		Decimal a{ 987654.321 };
		Decimal b{ 123.456 };
		for ( auto _ : state )
		{
			Decimal result{ a / b };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalDivisionHighPrecision( ::benchmark::State& state )
	{
		Decimal a{ 1.0 };
		Decimal b{ 3.0 };
		for ( auto _ : state )
		{
			Decimal result{ a / b };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalUnaryMinus( ::benchmark::State& state )
	{
		Decimal value{ 123456.789 };
		for ( auto _ : state )
		{
			Decimal result{ -value };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalAdditionAssignment( ::benchmark::State& state )
	{
		Decimal a{ 123456.789 };
		Decimal b{ 987.654 };
		for ( auto _ : state )
		{
			Decimal temp{ a };
			temp += b;
			::benchmark::DoNotOptimize( temp );
		}
	}

	static void BM_DecimalSubtractionAssignment( ::benchmark::State& state )
	{
		Decimal a{ 987654.321 };
		Decimal b{ 123.456 };
		for ( auto _ : state )
		{
			Decimal temp{ a };
			temp -= b;
			::benchmark::DoNotOptimize( temp );
		}
	}

	static void BM_DecimalMultiplicationAssignment( ::benchmark::State& state )
	{
		Decimal a{ 123.456 };
		Decimal b{ 789.012 };
		for ( auto _ : state )
		{
			Decimal temp{ a };
			temp *= b;
			::benchmark::DoNotOptimize( temp );
		}
	}

	static void BM_DecimalDivisionAssignment( ::benchmark::State& state )
	{
		Decimal a{ 987654.321 };
		Decimal b{ 123.456 };
		for ( auto _ : state )
		{
			Decimal temp{ a };
			temp /= b;
			::benchmark::DoNotOptimize( temp );
		}
	}

	//----------------------------------------------
	// Parsing
	//----------------------------------------------

	static void BM_DecimalParseInteger( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			Decimal result{ Decimal::parse( "12345" ) };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalParseSmallDecimal( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			Decimal result{ Decimal::parse( "123.456" ) };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalParseLargeDecimal( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			Decimal result{ Decimal::parse( "123456789012345678901234.567890" ) };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalParseHighPrecision( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			Decimal result{ Decimal::parse( "0.1234567890123456789012345678" ) };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalParseNegative( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			Decimal result{ Decimal::parse( "-987654321.123456789" ) };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalTryParseValid( ::benchmark::State& state )
	{
		Decimal result{};
		for ( auto _ : state )
		{
			bool success{ Decimal::tryParse( "123456.789", result ) };
			::benchmark::DoNotOptimize( success );
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalTryParseInvalid( ::benchmark::State& state )
	{
		Decimal result{};
		for ( auto _ : state )
		{
			bool success{ Decimal::tryParse( "not_a_decimal", result ) };
			::benchmark::DoNotOptimize( success );
			::benchmark::DoNotOptimize( result );
		}
	}

	//----------------------------------------------
	// Conversion
	//----------------------------------------------

	static void BM_DecimalToDouble( ::benchmark::State& state )
	{
		Decimal value{ 123456.789 };
		for ( auto _ : state )
		{
			double result{ value.toDouble() };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalToBits( ::benchmark::State& state )
	{
		Decimal value{ 123456.789 };
		for ( auto _ : state )
		{
			auto result{ value.toBits() };
			::benchmark::DoNotOptimize( result );
		}
	}

	//----------------------------------------------
	// Formatting
	//----------------------------------------------

	static void BM_DecimalToStringInteger( ::benchmark::State& state )
	{
		Decimal value{ static_cast<std::int32_t>( 12345 ) };
		for ( auto _ : state )
		{
			std::string result{ value.toString() };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalToStringSmall( ::benchmark::State& state )
	{
		Decimal value{ 123.456 };
		for ( auto _ : state )
		{
			std::string result{ value.toString() };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalToStringLarge( ::benchmark::State& state )
	{
		Decimal value{ 123456789012345678.901234567 };
		for ( auto _ : state )
		{
			std::string result{ value.toString() };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalToStringNegative( ::benchmark::State& state )
	{
		Decimal value{ -987654321.123456789 };
		for ( auto _ : state )
		{
			std::string result{ value.toString() };
			::benchmark::DoNotOptimize( result );
		}
	}

	//----------------------------------------------
	// Comparison
	//----------------------------------------------

	static void BM_DecimalEqualityDecimal( ::benchmark::State& state )
	{
		Decimal a{ 123456.789 };
		Decimal b{ 123456.789 };
		for ( auto _ : state )
		{
			bool result{ a == b };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalLessThanDecimal( ::benchmark::State& state )
	{
		Decimal a{ 123456.789 };
		Decimal b{ 987654.321 };
		for ( auto _ : state )
		{
			bool result{ a < b };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalGreaterThanDecimal( ::benchmark::State& state )
	{
		Decimal a{ 987654.321 };
		Decimal b{ 123456.789 };
		for ( auto _ : state )
		{
			bool result{ a > b };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalEqualityInt32( ::benchmark::State& state )
	{
		Decimal a{ static_cast<std::int32_t>( 12345 ) };
		std::int32_t b{ 12345 };
		for ( auto _ : state )
		{
			bool result{ a == b };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalEqualityInt64( ::benchmark::State& state )
	{
		Decimal a{ static_cast<std::int64_t>( 1234567890123456789LL ) };
		std::int64_t b{ 1234567890123456789LL };
		for ( auto _ : state )
		{
			bool result{ a == b };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalEqualityUint64( ::benchmark::State& state )
	{
		Decimal a{ static_cast<std::uint64_t>( 9876543210987654321ULL ) };
		std::uint64_t b{ 9876543210987654321ULL };
		for ( auto _ : state )
		{
			bool result{ a == b };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalEqualityDouble( ::benchmark::State& state )
	{
		Decimal a{ 123456.789 };
		double b{ 123456.789 };
		for ( auto _ : state )
		{
			bool result{ a == b };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalLessThanDouble( ::benchmark::State& state )
	{
		Decimal a{ 123456.789 };
		double b{ 987654.321 };
		for ( auto _ : state )
		{
			bool result{ a < b };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalEqualityFloat( ::benchmark::State& state )
	{
		Decimal a{ 123.456f };
		float b{ 123.456f };
		for ( auto _ : state )
		{
			bool result{ a == b };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalEqualityInt128( ::benchmark::State& state )
	{
		Decimal a{ static_cast<std::int64_t>( 123456789012345LL ) };
		Int128 b{ static_cast<std::int64_t>( 123456789012345LL ) };
		for ( auto _ : state )
		{
			bool result{ a == b };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalLessThanInt128( ::benchmark::State& state )
	{
		Decimal a{ static_cast<std::int64_t>( 123456789012345LL ) };
		Int128 b{ static_cast<std::int64_t>( 987654321098765LL ) };
		for ( auto _ : state )
		{
			bool result{ a < b };
			::benchmark::DoNotOptimize( result );
		}
	}

	//----------------------------------------------
	// State checking
	//----------------------------------------------

	static void BM_DecimalIsZero( ::benchmark::State& state )
	{
		Decimal value{};
		for ( auto _ : state )
		{
			bool result{ value.isZero() };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalIsZeroNonZero( ::benchmark::State& state )
	{
		Decimal value{ 123.456 };
		for ( auto _ : state )
		{
			bool result{ value.isZero() };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalIsNegative( ::benchmark::State& state )
	{
		Decimal value{ -123.456 };
		for ( auto _ : state )
		{
			bool result{ value.isNegative() };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalIsNegativePositive( ::benchmark::State& state )
	{
		Decimal value{ 123.456 };
		for ( auto _ : state )
		{
			bool result{ value.isNegative() };
			::benchmark::DoNotOptimize( result );
		}
	}

	//----------------------------------------------
	// Mathematical operations
	//----------------------------------------------

	static void BM_DecimalAbsPositive( ::benchmark::State& state )
	{
		Decimal value{ 123456.789 };
		for ( auto _ : state )
		{
			Decimal result{ Decimal::abs( value ) };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalAbsNegative( ::benchmark::State& state )
	{
		Decimal value{ -123456.789 };
		for ( auto _ : state )
		{
			Decimal result{ Decimal::abs( value ) };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalTruncate( ::benchmark::State& state )
	{
		Decimal value{ 123456.789 };
		for ( auto _ : state )
		{
			Decimal result{ value.truncate() };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalFloor( ::benchmark::State& state )
	{
		Decimal value{ 123456.789 };
		for ( auto _ : state )
		{
			Decimal result{ value.floor() };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalCeiling( ::benchmark::State& state )
	{
		Decimal value{ 123456.789 };
		for ( auto _ : state )
		{
			Decimal result{ value.ceiling() };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalRound( ::benchmark::State& state )
	{
		Decimal value{ 123456.789 };
		for ( auto _ : state )
		{
			Decimal result{ value.round() };
			::benchmark::DoNotOptimize( result );
		}
	}

	//----------------------------------------------
	// Property accessors
	//----------------------------------------------

	static void BM_DecimalGetScale( ::benchmark::State& state )
	{
		Decimal value{ 123.456 };
		for ( auto _ : state )
		{
			std::uint8_t result{ value.scale() };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalDecimalPlacesCount( ::benchmark::State& state )
	{
		Decimal value{ 123.4500 };
		for ( auto _ : state )
		{
			std::uint8_t result{ value.decimalPlacesCount() };
			::benchmark::DoNotOptimize( result );
		}
	}

	//----------------------------------------------
	// Constants
	//----------------------------------------------

	static void BM_DecimalConstantZero( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			Decimal result{ Decimal::zero() };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalConstantOne( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			Decimal result{ Decimal::one() };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalConstantMinValue( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			Decimal result{ Decimal::minValue() };
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_DecimalConstantMaxValue( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			Decimal result{ Decimal::maxValue() };
			::benchmark::DoNotOptimize( result );
		}
	}

	//=====================================================================
	// Benchmarks registration
	//=====================================================================

	//----------------------------------------------
	// Construction
	//----------------------------------------------

	BENCHMARK( BM_DecimalConstructDefault );
	BENCHMARK( BM_DecimalConstructFromInt32 );
	BENCHMARK( BM_DecimalConstructFromInt64 );
	BENCHMARK( BM_DecimalConstructFromUint32 );
	BENCHMARK( BM_DecimalConstructFromUint64 );
	BENCHMARK( BM_DecimalConstructFromFloat );
	BENCHMARK( BM_DecimalConstructFromDouble );
	BENCHMARK( BM_DecimalConstructFromInt128 );
	BENCHMARK( BM_DecimalCopyConstruct );

	//----------------------------------------------
	// Arithmetic operators
	//----------------------------------------------

	BENCHMARK( BM_DecimalAddition );
	BENCHMARK( BM_DecimalSubtraction );
	BENCHMARK( BM_DecimalMultiplication );
	BENCHMARK( BM_DecimalMultiplicationLarge );
	BENCHMARK( BM_DecimalDivision );
	BENCHMARK( BM_DecimalDivisionHighPrecision );
	BENCHMARK( BM_DecimalUnaryMinus );
	BENCHMARK( BM_DecimalAdditionAssignment );
	BENCHMARK( BM_DecimalSubtractionAssignment );
	BENCHMARK( BM_DecimalMultiplicationAssignment );
	BENCHMARK( BM_DecimalDivisionAssignment );

	//----------------------------------------------
	// Parsing
	//----------------------------------------------

	BENCHMARK( BM_DecimalParseInteger );
	BENCHMARK( BM_DecimalParseSmallDecimal );
	BENCHMARK( BM_DecimalParseLargeDecimal );
	BENCHMARK( BM_DecimalParseHighPrecision );
	BENCHMARK( BM_DecimalParseNegative );
	BENCHMARK( BM_DecimalTryParseValid );
	BENCHMARK( BM_DecimalTryParseInvalid );

	//----------------------------------------------
	// Conversion
	//----------------------------------------------

	BENCHMARK( BM_DecimalToDouble );
	BENCHMARK( BM_DecimalToBits );

	//----------------------------------------------
	// Formatting
	//----------------------------------------------

	BENCHMARK( BM_DecimalToStringInteger );
	BENCHMARK( BM_DecimalToStringSmall );
	BENCHMARK( BM_DecimalToStringLarge );
	BENCHMARK( BM_DecimalToStringNegative );

	//----------------------------------------------
	// Comparison
	//----------------------------------------------

	BENCHMARK( BM_DecimalEqualityDecimal );
	BENCHMARK( BM_DecimalLessThanDecimal );
	BENCHMARK( BM_DecimalGreaterThanDecimal );
	BENCHMARK( BM_DecimalEqualityInt32 );
	BENCHMARK( BM_DecimalEqualityInt64 );
	BENCHMARK( BM_DecimalEqualityUint64 );
	BENCHMARK( BM_DecimalEqualityDouble );
	BENCHMARK( BM_DecimalLessThanDouble );
	BENCHMARK( BM_DecimalEqualityFloat );
	BENCHMARK( BM_DecimalEqualityInt128 );
	BENCHMARK( BM_DecimalLessThanInt128 );

	//----------------------------------------------
	// State checking
	//----------------------------------------------

	BENCHMARK( BM_DecimalIsZero );
	BENCHMARK( BM_DecimalIsZeroNonZero );
	BENCHMARK( BM_DecimalIsNegative );
	BENCHMARK( BM_DecimalIsNegativePositive );

	//----------------------------------------------
	// Mathematical operations
	//----------------------------------------------

	BENCHMARK( BM_DecimalAbsPositive );
	BENCHMARK( BM_DecimalAbsNegative );
	BENCHMARK( BM_DecimalTruncate );
	BENCHMARK( BM_DecimalFloor );
	BENCHMARK( BM_DecimalCeiling );
	BENCHMARK( BM_DecimalRound );

	//----------------------------------------------
	// Property accessors
	//----------------------------------------------

	BENCHMARK( BM_DecimalGetScale );
	BENCHMARK( BM_DecimalDecimalPlacesCount );

	//----------------------------------------------
	// Constants
	//----------------------------------------------

	BENCHMARK( BM_DecimalConstantZero );
	BENCHMARK( BM_DecimalConstantOne );
	BENCHMARK( BM_DecimalConstantMinValue );
	BENCHMARK( BM_DecimalConstantMaxValue );
} // namespace nfx::datatypes::benchmark

BENCHMARK_MAIN();
