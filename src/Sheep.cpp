#include "Sheep.hpp"


void Sheep::process(const ProcessArgs& args){}

struct SheepWidget : ModuleWidget {
	SheepWidget(Sheep* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Sheep.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(10.16, 57.825)), module, Sheep::PITCH_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(34.138, 57.825)), module, Sheep::PITCHCVGAIN_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(10.16, 78.385)), module, Sheep::RANGE_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(34.544, 78.385)), module, Sheep::RANGECVGAIN_PARAM));
		addParam(createParamCentered<BefacoSwitch>(mm2px(Vec(10.16, 96.375)), module, Sheep::DISTRIB_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.128, 32.125)), module, Sheep::TRIG_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.758, 57.825)), module, Sheep::PITCHCV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(23.165, 78.385)), module, Sheep::RANGECV_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(29.667, 32.125)), module, Sheep::BERNOUILLI_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(30.48, 93.163)), module, Sheep::NOISE_OUTPUT));

		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(14.224, 32.125)), module, Sheep::TRIG_LIGHT));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(14.63, 91.235)), module, Sheep::DISTRIB_LIGHT));
	}
};


Model* modelSheep = createModel<Sheep, SheepWidget>("Sheep");
