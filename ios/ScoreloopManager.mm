// Copyright (c) 2012 ursinepaw.com
#import "ScoreloopManager.h"
#import "SLCSessionController.h"
#import <GameKit/GameKit.h>

#define SL_GAME_ID @"YOUR GAME ID"
#define SL_SECRET_KEY @"YOUR GAME SECRET KEY"

#ifndef DISTRIBUTION
#   define DLog NSLog
#else
#   define DLog(...)
#endif

static ScoreloopManager* mSelf = nil;

@implementation ScoreloopManager

@synthesize mClient;
@synthesize mCanOpenEula;
@synthesize mTOSCallback;

+ (ScoreloopManager*)sharedManager
{
    if (mSelf == nil)
    {
        mSelf = [[ScoreloopManager alloc] init];
    }
    
    return mSelf;
}

- (id)init
{
    if (self = [super init])
    {
		mClient = [[SLCClient alloc] initWithGameId:SL_GAME_ID secret:SL_SECRET_KEY delegate:self];
		
		[mClient setFeature:kSLCFeatureGameCenter enabled:mClient.isGameCenterAvailable];
		
		mSessionController = [[SLCSessionController alloc] initWithDelegate:self];
		if (mClient.gameCenterAvailable)
			[mSessionController authenticateGameCenterAccount];
		
		mCanOpenEula = false;
		mInternetAvaileble = true;
		mTOSCallback = 0;
		
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(receiveNetworkError:)
													 name:kSLCServerDidLoseNetworkConnectionNotification object:nil];
		
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(receiveNetwork:)
													 name:kSLCServerDidRegainNetworkConnectionNotification object:nil];
		
        return self;
    }
    return nil;
}

- (void)dealloc
{
	[mSessionController release];
	[mClient release];
    [super dealloc];
}

- (BOOL) client:(SLCClient*) client shouldAuthenticateSession:(SLCSession*) session
{
	DLog(@"%@", session);
	return true;
}

- (void) client:(SLCClient*) client didAuthenticateSession:(SLCSession*) session
{
	DLog(@"%@", session);
	
	/*
	 if (session.gameCenterAccountAuthenticated)
		[GKAchievement resetAchievementsWithCompletionHandler:^(NSError *error)
		 {
			 if (error != nil)
			 {
			 }
				 // handle errors
		}];
	 */
}

- (BOOL) intentionViewControl:(id<SLCIntentionViewControl>) control shouldShowView:(UIView*) view forIntention:(SLCIntention) intention userInfo:(NSDictionary*) userInfo
{
	return mCanOpenEula;
}

- (void) intentionViewControl:(id<SLCIntentionViewControl>) control didOrderOutView:(UIView*) view forIntention:(SLCIntention) intention userInfo:(NSDictionary*) userInfo
{
	NSNumber* value = [userInfo objectForKey:@"SLCClientIntentionAcceptTermsAndConditionsAcceptedKey"];
	
	if ([value boolValue] && mTOSCallback)
	{
		(*mTOSCallback)();
	}
}

- (void) requestAuthentication
{
	[mSessionController requestAuthentication];
}

- (void) requestControllerDidReceiveResponse:(SLCRequestController*) aController
{
}

- (void) requestController:(SLCRequestController*) aController didFailWithError:(NSError*) anError
{
	NSInteger error_code = anError.code;
	
	if (error_code == kSLCStatusIntentionTermsAndConditionsCancelled)
	{
		mCanOpenEula = false;
	}
}

- (void) receiveNetworkError:(NSNotification*) aNotification
{
	if (mInternetAvaileble)
	{
		mInternetAvaileble = false;
		//PopupNotifier::Show("Network is unavailable", PopupNotifier::ScoreloopMessage);
	}
}

- (void) receiveNetwork:(NSNotification*) aNotification
{
	if (!mInternetAvaileble)
	{
		mInternetAvaileble = true;
		//PopupNotifier::Show("Network is available", PopupNotifier::ScoreloopMessage);
	}
}
@end

@implementation SLCUserControllerSmart

- (id)initWithCompletionCallback:(Scoreloop::RequestCompletionCallback)callback
{
	if (self = [super initWithDelegate:self])
	{
		mCallback = callback;
		return self;
	}
	return nil;
}

- (void) requestControllerDidEnqueueRequest:(SLCRequestController*) aController
{
}

- (void) requestControllerDidReceiveResponse:(SLCRequestController*) aController
{
	if (self != aController)
	{
		[super requestControllerDidReceiveResponse:aController];
		return;
	}
	
	DLog(@"%@", self.user);
	
	if (mCallback)
		(*mCallback)(Scoreloop::ErrorCode::Ok);
}

