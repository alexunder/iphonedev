//head file for BoardView
#import <UIKit/UIKit.h>
//#import "tetrix_scene.h"
//class base_tetrix_scene;

typedef enum _GameStatus
{
	GAME_START,
	GAME_LOOP,
	GAME_OVER
}GameStatus;

@interface BoardView : UIView
{
	UIImage  *imgObjArray[5];
	UIButton *button;
	UILabel  *gameoverLabel;
	NSTimer  *timer;
	CGPoint   gestureStartPoint;
	CGPoint   gestureCurrentPoint;
	BOOL 	  needProcess;
	NSDate    *starttime;
//	int       gameStatus;
//	base_tetrix_scene * m_pScene;
}

- (id)initWithFrame:(CGRect) rect;  
- (void)dealloc;
- (void)buttonClick;
//+ (void)EndGame;

 - (void)BlockMoveRight;
 - (void)BlockMoveLeft;
 - (void)BlockMoveDown;
 - (void)InvalidateView;

@end
