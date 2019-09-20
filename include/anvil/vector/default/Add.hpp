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

#ifndef ANVIL_VECTOR_ADD_DEFAULT_HPP
#define ANVIL_VECTOR_ADD_DEFAULT_HPP

#include "anvil/vector/default/Type.hpp"

namespace anvil { namespace vector {

	namespace detail {
		template<uint32_t BYTES>
		static inline void _add_nomask_implement(const void* const lhs, const void* const rhs, void* const dst) throw() {
			const uint64_t* const l = static_cast<const uint64_t*>(lhs);
			const uint64_t* const r = static_cast<const uint64_t*>(rhs);
			uint64_t* const d = static_cast<uint64_t*>(dst);
			enum : uint32_t { LOOP = BYTES / 8u };
			for (uint32_t i = 0u; i < LOOP; ++i) {
				d[i] = l[i] + r[i];
			}
		}

		template<class T, uint32_t SIZE, const uint64_t MASK>
		static inline void _add_implement(const T* const lhs, const T* const rhs, const T* default_, T* const dst) throw() {
			if constexpr ((MASK & 1u) == 0u) {
				*dst = *default_;
			} else {
				*dst = *lhs + *rhs;
			}
			if constexpr (SIZE > 1u) {
				enum : uint64_t { MASK2 = MASK >> 1u };
				_add_implement<T, SIZE - 1u, MASK2>(lhs + 1u, rhs + 1u, default_ + 1u, dst + 1u);
			}
		}
	}

	template<class T, class V>
	static inline V _add(const V& lhs, const V& rhs) throw() {
		V dst;
		detail::_add_nomask_implement<sizeof(V)>(&lhs, &rhs, &dst);
		return dst;
	}

	template<class T, const uint64_t MASK, class V>
	static inline V _add(const V& lhs, const V& rhs, const V& default_) throw() {
		enum : uint64_t {
			ALL_SET = detail::_default_mask<sizeof(V) / sizeof(T)>(),
			MASK2 = MASK & ALL_SET
		};
		if constexpr (MASK2 == 0ull) {
			return default_;
		} else if constexpr(MASK2 == ALL_SET) {
			return _add<T,V>(lhs, rhs);
		} else {
			V dst;
			typedef typename detail::_blend_type<sizeof(T)>::type T2;
			detail::_add_implement<T2, sizeof(V) / sizeof(T2), MASK2>(reinterpret_cast<const T2*>(&lhs), reinterpret_cast<const T2*>(&rhs), reinterpret_cast<const T2*>(&default_), reinterpret_cast<T2*>(&dst));
			return dst;
		}
	}
}}

#endif
