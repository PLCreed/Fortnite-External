#include "includes.hpp"
int screen_width = GetSystemMetrics(SM_CXSCREEN);
int screen_height = GetSystemMetrics(SM_CYSCREEN);

namespace Identifiers {
    uintptr_t uworld;
    uintptr_t gobjects;
    uintptr_t game_instance;
    uintptr_t local_players;
    uintptr_t player_controller;
    uintptr_t local_pawn;
    uintptr_t root_component;
    uintptr_t player_state;
    uintptr_t game_state;
    uintptr_t player_array;
    uintptr_t closest_pawn;
    float closest_distance;
    Vector3 local_relative_location;
}

namespace game {
	namespace offsets {
        uintptr_t UWORLD = 0x1133C4C8;
        uintptr_t Viewpoint = 0xE49C7E0;

        uintptr_t Gameinstance = 0x1D0;
        uintptr_t LocalPlayers = 0x38;
        uintptr_t PlayerController = 0x30;
        uintptr_t LocalPawn = 0x338;
        uintptr_t PlayerState = 0x2B0;
        uintptr_t RootComponet = 0x198;
        uintptr_t GameState = 0x158;
        uintptr_t PersistentLevel = 0x30;
        uintptr_t LastSubmitTime = 0x368;
        uintptr_t LastRenderTimeOnScreen = 0x370;

        uintptr_t ActorCount = 0xA8;

        uintptr_t Cameramanager = 0x348;

        uintptr_t AActor = 0xA0;

        uintptr_t CurrentActor = 0x8;

        uintptr_t Mesh = 0x318;

        uintptr_t Revivefromdbnotime = 0x4AE0;

        uintptr_t TeamId = 0x10E0;

        uintptr_t ActorTeamId = 0x10E0;

        uintptr_t WeaponData = 0x498;

        uintptr_t tier = 0x73;

        uintptr_t LocalActorPos = 0x120;

        uintptr_t ComponetToWorld = 0x240;

        uintptr_t BoneArray = 0x608;
        uintptr_t Bonecache = 0x618;

        uintptr_t Velocity = 0x168;

        uintptr_t Private = 0x308;

        uintptr_t PlayerArray = 0x2A8;

        uintptr_t relativelocation = 0x120;

        uintptr_t UCharacterMovementComponent = 0x318;

        uintptr_t entity_actor = 0x310;

        uintptr_t bIsReloadingWeapon = 0x360;

        uintptr_t GlobalAnimRateScale = 0xA80;

        uintptr_t CurrentWeapon = 0xa20;

        uintptr_t Wireframe = 0x194;

        uintptr_t SkeletalMeshes = 0x56e;

        uintptr_t PawnMaterials_ALL = 0x5A60;
	}

    struct Camera
    {
        Vector3 Location;
        Vector3 Rotation;
        float FieldOfView;
        double zoom;
    };

    struct FQuat
    {
        double x;
        double y;
        double z;
        double w;
    };


    struct FTransform
    {
        FQuat rot;
        Vector3 translation;
        char pad[4];
        Vector3 scale;
        char pad1[4];
        D3DMATRIX ToMatrixWithScale()
        {
            D3DMATRIX m{};
            m._41 = translation.x;
            m._42 = translation.y;
            m._43 = translation.z;
            float x2 = rot.x + rot.x;
            float y2 = rot.y + rot.y;
            float z2 = rot.z + rot.z;
            float xx2 = rot.x * x2;
            float yy2 = rot.y * y2;
            float zz2 = rot.z * z2;
            m._11 = (1.0f - (yy2 + zz2)) * scale.x;
            m._22 = (1.0f - (xx2 + zz2)) * scale.y;
            m._33 = (1.0f - (xx2 + yy2)) * scale.y;
            float yz2 = rot.y * z2;
            float wx2 = rot.w * x2;
            m._32 = (yz2 - wx2) * scale.z;
            m._23 = (yz2 + wx2) * scale.y;
            float xy2 = rot.x * y2;
            float wz2 = rot.w * z2;
            m._21 = (xy2 - wz2) * scale.y;
            m._12 = (xy2 + wz2) * scale.x;
            float xz2 = rot.x * z2;
            float wy2 = rot.w * y2;
            m._31 = (xz2 + wy2) * scale.z;
            m._13 = (xz2 - wy2) * scale.x;
            m._14 = 0.0f;
            m._24 = 0.0f;
            m._34 = 0.0f;
            m._44 = 1.0f;
            return m;
        }
    };

