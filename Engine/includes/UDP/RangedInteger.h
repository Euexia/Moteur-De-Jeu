#pragma once

#include <UDP/Types.h>
#include <UDP/Serialization/Serialization.h>
#include <UDP/Serialization/Serializer.h>
#include <UDP/Serialization/Deserializer.h>
#include <TCP/Utils.h>


template<auto MIN, auto MAX>
class RangedInteger
{
	static_assert(MIN < MAX, "MIN et MAX doivent être strictement ordonnées");
	static_assert(possible, "Impossible avec ces valeurs de Min & Max ");
public:
	using Type = typename FittingType<MIN, MAX>::Type;
	static constexpr Type Min() { return MIN; }
	static constexpr Type Max() { return MAX; }
	static constexpr uint64 Range = Range<MIN, MAX>();
	static constexpr uint8 NbBits = NbBits<Range>::Value;

	static constexpr uint64 Range = Range<MIN, MAX>();
	static constexpr uint8 NbBits = NbBits<Range>::Value;

	RangedInteger() = default;
	explicit RangedInteger(Type v) : mValue(v) { checkValue(); }
	RangedInteger& operator=(Type v) { CheckValue(v); mValue = v; return *this; }

	static constexpr bool IsWithinRange(Type v) { return (v >= Min() && v <= Max()); }

	inline Type get() const { return mValue; }
	inline operator Type() const { return mValue; }

	template<typename OtherType>
	explicit RangedInteger(OtherType v) : { CheckValue(v); mValue = static_cast<Type>(v); }
	template<typename OtherType>
	RangedInteger& operator=(OtherType v) { CheckValue(v); mValue = static_cast<Type>(v); return *this; }
	…
		template<typename OtherType>
	static constexpr bool IsWithinRange(OtherType v)
	{
		if constexpr (!HoldingType<Type, OtherType>::IsPossible)
		{
			return false;
		}
		else
		{
			using CastType = typename HoldingType<Type, OtherType>::Type; // Déduire un type permettant de convertir le type interne et en paramètre
			return (static_cast<CastType>(v) >= static_cast<CastType>(Min()) && static_cast<CastType>(v) <= static_cast<CastType>(Max()));
		}
	}

private:
	void checkValue() { assert(IsWithinRange(mValue)); }
	static void CheckValue(Type v) { assert(IsWithinRange(v)); }
	template<typename OtherType>
	void CheckValue(OtherType v) { assert(IsWithinRange(v)); }
private:
	Type mValue{ Min() };
};

template<auto V, bool ForceUnsigned>
struct ExtractType
{
	static_assert(std::numeric_limits<decltype(V)>::is_integer, "ExtractType doit être utilisé avec des entiers");
	static_assert(!std::is_same_v<decltype(V), bool>, "ExtractType ne doit pas être utilisé avec bool");
	using Type = std::conditional_t < V < 0
		, std::conditional_t < V < std::numeric_limits<int32>::min(), int64
		, std::conditional_t< V < std::numeric_limits<int16>::min(), int32
		, std::conditional_t< V < std::numeric_limits<int8>::min(), int16
		, int8
		>>>
		// > 0 : doit-on forcer un type non signé ?
		, std::conditional_t< (V > std::numeric_limits<int64>::max()), uint64
		, std::conditional_t< (V > std::numeric_limits<uint32>::max()), std::conditional_t<ForceUnsigned, uint64, int64>
		, std::conditional_t< (V > std::numeric_limits<int32>::max()), std::conditional_t<ForceUnsigned, uint32, int64>
		, std::conditional_t< (V > std::numeric_limits<uint16>::max()), std::conditional_t<ForceUnsigned, uint32, int32>
		, std::conditional_t< (V > std::numeric_limits<int16>::max()), std::conditional_t<ForceUnsigned, uint16, int32>
		, std::conditional_t< (V > std::numeric_limits<uint8>::max()), std::conditional_t<ForceUnsigned, uint16, int16>
		, std::conditional_t< (V > std::numeric_limits<int8>::max()), std::conditional_t<ForceUnsigned, uint8, int16>
		, std::conditional_t<ForceUnsigned, uint8, int8>
		>>>>>>>
		>;
};

