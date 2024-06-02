#include "MyClock.hpp"

int MyClock::getBpm(){return (int)params[TEMPO_PARAM].getValue() ;}
int MyClock::getMul(){return (int)params[MULT_PARAM].getValue() ;}
void MyClock::resetClock(){pgen.reset() ;}

void MyClock::process(const ProcessArgs& args){
	/* Clock generator */

	int BPM = getBpm() ;
	int MUL = getMul() ;
	BPM *= MUL ;

	// Tempo CV modulation
	if(inputs[BPM_INPUT].isConnected())
		BPM += (int)((inputs[BPM_INPUT].getVoltage()) * params[TEMPOGAIN_PARAM].getValue()) ;

	// Trig time CV modulation
	float trigCV = params[TRIGTIME_PARAM].getValue() ;
	if(inputs[CVTRIG_INPUT].isConnected())
		trigCV += (inputs[CVTRIG_INPUT].getVoltage()/10.f) * params[CVGAIN_PARAM].getValue() ;

	// Computing Clock
	period = 60.f * args.sampleRate/BPM; // samples
	float maxTrigTime = (60.f / BPM) - 0.002 ; // Max trig time : T - 1ms
	float trigTime = 0.001 + maxTrigTime * trigCV ;

	// Hnadling reset button
	bool resetButtonTriggered = resetButtonTrigger.process(params[RESET_PARAM].getValue());
	lights[RESET_LIGHT].setSmoothBrightness(resetButtonTriggered, args.sampleTime);
	bool resetTriggered = resetTrigger.process(inputs[RESET_INPUT].getVoltage(), 0.1f, 2.f);
	if (resetButtonTriggered || resetTriggered) {
		resetClock() ;
		counter = 0 ;
		pgen.trigger(trigTime);
		params[RESET_PARAM].setValue(0.f) ;
	}

	// Handling run button
	if(params[RUN_PARAM].getValue() == 1 || inputs[RUN_INPUT].getVoltage() > 0.f){
		lights[RUN_LIGHT].setBrightness(1.f) ;
		running = true ;
		counter++;
	}else{
		lights[RUN_LIGHT].setBrightness(0.f) ;
		running = false ;
	}

	// Running clock
	if (counter > period && running) {
		pgen.trigger(trigTime);
		counter -= period; // keep the fractional part
	}

	float out = pgen.process( args.sampleTime );
	lights[CLOCK_LIGHT].setSmoothBrightness(out, args.sampleTime);

	//if(out){lights[CLOCK_LIGHT].setBrightness(1.f);}
	//else{lights[CLOCK_LIGHT].setBrightness(0.f);}

	outputs[CLOCK_OUTPUT].setVoltage(10.f * out);
}

struct BPMDisplayWidget : OpaqueWidget {
	/* Text display custom widget */

	std::basic_string<char> fontPath; // Chosen font
	MyClock *module;

	// Loading chosen font
	BPMDisplayWidget(MyClock *_module) : OpaqueWidget(),module(_module) {
		fontPath=asset::plugin(pluginInstance,"res/DSEG7ClassicMini-Bold.ttf");
	}

	// Draw layer for dark mode
	void drawLayer(const DrawArgs &args,int layer) override {
		if(layer==1) {_draw(args);}
		Widget::drawLayer(args,layer);
	}

	// Draw widget
	void _draw(const DrawArgs &args) {

		std::string text="090"; // Default text value

		// Loading text value from BPM knob
		if(module){
			int currentBpm  = module->getBpm() ;
			std::string stringBpm = std::to_string(currentBpm);
			std::string padding = "" ; // Padding : "001", "010" or "100"
			while(padding.length()+stringBpm.length() < 3){padding += "0";}
			text = padding+stringBpm;
		}

		// Config text
		std::shared_ptr<Font> font=APP->window->loadFont(fontPath);
		nvgFillColor(args.vg, nvgRGB(255,255,128));
		nvgFontFaceId(args.vg, font->handle);
		nvgFontSize(args.vg, 28);
		nvgTextAlign(args.vg, NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
		nvgText(args.vg, box.size.x/2, box.size.y/2, text.c_str(), nullptr);
	}
};

struct MULTDisplayWidget : OpaqueWidget {
	/* Text display custom widget */