    D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
    {
        D3DMATRIX pOut{};
        pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
        pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
        pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
        pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
        pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
        pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
        pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
        pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
        pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
        pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
        pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
        pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
        pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
        pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
        pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
        pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;
        return pOut;
    }

    D3DMATRIX Matrix(Vector3 rot, Vector3 origin = Vector3(0, 0, 0))
    {
        float radPitch = (rot.x * float(M_PI) / 180.f);
        float radYaw = (rot.y * float(M_PI) / 180.f);
        float radRoll = (rot.z * float(M_PI) / 180.f);

        float SP = sinf(radPitch);
        float CP = cosf(radPitch);
        float SY = sinf(radYaw);
        float CY = cosf(radYaw);
        float SR = sinf(radRoll);
        float CR = cosf(radRoll);

        D3DMATRIX matrix;
        matrix.m[0][0] = CP * CY;
        matrix.m[0][1] = CP * SY;
        matrix.m[0][2] = SP;
        matrix.m[0][3] = 0.f;

        matrix.m[1][0] = SR * SP * CY - CR * SY;
        matrix.m[1][1] = SR * SP * SY + CR * CY;
        matrix.m[1][2] = -SR * CP;
        matrix.m[1][3] = 0.f;

        matrix.m[2][0] = -(CR * SP * CY + SR * SY);
        matrix.m[2][1] = CY * SR - CR * SP * SY;
        matrix.m[2][2] = CR * CP;
        matrix.m[2][3] = 0.f;

        matrix.m[3][0] = origin.x;
        matrix.m[3][1] = origin.y;
        matrix.m[3][2] = origin.z;
        matrix.m[3][3] = 1.f;

        return matrix;
    }

    Vector3 GetBoneWithRotation(uintptr_t Mesh, int Id) {
        uintptr_t bone_array = Memory.Read<uintptr_t>(Mesh + 0x608);
        if (bone_array == NULL) 
            bone_array = Memory.Read<uintptr_t>(Mesh + 0x608 + 0x10);//BoneArray
        FTransform bone = Memory.Read<FTransform>(bone_array + (Id * 0x60));
        FTransform component_to_world = Memory.Read<FTransform>(Mesh + 0x230);
        D3DMATRIX matrix = MatrixMultiplication(bone.ToMatrixWithScale(), component_to_world.ToMatrixWithScale());
        return Vector3(matrix._41, matrix._42, matrix._43);
    }

    bool IsPlayerVisible(uintptr_t Mesh) {
        float last_sumbit_time = Memory.Read<float>(Mesh + 0x358);
        float last_render_time_on_screen = Memory.Read<float>(Mesh + 0x360);
        bool visible = last_render_time_on_screen + 0.06f >= last_sumbit_time;
        return visible;
    }

    class FRotator
    {
    public:
        FRotator() : Pitch(0.f), Yaw(0.f), Roll(0.f)
        {

        }

        FRotator(double _Pitch, double _Yaw, double _Roll) : Pitch(_Pitch), Yaw(_Yaw), Roll(_Roll)
        {

        }
        ~FRotator()
        {

        }

        double Pitch;
        double Yaw;
        double Roll;
        inline FRotator get() {
            return FRotator(Pitch, Yaw, Roll);
        }
        inline void set(double _Pitch, double _Yaw, double _Roll) {
            Pitch = _Pitch;
            Yaw = _Yaw;
            Roll = _Roll;
        }

