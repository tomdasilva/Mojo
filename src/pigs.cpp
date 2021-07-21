// Tom DA SILVA-FARIA (Mojo Mods)
// VCV Rack Module : pigs
// Massive bass sounds generator

#include "plugin.hpp"

struct Pigs : Module {

	template <typename T> int inline sign(T val){
			return (T(0) < val) - (val < T(0));
	}
	
	float phase = 0.f;

	enum ParamIds {
		VOLUME_PARAM,
		DRIVE_PARAM,
		TRIMPOT_PARAM,
		PITCH_PARAM,
		CALMSWITCH_PARAM,
		UP_PARAM,
		CALMLEVEL_PARAM,
		SUB_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		GAIN_INPUT,
		DRIVEIN_INPUT,
		PITCH_INPUT,
		UP_INPUT,
		SUB_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		LEDCALM_LIGHT,
		NUM_LIGHTS
	};

	Pigs() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(VOLUME_PARAM, 0.f, 2.f, 1.f, "");
		configParam(DRIVE_PARAM, 0.f, 2.f, 0.f, "");
		configParam(TRIMPOT_PARAM, 0.f, 1.f, 0.f, "");
		configParam(PITCH_PARAM, -3.f, 0.f, -2.f, "");
		configParam(CALMSWITCH_PARAM, 0.f, 1.f, 0.f, "");
		configParam(UP_PARAM, 0.f, 2.f, 0.6f, "");
		configParam(CALMLEVEL_PARAM, 0.f, 1.f, 0.2f, "");
		configParam(SUB_PARAM, 0.f, 2.f, 0.6f, "");
	}

	void process(const ProcessArgs& args) override {

		// frequency parameter
		float pitch = params[PITCH_PARAM].getValue();
		pitch += inputs[PITCH_INPUT].getVoltage();
		pitch = clamp(pitch, -4.f, 4.f);
		// The default pitch is C4 = 261.6256f
		float freq = dsp::FREQ_C4 * std::pow(2.f, pitch);
		
		// sub volume
		float subvol = params[SUB_PARAM].getValue();
		subvol += inputs[SUB_INPUT].getVoltage();
		
		// up volume
		float upvol = params[UP_PARAM].getValue();
		upvol += inputs[UP_INPUT].getVoltage();
		
		// gain 
		float gainvol = params[VOLUME_PARAM].getValue();
		gainvol += inputs[GAIN_INPUT].getVoltage();
		
		// Accumulate the phase
		phase += freq * args.sampleTime;
		if (phase >= 0.5f)
			phase -= 1.f;
		
		// calm level
		float calmlev = params[CALMLEVEL_PARAM].getValue();
		float secondvoice = std::sin(2.f * M_PI * phase) + std::sin(1.f * M_PI * phase); 
		float outcalm = calmlev * secondvoice;
		
		// Compute the sine output
		float sine = std::sin(2.f * M_PI * phase); 
		float subsine = std::sin(6.f * M_PI * phase);
		float upsine = std::sin(4.f * M_PI * phase);
		float out1 = sine + subvol*subsine + upvol*upsine ;
		float out = gainvol * out1 ; 
		
		// drive
		float drive = params[DRIVE_PARAM].getValue();
		float trimval = params[TRIMPOT_PARAM].getValue();
		drive += inputs[DRIVEIN_INPUT].getVoltage()*trimval;
		if(out > drive || out < -drive){
			out = sign(out)*2*drive - out;
		}
		
		// Calm mode 
		int calmswitchval = params[CALMSWITCH_PARAM].getValue();
		
		if(calmswitchval == 1){
			lights[LEDCALM_LIGHT].setBrightness(1.f);
			outputs[OUT_OUTPUT].setVoltage(5.f * (out + outcalm));
		}else{
			lights[LEDCALM_LIGHT].setBrightness(0.f);
			outputs[OUT_OUTPUT].setVoltage(5.f * out);
			
		}
	}
};


struct PigsWidget : ModuleWidget {
	PigsWidget(Pigs* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/pigs.svg")));

		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(7.266, 11.058)), module, Pigs::VOLUME_PARAM));
		addParam(createParamCentered<Rogan5PSGray>(mm2px(Vec(30.939, 25.5)), module, Pigs::DRIVE_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(7.244, 50.1)), module, Pigs::TRIMPOT_PARAM));
		addParam(createParamCentered<SynthTechAlco>(mm2px(Vec(49.602, 66.302)), module, Pigs::PITCH_PARAM));
		addParam(createParamCentered<BefacoSwitch>(mm2px(Vec(9.719, 67.993)), module, Pigs::CALMSWITCH_PARAM));
		addParam(createParamCentered<SynthTechAlco>(mm2px(Vec(49.602, 88.301)), module, Pigs::UP_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(9.719, 92.893)), module, Pigs::CALMLEVEL_PARAM));
		addParam(createParamCentered<SynthTechAlco>(mm2px(Vec(49.603, 110.301)), module, Pigs::SUB_PARAM));

		addInput(createInputCentered<CL1362Port>(mm2px(Vec(7.267, 23.258)), module, Pigs::GAIN_INPUT));
		addInput(createInputCentered<CL1362Port>(mm2px(Vec(7.244, 41.1)), module, Pigs::DRIVEIN_INPUT));
		addInput(createInputCentered<CL1362Port>(mm2px(Vec(30.883, 66.301)), module, Pigs::PITCH_INPUT));
		addInput(createInputCentered<CL1362Port>(mm2px(Vec(30.883, 88.301)), module, Pigs::UP_INPUT));
		addInput(createInputCentered<CL1362Port>(mm2px(Vec(30.883, 110.301)), module, Pigs::SUB_INPUT));

		addOutput(createOutputCentered<PJ3410Port>(mm2px(Vec(30.939, 43.2)), module, Pigs::OUT_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(9.719, 80.593)), module, Pigs::LEDCALM_LIGHT));
	}
};


Model* modelPigs = createModel<Pigs, PigsWidget>("pigs");