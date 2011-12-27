
#import <UIKit/UIKit.h>

@class CustomViewController;

@interface CustomViewControllerAppDelegate : NSObject<UIApplicationDelegate>  
{
    UIWindow *window;
    CustomViewController* controller;
}

- (void)applicationDidFinishLaunching:(UIApplication *)application;
@end
