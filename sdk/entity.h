#pragma once

class c_networkable;
class c_unknown;
class c_thinkable;
class c_cliententity;
class c_entity;

enum e_classid : int {
	DT_AI_BaseNPC,
	DT_WeaponAK47,
	DT_BaseAnimating,
	DT_BaseAnimatingOverlay,
	DT_BaseAttributableItem,
	DT_BaseButton,
	DT_BaseCombatCharacter,
	DT_BaseCombatWeapon,
	DT_BaseCSGrenade,
	DT_BaseCSGrenadeProjectile,
	DT_BaseDoor,
	DT_BaseEntity,
	DT_BaseFlex,
	DT_BaseGrenade,
	DT_BaseParticleEntity,
	DT_BasePlayer,
	DT_BasePropDoor,
	DT_BaseTeamObjectiveResource,
	DT_BaseTempEntity,
	DT_BaseToggle,
	DT_BaseTrigger,
	DT_BaseViewModel,
	DT_BaseVPhysicsTrigger,
	DT_BaseWeaponWorldModel,
	DT_Beam,
	DT_BeamSpotlight,
	DT_BoneFollower,
	DT_BRC4Target,
	DT_WeaponBreachCharge,
	DT_BreachChargeProjectile,
	DT_BreakableProp,
	DT_BreakableSurface,
	DT_WeaponBumpMine,
	DT_BumpMineProjectile,
	DT_WeaponC4,
	DT_CascadeLight,
	DT_CChicken,
	DT_ColorCorrection,
	DT_ColorCorrectionVolume,
	DT_CSGameRulesProxy,
	DT_CSPlayer,
	DT_CSPlayerResource,
	DT_CSRagdoll,
	DT_CSTeam,
	DT_DangerZone,
	DT_DangerZoneController,
	DT_WeaponDEagle,
	DT_DecoyGrenade,
	DT_DecoyProjectile,
	DT_Drone,
	DT_Dronegun,
	DT_DynamicLight,
	DT_DynamicProp,
	DT_EconEntity,
	DT_WearableItem,
	DT_Embers,
	DT_EntityDissolve,
	DT_EntityFlame,
	DT_EntityFreezing,
	DT_EntityParticleTrail,
	DT_EnvAmbientLight,
	DT_DetailController,
	DT_EnvDOFController,
	DT_EnvGasCanister,
	DT_EnvParticleScript,
	DT_EnvProjectedTexture,
	DT_QuadraticBeam,
	DT_EnvScreenEffect,
	DT_EnvScreenOverlay,
	DT_EnvTonemapController,
	DT_EnvWind,
	DT_FEPlayerDecal,
	DT_FireCrackerBlast,
	DT_FireSmoke,
	DT_FireTrail,
	DT_CFish,
	DT_WeaponFists,
	DT_Flashbang,
	DT_FogController,
	DT_FootstepControl,
	DT_Func_Dust,
	DT_Func_LOD,
	DT_FuncAreaPortalWindow,
	DT_FuncBrush,
	DT_FuncConveyor,
	DT_FuncLadder,
	DT_FuncMonitor,
	DT_FuncMoveLinear,
	DT_FuncOccluder,
	DT_FuncReflectiveGlass,
	DT_FuncRotating,
	DT_FuncSmokeVolume,
	DT_FuncTrackTrain,
	DT_GameRulesProxy,
	DT_GrassBurn,
	DT_HandleTest,
	DT_HEGrenade,
	DT_CHostage,
	DT_HostageCarriableProp,
	DT_IncendiaryGrenade,
	DT_Inferno,
	DT_InfoLadderDismount,
	DT_InfoMapRegion,
	DT_InfoOverlayAccessor,
	DT_Item_Healthshot,
	DT_ItemCash,
	DT_ItemDogtags,
	DT_WeaponKnife,
	DT_WeaponKnifeGG,
	DT_LightGlow,
	DT_MaterialModifyControl,
	DT_WeaponMelee,
	DT_MolotovGrenade,
	DT_MolotovProjectile,
	DT_MovieDisplay,
	DT_ParadropChopper,
	DT_ParticleFire,
	DT_ParticlePerformanceMonitor,
	DT_ParticleSystem,
	DT_PhysBox,
	DT_PhysBoxMultiplayer,
	DT_PhysicsProp,
	DT_PhysicsPropMultiplayer,
	DT_PhysMagnet,
	DT_PhysPropAmmoBox,
	DT_PhysPropLootCrate,
	DT_PhysPropRadarJammer,
	DT_PhysPropWeaponUpgrade,
	DT_PlantedC4,
	DT_Plasma,
	DT_PlayerPing,
	DT_PlayerResource,
	DT_PointCamera,
	DT_PointCommentaryNode,
	DT_PointWorldText,
	DT_PoseController,
	DT_PostProcessController,
	DT_Precipitation,
	DT_PrecipitationBlocker,
	DT_PredictedViewModel,
	DT_Prop_Hallucination,
	DT_PropCounter,
	DT_PropDoorRotating,
	DT_PropJeep,
	DT_PropVehicleDriveable,
	DT_RagdollManager,
	DT_Ragdoll,
	DT_Ragdoll_Attached,
	DT_RopeKeyframe,
	DT_WeaponSCAR17,
	DT_SceneEntity,
	DT_SensorGrenade,
	DT_SensorGrenadeProjectile,
	DT_ShadowControl,
	DT_SlideshowDisplay,
	DT_SmokeGrenade,
	DT_SmokeGrenadeProjectile,
	DT_SmokeStack,
	DT_Snowball,
	DT_SnowballPile,
	DT_SnowballProjectile,
	DT_SpatialEntity,
	DT_SpotlightEnd,
	DT_Sprite,
	DT_SpriteOriented,
	DT_SpriteTrail,
	DT_StatueProp,
	DT_SteamJet,
	DT_Sun,
	DT_SunlightShadowControl,
	DT_SurvivalSpawnChopper,
	DT_WeaponTablet,
	DT_Team,
	DT_TeamplayRoundBasedRulesProxy,
	DT_TEArmorRicochet,
	DT_BaseBeam,
	DT_TEBeamEntPoint,
	DT_TEBeamEnts,
	DT_TEBeamFollow,
	DT_TEBeamLaser,
	DT_TEBeamPoints,
	DT_TEBeamRing,
	DT_TEBeamRingPoint,
	DT_TEBeamSpline,
	DT_TEBloodSprite,
	DT_TEBloodStream,
	DT_TEBreakModel,
	DT_TEBSPDecal,
	DT_TEBubbles,
	DT_TEBubbleTrail,
	DT_TEClientProjectile,
	DT_TEDecal,
	DT_TEDust,
	DT_TEDynamicLight,
	DT_TEEffectDispatch,
	DT_TEEnergySplash,
	DT_TEExplosion,
	DT_TEFireBullets,
	DT_TEFizz,
	DT_TEFootprintDecal,
	DT_TEFoundryHelpers,
	DT_TEGaussExplosion,
	DT_TEGlowSprite,
	DT_TEImpact,
	DT_TEKillPlayerAttachments,
	DT_TELargeFunnel,
	DT_TEMetalSparks,
	DT_TEMuzzleFlash,
	DT_TEParticleSystem,
	DT_TEPhysicsProp,
	DT_TEPlantBomb,
	DT_TEPlayerAnimEvent,
	DT_TEPlayerDecal,
	DT_TEProjectedDecal,
	DT_TERadioIcon,
	DT_TEShatterSurface,
	DT_TEShowLine,
	DT_Tesla,
	DT_TESmoke,
	DT_TESparks,
	DT_TESprite,
	DT_TESpriteSpray,
	DT_ProxyToggle,
	DT_TestTraceline,
	DT_TEWorldDecal,
	DT_TriggerPlayerMovement,
	DT_TriggerSoundOperator,
	DT_VGuiScreen,
	DT_VoteController,
	DT_WaterBullet,
	DT_WaterLODControl,
	DT_WeaponAug,
	DT_WeaponAWP,
	DT_WeaponBaseItem,
	DT_WeaponBizon,
	DT_WeaponCSBase,
	DT_WeaponCSBaseGun,
	DT_WeaponCycler,
	DT_WeaponElite,
	DT_WeaponFamas,
	DT_WeaponFiveSeven,
	DT_WeaponG3SG1,
	DT_WeaponGalil,
	DT_WeaponGalilAR,
	DT_WeaponGlock,
	DT_WeaponHKP2000,
	DT_WeaponM249,
	DT_WeaponM3,
	DT_WeaponM4A1,
	DT_WeaponMAC10,
	DT_WeaponMag7,
	DT_WeaponMP5Navy,
	DT_WeaponMP7,
	DT_WeaponMP9,
	DT_WeaponNegev,
	DT_WeaponNOVA,
	DT_WeaponP228,
	DT_WeaponP250,
	DT_WeaponP90,
	DT_WeaponSawedoff,
	DT_WeaponSCAR20,
	DT_WeaponScout,
	DT_WeaponSG550,
	DT_WeaponSG552,
	DT_WeaponSG556,
	DT_WeaponShield,
	DT_WeaponSSG08,
	DT_WeaponTaser,
	DT_WeaponTec9,
	DT_WeaponTMP,
	DT_WeaponUMP45,
	DT_WeaponUSP,
	DT_WeaponXM1014,
	DT_WORLD,
	DT_WorldVguiText,
	DT_DustTrail,
	DT_MovieExplosion,
	DT_ParticleSmokeGrenade,
	DT_RocketTrail,
	DT_SmokeTrail,
	DT_SporeExplosion,
	DT_SporeTrail
};

