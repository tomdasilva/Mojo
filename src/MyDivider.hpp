#pragma once
#include "plugin.hpp"

struct MyDivider : Module {
	enum ParamId {
		ENUMS(RATIO_PARAM, 8),
		PARAMS_LEN
	};
	enum InputId {
		CLOCK_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		ENUMS(OUT_OUTPUT, 8),
		OUTPUTS_LEN
	};
	enum LightId {
		CLOCK_LIGHT,
		LIGHTS_LEN
	};

	MyDivider() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

		for(int c=0 ; c<PARAMS_LEN ; c++){
			configParam(RATIO_PARAM+c, 1.f, 16.f, 1.f, "1/") ;
			paramQuantities[RATIO_PARAM+c]->snapEnabled = true;
		}

		configInput(CLOCK_INPUT, "");

		for(int c=0 ; c<OUTPUTS_LEN ; c++){
			configOutput(OUT_OUTPUT+c, "Divided clock");
		}
	}

	dsp::SchmittTrigger edgeDetector;
	int counters[8] = {0, 0, 0, 0, 0, 0, 0, 0} ;

	void process(const ProcessArgs& args) override ;
};