        inline FRotator Clamp() {
            FRotator result = get();
            if (result.Pitch > 180)
                result.Pitch -= 360;
            else if (result.Pitch < -180)
                result.Pitch += 360;
            if (result.Yaw > 180)
                result.Yaw -= 360;
            else if (result.Yaw < -180)
                result.Yaw += 360;
            if (result.Pitch < -89)
                result.Pitch = -89;
            if (result.Pitch > 89)
                result.Pitch = 89;
            while (result.Yaw < -180.0f)
                result.Yaw += 360.0f;
            while (result.Yaw > 180.0f)
                result.Yaw -= 360.0f;

            result.Roll = 0;
            return result;

        }
        double Length() {
            return sqrt(Pitch * Pitch + Yaw * Yaw + Roll * Roll);
        }

        FRotator operator+(FRotator angB) { return FRotator(Pitch + angB.Pitch, Yaw + angB.Yaw, Roll + angB.Roll); }
        FRotator operator-(FRotator angB) { return FRotator(Pitch - angB.Pitch, Yaw - angB.Yaw, Roll - angB.Roll); }
        FRotator operator/(float flNum) { return FRotator(Pitch / flNum, Yaw / flNum, Roll / flNum); }
        FRotator operator*(float flNum) { return FRotator(Pitch * flNum, Yaw * flNum, Roll * flNum); }
        bool operator==(FRotator angB) { return Pitch == angB.Pitch && Yaw == angB.Yaw && Yaw == angB.Yaw; }
        bool operator!=(FRotator angB) { return Pitch != angB.Pitch || Yaw != angB.Yaw || Yaw != angB.Yaw; }

    };

    double __fastcall Atan2(double a1, double a2)
    {
        double result; // xmm0_8

        result = 0.0;
        if (a2 != 0.0 || a1 != 0.0)
            return atan2(a1, a2);
        return result;
    }
    double __fastcall FMod(double a1, double a2)
    {
        if (fabs(a2) > 0.00000001)
            return fmod(a1, a2);
        else
            return 0.0;
    }

    double ClampAxis(double Angle)
    {
        // returns Angle in the range (-360,360)
        Angle = FMod(Angle, (double)360.0);

        if (Angle < (double)0.0)
        {
            // shift to [0,360) range
            Angle += (double)360.0;
        }

        return Angle;
    }

    double NormalizeAxis(double Angle)
    {
        // returns Angle in the range [0,360)
        Angle = ClampAxis(Angle);

        if (Angle > (double)180.0)
        {
            // shift to (-180,180]
            Angle -= (double)360.0;
        }

        return Angle;
    }

    FRotator Rotator(FQuat* F)
    {
        const double SingularityTest = F->z * F->x - F->w * F->y;
        const double YawY = 2.f * (F->w * F->z + F->x * F->y);
        const double YawX = (1.f - 2.f * ((F->y * F->y) + (F->z * F->z)));

        const double SINGULARITY_THRESHOLD = 0.4999995f;
        const double RAD_TO_DEG = 57.295776;
        double Pitch, Yaw, Roll;

        if (SingularityTest < -SINGULARITY_THRESHOLD)
        {
            Pitch = -90.f;
            Yaw = (Atan2(YawY, YawX) * RAD_TO_DEG);
            Roll = NormalizeAxis(-Yaw - (2.f * Atan2(F->x, F->w) * RAD_TO_DEG));
        }
        else if (SingularityTest > SINGULARITY_THRESHOLD)
        {
            Pitch = 90.f;
            Yaw = (Atan2(YawY, YawX) * RAD_TO_DEG);
            Roll = NormalizeAxis(Yaw - (2.f * Atan2(F->x, F->w) * RAD_TO_DEG));
        }
        else
        {
            Pitch = (asin(2.f * SingularityTest) * RAD_TO_DEG);
            Yaw = (Atan2(YawY, YawX) * RAD_TO_DEG);
            Roll = (Atan2(-2.f * (F->w * F->x + F->y * F->z), (1.f - 2.f * ((F->x * F->x) + (F->y * F->y)))) * RAD_TO_DEG);
        }

        FRotator RotatorFromQuat = FRotator{ Pitch, Yaw, Roll };
        return RotatorFromQuat;
    }
    struct CamInfo
    {
        float fov;
        Vector3 rotation;
        Vector3 location;
    };

