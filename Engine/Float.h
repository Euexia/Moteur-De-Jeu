#pragma once

#include <UDP/RangedInteger.h>
#include <UDP/Serialization/Serialization.h>
#include <UDP/Serialization/Serializer.h>
#include <UDP/Serialization/Deserializer.h>

template<class FLOATTYPE, int32_t MIN, int32_t MAX, uint8_t NBDECIMALS, uint8_t STEP = 1 >
class Float
{
	static_assert(std::is_same_v<FLOATTYPE, float32> || std::is_same_v<FLOATTYPE, float64>, "Float peut seulement être utilisé avec float32 ou float64");
	static_assert(NBDECIMALS > 0, "Au moins 1 décimale est nécessaire");
	static_assert(NBDECIMALS < 10, "Maximum de 10 décimales");
	static_assert(STEP != 0, "Le pas ne peut être nul");
	static_assert(STEP % 10 != 0, "Le pas ne peut être un multiple de 10. Retirez une décimale");
	using FloatType = FLOATTYPE;
	static constexpr int32 Min = MIN;
	static constexpr int32 Max = MAX;
	static constexpr uint32 Diff = Max - Min;
	static constexpr uint8 NbDecimals = NBDECIMALS;
	static constexpr uint8 Step = STEP;
};

