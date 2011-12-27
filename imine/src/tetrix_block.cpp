//Second Foundation Copyright @
#include "stdafx.h"
#include "tetrix_block.h"

void base_matrix_rotate_90_ckw( unsigned char * pMatrix, int dim )
{
	if( pMatrix == NULL )
	{
		return;		  
	}

	int i;
	int temp = 0;

	int start_index = 0;
	int end_index = dim - 1;

	for(;start_index < end_index; start_index++, end_index--)
	{
		for(i = start_index; i < end_index; i++)
		{
			temp = pMatrix[i+start_index*dim];
			pMatrix[i + start_index*dim] = pMatrix[start_index + (end_index - i + start_index)*dim];
			pMatrix[start_index + (end_index - i + start_index)*dim] = pMatrix[(end_index - i + start_index) + end_index*dim];
			pMatrix[(end_index - i + start_index) + end_index*dim]   = pMatrix[end_index + i*dim];
			pMatrix[end_index + i*dim]   = temp; 	
		}
	}
}

base_block::base_block( )
{
	m_istart_x = 0;
	m_istart_y = 0;
	m_datasize = 4;

	m_iwidth_CompareDate = 0;
	m_iheight_CompareDate = 0;

	memset( m_data, 0, 16*sizeof(unsigned char) );
	m_pCompareData = NULL;

	m_enum_category = BLOCK_CUBIC;
	m_i_Is_falldown = 0;
}

base_block::~base_block()
{
	if ( m_pCompareData != NULL )
	{
		free(m_pCompareData);
		m_pCompareData =NULL;
	}
}

void base_block::rotate()
{
	if( m_enum_category == BLOCK_CUBIC )
	{
		return;
	}

	if( can_transform() )
	{
		base_matrix_rotate_90_ckw(m_data, 4);
	}
}
	
void base_block::move_right()
{
	if ( can_move_right() )
	{
		m_istart_x++;
	}
}
	
void base_block::move_left()
{
	if ( can_move_left() )
	{
		m_istart_x--;
	}
}


void base_block::fall_slow()
{
	if ( is_not_down() )
	{
		m_istart_y++;
	}
	else
	{
		m_i_Is_falldown = 1;
	}
}

	
void base_block::draw( scene_context * pcontext )
{
	if ( pcontext == NULL )
	{
		return;
	}

	if ( m_istart_x >= pcontext->b_x_size ||
		 m_istart_x + m_datasize <= 0     ||
		 m_istart_y >= pcontext->b_y_size )
	{
		return;
	}

	int i;
    int j;

	int isrc_offset = 0;
	int ides_offset = 0;
    int icopy_size  = 0;

	if ( m_istart_x < 0 )
	{
		ides_offset = 0;
		isrc_offset = - m_istart_x;
		icopy_size = m_datasize + m_istart_x; 
	}
	else if ( m_istart_x + m_datasize >= pcontext->b_y_size )
	{
		ides_offset = m_istart_x;
		isrc_offset = 0;
		icopy_size = m_datasize + pcontext->b_y_size - m_istart_x - m_datasize;
	}
	else
	{
		ides_offset = m_istart_x;
		isrc_offset = 0;
		icopy_size = m_datasize;
	}

	for ( i = 0; i < m_datasize; i++ )
	{
		for ( j = 0; j < icopy_size; j++ )
		{
			if ( m_data[isrc_offset+j+i*m_datasize] != 0 )
			{
				pcontext->pSceneData[ides_offset + j + (m_istart_y + i)*pcontext->b_x_size] = m_data[isrc_offset+j+i*m_datasize];
			}
		} 
		//memcpy( pcontext->pSceneData + ides_offset + (m_istart_y + i)*pcontext->b_y_size,
		//	    m_data + isrc_offset + i*m_datasize,
		//		icopy_size );
	}
}


