// Copyright (c) 2012 ursinepaw.com
#import <Foundation/Foundation.h>
#import "Scoreloop/CoreSocial/CoreSocial.h"
#import "../Scoreloop.h"

@interface ScoreloopManager : NSObject <SLCClientDelegate, SLCIntentionViewControl, SLCRequestControllerDelegate>
{
	SLCClient*	mClient;
	SLCSessionController* mSessionController;
	bool mCanOpenEula;
	bool mInternetAvaileble;
	
	Scoreloop::TermsOfServiceAcceptionCallback mTOSCallback;
}
@property(readonly) SLCClient* mClient;
@property(assign)	bool mCanOpenEula;
@property(assign)	Scoreloop::TermsOfServiceAcceptionCallback mTOSCallback;


+ (ScoreloopManager*)sharedManager;
- (id)init;

//SLCClientDelegate
- (BOOL) client:(SLCClient*) client shouldAuthenticateSession:(SLCSession*) session;
- (void) client:(SLCClient*) client didAuthenticateSession:(SLCSession*) session;
//SLCClientDelegate
- (BOOL) intentionViewControl:(id<SLCIntentionViewControl>) control shouldShowView:(UIView*) view forIntention:(SLCIntention) intention userInfo:(NSDictionary*) userInfo;
- (void) intentionViewControl:(id<SLCIntentionViewControl>) control didOrderOutView:(UIView*) view forIntention:(SLCIntention) intention userInfo:(NSDictionary*) userInfo;
- (void) requestAuthentication;
//SLCRequestControllerDelegate
- (void) requestControllerDidReceiveResponse:(SLCRequestController*) aController;
- (void) requestController:(SLCRequestController*) aController didFailWithError:(NSError*) anError;

- (void) receiveNetworkError:(NSNotification*) aNotification;
- (void) receiveNetwork:(NSNotification*) aNotification;

- (void)dealloc;
@end

@interface SLCUserControllerSmart : SLCUserController <SLCRequestControllerDelegate>
{
	Scoreloop::RequestCompletionCallback mCallback;
}
- (id)initWithCompletionCallback:(Scoreloop::RequestCompletionCallback)callback;

- (void) requestControllerDidEnqueueRequest:(SLCRequestController*) aController;
- (void) requestControllerDidReceiveResponse:(SLCRequestController*) aController;
- (void) requestController:(SLCRequestController*) aController didFailWithError:(NSError*) anError;
- (void) dealloc;
@end

@interface SLCScoreControllerSmart : SLCScoreController <SLCRequestControllerDelegate>
{
	Scoreloop::RequestCompletionCallback mCallback;
}
- (id)initWithCompletionCallback:(Scoreloop::RequestCompletionCallback)callback;

- (void) requestControllerDidEnqueueRequest:(SLCRequestController*) aController;
- (void) requestControllerDidReceiveResponse:(SLCRequestController*) aController;
- (void) requestController:(SLCRequestController*) aController didFailWithError:(NSError*) anError;
- (void) dealloc;
@end

@interface SLCScoresControllerSmart : SLCScoresController <SLCRequestControllerDelegate>
{
	Scoreloop::RequestCompletionCallback mCallback;
}
- (id)initWithCompletionCallback:(Scoreloop::RequestCompletionCallback)callback;

- (void) requestControllerDidEnqueueRequest:(SLCRequestController*) aController;
- (void) requestControllerDidReceiveResponse:(SLCRequestController*) aController;
- (void) requestController:(SLCRequestController*) aController didFailWithError:(NSError*) anError;
- (void) dealloc;
@end

@interface SLCUsersControllerSmart : SLCUsersController <SLCRequestControllerDelegate>
{
	Scoreloop::RequestCompletionCallback mCallback;
}
- (id)initWithCompletionCallback:(Scoreloop::RequestCompletionCallback)callback;

- (void) requestControllerDidEnqueueRequest:(SLCRequestController*) aController;
- (void) requestControllerDidReceiveResponse:(SLCRequestController*) aController;
- (void) requestController:(SLCRequestController*) aController didFailWithError:(NSError*) anError;
- (void) dealloc;
@end

@interface SLCAchievementsControllerSmart : SLCAchievementsController <SLCRequestControllerDelegate>
{
	Scoreloop::RequestCompletionCallback mCallback;
	BOOL mGameCenterSync;
}
- (id)initWithCompletionCallback:(Scoreloop::RequestCompletionCallback)callback;

- (void) requestControllerDidEnqueueRequest:(SLCRequestController*) aController;
- (void) requestControllerDidReceiveResponse:(SLCRequestController*) aController;
- (void) requestController:(SLCRequestController*) aController didFailWithError:(NSError*) anError;
- (void) dealloc;
@end

@interface SLCRankingControllerSmart : SLCRankingController <SLCRequestControllerDelegate>
{
	Scoreloop::RequestCompletionCallback mCallback;
}
- (id)initWithCompletionCallback:(Scoreloop::RequestCompletionCallback)callback;

- (void) requestControllerDidEnqueueRequest:(SLCRequestController*) aController;
- (void) requestControllerDidReceiveResponse:(SLCRequestController*) aController;
- (void) requestController:(SLCRequestController*) aController didFailWithError:(NSError*) anError;
- (void) dealloc;
@end