template<auto MIN, auto MAX>
struct FittingType
{
	using MinType = typename ExtractType<MIN, (MIN >= 0) >::Type;
	using MaxType = typename ExtractType<MAX, (MIN >= 0)>::Type;
	// Si nous avons une valeur < 0 et l’autre > int64 max, impossible de trouver un type pouvant les accueillir
	static constexpr bool IsPossible = !(MIN < 0 && MAX > std::numeric_limits<int64>::max()) || HoldingType<MinType, MaxType>::IsPossible;
	using Type =
		std::conditional_t<!IsPossible, void,
		// Est-ce que MIN est compatible avec MaxType ?
		std::conditional_t<(MIN >= std::numeric_limits<MaxType>::min()), MaxType,
		// Est-ce que MAX est compatible avec MinType ?
		std::conditional_t<(MAX <= std::numeric_limits<MinType>::max()), MinType,
		// Sinon, trouver un type suffisamment grand pour accommoder MIN et MAX
		typename HoldingType<MinType, MaxType>::Type
		>>>;
};

// Trouver un type qui permette d’accueillir les valeurs du type A et du type B
template<class A, class B>
struct HoldingType
{
	// Est-ce qu’un tel type existe ?
	// Un cas est impossible : si l’un est uint64 alors que l’autre est signé
	static constexpr bool IsPossible = !((std::is_same_v<A, uint64> && std::is_signed_v<B>) || (std::is_same_v<B, uint64> && std::is_signed_v<A>));

	using Type = typename std::conditional_t<!IsPossible, void
		// Ce sont les mêmes types : utilisons-le
		, std::conditional_t<std::is_same_v<A, B>, A
		// Tous deux signés ou non : utiliser le plus large type
		, std::conditional_t<std::is_signed_v<A> == std::is_signed_v<B>, typename Biggest<A, B>::Type
		// Le plus large est signé, utilisons-le
		, std::conditional_t<std::is_signed_v<typename Biggest<A, B>::Type>, typename Biggest<A, B>::Type
		// Sinon, utiliser le type signé plus large que le plus large des deux
		, std::make_signed_t<typename Promote<typename Biggest<A, B>::Type>::Type>
		>>>>;
};

// Retourne le type le plus large
template<class A, class B>
struct Biggest
{
	using Type = std::conditional_t<(sizeof(A) > sizeof(B)), A, B>;
};

// Promeut un type vers un type plus large (int32 => int64, uint8 => uint16...)
template<class T>
struct Promote { using Type = std::conditional_t<std::is_signed_v<T>, int64, uint64>; }; // Par défaut, utiliser le type le plus large possible. Ne devrait pas être utilisé mais nécessaire pour compiler.
template<> struct Promote<int32_t> { using Type = int64_t; };
template<> struct Promote<uint32_t> { using Type = uint64_t; };
template<> struct Promote<int16_t> { using Type = int32_t; };
template<> struct Promote<uint16_t> { using Type = uint32_t; };
template<> struct Promote<int8_t> { using Type = int16_t; };
template<> struct Promote<uint8_t> { using Type = uint16_t; };


template<auto MIN, auto MAX>
constexpr uint64_t Range()
{
	static_assert(MIN < MAX);
	if constexpr (MAX < 0)
	{
		// MIN & MAX < 0
		return static_cast<uint64>(static_cast<int64>(-1) * MIN) - static_cast<uint64>(static_cast<int64>(-1) * MAX);
	}
	else if constexpr (MIN < 0)
	{
#pragma warning(push)
#pragma warning(disable: 4307) // '*': signed integral constant overflow
		return static_cast<uint64>(MAX) + static_cast<uint64>(static_cast<int64>(-1) * MIN);
#pragma warning(pop)
	}
	else
	{
		return static_cast<uint64>(MAX) - static_cast<uint64>(MIN);
	}
}