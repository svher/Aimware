#pragma once
#include "vector.h"

class Entity;
// interface.h
typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);

// use new EngineTraceClient
#define INTERFACEVERSION_ENGINETRACE_CLIENT	"EngineTraceClient004"
#define	CONTENTS_SOLID			0x1		// an eye is never valid in a solid
#define	CONTENTS_WINDOW			0x2		// translucent, but not watery (glass)
#define	CONTENTS_GRATE			0x8		// alpha-tested "grate" textures.  Bullets/sight pass through, but solids don't
#define CONTENTS_MOVEABLE		0x4000
#define	CONTENTS_MONSTER		0x2000000
#define	CONTENTS_MONSTERCLIP	0x20000
#define	CONTENTS_DEBRIS			0x4000000
#define CONTENTS_HITBOX			0x40000000

#define MASK_NPCWORLDSTATIC	    ( CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE )
#define STANDARD_TRACE_MASK     ( MASK_NPCWORLDSTATIC | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_DEBRIS | CONTENTS_HITBOX )
#define   MASK_SHOT                     (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)

struct cplane_t
{
    Vec3	normal;
    float	dist;
    byte	type;			// for fast side tests
    byte	signbits;		// signx + (signy<<1) + (signz<<1)
    byte	pad[2];
};

struct csurface_t
{
    const char	*name;
    short		surfaceProps;
    unsigned short	flags;		// BUGBUG: These are declared per surface, not per material, but this database is per-material now
};

typedef void C_BaseEntity;

class CBaseTrace
{
    // these members are aligned!!
    Vec3			startpos;				// start position
    Vec3			endpos;					// final position
    cplane_t		plane;					// surface normal at impact

    float			fraction;				// time completed, 1.0 = didn't hit anything

    int				contents;				// contents on other side of surface hit
    unsigned short	dispFlags;				// displacement flags for marking surfaces with data

    bool			allsolid;				// if true, plane is not valid
    bool			startsolid;				// if true, the initial point was in a solid area
};


class CGameTrace : public CBaseTrace {
public:
    float		fractionleftsolid;		// time we left a solid, only valid if we started in solid
    csurface_t	surface;				// surface hit (impact surface)

    int			hitgroup;				// 0 == generic, non-zero is specific body part
    short               physicsbone;        // physics bone hit by trace in studio
    unsigned short      worldSurfaceIndex;  // Index of the msurface2_t, if applicable

    Entity *m_pEnt;

    // NOTE: this member is overloaded.
    // If hEnt points at the world entity, then this is the static prop index.
    // Otherwise, this is the hitbox index.
    int			hitbox;					// box hit by trace in studio
};

typedef CGameTrace trace_t;
typedef void IHandleEntity;

//-----------------------------------------------------------------------------
// The standard trace filter... NOTE: Most normal traces inherit from CTraceFilter!!!
//-----------------------------------------------------------------------------
enum TraceType_t
{
    TRACE_EVERYTHING = 0,
    TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
    TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
    TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

class ITraceFilter {
    virtual bool ShouldHitEntity( IHandleEntity *pEntity, int contentsMask ) = 0;
    virtual TraceType_t	GetTraceType() const = 0;
};

//-----------------------------------------------------------------------------
// A ray...
//-----------------------------------------------------------------------------

struct Ray_t
{
    Vec3Aligned  m_Start;	// starting point, centered within the extents
    Vec3Aligned  m_Delta;	// direction + length of the ray
    Vec3Aligned  m_StartOffset;	// Add this to m_Start to get the actual ray start
    Vec3Aligned  m_Extents;	// Describes an axis aligned box extruded along a ray
    const void* m_pWorldAxisTransform;
    bool	m_IsRay;	// are the extents zero?
    bool	m_IsSwept;	// is delta != 0?

    void Init( Vec3 const& start, Vec3 const& end )
    {
        m_Delta = end - start;

        m_IsSwept = (m_Delta.LengthSqr() != 0);

        m_pWorldAxisTransform = nullptr;
        m_Extents.Clear();
        m_IsRay = true;

        // Offset m_Start to be in the center of the box...
        m_StartOffset.Clear();
        m_Start = start;
    }
};

class CTraceFilter : public ITraceFilter {
public:
    void *pSkip;
public:
    TraceType_t GetTraceType() const override {
        return TRACE_EVERYTHING;
    }

    bool ShouldHitEntity(IHandleEntity *pEntity, int contentsMask) override {
        return pEntity != pSkip;
    }
};

class IEngineTrace {
    virtual void fn0() = 0;
    virtual void fn1() = 0;
    virtual void fn2() = 0;
    virtual void fn3() = 0;
    virtual void fn4() = 0;
public:
    virtual void TraceRay( const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, trace_t *pTrace ) = 0;
};