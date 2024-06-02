#pragma once
#include <cstdio>
#include "plugin.hpp"

struct MyClock : Module {
	enum ParamId {
		MULT_PARAM,
		TEMPO_PARAM,
		TEMPOGAIN_PARAM,
		RUN_PARAM,
		RESET_PARAM,
		CVGAIN_PARAM,
		TRIGTIME_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		BPM_INPUT,
		RUN_INPUT,
		RESET_INPUT,
		CVTRIG_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		CLOCK_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		RUN_LIGHT,
		RESET_LIGHT,
		CLOCK_LIGHT,
		LIGHTS_LEN
	};

	dsp::PulseGenerator pgen;
	dsp::BooleanTrigger resetButtonTrigger;
	dsp::SchmittTrigger resetTrigger;
	float counter, period;
	bool running = false ;

	MyClock() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(TEMPO_PARAM, 30.f, 360.f, 90.f, "Tempo", "BPM");
		paramQuantities[TEMPO_PARAM]->snapEnabled = true;
		configParam(MULT_PARAM, 1.f, 16.f, 1.f, "Clock multiplication");
		paramQuantities[MULT_PARAM]->snapEnabled = true;
		configParam(TEMPOGAIN_PARAM, 0.f, 100.f, 0.f, "CV gain", "");
		configParam(RUN_PARAM, 0.f, 1.f, 0.f, "Run");
		configButton(RESET_PARAM, "Reset");
		configParam(CVGAIN_PARAM, 0.f, 1.f, 0.f, "CV gain", "",  2.f, 1.f, -1.f);
		configParam(TRIGTIME_PARAM, 0.f, 1.f, 0.001f, "Pulse width ratio");
		configInput(BPM_INPUT, "");
		configInput(RUN_INPUT, "");
		configInput(RESET_INPUT, "");
		configInput(CVTRIG_INPUT, "");
		configOutput(CLOCK_OUTPUT, "");

		counter = period = 0.f; // Init
	}

	void process(const ProcessArgs& args) override ;
	int getBpm() ;
	int getMul() ;
	void resetClock() ;
};
