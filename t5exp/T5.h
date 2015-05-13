#include <d3d9.h>

typedef float vec3_t[3];

struct DObjAnimMat
{
	float quat[4];
	float trans[3];
	float transWeight;
};

struct XSurfaceVertexInfo
{
	__int16 vertCount[4];
	unsigned __int16 *vertsBlend;
	float *tensionData;
};

union GfxColor
{
	unsigned int packed;
	char array[4];
};

union PackedTexCoords
{
	unsigned int packed;
};

union PackedUnitVec
{
	unsigned int packed;
	char array[4];
};

struct GfxPackedVertex
{
	float xyz[3];
	float binormalSign;
	GfxColor color;
	PackedTexCoords texCoord;
	PackedUnitVec normal;
	PackedUnitVec tangent;
};

struct XSurfaceCollisionAabb
{
	unsigned __int16 mins[3];
	unsigned __int16 maxs[3];
};

struct XSurfaceCollisionNode
{
	XSurfaceCollisionAabb aabb;
	unsigned __int16 childBeginIndex;
	unsigned __int16 childCount;
};

struct XSurfaceCollisionLeaf
{
	unsigned __int16 triangleBeginIndex;
};

struct XSurfaceCollisionTree
{
	float trans[3];
	float scale[3];
	unsigned int nodeCount;
	XSurfaceCollisionNode *nodes;
	unsigned int leafCount;
	XSurfaceCollisionLeaf *leafs;
};

struct XRigidVertList
{
	unsigned __int16 boneOffset;
	unsigned __int16 vertCount;
	unsigned __int16 triOffset;
	unsigned __int16 triCount;
	XSurfaceCollisionTree *collisionTree;
};

struct XSurface
{
	char tileMode;
	char vertListCount;
	unsigned __int16 flags;
	unsigned __int16 vertCount;
	unsigned __int16 triCount;
	unsigned __int16 baseTriIndex;
	unsigned __int16 baseVertIndex;
	unsigned __int16 *triIndices;
	XSurfaceVertexInfo vertInfo;
	GfxPackedVertex *verts0;
	IDirect3DVertexBuffer9 *vb0;
	XRigidVertList *vertList;
	IDirect3DIndexBuffer9 *indexBuffer;
	int partBits[5];
};

struct GfxDrawSurfFields
{
	__int64 _bf0;
};

union GfxDrawSurf
{
	GfxDrawSurfFields fields;
	unsigned __int64 packed;
};

struct MaterialInfo
{
	const char *name;
	unsigned int gameFlags;
	char pad;
	char sortKey;
	char textureAtlasRowCount;
	char textureAtlasColumnCount;
	GfxDrawSurf drawSurf;
	unsigned int surfaceTypeBits;
	unsigned int layeredSurfaceTypes;
	unsigned __int16 hashIndex;
};

struct MaterialStreamRouting
{
	char source;
	char dest;
};

struct MaterialVertexStreamRouting
{
	MaterialStreamRouting data[16];
	IDirect3DVertexDeclaration9 *decl[18];
};

struct MaterialVertexDeclaration
{
	char streamCount;
	bool hasOptionalSource;
	bool isLoaded;
	MaterialVertexStreamRouting routing;
};

struct GfxVertexShaderLoadDef
{
	unsigned int *program;
	unsigned __int16 programSize;
};

struct MaterialVertexShaderProgram
{
	IDirect3DVertexShader9 *vs;
	GfxVertexShaderLoadDef loadDef;
};

struct MaterialVertexShader
{
	const char *name;
	MaterialVertexShaderProgram prog;
};

struct GfxPixelShaderLoadDef
{
	unsigned int *program;
	unsigned __int16 programSize;
};

struct MaterialPixelShaderProgram
{
	IDirect3DPixelShader9 *ps;
	GfxPixelShaderLoadDef loadDef;
};

struct MaterialPixelShader
{
	const char *name;
	MaterialPixelShaderProgram prog;
};

struct MaterialArgumentCodeConst
{
	unsigned __int16 index;
	char firstRow;
	char rowCount;
};

