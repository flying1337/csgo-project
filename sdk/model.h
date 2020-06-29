#pragma once

enum e_hitboxlist
{
    HITBOX_HEAD = 0,
    HITBOX_NECK,
    HITBOX_PELVIS,
    HITBOX_STOMACH,
    HITBOX_LOWER_CHEST,
    HITBOX_CHEST,
    HITBOX_UPPER_CHEST,
    HITBOX_RIGHT_THIGH,
    HITBOX_LEFT_THIGH,
    HITBOX_RIGHT_CALF,
    HITBOX_LEFT_CALF,
    HITBOX_RIGHT_FOOT,
    HITBOX_LEFT_FOOT,
    HITBOX_RIGHT_HAND,
    HITBOX_LEFT_HAND,
    HITBOX_RIGHT_UPPER_ARM,
    HITBOX_RIGHT_LOWER_ARM,
    HITBOX_LEFT_UPPER_ARM,
    HITBOX_LEFT_LOWER_ARM,
    HITBOX_MAX,
};

struct mstudioanimdesc_t {};
struct mstudioseqdesc_t {};

struct mstudiobbox_t
{
    int					bone;
    int					group;				// intersection group
    c_vector				bbmin;				// bounding box
    c_vector				bbmax;
    int					szhitboxnameindex;	// offset to the name of the hitbox.
    int					unused[3];
    float				radius;
    int					unused2[4];

    const char* pszHitboxName()
    {
        if (szhitboxnameindex == 0)
            return "";

        return ((char*)this) + szhitboxnameindex;
    }

    mstudiobbox_t() {}

private:
    mstudiobbox_t(const mstudiobbox_t& vOther);
};

struct mstudiobone_t
{
    int					sznameindex;
    inline char* const pszName(void) const
    {
        return ((char*)this) + sznameindex;
    }
    int		 			parent;		// parent bone
    int					bonecontroller[6];	// bone controller index, -1 == none

    // default values
    c_vector				pos;
    void*			quat;
    void*			rot;
    // compression scale
    c_vector				posscale;
    c_vector				rotscale;

    c_matrix3x4			poseToBone;
    void*			qAlignment;
    int					flags;
    int					proctype;
    int					procindex;		// procedural rule
    mutable int			physicsbone;	// index into physically simulated bone
    inline void* get_procedure() const
    {
        if (procindex == 0)
            return NULL;

        else
            return (void*)(((byte*)this) + procindex);
    };
    int					surfacepropidx;	// index into string tablefor property name
    inline char* const get_surface_prop(void) const
    {
        return ((char*)this) + surfacepropidx;
    }
    int					contents;		// See BSPFlags.h for the contents flags

    int					unused[8];		// remove as appropriate

    mstudiobone_t() {}
private:
    // No copy constructors allowed
    mstudiobone_t(const mstudiobone_t& vOther);
};

struct mstudiohitboxset_t
{
    int	sznameindex;
    int	numhitboxes;
    int	hitboxindex;

    inline mstudiobbox_t* get_hitbox(int i) const
    {
        return (mstudiobbox_t*)(((unsigned char*)this) + hitboxindex) + i;
    };
};

class c_studiohdr {
public:
    int	id;
    int	version;

    long checksum;	// this has to be the same in the phy and vtx files to load!

    char name[64];
    int	length;

    c_vector	eyeposition;
    c_vector	illumposition;
    c_vector	hull_min;
    c_vector	hull_max;
    c_vector	view_bbmin;
    c_vector	view_bbmax;

    int	flags;

    int	numbones;	// bones
    int	boneindex;
    inline mstudiobone_t* get_bone(int i) const
    {
        return (mstudiobone_t*)(((byte*)this) + boneindex) + i;
    };

    int	numbonecontrollers;	// bone controllers
    int	bonecontrollerindex;
    inline void* get_bone_controller(int i) const
    {
        return (((byte*)this) + bonecontrollerindex) + i;
    };

    int	numhitboxsets;
    int	hitboxsetindex;

    // Look up hitbox set by index
    mstudiohitboxset_t* get_hitbox_set(int i) const
    {
        return (mstudiohitboxset_t*)(((byte*)this) + hitboxsetindex) + i;
    };

    // Calls through to hitbox to determine size of specified set
    inline mstudiobbox_t* get_hitbox(int i, int set) const
    {
        mstudiohitboxset_t* s = get_hitbox_set(set);

        if (!s)
            return NULL;

        return s->get_hitbox(i);
    };