class c_clientclass {
public:
	void* create;
	void* create_event;
	const char* name;
	c_recvtable* table;
	c_clientclass* next;
	int	class_id;
};

class c_ehandle {
public:
	unsigned long index;

	inline bool operator==(const c_ehandle& o) { return o.index == index; }
};

class c_handle
{
public:
	virtual			~c_handle() {}
	virtual void	set_ref_ehandle(const c_ehandle& handle) = 0;
	virtual const	c_ehandle& get_ref_ehandle() const = 0;
};



class c_renderable
{
public:
	virtual c_unknown*			get_unknown() = 0;
	virtual c_vector const&		get_render_origin(void) = 0;
	virtual c_vector const&		get_render_angles(void) = 0;
	virtual bool                should_draw(void) = 0;
	virtual int                 get_render_flags(void) = 0;
	virtual void                unused(void) const {}
	virtual void*				get_shadow_handle() const = 0;
	virtual void*				render_handle() = 0;
	virtual const c_model*		get_model() const = 0;
	virtual int                 draw_model(int flags, const int& instance) = 0;
	virtual int                 get_body() = 0;
	virtual void                get_color_modulation(float* color) = 0;
	virtual bool                lod_test() = 0;
	virtual bool                setup_bones(c_matrix3x4* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) = 0;
	virtual void                setup_weights(const c_matrix3x4* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights) = 0;
	virtual void                do_animation_events(void) = 0;
	virtual void*				get_pvs_notify_interface() = 0;
	virtual void                get_render_bounds(c_vector& mins, c_vector& maxs) = 0;
	virtual void                get_render_bounds_ws(c_vector& mins, c_vector& maxs) = 0;
	virtual void                get_shadow_render_bounds(c_vector& mins, c_vector& maxs, int shadowType) = 0;
	virtual bool                should_receive_projected_textures(int flags) = 0;
	virtual bool                get_shadow_cast_distance(float* pDist, int shadowType) const = 0;
	virtual bool                get_shadow_cast_direction(c_vector* pDirection, int shadowType) const = 0;
	virtual bool                is_shadow_dirty() = 0;
	virtual void                mark_shadow_dirty(bool bDirty) = 0;
	virtual c_renderable*		get_shadow_parent() = 0;
	virtual c_renderable*		first_shadow_child() = 0;
	virtual c_renderable*		next_shadow_peer() = 0;
	virtual int					shadow_cast_type() = 0;
	virtual void                create_model_instance() = 0;
	virtual void*				get_model_instance() = 0;
	virtual const c_matrix3x4&	renderable_to_wt() = 0;
	virtual int					lookup_attachment(const char* pAttachmentName) = 0;
	virtual bool				get_attachment(int number, c_vector& origin, c_vector& angles) = 0;
	virtual bool				get_attachment(int number, c_matrix3x4& matrix) = 0;
	virtual float*				get_renter_clip_pane(void) = 0;
	virtual int					get_skin() = 0;
	virtual void				on_threaded_draw_setup() = 0;
	virtual bool				uses_flexed_display_weights() = 0;
	virtual void				record_tool_message() = 0;
	virtual bool				should_draw_for_ss(int nSlot) = 0;
	virtual unsigned char		override_alpha_modulation(unsigned char nAlpha) = 0;
	virtual unsigned char		override_shadow_alpha_modulation(unsigned char nAlpha) = 0;
};

