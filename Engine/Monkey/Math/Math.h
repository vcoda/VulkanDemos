#pragma once

#include "Math/PlatformMath.h"
#include "Common/Common.h"

#include <string>

#undef  PI
#define PI 					(3.1415926535897932f)
#define SMALL_NUMBER		(1.e-8f)
#define KINDA_SMALL_NUMBER	(1.e-4f)
#define BIG_NUMBER			(3.4e+38f)
#define EULERS_NUMBER       (2.71828182845904523536f)

#define MAX_FLT				3.402823466e+38F
#define INV_PI				(0.31830988618f)
#define HALF_PI				(1.57079632679f)
#define DELTA				(0.00001f)

struct MMath : public PlatformMath
{
	static FORCEINLINE int32 RandHelper(int32 value)
	{
		return value > 0 ? Min(TruncToInt(FRand() * value), value - 1) : 0;
	}

	static FORCEINLINE int32 RandRange(int32 inMin, int32 inMax)
	{
		const int32 range = (inMax - inMin) + 1;
		return inMin + RandHelper(range);
	}

	static FORCEINLINE float RandRange(float inMin, float inMax)
	{
		return FRandRange(inMin, inMax);
	}

	static FORCEINLINE float FRandRange(float inMin, float inMax)
	{
		return inMin + (inMax - inMin) * FRand();
	}

	static FORCEINLINE bool RandBool()
	{
		return (RandRange(0, 1) == 1) ? true : false;
	}

	template< class U >
	static FORCEINLINE bool IsWithin(const U& testValue, const U& minValue, const U& maxValue)
	{
		return ((testValue >= minValue) && (testValue < maxValue));
	}

	template< class U >
	static FORCEINLINE bool IsWithinInclusive(const U& testValue, const U& minValue, const U& maxValue)
	{
		return ((testValue >= minValue) && (testValue <= maxValue));
	}

	static FORCEINLINE bool IsNearlyEqual(float a, float b, float errorTolerance = SMALL_NUMBER)
	{
		return Abs<float>(a - b) <= errorTolerance;
	}

	static FORCEINLINE bool IsNearlyEqual(double a, double b, double errorTolerance = SMALL_NUMBER)
	{
		return Abs<double>(a - b) <= errorTolerance;
	}

	static FORCEINLINE bool IsNearlyZero(float value, float errorTolerance = SMALL_NUMBER)
	{
		return Abs<float>(value) <= errorTolerance;
	}

	static FORCEINLINE bool IsNearlyZero(double value, double errorTolerance = SMALL_NUMBER)
	{
		return Abs<double>(value) <= errorTolerance;
	}

	template <typename T>
	static FORCEINLINE bool IsPowerOfTwo(T value)
	{
		return ((value & (value - 1)) == (T)0);
	}

	template< class T >
	static FORCEINLINE T Max3(const T a, const T b, const T c)
	{
		return Max(Max(a, b), c);
	}

	template< class T >
	static FORCEINLINE T Min3(const T a, const T b, const T c)
	{
		return Min(Min(a, b), c);
	}

	template< class T >
	static FORCEINLINE T Square(const T a)
	{
		return a * a;
	}

	template< class T >
	static FORCEINLINE T Clamp(const T x, const T inMin, const T inMax)
	{
		return x < inMin ? inMin : x < inMax ? x : inMax;
	}

	static FORCEINLINE float GridSnap(float location, float grid)
	{
		if (grid == 0.0f)
		{
			return location;
		}
		else
		{
			return FloorToFloat((location + 0.5 * grid) / grid) * grid;
		}
	}

	static FORCEINLINE double GridSnap(double location, double grid)
	{
		if (grid == 0.0)
		{
			return location;
		}
		else
		{
			return FloorToDouble((location + 0.5 * grid) / grid) * grid;
		}
	}

	template <class T>
	static FORCEINLINE T DivideAndRoundUp(T dividend, T divisor)
	{
		return (dividend + divisor - 1) / divisor;
	}

	template <class T>
	static FORCEINLINE T DivideAndRoundDown(T dividend, T divisor)
	{
		return dividend / divisor;
	}

	template <class T>
	static FORCEINLINE T DivideAndRoundNearest(T dividend, T divisor)
	{
		return (dividend >= 0)
			? (dividend + divisor / 2) / divisor
			: (dividend - divisor / 2 + 1) / divisor;
	}