void base_block::initblock( unsigned char * pdata, int iwidth, int iheight, block_category enum_category )
{
	if ( pdata == NULL )
	{
		return;
	}

	int iSize = iwidth*iheight;
	m_pCompareData = (unsigned char *)malloc(iSize);

	memcpy( m_pCompareData, pdata, iSize );

	m_iwidth_CompareDate = iwidth;
	m_iheight_CompareDate = iheight;
	m_enum_category = enum_category;

	m_i_Is_falldown = 0;
	m_istart_x = (iwidth - m_datasize)/2 ;
	m_istart_y = 0;

	memset( m_data, 0, 16*sizeof(unsigned char) );

	switch ( m_enum_category )
	{
	case BLOCK_CUBIC:
		 {
			m_data[5]  = 1;
			m_data[6]  = 1;
			m_data[9]  = 1;
			m_data[10] = 1;
		 }
		 break;
	case BLOCK_BAR:
		 {
			 m_data[1]  = 2;
			 m_data[5]  = 2;
			 m_data[9]  = 2;
			 m_data[13] = 2;
		 }
		 break;
	case BLOCK_LHOOK:
		 {
			 m_data[1]  = 3;
			 m_data[2]  = 3;
			 m_data[6]  = 3;
			 m_data[10] = 3;
		 }
		 break;
	case BLOCK_RHOOK:
		{
			m_data[1]  = 4;
			m_data[2]  = 4;
			m_data[5]  = 4;
			m_data[9]  = 4;
		}
		break;
	case BLOCK_MIDDLE:
		 {
			 m_data[1]  = 5;
			 m_data[6]  = 5;
			 m_data[5]  = 5;
			 m_data[9]  = 5;
		 }
		 break;
	}
}

bool base_block::can_move_right()
{
	int i;
	int index_buffer[10];
	memset( index_buffer, 0, 10*sizeof(int) );
   
	int icount = 0;

	for ( i = 0; i < 16; i++ )
	{
		if ( (m_data[i] != 0   && 
			  m_data[i+1] == 0 &&
			  (i + 1)%m_datasize != 0) ||
			  ((i + 1)%m_datasize == 0 &&
			  m_data[i] != 0) )
		{
			index_buffer[icount++] = i;
		}
	}

	for ( i = 0; i < icount; i++ )
	{
		int idelta_x = index_buffer[i]%m_datasize;
		int idelta_y = index_buffer[i]/m_datasize;

		int iX_offset = m_istart_x + idelta_x;
		int iY_offset = m_istart_y + idelta_y;
		if ( iX_offset == m_iwidth_CompareDate - 1|| 
			 m_pCompareData[iX_offset + 1 + iY_offset*m_iwidth_CompareDate] != 0 )
		{
			return false;
		}
	}

	return true;
}

bool base_block::can_move_left()
{
	int i;
	int index_buffer[10];
	memset( index_buffer, 0, 10*sizeof(int) );

	int icount = 0;

	for ( i = 0; i < 16; i++ )
	{
		if ( (m_data[i] != 0  && 
			  m_data[i-1] == 0 &&
			  i%m_datasize != 0)||
			  (i%m_datasize == 0 &&
			   m_data[i] != 0 ))
		{
			index_buffer[icount++] = i;
		}
	}

	for ( i = 0; i < icount; i++ )
	{
		int idelta_x = index_buffer[i]%m_datasize;
		int idelta_y = index_buffer[i]/m_datasize;

		int iX_offset = m_istart_x + idelta_x;
		int iY_offset = m_istart_y + idelta_y;
		if ( iX_offset == 0 || 
			m_pCompareData[iX_offset - 1 + iY_offset*m_iwidth_CompareDate] != 0 )
		{
			return false;
		}
	}

	return true;
}

bool base_block::is_not_down()
{
		int i;
		int index_buffer[10];
		memset( index_buffer, 0, 10*sizeof(int) );

		int icount = 0;

		for ( i = 0; i < 16; i++ )
		{
			if ( (m_data[i] != 0 && 
				 m_data[i + m_datasize] == 0 &&
				 i < 12)||
				 (i/m_datasize == 3 &&
				  m_data[i] != 0 ) )
			{ 
				index_buffer[icount++] = i;
			}
		}

		for ( i = 0; i < icount; i++ )
		{
			int idelta_x = index_buffer[i]%m_datasize;
			int idelta_y = index_buffer[i]/m_datasize;

			int iX_offset = m_istart_x + idelta_x;
			int iY_offset = m_istart_y + idelta_y;
			if ( iY_offset == m_iheight_CompareDate - 1 || 
				m_pCompareData[iX_offset + (iY_offset + 1)*m_iwidth_CompareDate] != 0 )
			{
				return false;
			}
		}

		return true;
}

bool base_block::can_transform()
{
	int i;
	if( m_istart_x >= 0 && m_istart_x + m_datasize <= m_iwidth_CompareDate - 1 )
	{
		for(i = 0; i < m_datasize; i++)
		{
			if( m_pCompareData[m_istart_x + i + (m_istart_y+3)*m_iwidth_CompareDate] != 0 )
			{
				return false;
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}