class c_thinkable
{
public:
	virtual c_unknown*	get_unknown() = 0;
	virtual void		think() = 0;
	virtual void*		get_think_handle() = 0;
	virtual void		set_think_handle(void* hThink) = 0;
	virtual void		release() = 0;
};

class c_unknown : public c_handle {
public:
	virtual c_collider*			get_collider() = 0;
	virtual c_networkable*		get_networkable() = 0;
	virtual c_renderable*		get_renderable() = 0;
	virtual c_cliententity*		get_client_entity() = 0;
	virtual c_entity*			get_entity() = 0;
	virtual c_thinkable*		get_thinkable() = 0;
};

class c_networkable {
public:
	virtual c_unknown*		get_unknown() = 0;
	virtual void			release() = 0;
	virtual c_clientclass*	get_client_class() = 0;
	virtual void			notify_should_trasmit(void* state) = 0;
	virtual void			on_pre_data_changed(void* updateType) = 0;
	virtual void			on_data_changed(void* updateType) = 0;
	virtual void			pre_data_update(void* updateType) = 0;
	virtual void			post_data_update(void* updateType) = 0;
	virtual void			unk() = 0;
	virtual bool			is_dormant(void) = 0;
	virtual int				index(void) const = 0;
	virtual void			receive_message(int classID, void* msg) = 0;
	virtual void*			get_dt_base() = 0;
	virtual void			set_destroyed_on_recreate(void) = 0;
	virtual void			on_data_unchanged_in_pvs() = 0;
};

