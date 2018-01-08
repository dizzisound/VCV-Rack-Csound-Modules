#include "rack.hpp"

using namespace rack;

extern Plugin *plugin;

struct MLKnob : RoundKnob {
	MLKnob() {
		setSVG(SVG::load(assetPlugin(plugin,"res/Knob.svg")));
		box.size = Vec(36, 36);
	}
};

struct CsoundReverbWidget : ModuleWidget {
	CsoundReverbWidget();
};

