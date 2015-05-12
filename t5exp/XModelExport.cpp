#include "stdinc.h"
#include <direct.h>

typedef float vec3_t[3];

enum scriptInstance_t
{
	SCRIPTINSTANCE_SERVER = 0x0,
	SCRIPTINSTANCE_CLIENT = 0x1,
	SCRIPT_INSTANCE_MAX = 0x2,
};

typedef void *(__cdecl * DB_FindXAssetHeader_t)(XAssetType type, const char *name, bool errorIfMissing, int waitTime);
DB_FindXAssetHeader_t DB_FindXAssetHeader = (DB_FindXAssetHeader_t)0x493A60;

typedef const char *(__cdecl * SL_ConvertToString_t)(unsigned int stringValue, scriptInstance_t inst);
SL_ConvertToString_t SL_ConvertToString = (SL_ConvertToString_t)0x624C70;

Stream* Buffer = 0;

// Stuff copied from T6, might be missing some data, but who cares :P

void Write_XSurfaceVertexInfo(XSurfaceVertexInfo* vertInfo, XSurfaceVertexInfo* destVertInfo)
{
	if (vertInfo->vertsBlend)
	{
		Buffer->Write(vertInfo->vertsBlend, sizeof(short), vertInfo->vertCount[0] + 3 * vertInfo->vertCount[1] + 5 * vertInfo->vertCount[2] + 7 * vertInfo->vertCount[3]);
		destVertInfo->vertsBlend = (unsigned short *)-1;
	}

	if (vertInfo->tensionData)
	{
		Buffer->Write(vertInfo->tensionData, sizeof(float), vertInfo->vertCount[0] + vertInfo->vertCount[1] + vertInfo->vertCount[2] + vertInfo->vertCount[3]);
		destVertInfo->tensionData = (float *)-1;
	}
}

void Write_XSurfaceCollisionTree(XSurfaceCollisionTree* collisionTree)
{
	XSurfaceCollisionTree* destCollisionTree = (XSurfaceCollisionTree*)Buffer->At();
	Buffer->Write(collisionTree, sizeof(XSurfaceCollisionTree));

	if (collisionTree->nodes)
	{
		Buffer->Write(collisionTree->nodes, sizeof(XSurfaceCollisionNode), collisionTree->nodeCount);
		destCollisionTree->nodes = (XSurfaceCollisionNode *)-1;
	}

	if (collisionTree->leafs)
	{
		Buffer->Write(collisionTree->leafs, sizeof(XSurfaceCollisionLeaf), collisionTree->leafCount);
		destCollisionTree->leafs = (XSurfaceCollisionLeaf *)-1;
	}
}

void Write_XRigidVertListArray(XRigidVertList* vertList, char vertListCount)
{
	XRigidVertList* destVertList = (XRigidVertList*)Buffer->At();
	Buffer->Write(vertList, sizeof(XRigidVertList), vertListCount);

	for (char i = 0; i < vertListCount; i++)
	{
		XRigidVertList* destVertListEntry = &destVertList[i];
		XRigidVertList* vertListEntry = &vertList[i];

		if (vertListEntry->collisionTree)
		{
			Write_XSurfaceCollisionTree(vertListEntry->collisionTree);
			destVertListEntry->collisionTree = (XSurfaceCollisionTree *)-1;
		}
	}
}

void Write_XSurfaceArray(XSurface* surfs, char numsurfs)
{
	XSurface* destSurfs = (XSurface*)Buffer->At();
	Buffer->Write(surfs, sizeof(XSurface), numsurfs);

	for (char i = 0; i < numsurfs; i++)
	{
		XSurface* destSurf = &destSurfs[i];
		XSurface* surf = &surfs[i];

		Write_XSurfaceVertexInfo(&surf->vertInfo, &destSurf->vertInfo);

		if (!(surf->flags & 1) && surf->verts0)
		{
			Buffer->Write(surf->verts0, sizeof(GfxPackedVertex), surf->vertCount);
			destSurf->verts0 = (GfxPackedVertex *)-1;
		}

		// DirectX buffers are handled by the game.
		//Write_VertexBuffer();

		if (surf->vertList)
		{
			Write_XRigidVertListArray(surf->vertList, surf->vertListCount);
			destSurf->vertList = (XRigidVertList *)-1;
		}

		if (surf->triIndices)
		{
			Buffer->Write(surf->triIndices, 6, surf->triCount);
			destSurf->triIndices = (unsigned __int16 *)-1;
		}

		// DirectX buffers are handled by the game.
		//Write_IndexBuffer();
	}
}

