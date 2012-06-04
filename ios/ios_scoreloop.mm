// Copyright (c) 2012 ursinepaw.com
#include "..Scoreloop.h"
#include "ScoreloopManager.h"
#import "SLCSessionController.h"
#include <string>

namespace Scoreloop
{
	
	void Client::Initialize(TermsOfServiceAcceptionCallback callback)
	{
		if ([ScoreloopManager sharedManager].mClient.session.termsAndConditionsAccepted)
		{
			if (callback)
				(*callback)();
		}
		else
		{
			[ScoreloopManager sharedManager].mCanOpenEula = true;
			[ScoreloopManager sharedManager].mTOSCallback = callback;
			[[ScoreloopManager sharedManager] requestAuthentication];
		}
	}

	void Client::Shutdown()
	{
		//do nothing
	}

	bool Client::IsInitialized()
	{
		return [ScoreloopManager sharedManager].mClient.session.termsAndConditionsAccepted;
	}

	const char* User::GetLogin(User* self)
	{
		static std::string result;
		SLCUser* slc_user = (SLCUser*)self;
		
		NSString* login = slc_user.login;
		result = login ? [login UTF8String] : "";
		
		return result.c_str();
	}

	const char* User::GetEmail(User* self)
	{
		static std::string result;
		SLCUser* slc_user = (SLCUser*)self;
		
		NSString* email = slc_user.emailAddress;
		result = email ? [email UTF8String] : "";
		
		return result.c_str();
	}
	
	const char* User::GetIdentifier(User* self)
	{
		static std::string result;
		SLCUser* slc_user = (SLCUser*)self;
		
		NSString* identifier = slc_user.identifier;
		result = [identifier UTF8String];
		
		return result.c_str();
	}

	void User::SetLogin(User* user, const char* login)
	{
		((SLCUser*)user).login = [NSString stringWithUTF8String:login];
	}

	void User::SetEmail(User* user, const char* email)
	{
		((SLCUser*)user).emailAddress = [NSString stringWithUTF8String:email];
	}
	
	UserList* User::GetBuddyUsers(User* user)
	{
		NSArray* array = ((SLCUser*)user).buddyUsers;
		return (UserList*)array;
	}
	
	User* Session::GetUser()
	{
		return (User*)[SLCSession currentSession].user;
	}

	UserController* UserController::Create(RequestCompletionCallback callback)
	{
		SLCUserControllerSmart* controller = [[SLCUserControllerSmart alloc] initWithCompletionCallback:callback];
		return (UserController*)controller;
	}
	
	void UserController::Release(UserController* self)
	{
		SLCUserControllerSmart* controller = (SLCUserControllerSmart*)self;
		[controller release];
	}
	
	User* UserController::GetUser(UserController* self)
	{
		SLCUserControllerSmart* controller = (SLCUserControllerSmart*)self;
		return (User*)controller.user;
	}

	void UserController::LoadUser(UserController* self)
	{
		SLCUserControllerSmart* controller = (SLCUserControllerSmart*)self;
		[controller requestUser];
	}

	void UserController::SubmitUser(UserController* self)
	{
		SLCUserControllerSmart* controller = (SLCUserControllerSmart*)self;
		[controller updateUser];
	}
	
	void UserController::SetUser(UserController* self, User* user)
	{
		SLCUserControllerSmart* controller = (SLCUserControllerSmart*)self;
		controller.user = (SLCUser*)user;
	}
	
	void UserController::AddAsBuddy(UserController* self)
	{
		SLCUserControllerSmart* controller = (SLCUserControllerSmart*)self;
		[controller addAsBuddy];
	}
	
	void UserController::RemoveAsBuddy(UserController* self)
	{
		SLCUserControllerSmart* controller = (SLCUserControllerSmart*)self;
		[controller removeAsBuddy];
	}
	
	void UserController::RequestBuddies(UserController* self)
	{
		SLCUserControllerSmart* controller = (SLCUserControllerSmart*)self;
		[controller requestBuddies];
	}
	
	ScoreController* ScoreController::Create(RequestCompletionCallback callback)
	{
		SLCScoreControllerSmart* controller = [[SLCScoreControllerSmart alloc] initWithCompletionCallback:callback];
		return (ScoreController*)controller;
	}
	