	static FORCEINLINE float Log2(float value)
	{
		static const float logToLog2 = 1.f / Loge(2.f);
		return Loge(value) * logToLog2;
	}

	static FORCEINLINE void SinCos(float* scalarSin, float* scalarCos, float value)
	{
		float quotient = (INV_PI * 0.5f) * value;
		if (value >= 0.0f)
		{
			quotient = (float)((int)(quotient + 0.5f));
		}
		else
		{
			quotient = (float)((int)(quotient - 0.5f));
		}
		float y = value - (2.0f * PI) * quotient;

		float sign;
		if (y > HALF_PI)
		{
			y = PI - y;
			sign = -1.0f;
		}
		else if (y < -HALF_PI)
		{
			y = -PI - y;
			sign = -1.0f;
		}
		else
		{
			sign = +1.0f;
		}

		float y2 = y * y;

		*scalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

		float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
		*scalarCos = sign * p;
	}

#define FASTASIN_HALF_PI (1.5707963050f)

	static FORCEINLINE float FastAsin(float value)
	{
		bool nonnegative = (value >= 0.0f);
		float x = MMath::Abs(value);
		float omx = 1.0f - x;
		if (omx < 0.0f)
		{
			omx = 0.0f;
		}
		float root = MMath::Sqrt(omx);
		float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + FASTASIN_HALF_PI;
		result *= root;
		return (nonnegative ? FASTASIN_HALF_PI - result : result - FASTASIN_HALF_PI);
	}
#undef FASTASIN_HALF_PI

	template<class T>
	static FORCEINLINE auto RadiansToDegrees(T const& radVal) -> decltype(radVal * (180.f / PI))
	{
		return radVal * (180.f / PI);
	}

	template<class T>
	static FORCEINLINE auto DegreesToRadians(T const& degVal) -> decltype(degVal * (PI / 180.f))
	{
		return degVal * (PI / 180.f);
	}

	static float FindDeltaAngleDegrees(float a1, float a2)
	{
		float delta = a2 - a1;

		if (delta > 180.0f)
		{
			delta = delta - 360.0f;
		}
		else if (delta < -180.0f)
		{
			delta = delta + 360.0f;
		}

		return delta;
	}

	static float FindDeltaAngleRadians(float a1, float a2)
	{
		float delta = a2 - a1;

		if (delta > PI)
		{
			delta = delta - (PI * 2.0f);
		}
		else if (delta < -PI)
		{
			delta = delta + (PI * 2.0f);
		}

		return delta;
	}

	static float UnwindRadians(float value)
	{
		while (value > PI)
		{
			value -= ((float)PI * 2.0f);
		}

		while (value < -PI)
		{
			value += ((float)PI * 2.0f);
		}

		return value;
	}

	static float UnwindDegrees(float value)
	{
		while (value > 180.f)
		{
			value -= 360.f;
		}

		while (value < -180.f)
		{
			value += 360.f;
		}

		return value;
	}

	static FORCEINLINE void CartesianToPolar(const float x, const float y, float& outRad, float& outAng)
	{
		outRad = Sqrt(Square(x) + Square(y));
		outAng = Atan2(y, x);
	}
	
	static FORCEINLINE void PolarToCartesian(const float rad, const float ang, float& outX, float& outY)
	{
		outX = rad * Cos(ang);
		outY = rad * Sin(ang);
	}

	static FORCEINLINE float GetRangePct(float minValue, float maxValue, float value)
	{
		const float divisor = maxValue - minValue;
		if (MMath::IsNearlyZero(divisor))
		{
			return (value >= maxValue) ? 1.f : 0.f;
		}

		return (value - minValue) / divisor;
	}

	template< class T, class U >
	static FORCEINLINE T Lerp(const T& a, const T& b, const U& alpha)
	{
		return (T)(a + alpha * (b - a));
	}

	template< class T >
	static FORCEINLINE T LerpStable(const T& a, const T& b, double alpha)
	{
		return (T)((a * (1.0 - alpha)) + (b * alpha));
	}

	template< class T >
	static FORCEINLINE T LerpStable(const T& a, const T& b, float alpha)
	{
		return (T)((a * (1.0f - alpha)) + (b * alpha));
	}