- (void) requestController:(SLCRequestController*) aController didFailWithError:(NSError*) anError
{
	if (self != aController)
	{
		[super requestController:aController didFailWithError:anError];
		return;
	}
	
	DLog(@"%@", anError);
	DLog(@"%@", @"-------");
	DLog(@"%@", anError.userInfo);
	
	NSNumber* subCode = [anError.userInfo objectForKey:@"subCode"];
	NSInteger error_code;
	if (subCode)
		error_code = [subCode integerValue];
	else
		error_code = anError.code;
	
	switch (error_code)
	{
			/*case 604:// eula
			 break;
			 case 403: //Device does not belong to user
			 break;*/
		case kSLCErrorUserInvalidArguments:
			if (mCallback)
			{
				NSArray* result = [[anError.userInfo objectForKey:@"error"] objectForKey:@"args"];
				
				if (!result)
				{
					(*mCallback)(Scoreloop::ErrorCode::UnknownError);
					return;
				}
				
				result = [result objectAtIndex:0];
				
				if ([result count] != 2)
				{
					(*mCallback)(Scoreloop::ErrorCode::UnknownError);
					return;
				}
				
				NSString* one = [result objectAtIndex:0];
				NSString* two = [result objectAtIndex:1];
				
				if ([one isEqualToString:@"login"] && [two isEqualToString:@"has already been taken"])
				{
					(*mCallback)(Scoreloop::ErrorCode::UsernameAlreadyTaken);
					return;
				}
				
				if ([one isEqualToString:@"login"] && [two isEqualToString:@"is too short (minimum is 3 characters)"])
				{
					(*mCallback)(Scoreloop::ErrorCode::UsernameTooShort);
					return;
				}
				
				if ([one isEqualToString:@"login"] && [two isEqualToString:@"is invalid"])
				{
					(*mCallback)(Scoreloop::ErrorCode::UsernameFormatInvalid);
					return;
				}
				
				if ([one isEqualToString:@"email"] && [two isEqualToString:@"has already been taken"])
				{
					(*mCallback)(Scoreloop::ErrorCode::EmailAlreadyTaken);
					return;
				}
				
				if ([one isEqualToString:@"email"] && [two isEqualToString:@"should look like an email address"])
				{
					(*mCallback)(Scoreloop::ErrorCode::EmailFormatInvalid);
					return;
				}
				
				(*mCallback)(Scoreloop::ErrorCode::UnknownError);
			}
		case kSLCErrorBuddyhoodInvalidArguments:
			if (mCallback)
				(*mCallback)(Scoreloop::ErrorCode::RequestFailed);
			break;
		default:
			if (mCallback)
				(*mCallback)(Scoreloop::ErrorCode::UnknownError);
			break;
	}
}

- (void) dealloc
{
	mCallback = 0;
	[super dealloc];
}
@end

@implementation SLCScoreControllerSmart

- (id)initWithCompletionCallback:(Scoreloop::RequestCompletionCallback)callback
{
	if (self = [super initWithDelegate:self])
	{
		mCallback = callback;
		return self;
	}
	return nil;
}

- (void) requestControllerDidEnqueueRequest:(SLCRequestController*) aController
{
}

- (void) requestControllerDidReceiveResponse:(SLCRequestController*) aController
{
	if (self != aController)
	{
		[super requestControllerDidReceiveResponse:aController];
		return;
	}
	
	DLog(@"%@", self.score);
	if (mCallback)
		(*mCallback)(Scoreloop::ErrorCode::Ok);
}

- (void) requestController:(SLCRequestController*) aController didFailWithError:(NSError*) anError
{
	if (self != aController)
	{
		[super requestController:aController didFailWithError:anError];
		return;
	}
	
	DLog(@"%@", anError);
	if (mCallback)
		(*mCallback)(Scoreloop::ErrorCode::UnknownError);
}

- (void) dealloc
{
	mCallback = 0;
	[super dealloc];
}
@end

@implementation SLCScoresControllerSmart

- (id)initWithCompletionCallback:(Scoreloop::RequestCompletionCallback)callback
{
	if (self = [super initWithDelegate:self])
	{
		mCallback = callback;
		return self;
	}
	return nil;
}

- (void) requestControllerDidEnqueueRequest:(SLCRequestController*) aController
{
}

- (void) requestControllerDidReceiveResponse:(SLCRequestController*) aController
{
	if (self != aController)
	{
		[super requestControllerDidReceiveResponse:aController];
		return;
	}
	
	DLog(@"%@", self.scores);
	
	if (mCallback)
		(*mCallback)(Scoreloop::ErrorCode::Ok);
}

