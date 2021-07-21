#include "plugin.hpp"


Plugin* pluginInstance;


void init(Plugin* p) {
	pluginInstance = p;

	p->addModel(modelDogs);
	p->addModel(modelPigs);
	p->addModel(modelSheep);
}