	template< class T, class U >
	static FORCEINLINE T BiLerp(const T& p00, const T& p10, const T& p01, const T& p11, const U& fracX, const U& fracY)
	{
		return Lerp(
			Lerp(p00, p10, fracX),
			Lerp(p01, p11, fracX),
			fracY
		);
	}

	template< class T, class U >
	static FORCEINLINE T CubicInterp(const T& p0, const T& t0, const T& p1, const T& t1, const U& a)
	{
		const float a2 = a * a;
		const float a3 = a2 * a;

		return (T)(((2 * a3) - (3 * a2) + 1) * p0) + ((a3 - (2 * a2) + a) * t0) + ((a3 - a2) * t1) + (((-2 * a3) + (3 * a2)) * p1);
	}

	template< class T, class U >
	static FORCEINLINE T CubicInterpDerivative(const T& p0, const T& t0, const T& p1, const T& t1, const U& a)
	{
		T x =  6.f * p0 + 3.f * t0 + 3.f * t1 - 6.f * p1;
		T y = -6.f * p0 - 4.f * t0 - 2.f * t1 + 6.f * p1;
		T z = t0;
		const float a2 = a * a;
		return (x * a2) + (y * a) + z;
	}

	template< class T, class U >
	static FORCEINLINE T CubicInterpSecondDerivative(const T& p0, const T& t0, const T& p1, const T& t1, const U& a)
	{
		T x = 12.f * p0 + 6.f * t0 + 6.f * t1 - 12.f * p1;
		T y = -6.f * p0 - 4.f * t0 - 2.f * t1 + 6.f  * p1;

		return (x * a) + y;
	}

	template< class T >
	static FORCEINLINE T InterpEaseIn(const T& a, const T& b, float alpha, float exp)
	{
		float const modifiedAlpha = Pow(alpha, exp);
		return Lerp<T>(a, b, modifiedAlpha);
	}

	template< class T >
	static FORCEINLINE T InterpEaseOut(const T& a, const T& b, float alpha, float exp)
	{
		float const modifiedAlpha = 1.f - Pow(1.f - alpha, exp);
		return Lerp<T>(a, b, modifiedAlpha);
	}

	template< class T >
	static FORCEINLINE T InterpEaseInOut(const T& a, const T& b, float alpha, float exp)
	{
		return Lerp<T>(a, b, (alpha < 0.5f) ?
			InterpEaseIn(0.f, 1.f, alpha * 2.f, exp) * 0.5f :
			InterpEaseOut(0.f, 1.f, alpha * 2.f - 1.f, exp) * 0.5f + 0.5f);
	}

	template< class T >
	static FORCEINLINE T InterpStep(const T& a, const T& b, float alpha, int32 steps)
	{
		if (steps <= 1 || alpha <= 0)
		{
			return a;
		}
		else if (alpha >= 1)
		{
			return b;
		}
		const float stepsAsFloat = static_cast<float>(steps);
		const float numIntervals = stepsAsFloat - 1.f;
		float const modifiedAlpha = FloorToFloat(alpha * stepsAsFloat) / numIntervals;
		return Lerp<T>(a, b, modifiedAlpha);
	}

	template< class T >
	static FORCEINLINE T InterpSinIn(const T& a, const T& b, float alpha)
	{
		float const modifiedAlpha = -1.f * Cos(alpha * HALF_PI) + 1.f;
		return Lerp<T>(a, b, modifiedAlpha);
	}

	template< class T >
	static FORCEINLINE T InterpSinOut(const T& a, const T& b, float alpha)
	{
		float const modifiedAlpha = Sin(alpha * HALF_PI);
		return Lerp<T>(a, b, modifiedAlpha);
	}

	template< class T >
	static FORCEINLINE T InterpSinInOut(const T& a, const T& b, float alpha)
	{
		return Lerp<T>(a, b, (alpha < 0.5f) ?
			InterpSinIn(0.f, 1.f, alpha * 2.f) * 0.5f :
			InterpSinOut(0.f, 1.f, alpha * 2.f - 1.f) * 0.5f + 0.5f);
	}

	template< class T >
	static FORCEINLINE T InterpExpoIn(const T& a, const T& b, float alpha)
	{
		float const modifiedAlpha = (alpha == 0.f) ? 0.f : Pow(2.f, 10.f * (alpha - 1.f));
		return Lerp<T>(a, b, modifiedAlpha);
	}

