
#import"imineApplication.h"
#import"imineViewControler.h"
#import"trace.h" 

//extern void iphone_trace(const char * format, ...);

@implementation CustomViewControllerAppDelegate 

- (void)applicationDidFinishLaunching:(UIApplication *)application 
{
    window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
   // windows.backgroundColor = [UIColor redColor];
    //CGRect rect = CGRectMake(50,150,150,150);
    controller = [[CustomViewController alloc] init];
    [window addSubview:controller.view];
    [window makeKeyAndVisible];
}

- (void)dealloc 
{
    [controller release];
    [window release];
    [super dealloc];
}

/*
- (void)motionBegan:(UIEventSubtype)motion withEvent:(UIEvent *)event
{
	iphone_trace("begin motionevent!\n");

	if(motion == UIEventSubtypeMotionShake)
	{
		iphone_trace("it's shake!\n");
	//	g_scene.user_rotate();
	//	[self setNeedsDisplay];
	}
}

- (void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event
{
	NSLog(@"end!");
}

- (void)motionCancelled:(UIEventSubtype)motion withEvent:(UIEvent *)event
{
	NSLog(@"cancelled!!");
}
*/

@end