    // Calls through to set to get hitbox count for set
    inline int  get_hitbox_count(int set) const
    {
        mstudiohitboxset_t const* s = get_hitbox_set(set);

        if (!s)
            return 0;

        return s->numhitboxes;
    };

    int	numanim;	// animations/poses
    int	animdescindex;	// animation descriptions
    inline void* get_animdesc(int i) const
    {
        return (((byte*)this) + animdescindex) + i;
    };

    int 	numanimgroup;
    int 	animgroupindex;
    inline  void* get_animgroup(int i) const
    {
        return (((byte*)this) + animgroupindex) + i;
    };

    int 	numbonedesc;
    int 	bonedescindex;
    inline  void* get_bonedesc(int i) const
    {
        return (((byte*)this) + bonedescindex) + i;
    };

    int	numseq;		// sequences
    int	seqindex;
    inline mstudioseqdesc_t* get_seqdesc(int i) const
    {
        if (i < 0 || i >= numseq)
            i = 0;

        return (mstudioseqdesc_t*)(((byte*)this) + seqindex) + i;
    };
    int	sequencesindexed;	// initialization flag - have the sequences been indexed?

    int	numseqgroups;		// demand loaded sequences
    int	seqgroupindex;
    inline  void* get_seqgroup(int i) const
    {
        return (((byte*)this) + seqgroupindex) + i;
    };

    int	numtextures;		// raw textures
    int	textureindex;
    inline void* get_texture(int i) const
    {
        return (((byte*)this) + textureindex) + i;
    };

    int	numcdtextures;		// raw textures search paths
    int	cdtextureindex;
    inline char* get_cd_texture(int i) const
    {
        return (((char*)this) + *((int*)(((byte*)this) + cdtextureindex) + i));
    };

    int	numskinref;		// replaceable textures tables
    int	numskinfamilies;
    int	skinindex;
    inline short* get_skinref(int i) const
    {
        return (short*)(((byte*)this) + skinindex) + i;
    };

    int	numbodyparts;
    int	bodypartindex;
    inline void* get_bodypart(int i) const
    {
        return (((byte*)this) + bodypartindex) + i;
    };

    int	numattachments;		// queryable attachable points
    int	attachmentindex;
    inline void* get_attachment(int i) const
    {
        return (((byte*)this) + attachmentindex) + i;
    };

    int	numtransitions;		// animation node to animation node transition graph
    int	transitionindex;
    inline byte* get_transition(int i) const
    {
        return (byte*)(((byte*)this) + transitionindex) + i;
    };

    int	numflexdesc;
    int	flexdescindex;
    inline void* get_flexdesc(int i) const
    {
        return (((byte*)this) + flexdescindex) + i;
    };

    int	numflexcontrollers;
    int	flexcontrollerindex;
    inline void* get_flexcontroller(int i) const
    {
        return (((byte*)this) + flexcontrollerindex) + i;
    };

    int	numflexrules;
    int	flexruleindex;
    inline void* get_flexrule(int i) const
    {
        return (((byte*)this) + flexruleindex) + i;
    };

    int	numikchains;
    int	ikchainindex;
    inline void* get_ik_chain(int i) const
    {
        return (((byte*)this) + ikchainindex) + i;
    };

    int	nummouths;
    int	mouthindex;
    inline void* get_mouth(int i) const
    {
        return (((byte*)this) + mouthindex) + i;
    };

    int	numposeparameters;
    int	poseparamindex;
    inline void* get_pose_param(int i) const
    {
        return (((byte*)this) + poseparamindex) + i;
    };

    int	surfacepropindex;
    inline char* const get_surface_prop(void) const
    {
        return ((char*)this) + surfacepropindex;
    }

    // Key values
    int	keyvalueindex;
    int	keyvaluesize;
    inline const char* get_kv(void) const
    {
        return keyvaluesize != 0 ? ((char*)this) + keyvalueindex : NULL;
    }

    int	numikautoplaylocks;
    int	ikautoplaylockindex;
    inline void* get_ik_autoplay(int i) const
    {
        return (((byte*)this) + ikautoplaylockindex) + i;
    };

    float mass;		// The collision model mass that jay wanted
    int	contents;
    int	unused[9];	// remove as appropriate
};

class c_model {
public:

};