	template< class T >
	static FORCEINLINE T InterpExpoOut(const T& a, const T& b, float alpha)
	{
		float const modifiedAlpha = (alpha == 1.f) ? 1.f : -Pow(2.f, -10.f * alpha) + 1.f;
		return Lerp<T>(a, b, modifiedAlpha);
	}

	template< class T >
	static FORCEINLINE T InterpExpoInOut(const T& a, const T& b, float alpha)
	{
		return Lerp<T>(a, b, (alpha < 0.5f) ?
			InterpExpoIn(0.f, 1.f, alpha * 2.f) * 0.5f :
			InterpExpoOut(0.f, 1.f, alpha * 2.f - 1.f) * 0.5f + 0.5f);
	}

	template< class T >
	static FORCEINLINE T InterpCircularIn(const T& a, const T& b, float alpha)
	{
		float const modifiedAlpha = -1.f * (Sqrt(1.f - alpha * alpha) - 1.f);
		return Lerp<T>(a, b, modifiedAlpha);
	}

	template< class T >
	static FORCEINLINE T InterpCircularOut(const T& a, const T& b, float alpha)
	{
		alpha -= 1.f;
		float const modifiedAlpha = Sqrt(1.f - alpha * alpha);
		return Lerp<T>(a, b, modifiedAlpha);
	}

	template< class T >
	static FORCEINLINE T InterpCircularInOut(const T& a, const T& b, float alpha)
	{
		return Lerp<T>(a, b, (alpha < 0.5f) ?
			InterpCircularIn(0.f, 1.f, alpha * 2.f) * 0.5f :
			InterpCircularOut(0.f, 1.f, alpha * 2.f - 1.f) * 0.5f + 0.5f);
	}

	template< class U >
	static FORCEINLINE U CubicCRSplineInterp(const U& p0, const U& p1, const U& p2, const U& p3, const float t0, const float t1, const float t2, const float t3, const float t)
	{
		float invT1MinusT0 = 1.0f / (t1 - t0);
		U l01 = (p0 * ((t1 - t) * invT1MinusT0)) + (p1 * ((t - t0) * invT1MinusT0));
		float invT2MinusT1 = 1.0f / (t2 - t1);
		U l12 = (p1 * ((t2 - t) * invT2MinusT1)) + (p2 * ((t - t1) * invT2MinusT1));
		float invT3MinusT2 = 1.0f / (t3 - t2);
		U l23 = (p2 * ((t3 - t) * invT3MinusT2)) + (p3 * ((t - t2) * invT3MinusT2));

		float invT2MinusT0 = 1.0f / (t2 - t0);
		U l012 = (l01 * ((t2 - t) * invT2MinusT0)) + (l12 * ((t - t0) * invT2MinusT0));
		float invT3MinusT1 = 1.0f / (t3 - t1);
		U l123 = (l12 * ((t3 - t) * invT3MinusT1)) + (l23 * ((t - t1) * invT3MinusT1));

		return  ((l012 * ((t2 - t) * invT2MinusT1)) + (l123 * ((t - t1) * invT2MinusT1)));
	}

	template< class U >
	static FORCEINLINE U CubicCRSplineInterpSafe(const U& p0, const U& p1, const U& p2, const U& p3, const float t0, const float t1, const float t2, const float t3, const float t)
	{
		float t1MinusT0 = (t1 - t0);
		float t2MinusT1 = (t2 - t1);
		float t3MinusT2 = (t3 - t2);
		float t2MinusT0 = (t2 - t0);
		float t3MinusT1 = (t3 - t1);
		if (MMath::IsNearlyZero(t1MinusT0) || MMath::IsNearlyZero(t2MinusT1) || MMath::IsNearlyZero(t3MinusT2) || MMath::IsNearlyZero(t2MinusT0) || MMath::IsNearlyZero(t3MinusT1))
		{
			return p1;
		}

		float invT1MinusT0 = 1.0f / t1MinusT0;
		U l01 = (p0 * ((t1 - t) * invT1MinusT0)) + (p1 * ((t - t0) * invT1MinusT0));
		float invT2MinusT1 = 1.0f / t2MinusT1;
		U l12 = (p1 * ((t2 - t) * invT2MinusT1)) + (p2 * ((t - t1) * invT2MinusT1));
		float invT3MinusT2 = 1.0f / t3MinusT2;
		U l23 = (p2 * ((t3 - t) * invT3MinusT2)) + (p3 * ((t - t2) * invT3MinusT2));

		float invT2MinusT0 = 1.0f / t2MinusT0;
		U l012 = (l01 * ((t2 - t) * invT2MinusT0)) + (l12 * ((t - t0) * invT2MinusT0));
		float invT3MinusT1 = 1.0f / t3MinusT1;
		U l123 = (l12 * ((t3 - t) * invT3MinusT1)) + (l23 * ((t - t1) * invT3MinusT1));

		return  ((l012 * ((t2 - t) * invT2MinusT1)) + (l123 * ((t - t1) * invT2MinusT1)));
	}

