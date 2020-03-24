#include "Interface.hxx"
#include "TestFilter.hxx"

VS_EXTERNAL_API(auto) VapourSynthPluginInit(VSConfigPlugin configFunc, VSRegisterFunction registerFunc, VSPlugin* plugin) {
	VaporGlobals::Identifier = "com.vsfilterscript.test";
	VaporGlobals::Namespace = "test";
	VaporGlobals::Description = "Test filters for vsFilterScript";
	VaporInterface::RegisterPlugin(configFunc, plugin);
	VaporInterface::RegisterFilter<Test>(registerFunc, plugin);
}
