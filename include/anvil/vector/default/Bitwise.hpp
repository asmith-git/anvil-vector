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

#ifndef ANVIL_VECTOR_BITWISE__DEFAULT_HPP
#define ANVIL_VECTOR_BITWISE__DEFAULT_HPP

#include "anvil/vector/default/Type.hpp"

namespace anvil { namespace vector {

	namespace detail {
		template<uint32_t BYTES>
		static inline void _and_nomask_implement(const void* const lhs, const void* const rhs, void* const dst) throw() {
			const uint64_t* const l = static_cast<const uint64_t*>(lhs);
			const uint64_t* const r = static_cast<const uint64_t*>(rhs);
			uint64_t* const d = static_cast<uint64_t*>(dst);
			enum : uint32_t { LOOP = BYTES / 8u };
			for (uint32_t i = 0u; i < LOOP; ++i) {
				d[i] = l[i] & r[i];
			}
		}

		template<class T, uint32_t SIZE, const uint64_t MASK>
		static inline void _and_implement(const T* const lhs, const T* const rhs, const T* default_, T* const dst) throw() {
			if constexpr ((MASK & 1u) == 0u) {
				*dst = *default_;
			} else {
				*dst = *lhs & *rhs;
			}
			if constexpr (SIZE > 1u) {
				enum : uint64_t { MASK2 = MASK >> 1u };
				_and_implement<T, SIZE - 1u, MASK2>(lhs + 1u, rhs + 1u, default_ + 1u, dst + 1u);
			}
		}

		template<uint32_t BYTES>
		static inline void _or_nomask_implement(const void* const lhs, const void* const rhs, void* const dst) throw() {
			const uint64_t* const l = static_cast<const uint64_t*>(lhs);
			const uint64_t* const r = static_cast<const uint64_t*>(rhs);
			uint64_t* const d = static_cast<uint64_t*>(dst);
			enum : uint32_t { LOOP = BYTES / 8u };
			for (uint32_t i = 0u; i < LOOP; ++i) {
				d[i] = l[i] | r[i];
			}
		}

		template<class T, uint32_t SIZE, const uint64_t MASK>
		static inline void _or_implement(const T* const lhs, const T* const rhs, const T* default_, T* const dst) throw() {
			if constexpr ((MASK & 1u) == 0u) {
				*dst = *default_;
			} else {
				*dst = *lhs | *rhs;
			}
			if constexpr (SIZE > 1u) {
				enum : uint64_t { MASK2 = MASK >> 1u };
				_or_implement<T, SIZE - 1u, MASK2>(lhs + 1u, rhs + 1u, default_ + 1u, dst + 1u);
			}
		}

		template<uint32_t BYTES>
		static inline void _xor_nomask_implement(const void* const lhs, const void* const rhs, void* const dst) throw() {
			const uint64_t* const l = static_cast<const uint64_t*>(lhs);
			const uint64_t* const r = static_cast<const uint64_t*>(rhs);
			uint64_t* const d = static_cast<uint64_t*>(dst);
			enum : uint32_t { LOOP = BYTES / 8u };
			for (uint32_t i = 0u; i < LOOP; ++i) {
				d[i] = l[i] ^ r[i];
			}
		}

		template<class T, uint32_t SIZE, const uint64_t MASK>
		static inline void _xor_implement(const T* const lhs, const T* const rhs, const T* default_, T* const dst) throw() {
			if constexpr ((MASK & 1u) == 0u) {
				*dst = *default_;
			} else {
				*dst = *lhs ^ *rhs;
			}
			if constexpr (SIZE > 1u) {
				enum : uint64_t { MASK2 = MASK >> 1u };
				_xor_implement<T, SIZE - 1u, MASK2>(lhs + 1u, rhs + 1u, default_ + 1u, dst + 1u);
			}
		}

		template<class T, uint32_t SIZE>
		static inline void _blend_nomask_implement(const T* const lhs, const T* const rhs, const T* mask, T* const dst) throw() {
			for (uint32_t i = 0u; i < SIZE; ++i) {
				dst[i] = (mask[i] != 0u ? lhs : rhs)[i];
			}
		}

