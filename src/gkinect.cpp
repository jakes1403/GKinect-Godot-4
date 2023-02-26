 /* godot-cpp integration testing project.
 *
 * This is free and unencumbered software released into the public domain.
 */

#include <Windows.h>
#include <Ole2.h>

#include <Kinect.h>

#include "gkinect.h"

#include <vector>
#include <string>

using namespace godot;

// C:/Users/MSU/Downloads/frugally-deep-master/frugally-deep-master/keras_export/model.json

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

IKinectSensor* sensor = NULL;
IMultiSourceFrameReader* reader = NULL;
ICoordinateMapper* mapper = NULL;

BOOLEAN tracked;

GKinect* GKinect::singleton = nullptr;

Joint bodies[BODY_COUNT][JointType_Count];

GKinect* GKinect::get_singleton()
{
	return singleton;
}

String GKinect::_to_string() const {
	return "[ GDExtension::GKinect ]";
}

void PoseGroup::load_model(String p_path)
{
    PoseGroup::modelLoc = p_path;
}

int PoseGroup::check_for_poses(int p_skeleton_num)
{
    //int noUse[] = { 15,19,21,23,22,24 };

    std::vector<float> bodyNet(25* 3);

    int c = 0;

    for (int i = 0; i < 25; i++) {
        // if (std::find(std::begin(noUse), std::end(noUse), i) != std::end(noUse))
        //     continue;
        bodyNet[c * 3] = bodies[p_skeleton_num][i].Position.X;
        bodyNet[(c * 3) + 1] = bodies[p_skeleton_num][i].Position.Y;
        bodyNet[(c * 3) + 2] = bodies[p_skeleton_num][i].Position.Z;

        c++;
    }

    //file->close();
    
    const auto input = {fdeep::tensor(fdeep::tensor_shape(static_cast<std::size_t>(25 * 3)),
        bodyNet)};

    //const auto result = model.predict(input);

    //if (!PoseGroup::loaded)
    //{
   // }

    Ref<FileAccess> file = FileAccess::open(modelLoc, FileAccess::READ);

    static const auto model = fdeep::read_model_from_string(std::string(file->get_line().utf8().get_data()));
    PoseGroup::loaded = true;
    

    const auto result = model.predict_class({input});
    return int(result);
    
    //const auto input = fdeep::from
    
    //std::cout << fdeep::show_tensors(result) << std::endl;
    
}

void GKinect::_bind_methods() {
    //getColorPoint
	ClassDB::bind_method(D_METHOD("get_bodies"), &GKinect::get_bodies);
    ClassDB::bind_method(D_METHOD("get_color_point", "id"), &GKinect::getColorPoint);
}

void PoseGroup::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("load_model", "path"), &PoseGroup::load_model);
    ClassDB::bind_method(D_METHOD("check_for_poses", "skeleton_num"), &PoseGroup::check_for_poses);
}

inline Vector3 JointToVec(Joint j)
{
    return Vector3(j.Position.X, j.Position.Y, j.Position.Z);
}


inline const char* JointToName(int j)
{
    switch (j)
    {
        case JointType_SpineBase:
            return "spine_base";
        case JointType_SpineMid:
            return "spine_mid";
        case JointType_Neck:
            return "neck";
        case JointType_Head:
            return "head";
        case JointType_ShoulderLeft:
            return "shoulder_left";
        case JointType_ElbowLeft:
            return "elbow_left";
        case JointType_WristLeft:
            return "wrist_left";
        case JointType_HandLeft:
            return "hand_left";
        case JointType_ShoulderRight:
            return "shoulder_right";
        case JointType_ElbowRight:
            return "elbow_right";
        case JointType_WristRight:
            return "wrist_right";
        case JointType_HandRight:
            return "hand_right";
        case JointType_HipLeft:
            return "hip_left";
        case JointType_KneeLeft:
            return "knee_left";
        case JointType_AnkleLeft:
            return "ankle_left";
        case JointType_FootLeft:
            return "foot_left";
        case JointType_HipRight:
            return "hip_right";
        case JointType_KneeRight:
            return "knee_right";
        case JointType_AnkleRight:
            return "ankle_right";
        case JointType_FootRight:
            return "foot_right";
        case JointType_SpineShoulder:
            return "spine_shoulder";
        case JointType_HandTipLeft:
            return "hand_tip_left";
        case JointType_ThumbLeft:
            return "thumb_left";
        case JointType_HandTipRight:
            return "hand_tip_right";
        case JointType_ThumbRight:
            return "thumb_right";
        default:
            return "null";
    }
}

Array GKinect::get_bodies()
{
    static int tracked_bodies = 0;

    IMultiSourceFrame* frame = NULL;
    if (SUCCEEDED(reader->AcquireLatestFrame(&frame)))
    {
        IBodyFrame* bodyframe;
        IBodyFrameReference* bodyframeref = NULL;
        frame->get_BodyFrameReference(&bodyframeref);
        bodyframeref->AcquireFrame(&bodyframe);
        if (bodyframeref) bodyframeref->Release();

        if (bodyframe)
        {
            IBody* body[BODY_COUNT] = { 0 };
            bodyframe->GetAndRefreshBodyData(BODY_COUNT, body);
            tracked_bodies = 0;
            for (int i = 0; i < BODY_COUNT; i++)
            {
                body[i]->get_IsTracked(&tracked);
                if (tracked)
                {
                    body[i]->GetJoints(JointType_Count, &bodies[tracked_bodies++][0]);
                }
            }

            if (bodyframe) bodyframe->Release();
        }


        if (frame)
        {
            frame->Release();
        }


    }

    Array bodies_array;
    
    for (int f = 0; f < tracked_bodies; f++)
    {
        Dictionary skeleton;
        skeleton["id"] = f;
        for (int i = 0; i < JointType_Count; i++)
        {
            skeleton[JointToName(i)] = JointToVec(bodies[f][i]);
        }

        bodies_array.push_back(skeleton);
    }

    //UtilityFunctions::print(output);

    return bodies_array;
    
}

Vector2 GKinect::getColorPoint(int p_id)
{
    ColorSpacePoint point;
	mapper->MapCameraPointToColorSpace(bodies[0][p_id].Position, &point);

    return Vector2(point.X, point.Y);

}

GKinect::GKinect()
{

	singleton = this;
	static bool already_initialized = false;
	if (!already_initialized)
    {
        if (FAILED(GetDefaultKinectSensor(&sensor)))
        {
            return;
        }

        if (sensor)
        {
            sensor->get_CoordinateMapper(&mapper);
            sensor->Open();

            sensor->OpenMultiSourceFrameReader(FrameSourceTypes::FrameSourceTypes_Body,
                &reader);
        }
        else
        {
            return;
        }
        already_initialized = true;
    }
}