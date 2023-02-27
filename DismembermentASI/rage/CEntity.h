#pragma once

//credits to Yimura and contributers for the classes https://github.com/Yimura/GTAV-Classes

namespace rage
{
#pragma pack(push, 1)
    class CNavigation
    {
    public:
        char pad_0000[16]; //0x0000
        void* m_damp; //0x0010
        char pad_0018[8]; //0x0018
        fmatrix44 m_transformationMatrix;

        fvector3* get_position()
        {
            return reinterpret_cast<rage::fvector3*>(&m_transformationMatrix.rows[3]);
        }

        void modelToWorld(const rage::fvector3& model_coords, rage::fvector3& worldCoords)
        {
            worldCoords.x = model_coords.x * m_transformationMatrix.data[0][0] + model_coords.y * m_transformationMatrix.data[1][0] + model_coords.z * m_transformationMatrix.data[2][0] + m_transformationMatrix.data[3][0];
            worldCoords.y = model_coords.x * m_transformationMatrix.data[0][1] + model_coords.y * m_transformationMatrix.data[1][1] + model_coords.z * m_transformationMatrix.data[2][1] + m_transformationMatrix.data[3][1];
            worldCoords.z = model_coords.x * m_transformationMatrix.data[0][2] + model_coords.y * m_transformationMatrix.data[1][2] + model_coords.z * m_transformationMatrix.data[2][2] + m_transformationMatrix.data[3][2];
        }
    }; //Size: 0x0060
    static_assert(sizeof(CNavigation) == 0x60);
#pragma pack(pop)

    template <class T>
    class fwRefAwareBaseImpl : public T
    {
    public:
        void *m_ref; // 0x08
    };

    class fwRefAwareBase : public fwRefAwareBaseImpl<datBase>
    {
    };
    static_assert(sizeof(fwRefAwareBase) == 0x10);

    class fwExtensibleBase : public fwRefAwareBase
    {
    public:
        virtual bool is_of_type(uint32_t hash) = 0;
        virtual uint32_t const &get_type() = 0;

        void* m_extensionContainer; // 0x0010
        void *m_extensibleUnk; // 0x0018

    }; //Size: 0x0020
    static_assert(sizeof(fwExtensibleBase) == 0x20);

#pragma pack(push, 4)
    class fwDrawData
    {
    public:
        uint64_t m_unk0;
        uint64_t m_unk8;
        char pad10[8];
        uint32_t m_unk18;
        uint32_t m_unk1C;
        uint64_t m_unk20;
        uint32_t m_unk28;
    };
    static_assert(sizeof(fwDrawData) == 0x2C);
#pragma pack(pop)

    class fwDynamicEntityComponent;
    class crmtRequestPose;
    class crmtRequestIk;
    class crFrameFilter;
    class fwAudEntity;
    class CMoveObjectPooledObject;
    class CEntityDrawHandler;

#pragma pack(push, 1)
    class fwEntity : public fwExtensibleBase
    {
    public:
        virtual void* _0x00() = 0;
        virtual void* _0x08() = 0;
        virtual uint32_t _0x10() = 0;
        virtual fwEntity* _0x18(void*) = 0;
        virtual bool _0x20(void*) = 0;
        virtual bool _0x28(void**) = 0;
        virtual ~fwEntity() = 0;
        virtual void* _0x38(void*, void*) = 0;
        virtual void AddExtension(void* extension) = 0; // 0x40
        virtual void _0x48() = 0; // not implemented
        virtual void _0x50() = 0; // only implemented by CEntityBatch
        virtual void _0x58() = 0;
        virtual void SetModelInfo(uint16_t* modelIndex) = 0; // 0x60
        virtual void _0x68(int, fvector4*) = 0;
        virtual void* _0x70(int) = 0;
        virtual CNavigation* GetNavigation() = 0; // 0x78
        virtual CMoveObjectPooledObject* CreateMoveObject() = 0; // 0x80
        virtual uint32_t* GetType() = 0; // 0x88
        virtual void _0x90() = 0;
        virtual float _0x98() = 0;
        virtual bool TryRequestInverseKinematics(crmtRequestPose* pose, crmtRequestIk* ik) = 0; // 0xA0 implemented only by CPed
        virtual bool TryRequestFacialAnims(void*) = 0; // 0xA8 implemented only by CPed
        virtual void* _0xB0() = 0;
        virtual uint8_t _0xB8() = 0; // implemented only by CPed
        virtual crFrameFilter* GetFrameFilter() = 0; // 0xC0
        virtual fwAudEntity* GetEntityAudio() = 0; // 0xC8
        virtual void _0xD0() = 0;
        virtual void SetTransform(fmatrix44* matrix, bool update_pos) = 0; // 0xD8
        virtual void SetTransform2(fmatrix44* matrix, bool update_pos) = 0; // 0xE0
        virtual void SetPosition(fvector4* pos, bool update_pos) = 0; // 0xE8
        virtual void SetHeading(float heading, bool update_pos) = 0; // 0xF0
        virtual void SetEntityTypeFlags() = 0; // 0xF8
        virtual void _0x100() = 0; // not implemented
        virtual void UpdatePhysics(CNavigation* navigation) = 0; // 0x108
        virtual void UpdatePhysics2(CNavigation* navigation) = 0; // 0x110
        virtual void UpdatePosition() = 0; // 0x118

