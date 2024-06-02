#include "MyDivider.hpp"
#include "MyClock.hpp"

void MyDivider::process(const ProcessArgs& args){

	Module* leftModule = getLeftExpander().module;
	float clock = 0.f ;
	int ratio = 1 ;

	// Expander section to get clock
	if(not inputs[CLOCK_INPUT].isConnected()){
		if (leftModule && leftModule->model == modelMyClock)
			clock = leftModule->getOutput(MyClock::CLOCK_OUTPUT).getVoltage() ;
	}else{clock = inputs[CLOCK_INPUT].getVoltage() ;} // Input beeing used as clock

	lights[CLOCK_LIGHT].setSmoothBrightness(clock, args.sampleTime) ;

	if(edgeDetector.process(clock))
		for(int c=0 ; c<MyDivider::OUTPUTS_LEN ; c++){counters[c] ++ ;}

	for(int c=0 ; c<8 ; c++){
		if(outputs[OUT_OUTPUT+c].isConnected()){
			ratio = (int)params[RATIO_PARAM+c].getValue() ;
			if(counters[c] % ratio == 0){
				outputs[OUT_OUTPUT+c].setVoltage(clock) ;
				counters[c] = 0 ;
			}else
				outputs[OUT_OUTPUT+c].setVoltage(0.f) ;
		}
	}
}

struct MyDividerWidget : ModuleWidget {
	MyDividerWidget(MyDivider* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/MyDivider.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH-4, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH+4, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH-4, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH+4, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		for(int c=0 ; c<MyDivider::PARAMS_LEN ; c++){
			addParam(createParamCentered<Trimpot>(mm2px(Vec(6.195, 42.848+(float)(9.f*c))), module, MyDivider::RATIO_PARAM+c));
		}

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.16, 23.13)), module, MyDivider::CLOCK_INPUT));

		for(int c=0 ; c<MyDivider::OUTPUTS_LEN ; c++){
			addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(14.323, 42.848+(float)(9.f*c))), module, MyDivider::OUT_OUTPUT+c));
		}

		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(17.069, 23.13)), module, MyDivider::CLOCK_LIGHT));
	}
};


Model* modelMyDivider = createModel<MyDivider, MyDividerWidget>("MyDivider");