		template<class T, uint32_t SIZE, const uint64_t MASK>
		static inline void _blend_implement(const T* const lhs, const T* const rhs, T* const dst) throw() {
			if constexpr (SIZE >= 8u) {
				_blend_implement<T, 8u, MASK>(lhs, rhs, dst);
				if constexpr (SIZE > 8u) {
					enum : uint64_t { MASK2 = MASK >> 8u };
					_blend_implement<T, SIZE - 8u, MASK2>(lhs + 8u, rhs + 8u, dst + 8u);
				}
			} else if constexpr (SIZE >= 4u) {
				_blend_implement<T, 4u, MASK>(lhs, rhs, dst);
				if constexpr (SIZE > 4u) {
					enum : uint64_t { MASK2 = MASK >> 4u };
					_blend_implement<T, SIZE - 4u, MASK2>(lhs + 4u, rhs + 4u, dst + 4u);
				}
			} else if constexpr (SIZE >= 2u) {
				_blend_implement<T, 2u, MASK>(lhs, rhs, dst);
				if constexpr (SIZE > 2u) {
					enum : uint64_t { MASK2 = MASK >> 2u };
					_blend_implement<T, SIZE - 2u, MASK2>(lhs + 2u, rhs + 2u, dst + 2u);
				}
			} else if constexpr (SIZE >= 1u) {
				//! \todo Optimise
				if constexpr ((MASK & 1u) == 0u) {
					*dst = *rhs;
				} else {
					*dst = *lhs;
				}
			}
		}

		template<>
		static inline void _blend_implement<uint32_t, 2u, 3u>(const uint32_t* const lhs, const uint32_t* const rhs, uint32_t* const dst) throw() {
			*reinterpret_cast<uint64_t*>(dst) = *reinterpret_cast<const uint64_t*>(lhs);
		}

		template<>
		static inline void _blend_implement<uint16_t, 4u, 15u>(const uint16_t* const lhs, const uint16_t* const rhs, uint16_t* const dst) throw() {
			*reinterpret_cast<uint64_t*>(dst) = *reinterpret_cast<const uint64_t*>(lhs);
		}

		template<>
		static inline void _blend_implement<uint16_t, 2u, 3u>(const uint16_t* const lhs, const uint16_t* const rhs, uint16_t* const dst) throw() {
			*reinterpret_cast<uint32_t*>(dst) = *reinterpret_cast<const uint32_t*>(lhs);
		}

		template<>
		static inline void _blend_implement<uint8_t, 8u, 255u>(const uint8_t* const lhs, const uint8_t* const rhs, uint8_t* const dst) throw() {
			*reinterpret_cast<uint64_t*>(dst) = *reinterpret_cast<const uint64_t*>(lhs);
		}

		template<>
		static inline void _blend_implement<uint8_t, 4u, 15u>(const uint8_t* const lhs, const uint8_t* const rhs, uint8_t* const dst) throw() {
			*reinterpret_cast<uint32_t*>(dst) = *reinterpret_cast<const uint32_t*>(lhs);
		}

		template<>
		static inline void _blend_implement<uint8_t, 2u, 3u>(const uint8_t* const lhs, const uint8_t* const rhs, uint8_t* const dst) throw() {
			*reinterpret_cast<uint16_t*>(dst) = *reinterpret_cast<const uint16_t*>(lhs);
		}

		template<uint32_t BYTES>
		struct _blend_type;

		template<>
		struct _blend_type<1u> {
			typedef uint8_t type;
		};

		template<>
		struct _blend_type<2u> {
			typedef uint16_t type;
		};

		template<>
		struct _blend_type<4u> {
			typedef uint32_t type;
		};

		template<>
		struct _blend_type<8u> {
			typedef uint64_t type;
		};
	}

	template<class T, class V>
	static inline V _and(const V& lhs, const V& rhs) throw() {
		V dst;
		detail::_and_nomask_implement<sizeof(V)>(&lhs, &rhs, &dst);
		return dst;
	}