class c_cliententity : public c_unknown, public c_renderable, public c_networkable, public c_thinkable
{
public:
	virtual void				release(void) = 0;
	virtual const c_vector&		get_abs_origin(void) const = 0;
	virtual const c_angle&		get_abs_angles(void) const = 0;
	virtual void*				get_mouth(void) = 0;
	virtual bool				get_sound_spatialization(void* info) = 0;
};

class c_entity : public c_cliententity {
public:
	bool is_player() {
		return g_vfunc(bool(__thiscall*)(void*), this, 157)(this);
	}

	bool is_weapon() {
		return g_vfunc(bool(__thiscall*)(void*), this, 165)(this);
	}
    float get_spawn_time(){
        return g_offset(float, this, 0x20);
    }

    static float get_expiry_time()
    {
        return 7.f;
    }

	bool is_projectile();
	bool is_hostage();

	c_angle& get_abs_angles() {
		return g_vfunc(c_angle & (__thiscall*)(void*), this, 11)(this);
	}

	c_vector& get_abs_origin() {
		return g_vfunc(c_vector & (__thiscall*)(void*), this, 10)(this);
	}

    datamap_t* get_datamap() {
	    return g_vfunc(datamap_t*(__thiscall*)(void*), this, 17)(this);
	}

	c_vector& origin();
	c_vector& abs_velocity();
};