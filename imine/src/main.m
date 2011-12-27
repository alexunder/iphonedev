/*

File: main.m
Abstract: Standard main file.

Version: 1.0
Copyright (C) 2010 Foundation Inc. All Rights Reserved.

*/

#import <UIKit/UIKit.h>
#import "stdafx.h"
#import "trace.h"

void iphone_trace(const char * format, ...)
{
#if 1
	/*
	NSArray *paths = NSSearchPathForDirectoriesInDomains (NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *doc_dir = [paths objectAtIndex:0];
	      
	char path[MAX_PATH_LEN + 1];
	*/ 
	FILE * pfile = NULL;
	//sprintf(path, "%s%s", [doc_dir UTF8String], "log.txt");
	pfile = fopen("/tmp/log.txt", "a+");
	
	char buf[256];
	memset(buf, 0, 256);
		
	if(pfile != NULL)
	{
		va_list ap;
		va_start(ap, format);
		vsnprintf(buf, 256, format, ap);
		va_end(ap);
					 
		fwrite(buf, strlen(buf), 1, pfile);
		fclose(pfile);
	}
#endif
}


int main(int argc, char *argv[])
{
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, @"CustomViewControllerAppDelegate");
    [pool release];
    return retVal;
}