	template<class T, const uint64_t MASK, class V>
	static inline V _and(const V& lhs, const V& rhs, const V& default_) throw() {
		enum : uint64_t {
			ALL_SET = detail::_default_mask<sizeof(V) / sizeof(T)>(),
			MASK2 = MASK & ALL_SET
		};
		if constexpr (MASK2 == 0ull) {
			return default_;
		} else if constexpr(MASK2 == ALL_SET) {
			return _and<T,V>(lhs, rhs);
		} else {
			V dst;
			typedef typename detail::_blend_type<sizeof(T)>::type T2;
			detail::_and_implement<T2, sizeof(V) / sizeof(T2), MASK2>(reinterpret_cast<const T2*>(&lhs), reinterpret_cast<const T2*>(&rhs), reinterpret_cast<const T2*>(&default_), reinterpret_cast<T2*>(&dst));
			return dst;
		}
	}

	template<class T, class V>
	static inline V _or(const V& lhs, const V& rhs) throw() {
		V dst;
		detail::_or_nomask_implement<sizeof(V)>(&lhs, &rhs, &dst);
		return dst;
	}

	template<class T, const uint64_t MASK, class V>
	static inline V _or(const V& lhs, const V& rhs, const V& default_) throw() {
		enum : uint64_t {
			ALL_SET = detail::_default_mask<sizeof(V) / sizeof(T)>(),
			MASK2 = MASK & ALL_SET
		};
		if constexpr (MASK2 == 0ull) {
			return default_;
		} else if constexpr(MASK2 == ALL_SET) {
			return _or<T, V>(lhs, rhs);
		} else {
			V dst;
			typedef typename detail::_blend_type<sizeof(T)>::type T2;
			detail::_or_implement<T2, sizeof(V) / sizeof(T2), MASK2>(reinterpret_cast<const T2*>(&lhs), reinterpret_cast<const T2*>(&rhs), reinterpret_cast<const T2*>(&default_), reinterpret_cast<T2*>(&dst));
			return dst;
		}
	}

	template<class T, class V>
	static inline V _xor(const V& lhs, const V& rhs) throw() {
		V dst;
		detail::_xor_nomask_implement<sizeof(V)>(&lhs, &rhs, &dst);
		return dst;
	}

	template<class T, const uint64_t MASK, class V>
	static inline V _xor(const V& lhs, const V& rhs, const V& default_) throw() {
		enum : uint64_t {
			ALL_SET = detail::_default_mask<sizeof(V) / sizeof(T)>(),
			MASK2 = MASK & ALL_SET
		};
		if constexpr (MASK2 == 0ull) {
			return default_;
		} else if constexpr(MASK2 == ALL_SET) {
			return _xor<T, V>(lhs, rhs);
		} else {
			V dst;
			typedef typename detail::_blend_type<sizeof(T)>::type T2;
			detail::_xor_implement<T2, sizeof(V) / sizeof(T2), MASK2>(reinterpret_cast<const T2*>(&lhs), reinterpret_cast<const T2*>(&rhs), reinterpret_cast<const T2*>(&default_), reinterpret_cast<T2*>(&dst));
			return dst;
		}
	}

	template<class T, const uint64_t MASK, class V>
	static inline V _blend(const V& lhs, const V& rhs) throw() {
		enum : uint64_t {
			ALL_SET = detail::_default_mask<sizeof(V) / sizeof(T)>(),
			MASK2 = MASK & ALL_SET
		};
		if constexpr (MASK2 == 0ull) {
			return rhs;
		} else if constexpr(MASK2 == ALL_SET) {
			return lhs;
		} else {
			V dst;
			typedef typename detail::_blend_type<sizeof(T)>::type T2;
			detail::_blend_implement<T2, sizeof(V) / sizeof(T2), MASK2>(reinterpret_cast<const T2*>(&lhs), reinterpret_cast<const T2*>(&rhs), reinterpret_cast<T2*>(&dst));
			return dst;
		}
	}

	template<class T, class V>
	static inline V _blend(const V& lhs, const V& rhs, const V& mask) throw() {
		V dst;
		typedef typename detail::_blend_type<sizeof(T)>::type T2;
		detail::_blend_nomask_implement<T2, sizeof(V) / sizeof(T2)>(reinterpret_cast<const T2*>(&lhs), reinterpret_cast<const T2*>(&rhs), reinterpret_cast<const T2*>(&mask), reinterpret_cast<T2*>(&dst));
		return dst;
	}
}}

#endif