        enum class EntityFlags
        {
            IS_VISIBLE = (1 << 0)
        };

        CBaseModelInfo* m_modelInfo; //0x0020
        uint8_t m_entityType; //0x0028
        char gap29; //0x0029
        uint16_t gap2A; //0x002A
        uint32_t m_flags; //0x002D
        CNavigation* m_navigation; //0x0030
        uint16_t gap38; //0x0038
        uint16_t gap3A; //0x003A
        uint32_t gap3C; //0x003C
        class fwDynamicEntityComponent* m_dynamicEntityComponent; //0x0040 (stores attachments and stuff)
        fwDrawData* m_drawData; //0x0048
        class fwDynamicEntityComponent* gap50; //0x0050
        uint64_t gap58; //0x0058
        fmatrix44 m_transformation_matrix; //0x0060
        fwEntity* m_renderFocusEntity; //0x00A0
        uint32_t m_renderFocusDistance; //0x00A8
        uint32_t m_flags_2; //0x00AC
        uint32_t m_shadowFlags; //0x00B0
        char gapB4[4]; //0x00B4
        uint8_t byteB8; //0x00B8

        fvector3* get_position()
        {
            return reinterpret_cast<fvector3*>(&m_transformation_matrix.rows[3]);
        }

        void modelToWorld(const fvector3& model_coords, fvector3& world_coords)
        {
            world_coords.x = model_coords.x * m_transformation_matrix.data[0][0] + model_coords.y * m_transformation_matrix.data[1][0] + model_coords.z * m_transformation_matrix.data[2][0] + m_transformation_matrix.data[3][0];
            world_coords.y = model_coords.x * m_transformation_matrix.data[0][1] + model_coords.y * m_transformation_matrix.data[1][1] + model_coords.z * m_transformation_matrix.data[2][1] + m_transformation_matrix.data[3][1];
            world_coords.z = model_coords.x * m_transformation_matrix.data[0][2] + model_coords.y * m_transformation_matrix.data[1][2] + model_coords.z * m_transformation_matrix.data[2][2] + m_transformation_matrix.data[3][2];
        }
    };
    static_assert(sizeof(fwEntity) == 0xB9);
#pragma pack(pop)

    class CEntity : public fwEntity
    {
    public:
        virtual void* _0x120() = 0; // implemented only by CPed
        virtual void UpdatePositionImpl() = 0; // 0x128
        virtual void _0x130() = 0;
        virtual void _0x138(void*) = 0;
        virtual void _0x140() = 0;
        virtual void _0x148(int) = 0;
        virtual bool _0x150() = 0;
        virtual CEntityDrawHandler* CreateDrawHandler() = 0; // 0x158
        virtual int GetTypeFlags() = 0; // 0x160
        virtual int GetTypeFlags2() = 0; // 0x168
        virtual bool _0x170() = 0; // implemented only by CPickup
        virtual bool _0x178() = 0;
        virtual void _0x180(bool) = 0;
        virtual bool _0x188() = 0;
        virtual bool _0x190() = 0;
        virtual void ClearDecals() = 0; // 0x198
        virtual void GetModelBounds(rage::fvector3* bounds) = 0; // 0x1A0
        virtual void GetModelBounds2(rage::fvector3* bounds) = 0; // 0x1A8
        virtual float GetBoundingBoxSize() = 0; // 0x1B0
        virtual float _0x1B8(void*) = 0;
        virtual float _0x1C0(void*) = 0;
        virtual fvector3* _0x1C8() = 0;
        virtual fvector3* GetCameraOffset() = 0; // 0x1D0
        virtual void GetCameraBasePosition(rage::fvector3* pos) = 0; // 0x1D8
        virtual bool _0x1E0() = 0;
        virtual bool Update() = 0; // 0x1E8 always returns true
        virtual bool _0x1F0() = 0;
        virtual void Warp(rage::fvector3* pos, float heading, bool) = 0; // 0x1F8

        uint8_t gapB9; //0x00B9
        char gapBA[6]; //0x00BA
        uint32_t m_flags3; //0x00C0
        uint32_t m_flags4; //0x00C4
        uint32_t dwordC8;
        uint32_t dwordCC;
    };
    static_assert(sizeof(CEntity) == 0xD0, "CEntity is of Wrong size!");

    class CDynamicEntity : public CEntity
    {
    public:
        class netObject* m_netObject; //0x00D0
        char padD8[16];
        uint64_t m_unkE8;
    };
    static_assert(sizeof(CDynamicEntity) == 0xF0);
    
#pragma pack(push, 1)
    class CPhysical : public CDynamicEntity
    {
    public:
        char pad0[144];
        uint64_t m_unk180;
        uint32_t m_damage_bits; //0x0188
        uint8_t m_hostility; //0x018C
        char pad18D[3];
        uint8_t byte190;
        char pad191[3];
        uint32_t m_unk194;
        char pad198[232];
        float m_health; //0x0280
        float m_maxhealth; //0x0284
        void* m_attackers;
        char pad2B0[72];
        uint64_t m_unk2F8;
        uint64_t m_unk300;
        uint32_t m_unk308;
    };
    static_assert(sizeof(CPhysical) == 0x2EC);
#pragma pack(pop)
}