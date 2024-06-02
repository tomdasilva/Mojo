#include "MyComparator.hpp"


void MyComparator::process(const ProcessArgs& args){
	/* Main DSP method */

	// Getting Threshold value + CV modulation
	float thresh = params[THRESH_PARAM].getValue() ;
	float prec   = params[PRECISION_PARAM].getValue() ;
	if(inputs[CVTHRESH_INPUT].isConnected())
		thresh += inputs[CVTHRESH_INPUT].getVoltage() * params[CVGAIN_PARAM].getValue() ;

	// Polyphony channels
	int channels = std::max(1, inputs[INA_INPUT].getChannels());

	// Comparing input A to constant threshold #####################################
	if(inputs[INA_INPUT].isConnected() && not inputs[INB_INPUT].isConnected()){

		// Setting up lights for threshold button control
		lights[LEDTHR_LIGHT].setBrightness(1.f) ;
		lights[LEDHYST_LIGHT].setBrightness(0.f);

		// Polyphony
		for(int c=0 ; c<channels ; c++){
			float outG  = inputs[INA_INPUT].getPolyVoltage(c) >  thresh ;
			float outL  = inputs[INA_INPUT].getPolyVoltage(c) <  thresh ;
			float outEQ = (inputs[INA_INPUT].getPolyVoltage(c) > thresh - prec) && (inputs[INA_INPUT].getPolyVoltage(c) < thresh + prec) ;
			outputs[OUTG_OUTPUT].setVoltage(10.f*outG, c) ;
			outputs[OUTL_OUTPUT].setVoltage(10.f*outL, c) ;
			outputs[OUTEQ_OUTPUT].setVoltage(10.f*outEQ, c) ;
		}
		outputs[OUTG_OUTPUT].setChannels(channels) ;
		outputs[OUTL_OUTPUT].setChannels(channels) ;
		outputs[OUTEQ_OUTPUT].setChannels(channels) ;


	// Comparing input A and B signals #############################################
	}else if(inputs[INA_INPUT].isConnected() && inputs[INB_INPUT].isConnected()){

		// Setting up lights for threshold button control
		lights[LEDTHR_LIGHT].setBrightness(0.f) ;
		lights[LEDHYST_LIGHT].setBrightness(1.f);

		// Polyphony (IN A) with monophonic IN B
		if(inputs[INB_INPUT].getChannels() == 1){
			for(int c=0 ; c<channels ; c++){
				float outG = inputs[INA_INPUT].getPolyVoltage(c) >   (inputs[INB_INPUT].getVoltage() + thresh) ;
				float outL = inputs[INA_INPUT].getPolyVoltage(c) <   (inputs[INB_INPUT].getVoltage() - thresh) ;
				float outEQ = (inputs[INA_INPUT].getPolyVoltage(c) > (inputs[INB_INPUT].getVoltage() + thresh) - prec) && (inputs[INA_INPUT].getPolyVoltage(c) < (inputs[INB_INPUT].getVoltage() - thresh) + prec) ;
				outputs[OUTG_OUTPUT].setVoltage(10.f*outG, c) ;
				outputs[OUTL_OUTPUT].setVoltage(10.f*outL, c) ;
				outputs[OUTEQ_OUTPUT].setVoltage(10.f*outEQ, c) ;
			}

		// Both IN A and IN B are polyphonic
		}else{
			int channels = std::max(1, std::min(inputs[INA_INPUT].getChannels(), inputs[INB_INPUT].getChannels()));
			for(int c=0 ; c<channels ; c++){
				float outG = inputs[INA_INPUT].getPolyVoltage(c) >   (inputs[INB_INPUT].getPolyVoltage(c) + thresh) ;
				float outL = inputs[INA_INPUT].getPolyVoltage(c) <   (inputs[INB_INPUT].getPolyVoltage(c) - thresh) ;
				float outEQ = (inputs[INA_INPUT].getPolyVoltage(c) > (inputs[INB_INPUT].getPolyVoltage(c) + thresh) - prec) && (inputs[INA_INPUT].getPolyVoltage(c) < (inputs[INB_INPUT].getPolyVoltage(c) - thresh) + prec) ;
				outputs[OUTG_OUTPUT].setVoltage(10.f*outG, c) ;
				outputs[OUTL_OUTPUT].setVoltage(10.f*outL, c) ;
				outputs[OUTEQ_OUTPUT].setVoltage(10.f*outEQ, c) ;
			}
		}
		outputs[OUTG_OUTPUT].setChannels(channels) ;
		outputs[OUTL_OUTPUT].setChannels(channels) ;
		outputs[OUTEQ_OUTPUT].setChannels(channels) ;

	// Switching off LEDs #########################################################
	}else{
		lights[LEDTHR_LIGHT].setBrightness(0.f) ;
		lights[LEDHYST_LIGHT].setBrightness(0.f);
		outputs[OUTG_OUTPUT].setVoltage(0.f) ;
		outputs[OUTL_OUTPUT].setVoltage(0.f) ;
		outputs[OUTEQ_OUTPUT].setVoltage(0.f) ;
	}
}


struct MyComparatorWidget : ModuleWidget {
	MyComparatorWidget(MyComparator* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/MyComparator.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(7.62, 64.625)), module, MyComparator::THRESH_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(18.288, 78.118)), module, MyComparator::CVGAIN_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(23.533, 98.302)), module, MyComparator::PRECISION_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.534, 24.378)), module, MyComparator::INA_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(21.946, 24.378)), module, MyComparator::INB_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.62, 78.118)), module, MyComparator::CVTHRESH_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(8.534, 41.884)), module, MyComparator::OUTG_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(21.946, 41.884)), module, MyComparator::OUTL_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(8.534, 98.379)), module, MyComparator::OUTEQ_OUTPUT));

		addChild(createLightCentered<MediumLight<BlueLight>>(mm2px(Vec(20.422, 59.602)), module, MyComparator::LEDTHR_LIGHT));
		addChild(createLightCentered<MediumLight<BlueLight>>(mm2px(Vec(20.422, 69.625)), module, MyComparator::LEDHYST_LIGHT));
	}
};

Model* modelMyComparator = createModel<MyComparator, MyComparatorWidget>("MyComparator");