	std::basic_string<char> fontPath; // Chosen font
	MyClock *module;

	// Loading chosen font
	MULTDisplayWidget (MyClock *_module) : OpaqueWidget(),module(_module) {
		fontPath=asset::plugin(pluginInstance,"res/DSEG7ClassicMini-Bold.ttf");
	}

	// Draw layer for dark mode
	void drawLayer(const DrawArgs &args,int layer) override {
		if(layer==1) {_draw(args);}
		Widget::drawLayer(args,layer);
	}

	// Draw widget
	void _draw(const DrawArgs &args) {

		std::string text="1"; // Default text value

		// Loading text value from BPM knob
		if(module){
			int currentMul  = module->getMul() ;
			std::string stringMul = std::to_string(currentMul);
			std::string padding = "" ; // Padding : "01", "10"
			while(padding.length()+stringMul.length() < 2){padding += "0";}
			text = padding+stringMul;
		}

		// Config text
		std::shared_ptr<Font> font=APP->window->loadFont(fontPath);
		nvgFillColor(args.vg, nvgRGB(255,255,128));
		nvgFontFaceId(args.vg, font->handle);
		nvgFontSize(args.vg, 14);
		nvgTextAlign(args.vg, NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
		nvgText(args.vg, box.size.x/2, box.size.y/2, text.c_str(), nullptr);
	}
};

struct MyClockWidget : ModuleWidget {
	MyClockWidget(MyClock* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/MyClock.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<Trimpot>(mm2px(Vec(28.042, 43.048)), module, MyClock::MULT_PARAM));
		addParam(createParamCentered<Rogan5PSGray>(mm2px(Vec(15.037, 52.042)), module, MyClock::TEMPO_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(34.544, 56.54)), module, MyClock::TEMPOGAIN_PARAM));
		addParam(createLightParamCentered<VCVLightLatch<MediumSimpleLight<BlueLight>>>(mm2px(Vec(6.096, 74.774)), module, MyClock::RUN_PARAM, MyClock::RUN_LIGHT));
		addParam(createLightParamCentered<VCVLightLatch<MediumSimpleLight<BlueLight>>>(mm2px(Vec(34.544, 74.774)), module, MyClock::RESET_PARAM, MyClock::RESET_LIGHT));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(21.16, 87.388)), module, MyClock::CVGAIN_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(10.795, 100.525)), module, MyClock::TRIGTIME_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(28.448, 62.322)), module, MyClock::BPM_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(14.224, 74.774)), module, MyClock::RUN_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(26.416, 74.774)), module, MyClock::RESET_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.795, 87.296)), module, MyClock::CVTRIG_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(29.316, 100.525)), module, MyClock::CLOCK_OUTPUT));

		// Displaying BPM and mult/div
		auto bpmDisplay = new BPMDisplayWidget(module);
		bpmDisplay->box.pos = mm2px(Vec(14.834-(24.790/2), 29.555-(12.850/2))); //Widget pos - (size / 2)
		bpmDisplay->box.size = mm2px(Vec(24.790, 12.850));
		addChild(bpmDisplay);
		auto multDisplay = new MULTDisplayWidget(module);
		multDisplay->box.pos = mm2px(Vec(32.918-(10.566/2), 29.555-(12.850/2)));
		multDisplay->box.size = mm2px(Vec(10.566, 12.850));
		addChild(multDisplay);

		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(34.760, 96.786)), module, MyClock::CLOCK_LIGHT));
	}
};

Model* modelMyClock = createModel<MyClock, MyClockWidget>("MyClock");