	void ScoreController::Release(ScoreController* self)
	{
		SLCScoreControllerSmart* controller = (SLCScoreControllerSmart*)self;
		[controller release];
	}
	
	Score* ScoreController::GetScore(ScoreController* self)
	{
		SLCScoreControllerSmart* controller = (SLCScoreControllerSmart*)self;
		return (Score*)controller.score;
	}
	
	void ScoreController::SubmitScore(ScoreController* self, double result, double minor, unsigned int level, unsigned int mode, std::map<std::string, std::string>* context)
	{
		SLCScoreControllerSmart* controller = (SLCScoreControllerSmart*)self;
		
		NSMutableDictionary* dict_context = nil;
		if (context)
		{
			dict_context = [NSMutableDictionary dictionary];
			for (std::map<std::string, std::string>::const_iterator it = context->begin(); it != context->end(); ++it)
			{
				NSString* key = [NSString stringWithUTF8String:it->first.c_str()];
				NSString* value = [NSString stringWithUTF8String:it->second.c_str()];
				
				[dict_context setObject:value forKey:key];
			}
		}
		
		
		SLCScore *aScore = [[[SLCScore alloc] initWithResult:[NSNumber numberWithDouble:result] context:dict_context] autorelease];
		
		aScore.minorResult = [NSNumber numberWithDouble:minor];
		aScore.level = [NSNumber numberWithUnsignedInt:level];
		aScore.mode = [NSNumber numberWithUnsignedInt:mode];
		
		[controller submitScore:aScore];
	}
	
	
	ScoresController* ScoresController::Create(RequestCompletionCallback callback)
	{
		SLCScoresControllerSmart* controller = [[SLCScoresControllerSmart alloc] initWithCompletionCallback:callback];
		return (ScoresController*)controller;
	}
	
	void ScoresController::Release(ScoresController* self)
	{
		SLCScoresControllerSmart* controller = (SLCScoresControllerSmart*)self;
		[controller release];
	}
	
	void ScoresController::SetSearchCriteria(ScoresController* self, SearchCriteria::Enum criteria)
	{
		SLCScoresControllerSmart* controller = (SLCScoresControllerSmart*)self;
		
		switch (criteria)
		{
			case SearchCriteria::Global:
				controller.searchList = [SLCSearchList globalScoreSearchList];
				break;
			case SearchCriteria::TwentyFourHour:
				controller.searchList = [SLCSearchList twentyFourHourScoreSearchList];
				break;
			case SearchCriteria::UserCountry:
				controller.searchList = [SLCSearchList userCountryScoreSearchList];
				break;
			case SearchCriteria::BuddyhoodOnly:
				controller.searchList = [SLCSearchList buddiesScoreSearchList];
				break;
		}
	}
	
	void ScoresController::SetGameMode(ScoresController* self, unsigned int mode)
	{
		SLCScoresControllerSmart* controller = (SLCScoresControllerSmart*)self;
		controller.mode = mode;
	}
	
	void ScoresController::LoadRange(ScoresController* self, unsigned int range_start, unsigned int range_length)
	{
		SLCScoresControllerSmart* controller = (SLCScoresControllerSmart*)self;
		controller.limit = range_length;
		[controller loadRangeAtRank:range_start + 1];
	}
	void ScoresController::LoadRangeForUser(ScoresController* self, User* user, unsigned int range_length)
	{
		SLCScoresControllerSmart* controller = (SLCScoresControllerSmart*)self;
		controller.limit = range_length;
		[controller loadRangeForUser:(SLCUser*)user];
	}
	
	void ScoresController::LoadRangeAtRank(ScoresController* self, unsigned int rank, unsigned int range_length)
	{
		SLCScoresControllerSmart* controller = (SLCScoresControllerSmart*)self;
		controller.limit = range_length;
		[controller loadRangeAtRank:rank];
	}
	void ScoresController::LoadRangeForScore (ScoresController* self, Score* score, unsigned int range_length)
	{
		SLCScoresControllerSmart* controller = (SLCScoresControllerSmart*)self;
		controller.limit = range_length;
		[controller loadRangeForScore:(SLCScore*)score];
	}
	
