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

#ifndef SOURCE_LISTING_PROVIDER_H_
#define SOURCE_LISTING_PROVIDER_H_

#include "engines/wintermute/debugger/error.h"
#include "engines/wintermute/debugger/listing_provider.h"
#include "common/path.h"
#include "common/str.h"

namespace Wintermute {

class SourceListing;
class Listing;

class SourceListingProvider : ListingProvider {
public:
	~SourceListingProvider() override {};
	/**
	 * Get a listing. When implementing this, the result should be safe to delete for the caller.
	 */
	Listing *getListing(const Common::Path &filename, ErrorCode &err) override = 0;
	virtual ErrorCode setPath(const Common::Path &path) = 0;
	virtual Common::Path getPath() const = 0;

};

} // End of namespace Wintermute

#endif /* SOURCE_LISTING_PROVIDER_H_ */
