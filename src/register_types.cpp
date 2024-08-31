/* godot-cpp integration testing project.
 *
 * This is free and unencumbered software released into the public domain.
 */

#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/godot.hpp>

#include "gkinect.h"
#include "tests.h"

using namespace godot;

static GKinect* gsingleton;

void initialize_gkinect_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	
	ClassDB::register_class<PoseGroup>("path");
	ClassDB::register_class<GKinect>();

	gsingleton = memnew(GKinect);
    Engine::get_singleton()->register_singleton("GKinect", GKinect::get_singleton());
}

void uninitialize_gkinect_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	memdelete(gsingleton);
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT gkinect_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_gkinect_module);
	init_obj.register_terminator(uninitialize_gkinect_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
