
#import"imineViewControler.h"
//#import"imineBoardView.h"
#import "trace.h"

@implementation CustomViewController

#define kFilteringFactor 0.1
#define THREHOLD         60

CGFloat MAX_ACCEL_PER_SEC = 200;

-(void)viewDidLoad 
{

	bdv = [[BoardView alloc] initWithFrame:self.view.frame];
	self.view = bdv;
    bdv.backgroundColor = [UIColor whiteColor];
	[bdv becomeFirstResponder];

	[[UIAccelerometer sharedAccelerometer] setUpdateInterval: 0.03]; // 30 fps
	[[UIAccelerometer sharedAccelerometer] setDelegate:self];
	[super viewDidLoad];
}

- (void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration
{
	/*
	if(acceleration.x > 0 || acceleration.y > 0 || acceleration.z > 0)
	{
		iphone_trace("acceleration.x=%f\n",acceleration.x);
		iphone_trace("acceleration.y=%f\n",acceleration.y);
		iphone_trace("acceleration.z=%f\n",acceleration.z);
	}
	*/
	//iphone_trace("start accelerometer\n");
	
	accelX = acceleration.x - ( (acceleration.x*kFilteringFactor) + (accelX*(1.0 - kFilteringFactor)) );
	accelY = acceleration.y - ( (acceleration.y*kFilteringFactor) + (accelY*(1.0 - kFilteringFactor)) );
	accelZ = acceleration.z - ( (acceleration.z*kFilteringFactor) + (accelZ*(1.0 - kFilteringFactor)) );

	/*
	iphone_trace("accelX=%f\n",accelX);
	iphone_trace("accely=%f\n",accelY);
	iphone_trace("accelZ=%f\n",accelZ);
	*/
	
	CGFloat X = accelX*MAX_ACCEL_PER_SEC;
	CGFloat Y = accelY*MAX_ACCEL_PER_SEC;

//	iphone_trace("X=%f\n",X);
//	iphone_trace("Y=%f\n",Y);
	
	/*
	if(fabs(X) < 60 || fabs(Y) < 60)
	{
		return;
	}
	*/

	BOOL isneedDraw = NO;
	
	if(fabs(X) >= THREHOLD)
	{
		if(X > 0)
		{
			[bdv BlockMoveRight];		
		}
		else
		{
			[bdv BlockMoveLeft];		
		}

		isneedDraw = YES;
	}
	
	if(fabs(Y) >= THREHOLD*2)
	{
		if(Y < 0)
		{
			[bdv BlockMoveDown];
			isneedDraw = YES;
		}
	}

	if(isneedDraw)
	{
		[bdv InvalidateView];	
	}
	/*
	NSTimeInterval elapsedTime = acceleration.timestamp - lastAccelTimestamp;
	ballVelocityX = ballVelocityX + (acceleration.x * MAX_ACCEL_PER_SEC * elapsedTime);
	ballVelocityY = ballVelocityY - (acceleration.y * MAX_ACCEL_PER_SEC * elapsedTime);

	iphone_trace("lastAccelTimesstamp=%f\n", lastAccelTimestamp);

	if (lastAccelTimestamp > 0) 
	{
		// recalc ball position
		X = (ballVelocityX * elapsedTime);
		Y = (ballVelocityY * elapsedTime);

		iphone_trace("X=%f\n", X);
		iphone_trace("Y=%f\n", Y);

		if( X >= 20.0 )
		{
			[bdv BlockMoveRight];
		}
		else if( X <= -20.0)
		{
			[bdv BlockMoveLeft];
		}

		[bdv InvalidateView];
	}

	lastAccelTimestamp = acceleration.timestamp;
	*/
	//iphone_trace("end accelerometer\n");
}

- (void)dealloc 
{
	[[UIAccelerometer sharedAccelerometer] setDelegate: nil];
    [bdv release];
	[super dealloc];
}
@end
