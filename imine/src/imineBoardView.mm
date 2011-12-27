//imineBoardView.m
#import "imineBoardView.h"
#import "tetrix_scene.h"
#import <Foundation/Foundation.h>
#import "stdafx.h"
#import "trace.h"

#define MAX_PATH_LEN 256

#define kMinimumGestureLength       25
#define kMaximumVariance            5

//bool g_bisgameover  = false;
//bool g_bisgamestart = false;

base_tetrix_scene g_scene;


//void EndGame(void)
//{
//	[button setHidden: NO];
//	[gameoverLabel setHidden: NO];
//	g_bisgameover  = true;
//	g_bisgamestart = false;
	
//}

GameStatus gameStatus;

void EndGame()
{
	gameStatus = GAME_OVER;
}

@implementation BoardView

/*
+ (void)EndGame
{
	gameStatus = GAME_OVER;
}
*/

- (void)timerTask:(NSTimer *)theTimer 
{
	g_scene.user_down();
	[self setNeedsDisplay];
}


-(void)buttonClick
{
    //count++;
	//iphone_trace("start buttonClick\n");
	g_scene.StartGame();
	//g_bisgameover  = false;
	//g_bisgamestart = true;
	gameStatus = GAME_LOOP;

	[button setHidden: YES];
	[gameoverLabel setHidden: YES];

	timer = [NSTimer scheduledTimerWithTimeInterval:1.0f
	         	 	 target:self
				 	 selector:@selector(timerTask:)
					 userInfo:nil
					 repeats:YES];

	[self setNeedsDisplay];
}


- (id)initWithFrame:(CGRect)rect
{
	int i;

	self = [ super initWithFrame: rect ];
	if(self != nil)
	{
		imgObjArray[0] = [UIImage imageAtPath:@"/Applications/imine.app/block_red.bmp"];
		imgObjArray[1] = [UIImage imageAtPath:@"/Applications/imine.app/block_yellow.bmp"];
		imgObjArray[2] = [UIImage imageAtPath:@"/Applications/imine.app/block_blue.bmp"];
		imgObjArray[3] = [UIImage imageAtPath:@"/Applications/imine.app/block_green.bmp"];
		imgObjArray[4] = [UIImage imageAtPath:@"/Applications/imine.app/block_purple.bmp"];

		gameStatus = GAME_START;
	
		button = [UIButton buttonWithType:UIButtonTypeRoundedRect];
		button.frame =  CGRectMake(40, 200, 240, 80);
		//button.backgroundColor = [UIColor greenColor];
		[button setTitle:@"Play game" forState:UIControlStateNormal];
		[button addTarget:self action:@selector(buttonClick)forControlEvents:UIControlEventTouchUpInside];
		[self addSubview:button];

		gameoverLabel = [[UILabel alloc] initWithFrame:CGRectMake(10, 10, 300, 100)];
		//[gameoverLabel setFont:uifont];
		[gameoverLabel setBackgroundColor:[UIColor redColor]];
		[gameoverLabel setText:@"Game Over!"];
		
		gameoverLabel.adjustsFontSizeToFitWidth = YES;
		gameoverLabel.textAlignment = UITextAlignmentCenter;
		gameoverLabel.font = [UIFont systemFontOfSize:50];
		gameoverLabel.textColor = [UIColor redColor];

		[gameoverLabel setHidden: YES];
		[self addSubview:gameoverLabel];

		g_scene.CreateScene(10, 20, EndGame);
	
		needProcess = NO;
	//	[self becomeFirstResponder];
	}

	return self;
}



- (void)drawRect:(CGRect)rect
{
	// Draw the image
	//[imgObjArray[0] drawAtPoint:CGPointMake(20, 20)];
	switch(gameStatus)
	{
		case GAME_START:
		{

		}
		break;
		case GAME_LOOP:
		{

			CGContextRef context = UIGraphicsGetCurrentContext();

			CGContextSetStrokeColorWithColor(context, [UIColor blackColor].CGColor);
			CGContextSetLineWidth(context, 2.0);
	
			// Draw a single line from left to right
			CGContextMoveToPoint(context, 20.0, 10.0);
			CGContextAddLineToPoint(context, 250.0, 10.0);
			CGContextAddLineToPoint(context, 250.0, 470.0);
			CGContextAddLineToPoint(context, 20.0,  470.0);
			CGContextAddLineToPoint(context, 20.0, 10.0);
			CGContextStrokePath(context);

			int idrawindex = 0;

			for ( int i = 0; i < 20; i++ )
			for ( int j = 0; j < 10; j++ )
			{
				idrawindex = g_scene.isOneGridNeedShow(j + 10*i);

				if( idrawindex > 0 )
				{
					//DrawBitmap( hmemDC, g_blockbmp_array[idrawindex-1], 20 + 23*j , 20 + 23*i );
					[imgObjArray[idrawindex - 1] drawAtPoint:CGPointMake(20 + 23*j, 10 + 23*i)];
				}
			}	

			unsigned char cdata[16];

			if(g_scene.GetNextBlockData(cdata, 16) == true)
			{
				for( int i = 0; i < 4; i++)
				for( int j = 0; j < 4; j++)
				{
					if(cdata[j+i*4] != 0)
					{
						[imgObjArray[cdata[j+i*4] - 1] drawAtPoint:CGPointMake(245 + 23*j, 10 + 23*i)];
					}
				}
			
			}

		}
		break;
		case GAME_OVER:
		{
			[gameoverLabel setHidden: NO];
			[button setHidden: NO];
		
			[timer invalidate];
			timer = nil;
		}
		break;
	};

	/*
	int i;

	for(i = 0; i < 10; i++)
	{
		CGSize  size  = CGSizeMake(16, 256);
		CGPoint point = CGPointMake(10, 10 + i*size.width);
		DrawImageList(contex, imgObj, CGPoint point, CGSize size, 0);
	}
	*/
}

