#pragma once

#include <functional>

namespace lve
{
	// from: https://stackoverflow.com/a/57595105
	template <typename T, typename... Rest>

	/**
	 * @brief Combine plusieurs valeurs pour g�n�rer un hash.
	 *
	 * Cette fonction combine plusieurs valeurs pour g�n�rer un hash en utilisant l'algorithme
	 * de mixage de MurmurHash3. Chaque valeur est hash�e individuellement et combin�e avec
	 * le hash pr�c�dent en utilisant une op�ration XOR et des rotations de bits.
	 *
	 * @tparam T Type de la premi�re valeur � hasher.
	 * @tparam Rest Types des valeurs restantes � hasher.
	 * @param seed R�f�rence vers le seed du hash.
	 * @param v La premi�re valeur � hasher.
	 * @param rest Les valeurs restantes � hasher.
	 */
	void HashCombine(std::size_t& _seed, const T& _v, const Rest&... _rest)
	{
		_seed ^= std::hash<T>{}(_v) + 0x9e3779b9 + (_seed << 6) + (_seed >> 2);
		(HashCombine(_seed, _rest), ...);
	};
} // namespace lve
