/**
 * @file Sample_Datatypes.cpp
 * @brief Demonstrates comprehensive usage of nfx-datatypes library
 * @details This sample shows how to use Int128 and Decimal for high-precision
 *          mathematical operations, exact arithmetic, and business calculations
 */

#include <iomanip>
#include <iostream>
#include <string>

#include <nfx/datatypes/Int128.h>
#include <nfx/datatypes/Decimal.h>

int main()
{
	std::cout << "=== nfx-datatypes Library - Int128 and Decimal Usage ===" << std::endl;
	std::cout << std::endl;

	//=========================================================================
	// Int128 - High-precision 128-bit integer arithmetic
	//=========================================================================

	std::cout << "--- Int128 Examples ---" << std::endl;

	// Construction from various types
	nfx::datatypes::Int128 smallInt{ 12345 };
	nfx::datatypes::Int128 largeInt{ static_cast<std::int64_t>( 9223372036854775807LL ) }; // max int64

	// Parse very large numbers from strings
	nfx::datatypes::Int128 hugeNumber = nfx::datatypes::Int128::parse( "123456789012345678901234567890" );

	std::cout << "Small integer: " << smallInt << std::endl;
	std::cout << "Large integer (max int64): " << largeInt << std::endl;
	std::cout << "Huge number from string: " << hugeNumber << std::endl;

	// Arithmetic operations
	auto sum = smallInt + largeInt;
	auto product = hugeNumber * nfx::datatypes::Int128{ 2 };
	auto difference = hugeNumber - largeInt;

	std::cout << "Sum of small + large: " << sum << std::endl;
	std::cout << "Huge number * 2: " << product << std::endl;
	std::cout << "Huge - large: " << difference << std::endl;

	// Comparisons
	if ( hugeNumber > largeInt )
	{
		std::cout << "Huge number is greater than large integer" << std::endl;
	}

	// State checking
	std::cout << "Is huge number negative? " << ( hugeNumber.isNegative() ? "Yes" : "No" ) << std::endl;
	std::cout << "Is huge number zero? " << ( hugeNumber.isZero() ? "Yes" : "No" ) << std::endl;

	// Division and modulo operations
	auto quotient = hugeNumber / nfx::datatypes::Int128{ 7 };
	auto remainder = hugeNumber % nfx::datatypes::Int128{ 7 };
	std::cout << "Huge number / 7: " << quotient << std::endl;
	std::cout << "Huge number % 7: " << remainder << std::endl;

	// Sign operations and mathematical functions
	nfx::datatypes::Int128 negativeVal{ -98765 };
	auto absoluteVal = negativeVal.isNegative() ? -negativeVal : negativeVal;
	std::cout << "Negative value: " << negativeVal << std::endl;
	std::cout << "Absolute value: " << absoluteVal << std::endl;

	// String conversion examples
	std::cout << "Huge number as string: " << hugeNumber.toString() << std::endl;
	std::cout << "Small int as string: " << smallInt.toString() << std::endl;

	std::cout << std::endl;

	//=========================================================================
	// Decimal - High-precision decimal arithmetic (28-29 digits)
	//=========================================================================

	std::cout << "--- Decimal Examples ---" << std::endl;

	// Construction from various types
	nfx::datatypes::Decimal priceFromString{ "999.99" };
	nfx::datatypes::Decimal quantityFromInt{ 1000000 };
	nfx::datatypes::Decimal rateFromDouble{ 0.0825 }; // 8.25% tax rate

	std::cout << std::fixed << std::setprecision( 6 );
	std::cout << "Price: " << priceFromString << std::endl;
	std::cout << "Quantity: " << quantityFromInt << std::endl;
	std::cout << "Tax rate: " << rateFromDouble << std::endl;

	// Exact decimal arithmetic (no floating-point errors)
	auto subtotal = priceFromString * quantityFromInt;
	auto tax = subtotal * rateFromDouble;
	auto total = subtotal + tax;

	std::cout << "Subtotal (price * quantity): " << subtotal << std::endl;
	std::cout << "Tax (subtotal * rate): " << tax << std::endl;
	std::cout << "Total (subtotal + tax): " << total << std::endl;

	// High-precision calculations
	nfx::datatypes::Decimal precisePi{ "3.1415926535897932384626433832795" };
	nfx::datatypes::Decimal radius{ "5.5" };
	auto area = precisePi * radius * radius;

	std::cout << "pi (high precision): " << precisePi << std::endl;
	std::cout << "Circle area (pi * r^2) with r=5.5: " << area << std::endl;

	// Rounding operations
	nfx::datatypes::Decimal unrounded{ "123.456789" };
	auto rounded2dp = nfx::datatypes::Decimal::round( unrounded, 2 );
	auto truncated = nfx::datatypes::Decimal::truncate( unrounded );
	auto ceilingVal = nfx::datatypes::Decimal::ceiling( unrounded );
	auto floorVal = nfx::datatypes::Decimal::floor( unrounded );

	std::cout << "Original: " << unrounded << std::endl;
	std::cout << std::defaultfloat << "Rounded to 2 dp: " << rounded2dp << std::endl;
	std::cout << "Truncated: " << truncated << std::endl;
	std::cout << "Ceiling: " << ceilingVal << std::endl;
	std::cout << "Floor: " << floorVal << std::endl;

	// State checking
	std::cout << "Is price zero? " << ( priceFromString.isZero() ? "Yes" : "No" ) << std::endl;
	std::cout << "Is tax negative? " << ( tax.isNegative() ? "Yes" : "No" ) << std::endl;
	std::cout << "Price scale (decimal places): " << static_cast<int>( priceFromString.scale() ) << std::endl;

	std::cout << std::endl;

	//=========================================================================
	// Advanced mathematical operations
	//=========================================================================

	std::cout << "--- Advanced Mathematical Examples ---" << std::endl;

	// Compound interest calculation using Decimal for precision
	nfx::datatypes::Decimal principal{ "10000.00" }; // $10,000 initial
	nfx::datatypes::Decimal annualRate{ "0.05" };	 // 5% annual interest
	nfx::datatypes::Decimal periodsPerYear{ "12" };	 // Monthly compounding
	nfx::datatypes::Decimal years{ "10" };			 // 10 years

	// A = P(1 + r/n)^(nt) - compound interest formula
	auto ratePerPeriod = annualRate / periodsPerYear;
	auto totalPeriods = periodsPerYear * years;

	// Calculate (1 + r/n)^(nt) step by step for demonstration
	auto onePlusRate = nfx::datatypes::Decimal{ "1.0" } + ratePerPeriod;
	auto annualRatePercentage = annualRate * nfx::datatypes::Decimal{ "100" };

	std::cout << "Principal: $" << principal << std::endl;
	std::cout << "Annual rate: " << annualRate << " (" << annualRatePercentage << "%)" << std::endl;
	std::cout << "Rate per period: " << ratePerPeriod << std::endl;
	std::cout << "Total periods: " << totalPeriods << std::endl;
	std::cout << "Base (1 + r/n): " << onePlusRate << std::endl;

	// High-precision factorial calculation with Int128
	nfx::datatypes::Int128 factorialInput{ 20 };
	nfx::datatypes::Int128 factorialResult{ 1 };
	nfx::datatypes::Int128 counter{ 1 };

	while ( counter <= factorialInput )
	{
		factorialResult = factorialResult * counter;
		counter = counter + nfx::datatypes::Int128{ 1 };
	}
	std::cout << "20! (factorial) = " << factorialResult << std::endl;

	// Large number arithmetic - Fibonacci sequence with Int128
	nfx::datatypes::Int128 fibA{ 0 };
	nfx::datatypes::Int128 fibB{ 1 };
	std::cout << "Fibonacci sequence (first 15 terms with Int128):" << std::endl;
	std::cout << "F(0) = " << fibA << std::endl;
	std::cout << "F(1) = " << fibB << std::endl;

	for ( int i = 2; i < 15; ++i )
	{
		auto fibNext = fibA + fibB;
		std::cout << "F(" << i << ") = " << fibNext << std::endl;
		fibA = fibB;
		fibB = fibNext;
	}

	// Financial precision demonstration
	std::cout << std::endl;
	std::cout << "--- Financial Precision Comparison ---" << std::endl;

	// Show why Decimal is better than double for financial calculations
	double doublePrice{ 0.1 };
	double doubleQuantity{ 3.0 };
	double doubleResult{ doublePrice * doubleQuantity };

	nfx::datatypes::Decimal decimalPrice{ "0.1" };
	nfx::datatypes::Decimal decimalQuantity{ "3" };
	auto decimalResult = decimalPrice * decimalQuantity;

	std::cout << std::fixed << std::setprecision( 17 );
	std::cout << "Double precision: 0.1 * 3 = " << doubleResult << std::endl;
	std::cout << "Decimal precision: 0.1 * 3 = " << decimalResult.toString() << std::endl;

	bool isExact{ decimalResult == nfx::datatypes::Decimal{ "0.3" } };
	std::cout << "Decimal is exact: " << ( isExact ? "Yes" : "No" ) << std::endl;

	return 0;
}
