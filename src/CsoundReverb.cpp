
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
  vprintf(format, valist);    //if commented -> disable csound message on console
  return;
}

struct MLKnob : RoundKnob {
	MLKnob() {
		setSVG(SVG::load(assetPlugin(plugin,"res/Knob.svg")));
		box.size = Vec(36, 36);
	}
};

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
    int ksmps, result, compileError;
    int const nchnls = 2;       // 2 inputs and 2 outputs in csd

    float feedback, cutoff;
    bool bypass = true;

    SchmittTrigger buttonTrigger;

    void csoundSession() {
        //csd sampling-rate override
        string sr_override = "--sample-rate=" + to_string(engineGetSampleRate());

        //compile instance of csound
        compileError = csound->Compile(assetPlugin(plugin, "csd/CsoundReverb.csd").c_str(), (char *) sr_override.c_str(), "--realtime");
		if(compileError) cout << "Csound csd compilation error!" << endl;
		
		spout = csound->GetSpout();                                     //access csound output buffer
        spin  = csound->GetSpin();                                      //access csound input buffer
        ksmps = csound->GetKsmps();		
    }

	CsoundReverb() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS)
	{
        csound = new Csound();                                          //Create an instance of Csound
        csound->SetMessageCallback(MessageCallback);
        csoundSession();
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
    csoundSession();
};

void CsoundReverb::reset() {
    //menu initialize: csound restart with modified (or not!) csound script csd
    //onSampleRateChange(); //sometime (?) give csound seg fault 
}

void CsoundReverb::step() {
    float out1=0.0, out2=0.0;

    if(compileError) return;            //outputs set to zero

    //bypass
    if(buttonTrigger.process(params[BYPASS_PARAM].value)) bypass = !bypass;
    lights[BYPASS_LIGHT].value = bypass?10.0:0.0;

    //Process
    float in1 = clamp(inputs[IN1_INPUT].value,-10.0f,10.0f);
    float in2 = clamp(inputs[IN2_INPUT].value,-10.0f,10.0f);
		
    if(!bypass) {
        if(nbSample == 0)   //param refresh at control rate
        {
            //params
            if(inputs[FEEDBACK_INPUT].active) {
        		feedback = clamp(inputs[FEEDBACK_INPUT].value*0.125, 0.0f, 1.0f);
        	} else {
        		feedback = params[FEEDBACK_PARAM].value;
        	};

        	if(inputs[CUTOFF_INPUT].active) {
        		cutoff = clamp(inputs[CUTOFF_INPUT].value*0.125, 0.0f, 1.0f);
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
			
			if (nbSample >= ksmps*nchnls)
			{
				nbSample = 0;
			}
        }
		
		outputs[OUT1_OUTPUT].value = out1;
		outputs[OUT2_OUTPUT].value = out2;
		
    } else {
        //bypass
        outputs[OUT1_OUTPUT].value = in1;
    	outputs[OUT2_OUTPUT].value = in2;
    }
}


struct CsoundReverbWidget : ModuleWidget {
	CsoundReverbWidget(CsoundReverb *module);
};



CsoundReverbWidget::CsoundReverbWidget(CsoundReverb *module) : ModuleWidget(module) {
	box.size = Vec(6 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/CsoundReverb.svg")));
		addChild(panel);
	}
	addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
	addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
	addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
	addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
	addInput(Port::create<PJ301MPort>(Vec(11, 54), Port::INPUT, module, CsoundReverb::IN1_INPUT));
	addInput(Port::create<PJ301MPort>(Vec(55, 54), Port::INPUT, module, CsoundReverb::IN2_INPUT));
	addInput(Port::create<PJ301MPort>(Vec(10, 130), Port::INPUT, module, CsoundReverb::FEEDBACK_INPUT));
	addInput(Port::create<PJ301MPort>(Vec(10, 191), Port::INPUT, module, CsoundReverb::CUTOFF_INPUT));
    addParam(ParamWidget::create<MLKnob>(Vec(45, 123), module, CsoundReverb::FEEDBACK_PARAM, 0.0, 1.0, 0.8));
    addParam(ParamWidget::create<MLKnob>(Vec(45, 185), module, CsoundReverb::CUTOFF_PARAM, 0.0, 1.0, 0.6));
    addParam(ParamWidget::create<LEDButton>(Vec(35, 246), module, CsoundReverb::BYPASS_PARAM, 0.0, 10.0, 10.0));
    addChild(ModuleLightWidget::create<MediumLight<RedLight>>(Vec(40,250), module, CsoundReverb::BYPASS_LIGHT));
	addOutput(Port::create<PJ301MPort>(Vec(11, 299), Port::OUTPUT, module, CsoundReverb::OUT1_OUTPUT));
	addOutput(Port::create<PJ301MPort>(Vec(55, 299), Port::OUTPUT, module, CsoundReverb::OUT2_OUTPUT));
}

Model *modelCsoundReverb = Model::create<CsoundReverb, CsoundReverbWidget>("Csound", "CsoundReverb", "Csound Reverb", REVERB_TAG);