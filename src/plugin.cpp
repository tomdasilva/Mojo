#include "plugin.hpp"


Plugin* pluginInstance;


void init(Plugin* p) {
	pluginInstance = p;

	p->addModel(modelDogs);
	p->addModel(modelPigs);
	p->addModel(modelSheep);

	p->addModel(modelMyComparator);
	p->addModel(modelMyClock);
	p->addModel(modelMyDivider);
}
