//Second Foundation Copyright @

#ifndef _H_TETRIX_CONTEX
#define _H_TETRIX_CONTEX

typedef struct _scene_context
{
	unsigned char   b_dimension;
	unsigned char   b_x_size;
	unsigned char   b_y_size;
	unsigned char   b_z_size;
	unsigned char * pSceneData;
}scene_context;

scene_context * CreateSceneContext( int idimension, int iX, int iY, int iZ = 0);

bool Sync_Scene_Data(scene_context *pcontextDes, scene_context *pcontextSrc);

void DestroySceneContext(scene_context *pcontext);

void ClearSceneContext(scene_context *pcontext);
#endif