- (void)dealloc
{
	[starttime release];
	g_scene.DestroyScene();
	[super dealloc];
}

/*
- (void)motionBegan:(UIEventSubtype)motion withEvent:(UIEvent *)event
{
	iphone_trace("begin motionevent!\n");

	if(motion == UIEventSubtypeMotionShake)
	{
		iphone_trace("it's shake!\n");
		g_scene.user_rotate();
		[self setNeedsDisplay];
	}
}

- (void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event
{
	iphone_trace("end!\n");
}

- (void)motionCancelled:(UIEventSubtype)motion withEvent:(UIEvent *)event
{
	iphone_trace("cancelled!!\n");
}
*/

- (BOOL)canBecomeFirstResponder 
{
    return YES;
}

- (void)BlockMoveRight
{
	g_scene.user_right();
}

- (void)BlockMoveLeft
{
	g_scene.user_left();
}

- (void)BlockMoveDown
{
	g_scene.user_down();
}

- (void)InvalidateView
{
	[self setNeedsDisplay];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event 
{   
	//iphone_trace("touches began\n");
	UITouch *touch = [touches anyObject];
	gestureStartPoint = [touch locationInView:self];
	gestureCurrentPoint = gestureStartPoint;
	needProcess = NO;

	starttime = [[NSDate date] retain];
/*
	int x = gestureStartPoint.x/23 ;
	int y = gestureStartPoint.y/23 ;

	iphone_trace("x=%d\n", x);
	iphone_trace("y=%d\n", y);

	needProcess = g_scene.isPointInActivityBlock(x + y*12);
	iphone_trace("needProcess=%d\n", needProcess);
*/
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	//iphone_trace("touches moved\n");
	
	 UITouch *touch = [touches anyObject];
	 CGPoint currentPosition = [touch locationInView:self];
		     
	 CGFloat deltaX = fabsf(gestureCurrentPoint.x - currentPosition.x);
	 CGFloat deltaY = fabsf(gestureCurrentPoint.y - currentPosition.y);
	 
	 CGFloat ratioXY = deltaX/deltaY;				     
	 
	 //NSTimeInterval timeInterval = [starttime timeIntervalSinceNow];
	 //iphone_trace("interval=%f\n", timeInterval);
	 
	 BOOL canMove = NO;

	 //iphone_trace("deltaX=%f\n", deltaX);
	 
//	 if (deltaX >= kMinimumGestureLength && deltaY <= kMaximumVariance) 
	 if( ratioXY > 2 /*&& deltaX >= 20 && fabsf(timeInterval) > 0.17*/)
	 {
	 	static int delta_interval = 0;
		int temp_interval = (int)(deltaX)/30;

//		iphone_trace("delta_interval=%d\n", delta_interval);
//		iphone_trace("temp_interval=%d\n",  temp_interval);

		if(temp_interval != delta_interval)
		{
			canMove = YES;
			delta_interval = temp_interval;

			if(temp_interval < delta_interval)
			{
				gestureCurrentPoint = currentPosition;
			}
		}

		if(canMove == YES)
		{
	 		if(currentPosition.x < gestureCurrentPoint.x)//right->left
			{
				g_scene.user_left();
			}
			else//left->right
			{
				g_scene.user_right();
			}

			[self setNeedsDisplay];
			
			needProcess = YES;
			canMove = NO;
		}

	 }

	 //gestureCurrentPoint = currentPosition;
	 /*
	 else if (deltaY >= kMinimumGestureLength && deltaX <= kMaximumVariance)
	 {
	 	if(currentPosition.y < gestureStartPoint.y)//bottom->top
		{
			
		}
		else//top->bottom
		{
			g_scene.user_fall();
			[self setNeedsDisplay];
		}

		needProcess = YES;
	 }
	*/																														    
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	NSTimeInterval timeInterval = [starttime timeIntervalSinceNow];
//	iphone_trace("interval=%f\n", timeInterval);

	if(needProcess && fabsf(timeInterval) > 0.18 )
	{
		return;
	}

	UITouch *touch = [touches anyObject];
	CGPoint currentPosition = [touch locationInView:self];
 
 	CGFloat deltaX = fabsf(gestureStartPoint.x - currentPosition.x);
	CGFloat deltaY = fabsf(gestureStartPoint.y - currentPosition.y);
	
	CGFloat ratioXY = deltaX/deltaY; 
   	//CGFloat ratioYX = deltaY/deltaX; 

	//iphone_trace("deltaX=%f\n", deltaX);
	//iphone_trace("deltaY=%f\n", deltaY);

	 //if (deltaX >= kMinimumGestureLength && deltaY <= kMaximumVariance) 
	/*
	if( ratioXY > 3) 
	{
		if(currentPosition.x < gestureStartPoint.x)//right->left
		{
			g_scene.user_left();
		}
		else//left->right
		{
			g_scene.user_right();
		}

		[self setNeedsDisplay];
		needProcess = YES;
	 }	
	 */
	 //else if (deltaY >= kMinimumGestureLength && deltaX <= kMaximumVariance)
	 if(ratioXY < 0.25 && deltaY >= 20)
	 {
	 	if(currentPosition.y > gestureStartPoint.y)//bottom<-top
		{
			g_scene.user_fall();
			[self setNeedsDisplay];
		}
	 }
	 else
	 {
		g_scene.user_rotate();
		[self setNeedsDisplay];
	 }

	  [starttime release];
      starttime = nil;

}

@end
