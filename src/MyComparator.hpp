#pragma once
#include "plugin.hpp"

struct MyComparator : Module {
	enum ParamId {
		THRESH_PARAM,
		CVGAIN_PARAM,
		PRECISION_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		INA_INPUT,
		INB_INPUT,
		CVTHRESH_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		OUTG_OUTPUT,
		OUTL_OUTPUT,
		OUTEQ_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LEDTHR_LIGHT,
		LEDHYST_LIGHT,
		LIGHTS_LEN
	};

	MyComparator() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(THRESH_PARAM,   -5.f, 5.f, 0.f,  "Comparator threshold", "V");
		configParam(CVGAIN_PARAM,    0.f, 1.f, 0.f,  "CV Gain", "", 2.f, 1.f, -1.f);
		configParam(PRECISION_PARAM, 0.f, 5.f, 2.5f, "Equality tolerance", "V");
		configInput(INA_INPUT, "Polyphonic IN A");
		configInput(INB_INPUT, "(Optional) Polyphonic IN B");
		configInput(CVTHRESH_INPUT, "CV Threshold");
		configOutput(OUTG_OUTPUT, "Polyphonic greater");
		configOutput(OUTL_OUTPUT, "Polyphonic lower");
		configOutput(OUTEQ_OUTPUT, "Polyphonic equality");
	}

	void process(const ProcessArgs& args) override ;
};
