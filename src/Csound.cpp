#include "Csound.hpp"

#include <stdio.h>
#include <string>

// The plugin-wide instance of the Plugin class
Plugin *plugin;

void init(rack::Plugin *p) {
	plugin = p;
	// This is the unique identifier for your plugin
	p->slug = TOSTRING(SLUG);
#ifdef VERSION
	p->version = TOSTRING(VERSION);
#endif
	//Deprecated 
	//p->website = "http://csound.com";
	//p->manual = "http://csound.com/docs/manual/index.html";

	// For each module, specify the ModuleWidget subclass, manufacturer slug (for saving in patches), manufacturer human-readable name, module slug, and module name
	p->addModel(modelCsoundReverb);

	// Any other plugin initialization may go here.
}