	void ScoresController::LoadNextRange(ScoresController* self)
	{
		SLCScoresControllerSmart* controller = (SLCScoresControllerSmart*)self;
		[controller loadNextRange];
	}
	void ScoresController::LoadPreviousRange(ScoresController* self)
	{
		SLCScoresControllerSmart* controller = (SLCScoresControllerSmart*)self;
		[controller loadPreviousRange];
	}
	
	bool ScoresController::HasPreviousRange(ScoresController* self)
	{
		SLCScoresControllerSmart* controller = (SLCScoresControllerSmart*)self;
		return controller.hasPreviousRange;
	}
	
	bool ScoresController::HasNextRange(ScoresController* self)
	{
		SLCScoresControllerSmart* controller = (SLCScoresControllerSmart*)self;
		return controller.hasNextRange;
	}
	
	ScoreList* ScoresController::GetScores(ScoresController* self)
	{
		SLCScoresControllerSmart* controller = (SLCScoresControllerSmart*)self;
		return (ScoreList*)controller.scores;
	}
	
	Score* ScoreList::GetScore (ScoreList* score_list, unsigned int idx)
	{
		return (Score*)[(NSArray*)score_list objectAtIndex:idx];
	}
	
	unsigned int ScoreList::GetScoresCount(ScoreList* score_list)
	{
		return [(NSArray*)score_list count];
	}
	
	double Score::GetResult(Score* score)
	{
		return [((SLCScore*)score).result doubleValue];
	}
	
	double Score::GetMinorResult(Score* score)
	{
		return [((SLCScore*)score).minorResult doubleValue];
	}
	
	unsigned int Score::GetLevel(Score* score)
	{
		return [((SLCScore*)score).level unsignedIntValue];
	}
	
	unsigned int Score::GetMode(Score* score)
	{
		return [((SLCScore*)score).mode unsignedIntValue];
	}
	
	unsigned int Score::GetRank(Score* score)
	{
		return ((SLCScore*)score).rank;
	}
	
	User* Score::GetUser(Score* score)
	{
		return (User*)((SLCScore*)score).user;
	}
	
	const char* Score::GetContextString(Score* score, const char* key)
	{
		NSString* context = [((SLCScore*)score).context objectForKey:[NSString stringWithUTF8String:key]];
		return [context UTF8String];
	}

	User* UserList::GetUser(UserList* user_list, unsigned int idx)
	{
		if (idx >= [(NSArray*)user_list count])
			return NULL;
		
		return (User*)[(NSArray*)user_list objectAtIndex:idx];
	}
	
	unsigned int UserList::GetUsersCount(UserList* user_list)
	{
		return [(NSArray*)user_list count];
	}
	
	UsersController* UsersController::Create(RequestCompletionCallback callback)
	{
		SLCUsersControllerSmart* controller = [[SLCUsersControllerSmart alloc] initWithCompletionCallback:callback];
		return (UsersController*)controller;
	}
	
	void UsersController::Release(UsersController* self)
	{
		SLCUsersControllerSmart* controller = (SLCUsersControllerSmart*)self;
		[controller release];
	}
	
	void UsersController::SearchByLogin(UsersController* self, const char* login, UsersSearchOperator::Enum search_operator, unsigned int limit, bool global)
	{
		SLCUsersControllerSmart* controller = (SLCUsersControllerSmart*)self;
		
		if (search_operator == UsersSearchOperator::Prefix)
			controller.searchOperator = kSLCSearchListSearchOperatorPrefix;
		else if (search_operator == UsersSearchOperator::ExactMatch)
			controller.searchOperator = kSLCSearchListSearchOperatorExactMatch;
		else
			assert(0);
		
		controller.limit = limit;
		controller.searchesGlobal = global;
		[controller searchByLogin:[NSString stringWithUTF8String:login]];
	}
	
