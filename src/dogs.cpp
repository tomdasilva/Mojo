// Tom DA SILVA-FARIA (Mojo Mods)
// VCV Rack Module : dogs
// Stereo drive effect

#include "plugin.hpp"


struct Dogs : Module {
	enum ParamIds {
		INLEVEL_PARAM,
		DRIVE_PARAM,
		OUTLEVEL_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		INL_INPUT,
		INR_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUTLEFT_OUTPUT,
		OUTRIGHT_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Dogs() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(INLEVEL_PARAM, 0.f, 2.f, 0.5f, "");
		configParam(DRIVE_PARAM, -1.f, -0.52f, -1.f, "");
		configParam(OUTLEVEL_PARAM, 0.f, 4.f, 1.f, "");
	}

	void process(const ProcessArgs& args) override {
		
		float invol = params[INLEVEL_PARAM].getValue();
		float outvol = params[OUTLEVEL_PARAM].getValue();
		float drive = params[DRIVE_PARAM].getValue();
		
		float outleft = std::tanh(outvol * inputs[INL_INPUT].getVoltage() * invol);
		float outright = std::sin(outvol * inputs[INR_INPUT].getVoltage() * invol);

		outputs[OUTLEFT_OUTPUT].setVoltage(outleft*drive + outright/(drive+0.5));
		outputs[OUTRIGHT_OUTPUT].setVoltage(outright*drive + outleft/(drive+0.5));

	}
};


struct DogsWidget : ModuleWidget {
	DogsWidget(Dogs* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/dogs.svg")));

		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<Trimpot>(mm2px(Vec(8.136, 24.567)), module, Dogs::INLEVEL_PARAM));
		addParam(createParamCentered<SynthTechAlco>(mm2px(Vec(12.7, 64.25)), module, Dogs::DRIVE_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(17.565, 85.409)), module, Dogs::OUTLEVEL_PARAM));

		addInput(createInputCentered<CL1362Port>(mm2px(Vec(8.135, 34.668)), module, Dogs::INL_INPUT));
		addInput(createInputCentered<CL1362Port>(mm2px(Vec(8.157, 45.78)), module, Dogs::INR_INPUT));

		addOutput(createOutputCentered<PJ3410Port>(mm2px(Vec(17.565, 95.51)), module, Dogs::OUTLEFT_OUTPUT));
		addOutput(createOutputCentered<PJ3410Port>(mm2px(Vec(17.565, 106.609)), module, Dogs::OUTRIGHT_OUTPUT));
	}
};


Model* modelDogs = createModel<Dogs, DogsWidget>("dogs");