//Second Foundation Copyright @

#include "stdafx.h"
#include "tetrix_scene.h"

static int g_i_count = 0;

unsigned int GenerateRandomNumber( unsigned int start, unsigned int end )
{
	if(start >= end)
	{
		return 0;
	}
	//srand(time(NULL) + g_i_count*200 );
	//srand(time(NULL));

	//g_i_count++;
	int num = end - start + 1;
	
//	return ( (arc4random()/(RAND_MAX/num) ) + start);
	unsigned int rand_num = arc4random()%num;
	return (rand_num + start);
}

base_tetrix_scene::base_tetrix_scene()
{
	m_pcontext_freeze   = NULL;
	m_pcontext_activity = NULL;
	m_pblock = NULL;
	m_pfngameover = NULL;
	m_i_current_block_index = 0;
	m_i_next_block_index    = 0;
}

base_tetrix_scene::~base_tetrix_scene()
{
	if(m_pcontext_freeze != NULL)
	{
		DestroySceneContext(m_pcontext_freeze);
		m_pcontext_freeze = NULL;
	}

	if(m_pcontext_activity != NULL)
	{
		DestroySceneContext(m_pcontext_activity);
		m_pcontext_activity = NULL;
	}

	if(m_pblock != NULL)
	{
		delete m_pblock;
	}
}


bool base_tetrix_scene::CreateScene( int iwidth, int ilength, gameovercallback pfncallback )
{
	m_pcontext_activity = CreateSceneContext( 2, iwidth, ilength );

	if(m_pcontext_activity == NULL)
	{
		return false;
	}

	m_pcontext_freeze = CreateSceneContext( 2, iwidth, ilength );
	
	if(m_pcontext_activity == NULL)
	{
		return false;
	}

	m_pfngameover = pfncallback;

	m_pblock = new base_block;

	return true;
}

void base_tetrix_scene::DestroyScene()
{	
	if(m_pcontext_freeze != NULL)
	{
		DestroySceneContext(m_pcontext_freeze);
		m_pcontext_freeze = NULL;
	}

	if(m_pcontext_activity != NULL)
	{
		DestroySceneContext(m_pcontext_activity);
		m_pcontext_activity = NULL;
	}
}

void base_tetrix_scene::StartGame()
{
	ClearSceneContext(m_pcontext_freeze);
	ClearSceneContext(m_pcontext_activity);
	//BlockFactory();
	m_i_current_block_index = GenerateRandomNumber( 0, 4);

	m_pblock->initblock(m_pcontext_freeze->pSceneData, 
						m_pcontext_freeze->b_x_size,
						m_pcontext_freeze->b_y_size,
						(block_category)m_i_current_block_index);
	m_i_next_block_index = GenerateRandomNumber( 0, 4);
	m_pblock->draw(m_pcontext_activity);
}
	
void base_tetrix_scene::EndGame()
{
	if ( m_pfngameover != NULL )
	{
		this->m_pfngameover();
	}
}
	
void base_tetrix_scene::user_right()
{
	m_pblock->move_right();
	DrawBlock();
}
	
void base_tetrix_scene::user_left()
{
	m_pblock->move_left();
	DrawBlock();
}
	
void base_tetrix_scene::user_fall()
{
	while ( m_pblock->isBlockDown() == 0 )
	{
		m_pblock->fall_slow();
		DrawBlock();
	}

	CheckGameStatus();
}
	
void base_tetrix_scene::user_down()
{
	m_pblock->fall_slow();
	DrawBlock();

	if ( m_pblock->isBlockDown() != 0 )
	{
		CheckGameStatus();
	}
}

void base_tetrix_scene::user_rotate()
{
	m_pblock->rotate();
	DrawBlock();
}

void base_tetrix_scene::DrawBlock()
{
	Sync_Scene_Data(m_pcontext_activity, m_pcontext_freeze);
	m_pblock->draw(m_pcontext_activity);
}

void base_tetrix_scene::CheckGameStatus()
{
	int i;
	int j;
	int flag = 0;

	int x;
	int y;
	//firstly,check that is need to clear blocks
	for(i = 0; i < m_pcontext_activity->b_y_size; i++)
	{
		for(j = 0; j < m_pcontext_activity->b_x_size; j++)
		{
			if(m_pcontext_activity->pSceneData[j + i*m_pcontext_activity->b_x_size] == 0)
			{                   
				flag=0;
				break;
			}
			else
			{
				flag=1;
			}
		}

		if(flag == 1)//when the ith floor can be clear
		{
			for(j=0; j<m_pcontext_activity->b_x_size; j++)
			{
				m_pcontext_activity->pSceneData[j + i*m_pcontext_activity->b_x_size] = 0;
			}

			for(x=i-1; x>=0; x--)
				for(y=0; y<m_pcontext_activity->b_x_size; y++)
				{
					if(m_pcontext_activity->pSceneData[y + x*m_pcontext_activity->b_x_size] != 0)
					{
						m_pcontext_activity->pSceneData[y + (x+1)*m_pcontext_activity->b_x_size] = m_pcontext_activity->pSceneData[y + x*m_pcontext_activity->b_x_size];
						m_pcontext_activity->pSceneData[y + x*m_pcontext_activity->b_x_size] = 0;
					}
				}
		}
	}

	for(i=0; i<m_pcontext_activity->b_x_size; i++)
	{
		if ( m_pcontext_activity->pSceneData[i] != 0 )
		{
			EndGame();
			return;
		}
	}
	
	Sync_Scene_Data(m_pcontext_freeze, m_pcontext_activity);

	BlockFactory();
}

void base_tetrix_scene::BlockFactory()
{
	m_i_current_block_index = m_i_next_block_index;
	m_i_next_block_index =	GenerateRandomNumber( 0, 4);

	m_pblock->initblock(m_pcontext_freeze->pSceneData, 
						m_pcontext_freeze->b_x_size,
						m_pcontext_freeze->b_y_size,
						(block_category)m_i_current_block_index);
	
	m_pblock->draw(m_pcontext_activity);
}

bool base_tetrix_scene::GetNextBlockData( unsigned char * data, int isize )
{
	if(data == NULL || isize < 16)
	{
		return false;
	}

	memset(data, 0, isize);

	switch ( m_i_next_block_index )
	{
	case BLOCK_CUBIC:
		 {
			data[5]  = 1;
			data[6]  = 1;
			data[9]  = 1;
			data[10] = 1;
		 }
		 break;
	case BLOCK_BAR:
		 {
			 data[1]  = 2;
			 data[5]  = 2;
			 data[9]  = 2;
			 data[13] = 2;
		 }
		 break;
	case BLOCK_LHOOK:
		 {
			 data[1]  = 3;
			 data[2]  = 3;
			 data[6]  = 3;
			 data[10] = 3;
		 }
		 break;
	case BLOCK_RHOOK:
		{
			data[1]  = 4;
			data[2]  = 4;
			data[5]  = 4;
			data[9]  = 4;
		}
		break;
	case BLOCK_MIDDLE:
		 {
			 data[1]  = 5;
			 data[6]  = 5;
			 data[5]  = 5;
			 data[9]  = 5;
		 }
		 break;
	}

	return true;
}