	void UsersController::SearchByEmail(UsersController* self, const char* email, UsersSearchOperator::Enum search_operator, unsigned int limit, bool global)
	{
		SLCUsersControllerSmart* controller = (SLCUsersControllerSmart*)self;
		
		if (search_operator == UsersSearchOperator::Prefix)
			controller.searchOperator = kSLCSearchListSearchOperatorPrefix;
		else if (search_operator == UsersSearchOperator::ExactMatch)
			controller.searchOperator = kSLCSearchListSearchOperatorExactMatch;
		else
			assert(0);
		
		controller.limit = limit;
		controller.searchesGlobal = global;
		[controller searchByEmail:[NSString stringWithUTF8String:email]];
	}
	
	bool UsersController::IsOverLimit(UsersController* self)
	{
		SLCUsersControllerSmart* controller = (SLCUsersControllerSmart*)self;
		return controller.overLimit;
	}
	
	UserList* UsersController::GetUsers(UsersController* self)
	{
		SLCUsersControllerSmart* controller = (SLCUsersControllerSmart*)self;
		return (UserList*)controller.users;
	}

	unsigned long Money::GetAmount(Money* self)
	{
		return [((SLCMoney*)self).amount unsignedLongValue];
	}
	
	
	const char* Award::GetIdentifier(Award* self)
	{
		return [((SLCAward*)self).identifier UTF8String];
	}
	
	int Award::GetInitialValue(Award* self)
	{
		return ((SLCAward*)self).initialValue;
	}
	
	int Award::GetAchievingValue(Award* self)
	{
		return ((SLCAward*)self).achievingValue;
	}
	
	const char* Award::GetLocalizedDescription(Award* self)
	{
		return [((SLCAward*)self).localizedDescription UTF8String];
	}
	
	const char* Award::GetLocalizedTitle(Award* self)
	{
		return [((SLCAward*)self).localizedTitle UTF8String];
	}
	
	Money* Award::GetRewardMoney(Award* self)
	{
		return (Money*)((SLCAward*)self).rewardMoney;
	}
	
	bool Award::IsAchievedByValue(Award* self, int value)
	{
		return [((SLCAward*)self) isAchievedByValue:value];
	}
	
	bool Award::IsValidCounterValue(Award* self, int value)
	{
		return [((SLCAward*)self) isValidCounterValue:value];
	}
	
	Award* Achievement::GetAward(Achievement* self)
	{
		return (Award*)((SLCAchievement*)self).award;
	}
	
	int Achievement::GetValue(Achievement* self)
	{
		return ((SLCAchievement*)self).value;
	}
	
	bool Achievement::IsAchieved(Achievement* self)
	{
		return [((SLCAchievement*)self) isAchieved];
	}
	
	Achievement* AchievementList::GetAchievement(AchievementList* self, unsigned int idx)
	{
		return (Achievement*)[(NSArray*)self objectAtIndex:idx];
	}
	
	unsigned int AchievementList::GetAchievementsCount(AchievementList* self)
	{
		return [(NSArray*)self count];
	}
	
	AchievementsController* AchievementsController::Create(RequestCompletionCallback callback)
	{
		SLCAchievementsControllerSmart* controller = [[SLCAchievementsControllerSmart alloc] initWithCompletionCallback:callback];
		return  (AchievementsController*)controller;
	}
	
	void AchievementsController::Release(AchievementsController* self)
	{
		SLCAchievementsControllerSmart* controller = (SLCAchievementsControllerSmart*)self;
		[controller release];
	}
	
	void AchievementsController::SetUser(AchievementsController* self, User* user)
	{
		((SLCAchievementsControllerSmart*)self).user = (SLCUser*)user;
	}
	
	User* AchievementsController::GetUser(AchievementsController* self)
	{
		return (User*)((SLCAchievementsControllerSmart*)self).user;
	}
	
	void AchievementsController::LoadAchievements(AchievementsController* self)
	{
		[((SLCAchievementsControllerSmart*)self) loadAchievements];
	}
	
	void AchievementsController::SynchronizeAchievements(AchievementsController* self)
	{
		[((SLCAchievementsControllerSmart*)self) synchronizeAchievements];
	}
	
	AchievementList* AchievementsController::GetAchievements(AchievementsController* self)
	{
		return (AchievementList*)((SLCAchievementsControllerSmart*)self).achievements;
	}
	
