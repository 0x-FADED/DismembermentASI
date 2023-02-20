#pragma once

//credits to Yimura and contributers for the classes https://github.com/Yimura/GTAV-Classes

enum class ePedBoneType
{
    HEAD,
    L_FOOT,
    R_FOOT,
    L_ANKLE,
    R_ANKLE,
    L_HAND,
    R_HAND,
    NECK,
    ABDOMEN
};

class CPedBoneInfo
{
public:
    rage::fvector3 modelCoords;
    char pad0[4];
};
static_assert(sizeof(CPedBoneInfo) == 0x10);


class CPedIntelligence
{
public:
    char pad_0000[632]; //0x0000
    float m_oxygenTime; //0x0278
}; //Size: 0x027C
static_assert(sizeof(CPedIntelligence) == 0x27C);

enum class eGameState : int32_t
{
    Invalid = -1,
    Playing,
    Died,
    Arrested,
    FailedMission,
    LeftGame,
    Respawn,
    InMPCutscene
};

#pragma pack(push, 4)
class CPlayerInfo
{
public:
    char pad_0000[32]; //0x0000
    char pad[0xF8]; //0x0020 rlGamerInfo
    char pad_00B8[184]; //0x00B8
    float m_swimSpeed; //0x0170
    char pad_0174[20]; //0x0174
    uint32_t m_waterProof; //0x0188
    char pad_018C[76]; //0x018C
    eGameState m_gameState; //0x01D8
    char pad_01DC[12]; //0x01DC
    class CPed* m_ped; //0x01E8
    char pad_01F0[40]; //0x01F0
    uint32_t m_frameFlags; //0x0218
    char pad_021C[52]; //0x021C
    uint32_t m_playerControls; //0x0250
    char pad_0254[1256]; //0x0254
    float m_wantedCaChange; //0x073C
    char pad_0740[144]; //0x0740
    float m_wantedLevelDifficulty; //0x07D0 min:-1 max:1
    char pad_07D4[52]; //0x07D4
    float m_wantedLevelMultiplier; //0x0808 min:0 max:1
    char pad_080C[100]; //0x080C
    uint32_t m_npcIgnore; //0x0870
    char pad_0874[12]; //0x0874
    bool m_isWanted; //0x0880
    char pad_0881[7]; //0x0881
    uint32_t m_wantedLevel; //0x0888
    uint32_t m_wantedLevelDisplay; //0x088C
    char pad_0890[1120]; //0x0890
    float m_runSpeed; //0x0CF0
    float m_stamina; //0x0CF4
    float m_staminaRegen; //0x0CF8
    char pad_0CFC[16]; //0x0CFC
    float m_weaponDamageMult; //0x0D0C
    float m_weaponDefenceMult; //0x0D10
    char pad_0D14[4]; //0x0D14
    float m_meleeWeaponDamageMult; //0x0D18
    float m_meleeDamageMult; //0x0D1C
    float m_meleeDefenceMult; //0x0D20
    char pad_0D24[8]; //0x0D24
    float m_meleeWeaponDefenceMult; //0x0D2C
}; //Size: 0x0D30
static_assert(sizeof(CPlayerInfo) == 0xD90);
#pragma pack(pop)

#pragma pack(push, 1)
class CPed : public rage::CPhysical
{
public:
    char gap2EC[20];
    rage::fvector3 m_velocity; //0x0300
    char pad_030C[260]; //0x030C
    CPedBoneInfo m_boneInfo[9]; //0x0410
    char pad_04A0[2160]; //0x04A0
    class CVehicle* m_vehicle; //0x0D10
    char pad_0D18[896]; //0x0D18
    uint32_t m_pedType; //0x1098
    char pad_109C[4]; //0x109C
    CPedIntelligence* m_pedIntelligence; //0x10A0
    CPlayerInfo* m_playerInfo; //0x10A8
    class CPedInventory* m_inventory; //0x10B0
    class CPedWeaponManager* m_weaponManager; //0x10B8
    char pad_10C0[892]; //0x10C0
    uint8_t m_seatbelt; //0x143C
    char pad_143D[13]; //0x143D
    uint8_t m_canSwitchWeapon; //0x144A
    uint8_t m_pedTaskFlag; //0x144B
    char pad_144C[4]; //0x144C
    uint8_t m_forcedAim; //0x1450 m_forced_aim ^= (m_forced_aim ^ -(char)toggle) & 0x20;
    char pad_1451[187]; //0x1451
    float m_armor; //0x150C
    float unk_healthThreshold; //0x1510
    float m_fatiguedHealthThreshold; //0x1514
    float m_injuredHealthThreshold; //0x1518
    float m_dyingHealthThreshold; //0x151C
    float m_hurtHealthThreshold; //0x1520
    char pad_1524[240]; //0x1524
    uint16_t m_cash; //0x1614
    char pad_1616[842]; //0x1616
    uint8_t firedStickyBombs; //0x1960
    uint8_t firedUnk_0; //0x1961
    uint8_t firedFlares; //0x1962
    uint8_t firedUnk_1; //0x1963

    uint32_t getPedType() { return m_pedType << 11 >> 25; }

    rage::fvector3 GetBoneCoords(ePedBoneType type)
    {
        rage::fvector3 worldCoords;
        modelToWorld(m_boneInfo[(uint32_t)type].modelCoords, worldCoords);
        return worldCoords;
    }

    CPed()
    {
        memset(this, 0x0, sizeof(CPed));
    }

}; //Size: 0x1616
static_assert(sizeof(CPed) == 0x1964, "CPed is of wrong Size!");
#pragma pack(pop)