    CamInfo GetCamera()
    {
        CamInfo VirtualCamera;

        auto location_pointer = Memory.Read<uintptr_t>(Identifiers::uworld + 0x110);
        auto rotation_pointer = Memory.Read<uintptr_t>(Identifiers::uworld + 0x120);

        struct FNRot
        {
            double a; //0x0000
            char pad_0008[24]; //0x0008
            double b; //0x0020
            char pad_0028[424]; //0x0028
            double c; //0x01D0
        }fnRot;

        fnRot.a = Memory.Read<double>(rotation_pointer);
        fnRot.b = Memory.Read<double>(rotation_pointer + 0x20);
        fnRot.c = Memory.Read<double>(rotation_pointer + 0x1d0);

        VirtualCamera.location = Memory.Read<Vector3>(location_pointer);
        VirtualCamera.rotation.x = asin(fnRot.c) * (180.0 / M_PI);
        VirtualCamera.rotation.y = ((atan2(fnRot.a * -1, fnRot.b) * (180.0 / M_PI)) * -1) * -1;
        VirtualCamera.fov = Memory.Read<float>((uintptr_t)Identifiers::player_controller + 0x394) * 90.f;

        return VirtualCamera;
    }


    Vector3 ProjectWorldToScreen(Vector3 WorldLocation)
    {
        CamInfo vCamera = GetCamera();
        //vCamera.rotation.x = (asin(vCamera.rotation.x)) * (180.0 / M_PI);
        D3DMATRIX tempMatrix = Matrix(vCamera.rotation, Vector3(0,0,0));
        Vector3 Screenlocation = Vector3(0, 0, 0);

        Vector3 vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
        Vector3 vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
        Vector3 vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);
        Vector3 vDelta = WorldLocation - vCamera.location;
        Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

        if (vTransformed.z < 1.f)
            vTransformed.z = 1.f;