	unsigned int AchievementsController::GetAchievedAwardsCount(AchievementsController* self)
	{
		return ((SLCAchievementsControllerSmart*)self).countAchievedAwards;
	}
	
	Achievement* AchievementsController::GetAchievementForAwardIdentifier(AchievementsController* self, const char* award_identifier)
	{
		return (Achievement*)[((SLCAchievementsControllerSmart*)self) achievementForAwardIdentifier:[NSString stringWithUTF8String:award_identifier]];
	}
	
	bool AchievementsController::ShouldSynchronizeAchievements(AchievementsController* /*self*/)
	{
		return [SLCAchievementsController shouldSynchronizeAchievements];
	}
	
	bool AchievementsController::IsAchievedForAwardIdentifier(AchievementsController* /*self*/, const char* award_identifier)
	{
		return [SLCAchievementsController isAchievedForAwardIdentifier:[NSString stringWithUTF8String:award_identifier]];
	}
	
	int AchievementsController::GetValueForAwardIdentifier(AchievementsController* /*self*/, const char* award_identifier)
	{
		return [SLCAchievementsController valueForAwardIdentifier:[NSString stringWithUTF8String:award_identifier]];
	}
	
	void AchievementsController::SetValueForAwardIdentifier(AchievementsController* /*self*/, const char* award_identifier, int value)
	{
		[SLCAchievementsController setValue:value forAwardIdentifier:[NSString stringWithUTF8String:award_identifier]];
	}
	
	void AchievementsController::SetAchievedValueForAwardIdentifier(AchievementsController* /*self*/, const char* award_identifier)
	{
		[SLCAchievementsController setValue:1 forAwardIdentifier:[NSString stringWithUTF8String:award_identifier]];
		//[SLCAchievementsController setAchievedForAwardIdentifier:[NSString stringWithUTF8String:award_identifier]];
	}
	
	void AchievementsController::IncrementValueForAwardIdentifier(AchievementsController* /*self*/, const char* award_identifier)
	{
		[SLCAchievementsController incrementValueForAwardIdentifier:[NSString stringWithUTF8String:award_identifier]];
	}
	
	RankingController* RankingController::Create(RequestCompletionCallback callback)
	{
		SLCRankingControllerSmart* controller = [[SLCRankingControllerSmart alloc] initWithCompletionCallback:callback];
		return (RankingController*)controller;
	}
	
	void RankingController::Release(RankingController* self)
	{
		SLCRankingControllerSmart* controller = (SLCRankingControllerSmart*)self;
		[controller release];
	}
	
	void RankingController::SetSearchCriteria(RankingController* self, SearchCriteria::Enum criteria)
	{
		SLCRankingControllerSmart* controller = (SLCRankingControllerSmart*)self;
		
		switch (criteria)
		{
			case SearchCriteria::Global:
				controller.searchList = [SLCSearchList globalScoreSearchList];
				break;
			case SearchCriteria::TwentyFourHour:
				controller.searchList = [SLCSearchList twentyFourHourScoreSearchList];
				break;
			case SearchCriteria::UserCountry:
				controller.searchList = [SLCSearchList userCountryScoreSearchList];
				break;
			case SearchCriteria::BuddyhoodOnly:
				controller.searchList = [SLCSearchList buddiesScoreSearchList];
				break;
			default:
				break;
		}
	}
	
	void RankingController::LoadRankingForScore(RankingController* self, Score* score)
	{
		SLCRankingControllerSmart* controller = (SLCRankingControllerSmart*)self;
		[controller requestRankingForScore:(SLCScore*)score];
	}
	
	unsigned int RankingController::GetRank(RankingController* self)
	{
		SLCRankingControllerSmart* controller = (SLCRankingControllerSmart*)self;
		NSNumber* rank = [controller.ranking valueForKey:@"rank"];
		return [rank unsignedIntValue];
	}
	
	unsigned int RankingController::GetTotal(RankingController* self)
	{
		SLCRankingControllerSmart* controller = (SLCRankingControllerSmart*)self;
		NSNumber* rank = [controller.ranking valueForKey:@"total"];
		return [rank unsignedIntValue];
	}
}