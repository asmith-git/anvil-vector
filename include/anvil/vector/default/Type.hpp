//Copyright 2019 Adam G. Smith
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//http ://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.

#ifndef ANVIL_VECTOR_TYPE_DEFAULT_HPP
#define ANVIL_VECTOR_TYPE_DEFAULT_HPP

#include <array>

namespace anvil { namespace vector {

	template<class A, class B>
	struct __vec_compound {
		A left;
		B right;
	};
	
	template<class T>
	using __vec64 = std::array<T, 8u / sizeof(T)>;

	template<class T>
	using __vec128 = __vec_compound<__vec64<T>, __vec64<T>>;

	template<class T>
	using __vec256 = __vec_compound<__vec128<T>, __vec128<T>>;

	template<class T>
	using __vec512 = __vec_compound<__vec256<T>, __vec256<T>>;

	typedef __vec64<uint8_t> __vec64_u8;
	typedef __vec64<uint16_t> __vec64_u16;
	typedef __vec64<uint32_t> __vec64_u32;
	typedef __vec64<uint64_t> __vec64_u64;
	typedef __vec64<int8_t> __vec64_s8;
	typedef __vec64<int16_t> __vec64_s16;
	typedef __vec64<int32_t> __vec64_s32;
	typedef __vec64<int64_t> __vec64_s64;
	typedef __vec64<float> __vec64_f32;
	typedef __vec64<double> __vec64_f64;

	typedef __vec128<uint8_t> __vec128_u8;
	typedef __vec128<uint16_t> __vec128_u16;
	typedef __vec128<uint32_t> __vec128_u32;
	typedef __vec128<uint64_t> __vec128_u64;
	typedef __vec128<int8_t> __vec128_s8;
	typedef __vec128<int16_t> __vec128_s16;
	typedef __vec128<int32_t> __vec128_s32;
	typedef __vec128<int64_t> __vec128_s64;
	typedef __vec128<float> __vec128_f32;
	typedef __vec128<double> __vec128_f64;

	typedef __vec256<uint8_t> __vec256_u8;
	typedef __vec256<uint16_t> __vec256_u16;
	typedef __vec256<uint32_t> __vec256_u32;
	typedef __vec256<uint64_t> __vec256_u64;
	typedef __vec256<int8_t> __vec256_s8;
	typedef __vec256<int16_t> __vec256_s16;
	typedef __vec256<int32_t> __vec256_s32;
	typedef __vec256<int64_t> __vec256_s64;
	typedef __vec256<float> __vec256_f32;
	typedef __vec256<double> __vec256_f64;

	typedef __vec512<uint8_t> __vec512_u8;
	typedef __vec512<uint16_t> __vec512_u16;
	typedef __vec512<uint32_t> __vec512_u32;
	typedef __vec512<uint64_t> __vec512_u64;
	typedef __vec512<int8_t> __vec512_s8;
	typedef __vec512<int16_t> __vec512_s16;
	typedef __vec512<int32_t> __vec512_s32;
	typedef __vec512<int64_t> __vec512_s64;
	typedef __vec512<float> __vec512_f32;
	typedef __vec512<double> __vec512_f64;

	namespace detail {
		template<uint32_t SIZE>
		static inline constexpr uint64_t _default_mask() throw() {
			if constexpr (SIZE == 0u) {
				return 0ull;
			} else {
				return 1ull | (_default_mask<SIZE - 1u>() << 1ull)
			}
		}
	}

}}

#endif
