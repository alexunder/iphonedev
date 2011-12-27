//Second Foundation Copyright @

#ifndef _H__TETRIX_BLOCK
#define _H__TETRIX_BLOCK

#include "tetrix_context.h"

typedef enum _block_category
{
	BLOCK_CUBIC  = 0,
	BLOCK_BAR    = 1,
    BLOCK_LHOOK  = 2,
	BLOCK_RHOOK  = 3,
	BLOCK_MIDDLE = 4
}block_category;

class base_block
{
public:
	base_block();
	~base_block();

	void initblock( unsigned char * pdata, int iwidth, int iheight, block_category enum_category );
	void rotate();
	void move_right();
	void move_left();
	void fall_slow();
	void draw( scene_context * pcontext );
	int isBlockDown()
	{
		return m_i_Is_falldown;
	}
private:
	bool can_move_right();
	bool can_move_left();
	bool is_not_down();
	bool can_transform();
protected:
	unsigned char m_data[16];
	int m_datasize;
	int m_istart_x;
	int m_istart_y;
	unsigned char * m_pCompareData;
	int m_iwidth_CompareDate;
	int m_iheight_CompareDate;
	block_category m_enum_category;
	int m_i_Is_falldown;
};


#endif
