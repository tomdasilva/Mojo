#pragma once
#include "plugin.hpp"

struct Sheep : Module {
	enum ParamId {
		PITCH_PARAM,
		PITCHCVGAIN_PARAM,
		RANGE_PARAM,
		RANGECVGAIN_PARAM,
		DISTRIB_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		TRIG_INPUT,
		PITCHCV_INPUT,
		RANGECV_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		BERNOUILLI_OUTPUT,
		NOISE_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		TRIG_LIGHT,
		DISTRIB_LIGHT,
		LIGHTS_LEN
	};

	Sheep() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(PITCH_PARAM, 0.f, 1.f, 0.f, "");
		configParam(PITCHCVGAIN_PARAM, 0.f, 1.f, 0.f, "Pitch CV amount");
		configParam(RANGE_PARAM, 0.f, 1.f, 0.f, "");
		configParam(RANGECVGAIN_PARAM, 0.f, 1.f, 0.f, "Range CV amount");
		configParam(DISTRIB_PARAM, 0.f, 1.f, 0.f, "-10V/+10V toggle");
		configInput(TRIG_INPUT, "");
		configInput(PITCHCV_INPUT, "");
		configInput(RANGECV_INPUT, "");
		configOutput(BERNOUILLI_OUTPUT, "");
		configOutput(NOISE_OUTPUT, "");
	}

	void process(const ProcessArgs& args) override ;
};