union MaterialArgumentDef
{
	const float *literalConst;
	MaterialArgumentCodeConst codeConst;
	unsigned int codeSampler;
	unsigned int nameHash;
};

struct MaterialShaderArgument
{
	unsigned __int16 type;
	unsigned __int16 dest;
	MaterialArgumentDef u;
};

struct MaterialPass
{
	MaterialVertexDeclaration *vertexDecl;
	MaterialVertexShader *vertexShader;
	MaterialPixelShader *pixelShader;
	char perPrimArgCount;
	char perObjArgCount;
	char stableArgCount;
	char customSamplerFlags;
	MaterialShaderArgument *args;
};

struct MaterialTechnique
{
	const char *name;
	unsigned __int16 flags;
	unsigned __int16 passCount;
	MaterialPass passArray[1];
};

struct MaterialTechniqueSet
{
	const char *name;
	char worldVertFormat;
	char unused[1];
	unsigned __int16 techsetFlags;
	MaterialTechnique *techniques[130];
};

struct GfxImageLoadDef
{
	char levelCount;
	char flags;
	int format;
	int resourceSize;
	char data[1];
};

union GfxTexture
{
	IDirect3DBaseTexture9 *basemap;
	IDirect3DTexture9 *map;
	IDirect3DVolumeTexture9 *volmap;
	IDirect3DCubeTexture9 *cubemap;
	GfxImageLoadDef *loadDef;
};

struct Picmip
{
	char platform[2];
};

struct CardMemory
{
	int platform[2];
};

struct GfxImage
{
	GfxTexture texture;
	char mapType;
	char semantic;
	char category;
	bool delayLoadPixels;
	Picmip picmip;
	bool noPicmip;
	char track;
	CardMemory cardMemory;
	unsigned __int16 width;
	unsigned __int16 height;
	unsigned __int16 depth;
	char levelCount;
	char streaming;
	unsigned int baseSize;
	char *pixels;
	unsigned int loadedSize;
	char skippedMipLevels;
	const char *name;
	unsigned int hash;
};

struct WaterWritable
{
	float floatTime;
};

struct complex_s
{
	float real;
	float imag;
};

struct water_t
{
	WaterWritable writable;
	complex_s *H0;
	float *wTerm;
	int M;
	int N;
	float Lx;
	float Lz;
	float gravity;
	float windvel;
	float winddir[2];
	float amplitude;
	float codeConstant[4];
	GfxImage *image;
};

union MaterialTextureDefInfo
{
	GfxImage *image;
	water_t *water;
};

struct MaterialTextureDef
{
	unsigned int nameHash;
	char nameStart;
	char nameEnd;
	char samplerState;
	char semantic;
	char isMatureContent;
	char pad[3];
	MaterialTextureDefInfo u;
};

struct MaterialConstantDef
{
	unsigned int nameHash;
	char name[12];
	float literal[4];
};

struct GfxStateBits
{
	unsigned int loadBits[2];
};

struct Material
{
	MaterialInfo info;
	char stateBitsEntry[130];
	char textureCount;
	char constantCount;
	char stateBitsCount;
	char stateFlags;
	char cameraRegion;
	char maxStreamedMips;
	MaterialTechniqueSet *techniqueSet;
	MaterialTextureDef *textureTable;
	MaterialConstantDef *constantTable;
	GfxStateBits *stateBitsTable;
};

struct XModelLodInfo
{
	float dist;
	unsigned __int16 numsurfs;
	unsigned __int16 surfIndex;
	int partBits[5];
	char lod;
	char smcIndexPlusOne;
	char smcAllocBits;
	char unused;
};

struct XModelCollTri_s
{
	float plane[4];
	float svec[4];
	float tvec[4];
};

struct XModelCollSurf_s
{
	XModelCollTri_s *collTris;
	int numCollTris;
	float mins[3];
	float maxs[3];
	int boneIdx;
	int contents;
	int surfFlags;
};

struct XBoneInfo
{
	float bounds[2][3];
	float offset[3];
	float radiusSquared;
	char collmap;
};

struct XModelHighMipBounds
{
	float center[3];
	float himipRadiusSq;
};