- (void) requestController:(SLCRequestController*) aController didFailWithError:(NSError*) anError
{
	if (self != aController)
	{
		[super requestController:aController didFailWithError:anError];
		return;
	}
	
	DLog(@"%@", anError);
	if (mCallback)
		(*mCallback)(Scoreloop::ErrorCode::UnknownError);
}

- (void) dealloc
{
	mCallback = 0;
	[super dealloc];
}
@end

@implementation SLCUsersControllerSmart

- (id)initWithCompletionCallback:(Scoreloop::RequestCompletionCallback)callback
{
	if (self = [super initWithDelegate:self])
	{
		mCallback = callback;
		return self;
	}
	return nil;
}

- (void) requestControllerDidEnqueueRequest:(SLCRequestController*) aController
{
}

- (void) requestControllerDidReceiveResponse:(SLCRequestController*) aController
{
	if (self != aController)
	{
		[super requestControllerDidReceiveResponse:aController];
		return;
	}
	
	DLog(@"%@", self.users);
	
	if (mCallback)
		(*mCallback)(Scoreloop::ErrorCode::Ok);
}

- (void) requestController:(SLCRequestController*) aController didFailWithError:(NSError*) anError
{
	if (self != aController)
	{
		[super requestController:aController didFailWithError:anError];
		return;
	}
	
	DLog(@"%@", anError);
	if (mCallback)
		(*mCallback)(Scoreloop::ErrorCode::UnknownError);
}

- (void) dealloc
{
	mCallback = 0;
	[super dealloc];
}
@end

@implementation SLCAchievementsControllerSmart

- (id)initWithCompletionCallback:(Scoreloop::RequestCompletionCallback)callback
{
	if (self = [super initWithDelegate:self])
	{
		mCallback = callback;
		mGameCenterSync = NO;
		return self;
	}
	return nil;
}

- (BOOL)synchronizeAchievements
{
	mGameCenterSync = YES;
	return [super synchronizeAchievements];
}

- (void) requestControllerDidEnqueueRequest:(SLCRequestController*) aController
{
}

- (void) requestControllerDidReceiveResponse:(SLCRequestController*) aController
{
	if (self != aController)
	{
		[super requestControllerDidReceiveResponse:aController];
		return;
	}
	
	DLog(@"%@", self.achievements);
	
	if (self.achievements && mGameCenterSync && [ScoreloopManager sharedManager].mClient.isGameCenterAvailable)
	{
		for(SLCAchievement* slc_achievement in self.achievements)
		{
			NSString* identifier = slc_achievement.award.identifier;
			BOOL is_achieved = slc_achievement.isAchieved;
			
			if (is_achieved)
			{
				GKAchievement *achievement = [[[GKAchievement alloc] initWithIdentifier: identifier] autorelease];
				if (achievement && !achievement.completed)
				{
					achievement.percentComplete = 100.f;
					[achievement reportAchievementWithCompletionHandler:^(NSError *error)
					{
						if (error)
							DLog(@"%@", error);
					}];
				}
			}
		}
		mGameCenterSync = NO;
	}
	
	if (mCallback)
		(*mCallback)(Scoreloop::ErrorCode::Ok);
}

- (void) requestController:(SLCRequestController*) aController didFailWithError:(NSError*) anError
{
	if (self != aController)
	{
		[super requestController:aController didFailWithError:anError];
		return;
	}
	
	DLog(@"%@", anError);
	if (mCallback)
		(*mCallback)(Scoreloop::ErrorCode::UnknownError);
}

- (void) dealloc
{
	mCallback = 0;
	[super dealloc];
}
@end

@implementation SLCRankingControllerSmart

- (id)initWithCompletionCallback:(Scoreloop::RequestCompletionCallback)callback
{
	if (self = [super initWithDelegate:self])
	{
		mCallback = callback;
		return self;
	}
	return nil;
}

- (void) requestControllerDidEnqueueRequest:(SLCRequestController*) aController
{
}

- (void) requestControllerDidReceiveResponse:(SLCRequestController*) aController
{
	if (self != aController)
	{
		[super requestControllerDidReceiveResponse:aController];
		return;
	}
	
	DLog(@"%@", self.searchList);
	
	if (mCallback)
		(*mCallback)(Scoreloop::ErrorCode::Ok);
}

- (void) requestController:(SLCRequestController*) aController didFailWithError:(NSError*) anError
{
	if (self != aController)
	{
		[super requestController:aController didFailWithError:anError];
		return;
	}
	
	DLog(@"%@", anError);
	if (mCallback)
		(*mCallback)(Scoreloop::ErrorCode::UnknownError);
}

- (void) dealloc
{
	mCallback = 0;
	[super dealloc];
}
@end