void Write_XModelCollSurfArray(XModelCollSurf_s* collSurfs, int numCollSurfs)
{
	XModelCollSurf_s* destCollSurfs = (XModelCollSurf_s*)Buffer->At();
	Buffer->Write(collSurfs, sizeof(XModelCollSurf_s), numCollSurfs);

	for (int i = 0; i < numCollSurfs; i++)
	{
		XModelCollSurf_s* destCollSurf = &destCollSurfs[i];
		XModelCollSurf_s* collSurf = &collSurfs[i];

		if (collSurf->collTris)
		{
			Buffer->Write(collSurf->collTris, sizeof(XModelCollTri_s), collSurf->numCollTris);
			destCollSurfs->collTris = (XModelCollTri_s *)-1;
		}
	}
}

void Write_cbrushside_tArray(cbrushside_t* sides, unsigned int numsides)
{
	cbrushside_t* destSides = (cbrushside_t*)Buffer->At();
	Buffer->Write(sides, sizeof(cbrushside_t), numsides);

	for (unsigned int i = 0; i < numsides; i++)
	{
		cbrushside_t* destSide = &destSides[i];
		cbrushside_t* side = &sides[i];

		if (side->plane)
		{
			Buffer->Write(side->plane, sizeof(cplane_s));
			destSide->plane = (cplane_s *)-1;
		}
	}
}

void Write_BrushWrapper(BrushWrapper* brush)
{
	BrushWrapper* destBrush = (BrushWrapper*)Buffer->At();
	Buffer->Write(brush, sizeof(BrushWrapper));

	if (brush->sides)
	{
		Write_cbrushside_tArray(brush->sides, brush->numsides);
		destBrush->sides = (cbrushside_t *)-1;
	}

	if (brush->verts)
	{
		Buffer->Write(brush->verts, sizeof(vec3_t), brush->numverts);
		destBrush->verts = (vec3_t *)-1;
	}

	if (brush->planes)
	{
		Buffer->Write(brush->planes, sizeof(cplane_s), brush->numsides);
		destBrush->planes = (cplane_s *)-1;
	}
}

void Write_PhysGeomInfoArray(PhysGeomInfo* geoms, unsigned int count)
{
	PhysGeomInfo* destGeoms = (PhysGeomInfo*)Buffer->At();
	Buffer->Write(geoms, sizeof(PhysGeomInfo), count);

	for (unsigned int i = 0; i < count; i++)
	{
		PhysGeomInfo* destGeom = &destGeoms[i];
		PhysGeomInfo* geom = &geoms[i];

		if (geom->brush)
		{
			Write_BrushWrapper(geom->brush);
			destGeom->brush = (BrushWrapper *)-1;
		}
	}
}

void Write_PhysGeomList(PhysGeomList* geomList)
{
	PhysGeomList* destGeomList = (PhysGeomList*)Buffer->At();
	Buffer->Write(geomList, sizeof(PhysGeomList));

	if (geomList->geoms)
	{
		Write_PhysGeomInfoArray(geomList->geoms, geomList->count);
		destGeomList->geoms = (PhysGeomInfo *)-1;
	}
}

void Write_CollmapArray(Collmap* collmaps, char numCollmaps)
{
	Collmap* destCollmaps = (Collmap*)Buffer->At();
	Buffer->Write(collmaps, sizeof(Collmap), numCollmaps);

	for (char i = 0; i < numCollmaps; i++)
	{
		Collmap* destCollmap = &destCollmaps[i];
		Collmap* collmap = &collmaps[i];

		if (collmap->geomList)
		{
			Write_PhysGeomList(collmap->geomList);
			destCollmap->geomList = (PhysGeomList *)-1;
		}
	}
}

