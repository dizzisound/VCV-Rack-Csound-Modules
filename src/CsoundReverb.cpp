
#include "Csound.hpp"
#include "dsp/digital.hpp"          //for SchmittTrigger

#include <csound/csound.hpp>

#include <iostream>
#include <string>
#include <math.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>


using namespace std;


void MessageCallback(CSOUND* cs, int attr, const char *format, va_list valist)
{
  //vprintf(format, valist);    //if commented -> disable csound message on console
  return;
}


struct CsoundReverb : Module {
	enum ParamIds {
		FEEDBACK_PARAM,
		CUTOFF_PARAM,
		BYPASS_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		IN1_INPUT,
		IN2_INPUT,
		FEEDBACK_INPUT,
		CUTOFF_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT1_OUTPUT,
		OUT2_OUTPUT,
		NUM_OUTPUTS
	};
	enum LighIds {
		BYPASS_LIGHT,
		NUM_LIGHTS
	};

    Csound* csound;

    MYFLT *spin, *spout;

    int nbSample = 0;
    int ksmps, result;
    int const nchnls = 2;       // 2 inputs and 2 outputs in csd

    float feedback, cutoff; 
    bool bypass = false;

    SchmittTrigger buttonTrigger;

    void csoundCession() {
        //csd sampling-rate override
        string sr_override = "--sample-rate=" + to_string(engineGetSampleRate());

        //compile instance of csound
        csound->Compile("./plugins/Csound/src/CsoundReverb.csd", (char *) sr_override.c_str(), "--realtime");

        spout = csound->GetSpout();                                     //access csound output buffer
        spin  = csound->GetSpin();                                      //access csound input buffer
        ksmps = csound->GetKsmps();
    }

	CsoundReverb() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS)
	{
        csound = new Csound();                                          //Create an instance of Csound
        csound->SetMessageCallback(MessageCallback);
        csoundCession();
	}

    ~CsoundReverb()
    {
        delete csound;                  //free Csound object
    }

	void step() override;
	void reset() override;
	void onSampleRateChange() override;
};

void CsoundReverb::onSampleRateChange() {
    //csound restart with new sample rate
    csound->Reset();
    csoundCession();
};

void CsoundReverb::reset() {
    //menu initialize: csound restart with modified (or not!) csound script csd
    onSampleRateChange(); 
}

void CsoundReverb::step() {
    float out1=0.0, out2=0.0;

    //bypass
	if(buttonTrigger.process(params[BYPASS_PARAM].value)) bypass = !bypass;
    lights[BYPASS_LIGHT].value = bypass?10.0:0.0;

    //Process
    float in1 = clampf(inputs[IN1_INPUT].value,-10.0,10.0);
    float in2 = clampf(inputs[IN2_INPUT].value,-10.0,10.0);

    if(!bypass) {
        if(nbSample == 0)   //param refresh at control rate
        {
            //params
            if(inputs[FEEDBACK_INPUT].active) {
        		feedback = clampf(inputs[FEEDBACK_INPUT].value*0.125, 0.0, 1.0);
        	} else {
        		feedback = params[FEEDBACK_PARAM].value;
        	};

        	if(inputs[CUTOFF_INPUT].active) {
        		cutoff = clampf(inputs[CUTOFF_INPUT].value*100, 0.0, 10000.0);
        	} else {
        		cutoff = params[CUTOFF_PARAM].value;
        	};

            csound->SetChannel("feedback", feedback);
            csound->SetChannel("cutoff", cutoff);
            result = csound->PerformKsmps();
        }

        if(!result)
        {
        spin[nbSample] = in1;
    	out1 = spout[nbSample];
        nbSample++;
        spin[nbSample] = in2;
    	out2 = spout[nbSample];
        nbSample++;
        if (nbSample == ksmps*nchnls)
            nbSample = 0;
        }
        outputs[OUT1_OUTPUT].value = out1;
    	outputs[OUT2_OUTPUT].value = out2;
    } else {
        //bypass
        outputs[OUT1_OUTPUT].value = in1;
    	outputs[OUT2_OUTPUT].value = in2;
    }
}

CsoundReverbWidget::CsoundReverbWidget() {
	CsoundReverb *module = new CsoundReverb();
	setModule(module);
	box.size = Vec(6 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/CsoundReverb.svg")));
		addChild(panel);
	}
	addChild(createScrew<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
	addChild(createScrew<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
	addInput(createInput<PJ301MPort>(Vec(11, 54), module, CsoundReverb::IN1_INPUT));
	addInput(createInput<PJ301MPort>(Vec(55, 54), module, CsoundReverb::IN2_INPUT));
	addInput(createInput<PJ301MPort>(Vec(10, 130), module, CsoundReverb::FEEDBACK_INPUT));
	addInput(createInput<PJ301MPort>(Vec(10, 191), module, CsoundReverb::CUTOFF_INPUT));
    addParam(createParam<MLKnob>(Vec(45, 123), module, CsoundReverb::FEEDBACK_PARAM, 0.0, 1.0, 0.87));
    addParam(createParam<MLKnob>(Vec(45, 185), module, CsoundReverb::CUTOFF_PARAM, 0.0, 10000.0, 5000.0));
    addParam(createParam<LEDButton>(Vec(35, 246), module, CsoundReverb::BYPASS_PARAM, 0.0, 10.0, 0.0));
    addChild(createLight<MediumLight<RedLight>>(Vec(40,250), module, CsoundReverb::BYPASS_LIGHT));
	addOutput(createOutput<PJ301MPort>(Vec(11, 299), module, CsoundReverb::OUT1_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(55, 299), module, CsoundReverb::OUT2_OUTPUT));
}