	static float MakePulsatingValue(const double inCurrentTime, const float inPulsesPerSecond, const float inPhase = 0.0f)
	{
		return 0.5f + 0.5f * MMath::Sin(((0.25f + inPhase) * PI * 2.0) + (inCurrentTime * PI * 2.0) * inPulsesPerSecond);
	}

	static FORCEINLINE float RoundFromZero(float f)
	{
		return (f < 0.0f) ? FloorToFloat(f) : CeilToFloat(f);
	}

	static FORCEINLINE double RoundFromZero(double f)
	{
		return (f < 0.0) ? FloorToDouble(f) : CeilToDouble(f);
	}

	static FORCEINLINE float RoundToZero(float f)
	{
		return (f < 0.0f) ? CeilToFloat(f) : FloorToFloat(f);
	}

	static FORCEINLINE double RoundToZero(double f)
	{
		return (f < 0.0) ? CeilToDouble(f) : FloorToDouble(f);
	}

	static FORCEINLINE float RoundToNegativeInfinity(float f)
	{
		return FloorToFloat(f);
	}

	static FORCEINLINE double RoundToNegativeInfinity(double f)
	{
		return FloorToDouble(f);
	}

	static FORCEINLINE float RoundToPositiveInfinity(float f)
	{
		return CeilToFloat(f);
	}

	static FORCEINLINE double RoundToPositiveInfinity(double f)
	{
		return CeilToDouble(f);
	}

	static float SmoothStep(float a, float b, float X)
	{
		if (X < a)
		{
			return 0.0f;
		}
		else if (X >= b)
		{
			return 1.0f;
		}
		const float InterpFraction = (X - a) / (b - a);
		return InterpFraction * InterpFraction * (3.0f - 2.0f * InterpFraction);
	}

	static inline bool ExtractBoolFromBitfield(uint8* ptr, uint32 index)
	{
		uint8* bytePtr = ptr + index / 8;
		uint8 mast = 1 << (index & 0x7);

		return (*bytePtr & mast) != 0;
	}

	static inline void SetBoolInBitField(uint8* ptr, uint32 index, bool bset)
	{
		uint8* bytePtr = ptr + index / 8;
		uint8 mast = 1 << (index & 0x7);

		if (bset)
		{
			*bytePtr |= mast;
		}
		else
		{
			*bytePtr &= ~mast;
		}
	}

	static uint8 Quantize8UnsignedByte(float x)
	{
		int32 Ret = (int32)(x * 255.999f);
		return Ret;
	}

	static uint8 Quantize8SignedByte(float x)
	{
		float y = x * 0.5f + 0.5f;
		return Quantize8UnsignedByte(y);
	}

	static int32 GreatestCommonDivisor(int32 a, int32 b)
	{
		while (b != 0)
		{
			int32 t = b;
			b = a % b;
			a = t;
		}
		return a;
	}

	static int32 LeastCommonMultiplier(int32 a, int32 b)
	{
		int32 currentGcd = GreatestCommonDivisor(a, b);
		return currentGcd == 0 ? 0 : (a / currentGcd) * b;
	}

	static float FInterpConstantTo(float current, float target, float deltaTime, float interpSpeed);

	static float FInterpTo(float current, float target, float deltaTime, float interpSpeed);

	static float TruncateToHalfIfClose(float f);

	static double TruncateToHalfIfClose(double f);

	static void WindRelativeAnglesDegrees(float inAngle0, float& inOutAngle1);

	static float RoundHalfToEven(float f);

	static double RoundHalfToEven(double f);

	static float RoundHalfFromZero(float f);

	static double RoundHalfFromZero(double f);

	static float RoundHalfToZero(float f);

	static double RoundHalfToZero(double f);

	static float PerlinNoise1D(const float value);

	static const uint32 BitFlag[32];
};
