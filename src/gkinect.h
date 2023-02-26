/* godot-cpp integration testing project.
 *
 * This is free and unencumbered software released into the public domain.
 */

#ifndef GKINECT_CLASS_H
#define GKINECT_CLASS_H

#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/defs.hpp>

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/file_access.hpp>

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/viewport.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include <fdeep/fdeep.hpp>

using namespace godot;

// class ResourceFormatLoaderGKinect : public ResourceFormatLoader {
//     GDCLASS(ResourceFormatLoaderGKinect, ResourceFormatLoader);
// public:
//     virtual RES load(const String &p_path, const String &p_original_path, Error *r_error = NULL);
//     virtual void get_recognized_extensions(List<String> *r_extensions) const;
//     virtual bool handles_type(const String &p_type) const;
//     virtual String get_resource_type(const String &p_path) const;
// }

// class GkPoseResource : public Resource {
//     GDCLASS(GkPoseResource, Resource);

// private:
// 	static fdeep::model model;

// public:
// 	static fdeep::model get_model();
//     Error load_file(const String &p_path);
// };

class PoseGroup : public Object {
	GDCLASS(PoseGroup, Object);

	String modelLoc = "";
	bool loaded = false;

protected:
	static void _bind_methods();

public:

	PoseGroup() {}
	~PoseGroup() {}

	int check_for_poses(int p_skeleton_num);

	void load_model(String p_path);
};

class GKinect : public Object {
	GDCLASS(GKinect, Object);

	static GKinect* singleton;

protected:
	static void _bind_methods();

	String _to_string() const;

public:
	static GKinect* get_singleton();

	GKinect();
	~GKinect() {}
	Array get_bodies();

	Vector2 getColorPoint(int p_id);
};


#endif