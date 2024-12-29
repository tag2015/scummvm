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

#include "fallout2/dialogs_gmm.h"
#include "fallout2/fallout2.h"
#include "fallout2/metaengine.h"

#include "gui/ThemeEval.h"
#include "gui/gui-manager.h"
#include "gui/message.h"
#include "gui/saveload.h"
#include "gui/widget.h"
#include "gui/widgets/popup.h"

#include "common/system.h"
#include "common/translation.h"


namespace Fallout2 {

OptionsWidget::OptionsWidget(GuiObject *boss, const Common::String &name, const Common::String &domain) :
		OptionsContainerWidget(boss, name, "Fallout2GameOptionsDialog", domain) {

	_loadHiResCheckbox = new GUI::CheckboxWidget(
		widgetsBoss(),
		"Fallout2GameOptionsDialog.loadHiRes",
		_("Enable high resolution patch"),
		_("Load the Hi-Res settings from f2_res.ini, enabling arbitrary resolutions. See wiki for details."));

	_loadSfallCheckbox = new GUI::CheckboxWidget(
		widgetsBoss(),
		"Fallout2GameOptionsDialog.loadSfall",
		_("Load SFALL mod settings"),
		_("Load the SFALL settings from ddraw.ini. Only a few mods are actually implemented."));

	Common::FSDirectory dir(ConfMan.getPath("path", _domain));
	Common::Path extraPath(ConfMan.getPath("extrapath", _domain));
	Common::FSDirectory extraDir(extraPath);
	Common::Array<Common::FSDirectory *> langdirs = {&dir, dir.getSubDirectory("language")};
}

void OptionsWidget::defineLayout(GUI::ThemeEval &layouts, const Common::String &layoutName, const Common::String &overlayedLayout) const {
	layouts.addDialog(layoutName, overlayedLayout)
		.addLayout(GUI::ThemeLayout::kLayoutVertical)
		.addPadding(16, 16, 16, 16)
		.addWidget("loadHiRes", "Checkbox")
		.addWidget("loadSfall", "Checkbox")
		.closeLayout()
		.closeDialog();
}

void OptionsWidget::load() {
	if (_loadHiResCheckbox) {
		_loadHiResCheckbox->setState(ConfMan.getBool("load_hires", _domain));
	}

	if (_loadSfallCheckbox) {
		_loadSfallCheckbox->setState(ConfMan.getBool("load_sfall", _domain));
	}
}

bool OptionsWidget::save() {
	if (_loadHiResCheckbox) {
		ConfMan.setBool("load_hires", _loadHiResCheckbox->getState(), _domain);
	}

	if (_loadSfallCheckbox) {
		ConfMan.setBool("load_sfall", _loadSfallCheckbox->getState(), _domain);
	}

	return true;
}

} // End of namespace Fallout2

GUI::OptionsContainerWidget *Fallout2MetaEngine::buildEngineOptionsWidget(GUI::GuiObject *boss, const Common::String &name, const Common::String &target) const {
	return new Fallout2::OptionsWidget(boss, name, target);
}
