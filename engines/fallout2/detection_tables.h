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

	//  Official English v1.0
	{
		"fallout2",
		"1.0",
		AD_ENTRY1s("critter.dat", "3d1634029b7806cfeb24c9e93ded578f", 166951131),
		Common::EN_ANY,
		Common::kPlatformWindows,
		ADGF_UNSTABLE | ADGF_DROPPLATFORM,
		GUIO1(GUIO_NOLAUNCHLOAD)
	},

	//  Official English v1.02
	{
		"fallout2",
		"1.02d",
		AD_ENTRY2s("critter.dat",  "3d1634029b7806cfeb24c9e93ded578f", 166951131,
				   "patch000.dat", "82ed82915fe07b82819e32014e80a970",   2355526),
		Common::EN_ANY,
		Common::kPlatformWindows,
		ADGF_UNSTABLE | ADGF_DROPPLATFORM,
		GUIO1(GUIO_NOLAUNCHLOAD)
	},

	//  Official German v1.02 (GOG.com)
	{
		"fallout2",
		"1.02d",
		AD_ENTRY2s("critter.dat",  "t:88cf42e26cbf287a594547423654b343", 131934118,
				   "patch000.dat", "t:e8c62b94afa14549e745fd88d758c463",   2285423),
		Common::DE_DEU,
		Common::kPlatformWindows,
		ADGF_UNSTABLE | ADGF_DROPPLATFORM,
		GUIO1(GUIO_NOLAUNCHLOAD)
	},

	//  Official French v1.02 (GOG.com)
	{
		"fallout2",
		"1.02d",
		AD_ENTRY2s("critter.dat",  "t:d664cc5ffb460839d99cacd9d950ffaa", 166273976,
				   "patch000.dat", "t:bcaede3c070ffe5352e70c8429618433",   2201944),
		Common::FR_FRA,
		Common::kPlatformWindows,
		ADGF_UNSTABLE | ADGF_DROPPLATFORM,
		GUIO1(GUIO_NOLAUNCHLOAD)
	},

	//  Sonora v1.12
	{
		"f2sonora",
		nullptr,
		AD_ENTRY1s("critter.dat", "2a6638e1798adbef4ad5d062430a1a80", 183044210),
		Common::RU_RUS,
		Common::kPlatformWindows,
		ADGF_UNSTABLE | ADGF_DROPPLATFORM,
		GUIO1(GUIO_NOLAUNCHLOAD)
	},

	//  Sonora v1.15
	{
		"f2sonora",
		nullptr,
		AD_ENTRY1s("critter.dat", "b01fedff1eab262c92dd134bc1dfc704", 157811041),
		Common::RU_RUS,
		Common::kPlatformWindows,
		ADGF_UNSTABLE | ADGF_DROPPLATFORM,
		GUIO1(GUIO_NOLAUNCHLOAD)
	},

	//  Sonora v1.15 with Dayglow DLC
	{
		"f2sonora",
		"Dayglow DLC",
		AD_ENTRY2s("critter.dat",  "b01fedff1eab262c92dd134bc1dfc704", 157811041,
				   "patch000.dat", "d62be1761d684c961e16d6faa861e6cf",  68564323),
		Common::RU_RUS,
		Common::kPlatformWindows,
		ADGF_UNSTABLE | ADGF_DROPPLATFORM,
		GUIO1(GUIO_NOLAUNCHLOAD)
	},

	//  Sonora v1.15
	//  Cambragol ENG translation v1.04
	{
		"f2sonora",
		nullptr,
		AD_ENTRY2s("critter.dat",  "b01fedff1eab262c92dd134bc1dfc704", 157811041,
				   "patch000.dat", "71ec83412fa143b94e90e13ccabb9ffd", 233414735),
		Common::EN_ANY,
		Common::kPlatformWindows,
		ADGF_UNSTABLE | ADGF_DROPPLATFORM,
		GUIO1(GUIO_NOLAUNCHLOAD)
	},

	//  Sonora v1.15 with Dayglow DLC
	//  Cambragol ENG translation v1.04
	{
		"f2sonora",
		"Dayglow DLC",
		AD_ENTRY2s("critter.dat",  "b01fedff1eab262c92dd134bc1dfc704", 157811041,
				   "patch000.dat", "7564874d36d3b892da4bd76f56931e95", 295105460),
		Common::EN_ANY,
		Common::kPlatformWindows,
		ADGF_UNSTABLE | ADGF_DROPPLATFORM,
		GUIO1(GUIO_NOLAUNCHLOAD)
	},

	//  Sonora v1.15 with Dayglow DLC
	//  Cambragol ENG translation v1.04 + script patch
	{
		"f2sonora",
		"Dayglow DLC (patched)",
		AD_ENTRY2s("critter.dat",  "b01fedff1eab262c92dd134bc1dfc704", 157811041,
				   "patch000.dat", "7564874d36d3b892da4bd76f56931e95", 295807037),
		Common::EN_ANY,
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
