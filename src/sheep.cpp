// Tom DA SILVA-FARIA (Mojo Mods)
// VCV Rack Module : sheep 
// Random voltage generator

#include "plugin.hpp"


struct Sheep : Module {
	enum ParamIds {
		TRIGACTIVE_PARAM,
		DURATION_PARAM,
		DISTRIBUTION_PARAM,
		RANGE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		TRIGIN_INPUT,
		DURATIONIN_INPUT,
		RANGEIN_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		BERNOULLIOUT_OUTPUT,
		OUT_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		LEDTRIG_LIGHT,
		LEDDURATION_LIGHT,
		LEDDISTRIB_LIGHT,
		NUM_LIGHTS
	};

	enum {
		DISTR_UNIFORM = 0,
		DISTR_NORMAL = 1,
		NUM_DISTRIBUTIONS
	};

	dsp::SchmittTrigger edgeDetector;
	int cpt = 0;
	float outputval = 0;

	Sheep() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(TRIGACTIVE_PARAM, 0.f, 1.f, 0.f, "");
		configParam(DURATION_PARAM, 0.f, 0.002f, 0.f, "");
		configParam(DISTRIBUTION_PARAM, 0.f, 1.f, 0.f, "");
		configParam(RANGE_PARAM, 0.f, 1.f, 0.5f, "");
	}

	void process(const ProcessArgs& args) override {

		int duration = std::floor(params[DURATION_PARAM].getValue()*args.sampleRate);
		duration += inputs[DURATIONIN_INPUT].getVoltage();
		int distrib = std::round(params[DISTRIBUTION_PARAM].getValue());
		float rangeval = params[RANGE_PARAM].getValue();
		rangeval += inputs[RANGEIN_INPUT].getVoltage()*0.05f;
		
		float compar;
		
		if(params[TRIGACTIVE_PARAM].getValue() == 0){
			lights[LEDTRIG_LIGHT].setBrightness(0.f);
			lights[LEDDURATION_LIGHT].setBrightness(1.f);
			if(cpt >= duration){
				if(distrib == DISTR_UNIFORM){
					lights[LEDDISTRIB_LIGHT].setBrightness(0.f);
					outputval = rangeval * 5.f * random::uniform();
					
					compar = 2.5f ; 
					if(outputval >= compar){
						outputs[BERNOULLIOUT_OUTPUT].setVoltage(rangeval * 5.f);
					}else{
						outputs[BERNOULLIOUT_OUTPUT].setVoltage(0.f);
					}
					
				}else{
					lights[LEDDISTRIB_LIGHT].setBrightness(1.f);
					outputval = clamp(rangeval * 2.5f * random::normal(), -10.f, 10.f);
					
					compar = 0 ; 
					if(outputval >= compar){
						outputs[BERNOULLIOUT_OUTPUT].setVoltage(rangeval * 5.f);
					}else{
						outputs[BERNOULLIOUT_OUTPUT].setVoltage(0.f);
					}
				}
				cpt = 0;
			}
			cpt ++;
			outputs[OUT_OUTPUT].setVoltage(outputval);
		}else{
			lights[LEDTRIG_LIGHT].setBrightness(1.f);
			lights[LEDDURATION_LIGHT].setBrightness(0.f);
			if(edgeDetector.process(inputs[TRIGIN_INPUT].getVoltage())){
				if(distrib == DISTR_UNIFORM){
					compar = 2.5f ; 
					lights[LEDDISTRIB_LIGHT].setBrightness(0.f);
					outputval = rangeval * 5.f * random::uniform();
					if(outputval >= compar){
						outputs[BERNOULLIOUT_OUTPUT].setVoltage(rangeval * 5.f);
					}else{
						outputs[BERNOULLIOUT_OUTPUT].setVoltage(0.f);
					}
				}else{
					compar = 0;
					lights[LEDDISTRIB_LIGHT].setBrightness(1.f);
					outputval = clamp(rangeval * 2.5f * random::normal(), -10.f, 10.f);
					if(outputval >= compar){
						outputs[BERNOULLIOUT_OUTPUT].setVoltage(rangeval * 5.f);
					}else{
						outputs[BERNOULLIOUT_OUTPUT].setVoltage(0.f);
					}
				}
			}
			outputs[OUT_OUTPUT].setVoltage(outputval);
		}		
	}
};


struct SheepWidget : ModuleWidget {
	SheepWidget(Sheep* module) {
		
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/sheep.svg")));

		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<BefacoSwitch>(mm2px(Vec(8.602, 32.763)), module, Sheep::TRIGACTIVE_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(8.909, 50.183)), module, Sheep::DURATION_PARAM));
		addParam(createParamCentered<BefacoSwitch>(mm2px(Vec(29.524, 74.262)), module, Sheep::DISTRIBUTION_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(8.909, 93.183)), module, Sheep::RANGE_PARAM));

		addInput(createInputCentered<CL1362Port>(mm2px(Vec(8.602, 17.454)), module, Sheep::TRIGIN_INPUT));
		addInput(createInputCentered<CL1362Port>(mm2px(Vec(8.909, 64.383)), module, Sheep::DURATIONIN_INPUT));
		addInput(createInputCentered<CL1362Port>(mm2px(Vec(8.909, 79.183)), module, Sheep::RANGEIN_INPUT));

		addOutput(createOutputCentered<PJ3410Port>(mm2px(Vec(30.754, 31.269)), module, Sheep::BERNOULLIOUT_OUTPUT));
		addOutput(createOutputCentered<PJ3410Port>(mm2px(Vec(30.66, 108.175)), module, Sheep::OUT_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(8.602, 25.763)), module, Sheep::LEDTRIG_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(18.009, 50.183)), module, Sheep::LEDDURATION_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(29.501, 85.458)), module, Sheep::LEDDISTRIB_LIGHT));
	}
};


Model* modelSheep = createModel<Sheep, SheepWidget>("sheep");