//Foundation lab
#import <UIKit/UIKit.h>
#import"imineBoardView.h"

@interface CustomViewController : UIViewController
{
	BoardView * bdv;

//	CGFloat ballX;
//	CGFloat ballY;
	CGFloat ballVelocityX;
	CGFloat ballVelocityY;
	NSTimeInterval lastAccelTimestamp;
							
	UIAccelerationValue filteredAccelX;
	UIAccelerationValue filteredAccelY;
	
	UIAccelerationValue accelX;
	UIAccelerationValue accelY;
	UIAccelerationValue accelZ;
}

@end
