#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum RenderOptions
	{
		Lighting = 0,
		Textures = 1,
		TexturesAndFog = 2
	};
	enum RESOURCETYPE
	{
		RESOURCE_STATIC,
		RESOURCE_DYNAMIC,
		RESOURCE_END
	};

	enum TEXTURETYPE
	{
		TEXTURE_NORMAL,
		TEXTURE_CUBE,
		TEXTURE_END
	};

	enum BUFFERTYPE
	{
		BUFFER_TRICOLOR,
		BUFFER_RCCOLOR,
		BUFFER_RCTEX,
		BUFFER_TERRAINTEX,
		BUFFER_END
	};

	enum ANGLE
	{
		ANGLE_X,
		ANGLE_Y,
		ANGLE_Z,
		ANGLE_END
	};



	enum MESHTYPE { MESH_STATIC, MESH_DYNAMIC, MESH_END};

	enum COLLISIONID { CLSN_TERRAIN, CLSN_MOUSE, CLSN_END };
}


#endif