        //return Vector3((screen_width / 2.0f) + vTransformed.x * (((screen_width / 2.0f) / tanf(vCamera.fov * (float)M_PI / 360.f))) / vTransformed.z, (screen_height / 2.0f) - vTransformed.y * (((screen_width / 2.0f) / tanf(vCamera.fov * (float)M_PI / 360.f))) / vTransformed.z, 0);
        float ScreenCenterX = screen_width / 2.0f;
        float ScreenCenterY = screen_height / 2.0f;
        Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(vCamera.fov * (float)M_PI / 360.f)) / vTransformed.z;
        Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(vCamera.fov * (float)M_PI / 360.f)) / vTransformed.z;

        return Screenlocation;
    }

    Vector3 RotatePoint(Vector3 EntityPos, Vector3 LocalPlayerPos, int posX, int posY, int sizeX, int sizeY, float angle, float zoom, bool angleInRadians)
    {
        float r_1, r_2;
        float x_1, y_1;

        r_1 = -(EntityPos.y - LocalPlayerPos.y);
        r_2 = EntityPos.x - LocalPlayerPos.x;

        float Yaw = angle - 90.0f;

        float yawToRadian = Yaw * (float)(M_PI / 180.0F);
        x_1 = (float)(r_2 * (float)cos((double)(yawToRadian)) - r_1 * sin((double)(yawToRadian))) / 20;
        y_1 = (float)(r_2 * (float)sin((double)(yawToRadian)) + r_1 * cos((double)(yawToRadian))) / 20;

        x_1 *= zoom;
        y_1 *= zoom;

        int sizX = sizeX / 2;
        int sizY = sizeY / 2;

        x_1 += sizX;
        y_1 += sizY;

        if (x_1 < 5)
            x_1 = 5;

        if (x_1 > sizeX - 5)
            x_1 = sizeX - 5;

        if (y_1 < 5)
            y_1 = 5;

        if (y_1 > sizeY - 5)
            y_1 = sizeY - 5;


        x_1 += posX;
        y_1 += posY;


        return Vector3(x_1, y_1, 0);
    }

    enum bones : int
    {
        HumanBase = 0,
        HumanPelvis = 2,
        HumanLThigh1 = 71,
        HumanLThigh2 = 77,
        HumanLThigh3 = 72,
        HumanLCalf = 74,
        HumanLFoot = 86,
        HumanLToe = 76,
        HumanRThigh1 = 78,
        HumanRThigh2 = 84,
        HumanRThigh3 = 79,
        HumanRCalf = 81,
        HumanRFoot = 87,
        HumanRToe = 83,
        HumanSpine1 = 7,
        HumanSpine2 = 5,
        HumanSpine3 = 2,
        HumanLCollarbone = 9,
        HumanLUpperarm = 35,
        HumanLForearm1 = 36,
        HumanLForearm23 = 10,
        HumanLForearm2 = 34,
        HumanLForearm3 = 33,
        HumanLPalm = 32,
        HumanRCollarbone = 98,
        HumanRUpperarm = 64,
        HumanRForearm1 = 65,
        HumanRForearm23 = 39,
        HumanRForearm2 = 63,
        HumanRForearm3 = 62,
        HumanRPalm = 58,
        HumanNeck = 67,
        HumanHead = 68,
        HumanChest = 66
    };

    namespace bounds
    {
        struct bounds_t
        {
            float left, right, top, bottom;
        };

        struct bone_t
        {
            Vector2 screen;
            int index;
            bool on_screen;
            bool head_bone = false;
        };

        static std::array<bone_t, 26> BonesArray = {
        bone_t{ Vector2{}, bones::HumanHead, false, true }, bone_t{ Vector2{}, bones::HumanNeck, false },
        bone_t{ Vector2{}, bones::HumanSpine3, false },  bone_t{ Vector2{}, bones::HumanSpine2, false },
        bone_t{ Vector2{}, bones::HumanSpine1, false },  bone_t{ Vector2{}, bones::HumanPelvis, false },
        bone_t{ Vector2{}, bones::HumanLToe, false },  bone_t{ Vector2{}, bones::HumanLFoot, false },
        bone_t{ Vector2{}, bones::HumanLCalf, false },  bone_t{ Vector2{}, bones::HumanLThigh2, false },
        bone_t{ Vector2{}, bones::HumanLThigh1, false },  bone_t{ Vector2{}, bones::HumanPelvis, false },
        bone_t{ Vector2{}, bones::HumanRThigh1, false },  bone_t{ Vector2{}, bones::HumanRThigh2, false },
        bone_t{ Vector2{}, bones::HumanRCalf, false },  bone_t{ Vector2{}, bones::HumanRFoot, false },
        bone_t{ Vector2{}, bones::HumanRToe, false },  bone_t{ Vector2{}, bones::HumanLPalm, false },
        bone_t{ Vector2{}, bones::HumanLForearm1, false },  bone_t{ Vector2{}, bones::HumanLUpperarm, false },
        bone_t{ Vector2{}, bones::HumanRUpperarm, false }, bone_t{ Vector2{}, bones::HumanLForearm23, false },
        bone_t{ Vector2{}, bones::HumanLForearm2, false },
        bone_t{ Vector2{}, bones::HumanLForearm3, false },
        bone_t{ Vector2{}, bones::HumanRForearm2, false },
        bone_t{ Vector2{}, bones::HumanRForearm23, false }
        };
    }

}