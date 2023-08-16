/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef FALLOUT2_STD_MAP_H
#define FALLOUT2_STD_MAP_H

#include "common/hashmap.h"

namespace Fallout2 {
namespace std {

template<class T1, class T2>
struct pair {
	T1 first;
	T2 second;

	pair() {
	}
	pair(T1 first_, T2 second_) : first(first_), second(second_) {
	}
};

template<class Key, class Val, class HashFunc = Common::Hash<Key>,
		 class EqualFunc = Common::EqualTo<Key> >
class unordered_map : public Common::HashMap<Key, Val, HashFunc, EqualFunc> {
public:
	pair<Key, Val> insert(pair<Key, Val> elem) {
		// unordered_map doesn't replace already existing keys
		if (this->contains(elem.first))
			return pair<Key, Val>(elem.first, this->operator[](elem.first));

		// Add item to map
		this->operator[](elem.first) = elem.second;
		return elem;
	}

	void reserve(size_t size) {
		// No implementation
	}
};

} // namespace std
} // namespace Fallout2

#endif