struct XModelStreamInfo
{
	XModelHighMipBounds *highMipBounds;
};

struct PhysPreset
{
	const char *name;
	int flags;
	float mass;
	float bounce;
	float friction;
	float bulletForceScale;
	float explosiveForceScale;
	const char *sndAliasPrefix;
	float piecesSpreadFraction;
	float piecesUpwardVelocity;
	int canFloat;
	float gravityScale;
	float centerOfMassOffset[3];
	float buoyancyBoxMin[3];
	float buoyancyBoxMax[3];
};

struct cplane_s
{
	float normal[3];
	float dist;
	char type;
	char signbits;
	char pad[2];
};

struct cbrushside_t
{
	cplane_s *plane;
	int cflags;
	int sflags;
};

struct BrushWrapper
{
	float mins[3];
	int contents;
	float maxs[3];
	unsigned int numsides;
	cbrushside_t *sides;
	int axial_cflags[2][3];
	int axial_sflags[2][3];
	unsigned int numverts;
	float(*verts)[3];
	cplane_s *planes;
};

struct PhysGeomInfo
{
	BrushWrapper *brush;
	int type;
	float orientation[3][3];
	float offset[3];
	float halfLengths[3];
};

struct PhysGeomList
{
	unsigned int count;
	PhysGeomInfo *geoms;
	int contents;
};

struct Collmap
{
	PhysGeomList *geomList;
};

enum ConstraintType
{
	CONSTRAINT_NONE = 0x0,
	CONSTRAINT_POINT = 0x1,
	CONSTRAINT_DISTANCE = 0x2,
	CONSTRAINT_HINGE = 0x3,
	CONSTRAINT_JOINT = 0x4,
	CONSTRAINT_ACTUATOR = 0x5,
	CONSTRAINT_FAKE_SHAKE = 0x6,
	CONSTRAINT_LAUNCH = 0x7,
	CONSTRAINT_ROPE = 0x8,
	CONSTRAINT_LIGHT = 0x9,
	NUM_CONSTRAINT_TYPES = 0xA,
};

enum AttachPointType
{
	ATTACH_POINT_WORLD = 0x0,
	ATTACH_POINT_DYNENT = 0x1,
	ATTACH_POINT_ENT = 0x2,
	ATTACH_POINT_BONE = 0x3,
};

struct PhysConstraint
{
	unsigned __int16 targetname;
	ConstraintType type;
	AttachPointType attach_point_type1;
	int target_index1;
	unsigned __int16 target_ent1;
	const char *target_bone1;
	AttachPointType attach_point_type2;
	int target_index2;
	unsigned __int16 target_ent2;
	const char *target_bone2;
	float offset[3];
	float pos[3];
	float pos2[3];
	float dir[3];
	int flags;
	int timeout;
	int min_health;
	int max_health;
	float distance;
	float damp;
	float power;
	float scale[3];
	float spin_scale;
	float minAngle;
	float maxAngle;
	Material *material;
	int constraintHandle;
	int rope_index;
	int centity_num[4];
};

struct PhysConstraints
{
	const char *name;
	unsigned int count;
	PhysConstraint data[16];
};

struct XModel
{
	const char *name;
	char numBones;
	char numRootBones;
	char numsurfs;
	char lodRampType;
	unsigned __int16 *boneNames;
	char *parentList;
	__int16 *quats;
	float *trans;
	char *partClassification;
	DObjAnimMat *baseMat;
	XSurface *surfs;
	Material **materialHandles;
	XModelLodInfo lodInfo[4];
	char lodDistAutoGenerated;
	XModelCollSurf_s *collSurfs;
	int numCollSurfs;
	int contents;
	XBoneInfo *boneInfo;
	float radius;
	float mins[3];
	float maxs[3];
	__int16 numLods;
	__int16 collLod;

	union
	{
		XModelStreamInfo streamInfo;
		float* himipInvSqRadii;
	};

	int memUsage;
	int flags;
	bool bad;
	PhysPreset *physPreset;
	char numCollmaps;
	Collmap *collmaps;
	PhysConstraints *physConstraints;
};

