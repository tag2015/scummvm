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

namespace Fallout2 {

const PlainGameDescriptor fallout2Games[] = {
	{ "fallout2", "Fallout 2" },
	{ "f2sonora", "Fallout: Sonora" },
	{ "olympus2207", "Olympus 2207" },
	{ 0, 0 }
};

const ADGameDescription gameDescriptions[] = {
	{
		"fallout2",
		nullptr,
		AD_ENTRY1s("critter.dat", "3d1634029b7806cfeb24c9e93ded578f", 166951131),
		Common::EN_ANY,
		Common::kPlatformWindows,
		ADGF_UNSTABLE | ADGF_DROPPLATFORM,
		GUIO1(GUIO_NOLAUNCHLOAD)
	},

	{
		"f2sonora",
		"v1.12",
		AD_ENTRY1s("critter.dat", "2a6638e1798adbef4ad5d062430a1a80", 183044210),
		Common::RU_RUS,
		Common::kPlatformWindows,
		ADGF_UNSTABLE | ADGF_DROPPLATFORM,
		GUIO1(GUIO_NOLAUNCHLOAD)
	},

	{
		"olympus2207",
		nullptr,
		AD_ENTRY1s("critter.dat", "00cc751243358bc005f77656c125e45f", 378144430),
		Common::EN_ANY,
		Common::kPlatformWindows,
		ADGF_UNSTABLE | ADGF_DROPPLATFORM,
		GUIO1(GUIO_NOLAUNCHLOAD)
	},

	AD_TABLE_END_MARKER
};

} // End of namespace Fallout2