void Write(XModel* Asset)
{
	XModel* dest = (XModel*)Buffer->At();
	Buffer->Write(Asset, sizeof(XModel));

	if (Asset->name)
	{
		Buffer->WriteString(Asset->name);
		dest->name = (const char*)-1;
	}

	if (Asset->boneNames)
	{
		unsigned short* destBoneNames = (unsigned short*)Buffer->At();
		Buffer->Write(Asset->boneNames, sizeof(short), Asset->numBones);

		for (int i = 0; i < Asset->numBones; i++)
		{
			Buffer->WriteString(SL_ConvertToString(destBoneNames[i], SCRIPTINSTANCE_SERVER));
		}

		dest->boneNames = (unsigned __int16 *)-1;
	}

	if (Asset->parentList)
	{
		Buffer->Write(Asset->parentList, Asset->numBones - Asset->numRootBones);
		dest->parentList = (char *)-1;
	}

	if (Asset->quats)
	{
		Buffer->Write(Asset->quats, 8, Asset->numBones - Asset->numRootBones);
		dest->quats = (short *)-1;
	}

	if (Asset->trans)
	{
		Buffer->Write(Asset->trans, 16, Asset->numBones - Asset->numRootBones);
		dest->trans = (float *)-1;
	}

	if (Asset->partClassification)
	{
		Buffer->Write(Asset->partClassification, Asset->numBones);
		dest->partClassification = (char *)-1;
	}

	if (Asset->baseMat)
	{
		Buffer->Write(Asset->baseMat, sizeof(DObjAnimMat), Asset->numBones);
		dest->baseMat = (DObjAnimMat *)-1;
	}

	if (Asset->surfs)
	{
		Write_XSurfaceArray(Asset->surfs, Asset->numsurfs);
		dest->surfs = (XSurface *)-1;
	}

	if (Asset->materialHandles)
	{
		Material** destMaterialHandles = (Material**)Buffer->At();
		Buffer->Write(Asset->materialHandles, sizeof(Material*), Asset->numsurfs);

		for (char i = 0; i < Asset->numsurfs; i++)
		{
			Buffer->WriteString(Asset->materialHandles[i]->info.name);
		}

		dest->materialHandles = (Material **)-1;
	}

	if (Asset->collSurfs)
	{
		Write_XModelCollSurfArray(Asset->collSurfs, Asset->numCollSurfs);
		dest->collSurfs = (XModelCollSurf_s *)-1;
	}

	if (Asset->boneInfo)
	{
		Buffer->Write(Asset->boneInfo, sizeof(XBoneInfo), Asset->numBones);
		dest->boneInfo = (XBoneInfo *)-1;
	}

	if (Asset->himipInvSqRadii)
	{
		Buffer->Write(Asset->himipInvSqRadii, 16, Asset->numsurfs);
		dest->himipInvSqRadii = (float *)-1;
	}

	if (Asset->physPreset)
	{
		Buffer->WriteString(Asset->physPreset->name);
	}

	if (Asset->collmaps)
	{
		Write_CollmapArray(Asset->collmaps, Asset->numCollmaps);
		dest->collmaps = (Collmap *)-1;
	}

	if (Asset->physConstraints)
	{
		Buffer->WriteString(Asset->physConstraints->name);
	}
}

void XModelExport(const char* name)
{
	XModel* model = (XModel*)DB_FindXAssetHeader(ASSET_TYPE_XMODEL, name, true, -1);

	if (model)
	{
		Stream _Buffer;
		Buffer = &_Buffer;

		Write(model);

		_mkdir("raw");
		std::string _name = "raw/";
		_name += name;

		FILE* fp = fopen(_name.c_str(), "wb");

		if (fp)
		{
			fwrite(_Buffer.Data(), _Buffer.Size(), 1, fp);
			fclose(fp);
		}
	}
}