enum XAssetType
{
	ASSET_TYPE_XMODELPIECES = 0x0,
	ASSET_TYPE_PHYSPRESET = 0x1,
	ASSET_TYPE_PHYSCONSTRAINTS = 0x2,
	ASSET_TYPE_DESTRUCTIBLEDEF = 0x3,
	ASSET_TYPE_XANIMPARTS = 0x4,
	ASSET_TYPE_XMODEL = 0x5,
	ASSET_TYPE_MATERIAL = 0x6,
	ASSET_TYPE_TECHNIQUE_SET = 0x7,
	ASSET_TYPE_IMAGE = 0x8,
	ASSET_TYPE_SOUND = 0x9,
	ASSET_TYPE_SOUND_PATCH = 0xA,
	ASSET_TYPE_CLIPMAP = 0xB,
	ASSET_TYPE_CLIPMAP_PVS = 0xC,
	ASSET_TYPE_COMWORLD = 0xD,
	ASSET_TYPE_GAMEWORLD_SP = 0xE,
	ASSET_TYPE_GAMEWORLD_MP = 0xF,
	ASSET_TYPE_MAP_ENTS = 0x10,
	ASSET_TYPE_GFXWORLD = 0x11,
	ASSET_TYPE_LIGHT_DEF = 0x12,
	ASSET_TYPE_UI_MAP = 0x13,
	ASSET_TYPE_FONT = 0x14,
	ASSET_TYPE_MENULIST = 0x15,
	ASSET_TYPE_MENU = 0x16,
	ASSET_TYPE_LOCALIZE_ENTRY = 0x17,
	ASSET_TYPE_WEAPON = 0x18,
	ASSET_TYPE_WEAPONDEF = 0x19,
	ASSET_TYPE_WEAPON_VARIANT = 0x1A,
	ASSET_TYPE_SNDDRIVER_GLOBALS = 0x1B,
	ASSET_TYPE_FX = 0x1C,
	ASSET_TYPE_IMPACT_FX = 0x1D,
	ASSET_TYPE_AITYPE = 0x1E,
	ASSET_TYPE_MPTYPE = 0x1F,
	ASSET_TYPE_MPBODY = 0x20,
	ASSET_TYPE_MPHEAD = 0x21,
	ASSET_TYPE_CHARACTER = 0x22,
	ASSET_TYPE_XMODELALIAS = 0x23,
	ASSET_TYPE_RAWFILE = 0x24,
	ASSET_TYPE_STRINGTABLE = 0x25,
	ASSET_TYPE_PACK_INDEX = 0x26,
	ASSET_TYPE_XGLOBALS = 0x27,
	ASSET_TYPE_DDL = 0x28,
	ASSET_TYPE_GLASSES = 0x29,
	ASSET_TYPE_EMBLEMSET = 0x2A,
	ASSET_TYPE_COUNT = 0x2B,
	ASSET_TYPE_STRING = 0x2B,
	ASSET_TYPE_ASSETLIST = 0x2C,
};

struct XZoneInfo
{
	const char *name;
	int allocFlags;
	int freeFlags;
};


enum scriptInstance_t
{
	SCRIPTINSTANCE_SERVER = 0x0,
	SCRIPTINSTANCE_CLIENT = 0x1,
	SCRIPT_INSTANCE_MAX = 0x2,
};

typedef void(__cdecl * Com_Printf_t)(int channel, const char *fmt, ...);
extern Com_Printf_t Com_Printf;

typedef void *(__cdecl * DB_FindXAssetHeader_t)(XAssetType type, const char *name, bool errorIfMissing, int waitTime);
extern DB_FindXAssetHeader_t DB_FindXAssetHeader;

typedef void(__cdecl * DB_LoadXAssets_t)(XZoneInfo *zoneInfo, unsigned int zoneCount, int sync);
extern DB_LoadXAssets_t DB_LoadXAssets;

typedef const char *(__cdecl * SL_ConvertToString_t)(unsigned int stringValue, scriptInstance_t inst);
extern SL_ConvertToString_t SL_ConvertToString;
