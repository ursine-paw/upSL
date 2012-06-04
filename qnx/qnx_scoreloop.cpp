// Copyright (c) 2012 ursinepaw.com
#include "../Scoreloop.h"

#include <scoreloop/scoreloopcore.h>

#define SL_GAME_ID "YOUR GAME ID"
#define SL_SECRET_KEY "YOUR GAME SECRET KEY"

namespace Scoreloop
{
	SC_InitData_t			scInitData;
	SC_Client_h				scClient = NULL;

	bool HandleBSPEvent(bps_event_t* event)
	{
		if (SC_HandleBPSEvent(&scInitData, event) == BPS_SUCCESS)
			return true;
		return false;
	}

	void _HandleError(SC_Error_t error)
	{
		const char* text = SC_MapErrorToStr(error);
		// NativeDialog::Show(text);
	}

	namespace Internal
	{
		struct UserControllerBundle
		{
			SC_UserController_h	Controller;
			void* 				Callback;
		};

		void _FetchUserRequestComplete(void* userData, SC_Error_t result)
		{
			UserControllerBundle* bundle = (UserControllerBundle*)userData;

			int error_code = -1;
			if (result == SC_OK)
				error_code = Scoreloop::ErrorCode::Ok;
			else if (result == SC_INVALID_USER_DATA)
			{
				SC_UserValidationError_t error = SC_UserController_GetValidationErrors(bundle->Controller);

				if (error & SC_USERNAME_ALREADY_TAKEN)
					error_code = Scoreloop::ErrorCode::UsernameAlreadyTaken;
				else if (error & SC_USERNAME_FORMAT_INVALID)
					error_code = Scoreloop::ErrorCode::UsernameFormatInvalid;
				else if (error & SC_USERNAME_TOO_SHORT)
					error_code = Scoreloop::ErrorCode::UsernameTooShort;
				else if (error & SC_EMAIL_ALREADY_TAKEN)
					error_code = Scoreloop::ErrorCode::EmailAlreadyTaken;
				else if (error & SC_EMAIL_FORMAT_INVALID)
					error_code = Scoreloop::ErrorCode::EmailFormatInvalid;
				else
					error_code = Scoreloop::ErrorCode::UnknownError;
			}
			else if (result == SC_REQUEST_FAILED)
				error_code = Scoreloop::ErrorCode::RequestFailed;
			else
				error_code = Scoreloop::ErrorCode::UnknownError;

			RequestCompletionCallback callback = (RequestCompletionCallback)bundle->Callback;
			if (callback)
				(*callback)(error_code);
		}

		void _RequestComplete(void* userData, SC_Error_t result)
		{
			RequestCompletionCallback callback = (RequestCompletionCallback)userData;
			if (callback)
			{
				switch (result)
				{
				case SC_OK:
					(*callback)(Scoreloop::ErrorCode::Ok);
					break;
				case SC_REQUEST_FAILED:
					(*callback)(Scoreloop::ErrorCode::RequestFailed);
					break;
				default:
					(*callback)(Scoreloop::ErrorCode::UnknownError);
					break;
				}
			}
		}
	}

	const char* User::GetLogin(User* user)
	{
		static std::string result;
		SC_String_h login = SC_User_GetLogin((SC_User_h)user);
		result = login != NULL ? SC_String_GetData(login) : "";
		return result.c_str();
	}

	const char* User::GetEmail(User* user)
	{
		static std::string result;
		SC_String_h email = SC_User_GetEmail((SC_User_h)user);
		result = email != NULL ? SC_String_GetData(email) : "";
		return result.c_str();
	}

	void User::SetLogin(User* user, const char* login)
	{
		SC_Error_t rc = SC_User_SetLogin((SC_User_h)user, login);
		if (rc != SC_OK)
			_HandleError(rc);
	}

	void User::SetEmail(User* user, const char* email)
	{
		SC_Error_t rc = SC_User_SetEmail((SC_User_h)user, email);
		if (rc != SC_OK)
			_HandleError(rc);
	}

	UserList* User::GetBuddyUsers(User* user)
	{
		return (UserList*)SC_User_GetBuddyUsers((SC_User_h)user);
	}

	User* Session::GetUser()
	{
		SC_Session_h session = SC_Client_GetSession(scClient);
		return (User*)SC_Session_GetUser(session);
	}

	UserController* UserController::Create(RequestCompletionCallback callback)
	{
		SC_UserController_h controller = NULL;
		Internal::UserControllerBundle* bundle = new Internal::UserControllerBundle();
		bundle->Callback = (void*)callback;
		SC_Error_t rc = SC_Client_CreateUserController(scClient, &controller, Internal::_FetchUserRequestComplete, (void*)bundle);
		if (rc != SC_OK)
			_HandleError(rc);
		if (controller)
			bundle->Controller = controller;
		else
		{
			delete bundle;
			bundle = NULL;
		}

		return (UserController*)bundle;
	}

	void UserController::Release(UserController* self)
	{
		Internal::UserControllerBundle* bundle = (Internal::UserControllerBundle*)self;
		SC_UserController_Release(bundle->Controller);
		delete bundle;
	}

	User* UserController::GetUser(UserController* self)
	{
		Internal::UserControllerBundle* bundle = (Internal::UserControllerBundle*)self;
		return (User*)SC_UserController_GetUser(bundle->Controller);
	}

	void UserController::LoadUser(UserController* self)
	{
		Internal::UserControllerBundle* bundle = (Internal::UserControllerBundle*)self;
		SC_Error_t rc = SC_UserController_RequestUser(bundle->Controller);
		if (rc != SC_OK)
			_HandleError(rc);
	}

	void UserController::SubmitUser(UserController* self)
	{
		Internal::UserControllerBundle* bundle = (Internal::UserControllerBundle*)self;
		SC_Error_t rc = SC_UserController_UpdateUser(bundle->Controller);
		if (rc != SC_OK)
			_HandleError(rc);
	}

	void UserController::SetUser(UserController* self, User* user)
	{
		Internal::UserControllerBundle* bundle = (Internal::UserControllerBundle*)self;
		SC_Error_t rc = SC_UserController_SetUser(bundle->Controller, (SC_User_h)user);
		if (rc != SC_OK)
			_HandleError(rc);
	}

	void UserController::AddAsBuddy(UserController* self)
	{
		Internal::UserControllerBundle* bundle = (Internal::UserControllerBundle*)self;
		SC_Error_t rc = SC_UserController_AddAsBuddy(bundle->Controller);
		if (rc != SC_OK)
			_HandleError(rc);
	}

	void UserController::RemoveAsBuddy(UserController* self)
	{
		Internal::UserControllerBundle* bundle = (Internal::UserControllerBundle*)self;
		SC_Error_t rc = SC_UserController_RemoveAsBuddy(bundle->Controller);
		if (rc != SC_OK)
			_HandleError(rc);
	}

	void UserController::RequestBuddies(UserController* self)
	{
		Internal::UserControllerBundle* bundle = (Internal::UserControllerBundle*)self;
		SC_Error_t rc = SC_UserController_RequestBuddies(bundle->Controller);
		if (rc != SC_OK)
			_HandleError(rc);
	}

	double Score::GetResult(Score* score)
	{
		return SC_Score_GetResult((SC_Score_h)score);
	}

	double Score::GetMinorResult(Score* score)
	{
		return SC_Score_GetMinorResult((SC_Score_h)score);
	}

	unsigned int Score::GetLevel(Score* score)
	{
		return SC_Score_GetLevel((SC_Score_h)score);
	}

	unsigned int Score::GetMode(Score* score)
	{
		return SC_Score_GetMode((SC_Score_h)score);
	}

	unsigned int Score::GetRank(Score* score)
	{
		return SC_Score_GetRank((SC_Score_h)score);
	}

	User* Score::GetUser(Score* score)
	{
		return (User*)SC_Score_GetUser((SC_Score_h)score);
	}

	const char* Score::GetContextString(Score* score, const char* key)
	{
		static std::string result;
		SC_String_h value = NULL;
		SC_Error_t rc = SC_Score_GetContextString((SC_Score_h)score, key, &value);
		if (rc != SC_OK)
		{
			result = "";
			_HandleError(rc);
		}
		else
			result = value != NULL ? SC_String_GetData(value) : "";
		return result.c_str();
	}

	Score* ScoreList::GetScore(ScoreList* score_list, unsigned int idx)
	{
		return (Score*)SC_ScoreList_GetScore((SC_ScoreList_h)score_list, idx);
	}

	unsigned int ScoreList::GetScoresCount(ScoreList* score_list)
	{
		return SC_ScoreList_GetScoresCount((SC_ScoreList_h)score_list);
	}

	ScoreController* ScoreController::Create(RequestCompletionCallback callback)
	{
		SC_ScoreController_h controller = NULL;
		SC_Error_t rc = SC_Client_CreateScoreController(scClient, &controller, Internal::_RequestComplete, (void*)callback);
		if (rc != SC_OK)
			_HandleError(rc);
		return (ScoreController*)controller;
	}

	void ScoreController::Release(ScoreController* self)
	{
		SC_ScoreController_Release((SC_ScoreController_h)self);
	}

	Score* ScoreController::GetScore(ScoreController* self)
	{
		return (Score*)SC_ScoreController_GetScore((SC_ScoreController_h)self);
	}

	void ScoreController::SubmitScore(ScoreController* self, double result, double minor, unsigned int level, unsigned int mode, std::map<std::string, std::string>* context)
	{
		SC_Error_t rc;
		SC_Score_h score;
		rc = SC_Score_New(&score);
		if (rc != SC_OK)
		{
			_HandleError(rc);
			return;
		}
		SC_Score_SetResult(score, result);
		SC_Score_SetMinorResult(score, minor);
		SC_Score_SetLevel(score, level);
		SC_Score_SetMode(score, mode);

		if (context)
		{
			for (std::map<std::string, std::string>::iterator it = context->begin(); it != context->end(); ++it)
			{
				SC_String_h value = NULL;
				rc = SC_String_New(&value, it->second.c_str());
				if (rc == SC_OK)
				{
					rc = SC_Score_SetContextString(score, it->first.c_str(), value);
					SC_String_Release(value);
				}
				else
				{
					_HandleError(rc);
					return;
				}
			}
		}

		rc = SC_ScoreController_SubmitScore((SC_ScoreController_h)self, score);
		if (rc != SC_OK)
		{
			SC_Score_Release(score);
			_HandleError(rc);
		}
	}


	ScoresController* ScoresController::Create(RequestCompletionCallback callback)
	{
		SC_ScoresController_h controller = NULL;
		SC_Error_t rc = SC_Client_CreateScoresController(scClient, &controller, Internal::_RequestComplete, (void*)callback);
		if (rc != SC_OK)
			_HandleError(rc);
		return (ScoresController*)controller;
	}

	void ScoresController::Release(ScoresController* self)
	{
		SC_ScoresController_Release((SC_ScoresController_h)self);
	}

	void ScoresController::SetSearchCriteria(ScoresController* self, SearchCriteria::Enum criteria)
	{
		SC_ScoresController_SetBuddyhoodOnly((SC_ScoresController_h)self, 0);
		switch (criteria)
		{
		case SearchCriteria::Global:
			SC_ScoresController_SetSearchList((SC_ScoresController_h)self, SC_SCORE_SEARCH_LIST_GLOBAL);
			break;
		case SearchCriteria::TwentyFourHour:
			SC_ScoresController_SetSearchList((SC_ScoresController_h)self, SC_SCORE_SEARCH_LIST_24H);
			break;
		case SearchCriteria::UserCountry:
			SC_ScoresController_SetSearchList((SC_ScoresController_h)self, SC_SCORE_SEARCH_LIST_USER_COUNTRY);
			break;
		case SearchCriteria::BuddyhoodOnly:
			SC_ScoresController_SetBuddyhoodOnly((SC_ScoresController_h)self, (SC_User_h)Session::GetUser());
			break;
		}
	}

	void ScoresController::SetGameMode(ScoresController* self, unsigned int mode)
	{
		SC_Error_t rc = SC_ScoresController_SetMode((SC_ScoresController_h)self, mode);
		if (rc != SC_OK)
			_HandleError(rc);
	}

	void ScoresController::LoadRange(ScoresController* self, unsigned int range_start, unsigned int range_length)
	{
		SC_Error_t rc = SC_ScoresController_LoadRange((SC_ScoresController_h)self, range_start, range_length);
		if (rc != SC_OK)
			_HandleError(rc);
	}
	void ScoresController::LoadRangeForUser(ScoresController* self, User* user, unsigned int range_length)
	{
		SC_Error_t rc = SC_ScoresController_LoadRangeForUser((SC_ScoresController_h)self, (SC_User_h)user, range_length);
		if (rc != SC_OK)
			_HandleError(rc);
	}

	void ScoresController::LoadRangeAtRank(ScoresController* self, unsigned int rank, unsigned int range_length){
		SC_Error_t rc = SC_ScoresController_LoadRangeAtRank((SC_ScoresController_h)self, rank, range_length);
		if (rc != SC_OK)
			_HandleError(rc);
	}
	void ScoresController::LoadRangeForScore(ScoresController* self, Score* score, unsigned int range_length)
	{
		SC_Error_t rc = SC_ScoresController_LoadRangeForScore((SC_ScoresController_h)self, (SC_Score_h)score, range_length);
		if (rc != SC_OK)
			_HandleError(rc);
	}

	void ScoresController::LoadNextRange(ScoresController* self)
	{
		SC_Error_t rc = SC_ScoresController_LoadNextRange((SC_ScoresController_h)self);
		if (rc != SC_OK)
			_HandleError(rc);
	}
	void ScoresController::LoadPreviousRange(ScoresController* self)
	{
		SC_Error_t rc = SC_ScoresController_LoadPreviousRange((SC_ScoresController_h)self);
		if (rc != SC_OK)
			_HandleError(rc);
	}

	bool ScoresController::HasPreviousRange(ScoresController* self)
	{
		SC_Bool_t* ret = SC_ScoresController_HasPreviousRange((SC_ScoresController_h)self);
		return (ret && *ret == SC_TRUE);
	}

	bool ScoresController::HasNextRange(ScoresController* self)
	{
		SC_Bool_t* ret = SC_ScoresController_HasNextRange((SC_ScoresController_h)self);
		return (ret && *ret == SC_TRUE);
	}

	ScoreList* ScoresController::GetScores(ScoresController* self)
	{
		return (ScoreList*)SC_ScoresController_GetScores((SC_ScoresController_h)self);
	}
	
	User* UserList::GetUser(UserList* self, unsigned int idx)
	{
		return (User*)SC_UserList_GetUser((SC_UserList_h)self, idx);
	}

	unsigned int UserList::GetUsersCount(UserList* self)
	{
		return SC_UserList_GetUsersCount((SC_UserList_h)self);
	}

	UsersController* UsersController::Create(RequestCompletionCallback callback)
	{
		SC_UsersController_h controller = NULL;
		SC_Error_t rc = SC_Client_CreateUsersController(scClient, &controller, Internal::_RequestComplete, (void*)callback);
		if (rc != SC_OK)
			_HandleError(rc);
		return (UsersController*)controller;
	}

	void UsersController::Release(UsersController* self)
	{
		SC_UsersController_Release((SC_UsersController_h)self);
	}

	void UsersController::SearchByLogin(UsersController* self, const char* login, UsersSearchOperator::Enum search_operator, unsigned int limit, bool global)
	{
		switch (search_operator)
		{
		case UsersSearchOperator::Prefix:
			SC_UsersController_SetLoginSearchOperator((SC_UsersController_h)self, SC_LOGIN_SEARCH_PREFIX);
			break;
		case UsersSearchOperator::ExactMatch:
			SC_UsersController_SetLoginSearchOperator((SC_UsersController_h)self, SC_LOGIN_SEARCH_EXACT_MATCH);
			break;
		}

		SC_UsersController_SetSearchesGlobal((SC_UsersController_h)self, global ? SC_TRUE : SC_FALSE);
		SC_UsersController_SetSearchLimit((SC_UsersController_h)self, limit);

		SC_Error_t rc = SC_UsersController_SearchByLogin((SC_UsersController_h)self, login);
		if (rc != SC_OK)
			_HandleError(rc);
	}

	void UsersController::SearchByEmail(UsersController* self, const char* email, UsersSearchOperator::Enum search_operator, unsigned int limit, bool global)
	{
		switch (search_operator)
		{
		case UsersSearchOperator::Prefix:
			SC_UsersController_SetLoginSearchOperator((SC_UsersController_h)self, SC_LOGIN_SEARCH_PREFIX);
			break;
		case UsersSearchOperator::ExactMatch:
			SC_UsersController_SetLoginSearchOperator((SC_UsersController_h)self, SC_LOGIN_SEARCH_EXACT_MATCH);
			break;
		}

		SC_UsersController_SetSearchesGlobal((SC_UsersController_h)self, global ? SC_TRUE : SC_FALSE);
		SC_UsersController_SetSearchLimit((SC_UsersController_h)self, limit);

		SC_Error_t rc = SC_UsersController_SearchByEmail((SC_UsersController_h)self, email);
		if (rc != SC_OK)
			_HandleError(rc);
	}

	bool UsersController::IsOverLimit(UsersController* self)
	{
		return SC_UsersController_IsOverLimit((SC_UsersController_h)self);
	}

	UserList* UsersController::GetUsers(UsersController* self)
	{
		return (UserList*)SC_UsersController_GetUsers((SC_UsersController_h)self);
	}

	unsigned long Money::GetAmount(Money* self)
	{
		return SC_Money_GetAmount((SC_Money_h)self);
	}

	const char* Award::GetIdentifier(Award* self)
	{
		static std::string result;
		SC_String_h identifier = SC_Award_GetIdentifier((SC_Award_h)self);
		result = identifier != NULL ? SC_String_GetData(identifier) : "";
		return result.c_str();
	}

	int Award::GetInitialValue(Award* self)
	{
		return SC_Award_GetInitialValue((SC_Award_h)self);
	}

	int Award::GetAchievingValue(Award* self)
	{
		return SC_Award_GetAchievingValue((SC_Award_h)self);
	}

	const char* Award::GetLocalizedDescription(Award* self)
	{
		static std::string result;
		SC_String_h desc = SC_Award_GetLocalizedDescription((SC_Award_h)self);
		result = desc != NULL ? SC_String_GetData(desc) : "";
		return result.c_str();
	}

	const char* Award::GetLocalizedTitle(Award* self)
	{
		static std::string result;
		SC_String_h title = SC_Award_GetLocalizedTitle((SC_Award_h)self);
		result = title != NULL ? SC_String_GetData(title) : "";
		return result.c_str();
	}

	Money* Award::GetRewardMoney(Award* self)
	{
		return (Money*)SC_Award_GetRewardedMoney((SC_Award_h)self);
	}

	bool Award::IsAchievedByValue(Award* self, int value)
	{
		return SC_Award_IsAchievedByValue((SC_Award_h)self, value) == SC_TRUE;
	}

	bool Award::IsValidCounterValue(Award* self, int value)
	{
		return SC_Award_IsValidCounterValue((SC_Award_h)self, value) == SC_TRUE;
	}

	Award* Achievement::GetAward(Achievement* self)
	{
		return (Award*)SC_Achievement_GetAward((SC_Achievement_h)self);
	}

	int Achievement::GetValue(Achievement* self)
	{
		return SC_Achievement_GetValue((SC_Achievement_h)self);
	}

	bool Achievement::IsAchieved(Achievement* self)
	{
		return SC_Achievement_IsAchieved((SC_Achievement_h)self) == SC_TRUE;
	}

	Achievement* AchievementList::GetAchievement(AchievementList* self, unsigned int idx)
	{
		return (Achievement*)SC_AchievementList_GetAchievement((SC_AchievementList_h)self, idx);
	}

	unsigned int AchievementList::GetAchievementsCount(AchievementList* self)
	{
		return SC_AchievementList_GetAchievementsCount((SC_AchievementList_h)self);
	}

	AchievementsController* AchievementsController::Create(RequestCompletionCallback callback)
	{
		SC_AchievementsController_h controller = NULL;
		SC_Error_t rc = SC_Client_CreateAchievementsController(scClient, &controller, Internal::_RequestComplete, (void*)callback);
		if (rc != SC_OK)
			_HandleError(rc);
		return (AchievementsController*)controller;
	}

	void AchievementsController::Release(AchievementsController* self)
	{
		SC_AchievementsController_Release((SC_AchievementsController_h)self);
	}

	void AchievementsController::SetUser(AchievementsController* self, User* user)
	{
		SC_Error_t rc = SC_AchievementsController_SetUser((SC_AchievementsController_h)self, (SC_User_h)user);
		if (rc != SC_OK)
			_HandleError(rc);
	}

	User* AchievementsController::GetUser(AchievementsController* self)
	{
		return (User*)SC_AchievementsController_GetUser((SC_AchievementsController_h)self);
	}

	void AchievementsController::LoadAchievements(AchievementsController* self)
	{
		SC_Error_t rc = SC_AchievementsController_LoadAchievements((SC_AchievementsController_h)self);
		if (rc != SC_OK)
			_HandleError(rc);
	}

	bool AchievementsController::ShouldSynchronizeAchievements(AchievementsController* self)
	{
		return SC_AchievementsController_ShouldSynchronizeAchievements((SC_AchievementsController_h)self) == SC_TRUE;
	}

	void AchievementsController::SynchronizeAchievements(AchievementsController* self)
	{
		SC_Error_t rc = SC_AchievementsController_SynchronizeAchievements((SC_AchievementsController_h)self);
		if (rc != SC_OK)
			_HandleError(rc);
	}

	AchievementList* AchievementsController::GetAchievements(AchievementsController* self)
	{
		return (AchievementList*)SC_AchievementsController_GetAchievements((SC_AchievementsController_h)self);
	}

	unsigned int AchievementsController::GetAchievedAwardsCount(AchievementsController* self)
	{
		return SC_AchievementsController_CountAchievedAwards((SC_AchievementsController_h)self);
	}

	Achievement* AchievementsController::GetAchievementForAwardIdentifier(AchievementsController* self, const char* award_identifier)
	{
		SC_Achievement_h result = NULL;
		SC_Error_t rc = SC_AchievementsController_GetAchievementForAwardIdentifier((SC_AchievementsController_h)self, award_identifier, &result);
		if (rc != SC_OK)
			_HandleError(rc);
		return (Achievement*)result;
	}

	bool AchievementsController::IsAchievedForAwardIdentifier(AchievementsController* self, const char* award_identifier)
	{
		SC_Bool_t result = SC_FALSE;
		SC_Error_t rc = SC_AchievementsController_IsAchievedForAwardIdentifier((SC_AchievementsController_h)self, award_identifier, &result);
		if (rc != SC_OK)
			_HandleError(rc);
		return result == SC_TRUE;
	}

	int AchievementsController::GetValueForAwardIdentifier(AchievementsController* self, const char* award_identifier)
	{
		int result = 0;
		SC_Error_t rc = SC_AchievementsController_GetValueForAwardIdentifier((SC_AchievementsController_h)self, award_identifier, &result);
		if (rc != SC_OK)
			_HandleError(rc);
		return result;
	}

	void AchievementsController::SetValueForAwardIdentifier(AchievementsController* self, const char* award_identifier, int value)
	{
		SC_Bool_t is_achieved = SC_FALSE;
		SC_Error_t rc = SC_AchievementsController_SetValueForAwardIdentifier((SC_AchievementsController_h)self, award_identifier, value, &is_achieved);
		if (rc != SC_OK)
			_HandleError(rc);
	}

	void AchievementsController::SetAchievedValueForAwardIdentifier(AchievementsController* self, const char* award_identifier)
	{
		SC_Bool_t success = SC_FALSE;
		SC_Error_t rc = SC_AchievementsController_SetAchievedValueForAwardIdentifier((SC_AchievementsController_h)self, award_identifier, &success);
		if (rc != SC_OK)
			_HandleError(rc);
	}

	void AchievementsController::IncrementValueForAwardIdentifier(AchievementsController* self, const char* award_identifier)
	{
		SC_Bool_t is_achieved = SC_FALSE;
		SC_Error_t rc = SC_AchievementsController_IncrementValueForAwardIdentifier((SC_AchievementsController_h)self, award_identifier, &is_achieved);
		if (rc != SC_OK)
			_HandleError(rc);
	}

	RankingController* RankingController::Create(RequestCompletionCallback callback)
	{
		SC_RankingController_h controller = NULL;
		SC_Error_t rc = SC_Client_CreateRankingController(scClient, &controller, Internal::_RequestComplete, (void*)callback);
		if (rc != SC_OK)
			_HandleError(rc);
		return (RankingController*)controller;
	}

	void RankingController::Release(RankingController* self)
	{
		SC_RankingController_Release((SC_RankingController_h)self);
	}

	void RankingController::SetSearchCriteria(RankingController* self, SearchCriteria::Enum criteria)
	{
		SC_RankingController_SetBuddyhoodOnly((SC_RankingController_h)self, 0);
		switch (criteria)
		{
		case SearchCriteria::Global:
			SC_RankingController_SetSearchList((SC_RankingController_h)self, SC_SCORE_SEARCH_LIST_GLOBAL);
			break;
		case SearchCriteria::TwentyFourHour:
			SC_RankingController_SetSearchList((SC_RankingController_h)self, SC_SCORE_SEARCH_LIST_24H);
			break;
		case SearchCriteria::UserCountry:
			SC_RankingController_SetSearchList((SC_RankingController_h)self, SC_SCORE_SEARCH_LIST_USER_COUNTRY);
			break;
		case SearchCriteria::BuddyhoodOnly:
			SC_RankingController_SetBuddyhoodOnly((SC_RankingController_h)self, (SC_User_h)Session::GetUser());
			break;
		}
	}

	void RankingController::LoadRankingForScore(RankingController* self, Score* score)
	{
		SC_Error_t rc = SC_RankingController_RequestRankingForScore((SC_RankingController_h)self, (SC_Score_h)score);
		if (rc != SC_OK)
			_HandleError(rc);
	}

	unsigned int RankingController::GetRank(RankingController* self)
	{
		return SC_RankingController_GetRanking((SC_RankingController_h)self);
	}

	unsigned int RankingController::GetTotal(RankingController* self)
	{
		return SC_RankingController_GetTotal((SC_RankingController_h)self);
	}

	void Client::Initialize(TermsOfServiceAcceptionCallback callback)
	{
		SC_InitData_Init(&scInitData);

		SC_Error_t rc = SC_Client_New(&scClient, &scInitData,
									 SL_GAME_ID,
									 SL_SECRET_KEY,
									 "1.0", "BLL", "en");

		if (rc != SC_OK)
		{
			if (rc == SC_DEV_PERMISSION_DENIED)
			{
				//NativeDialog::ShowHtml("To use Scoreloop you must permit Device Identifying Information for application.\nIt can be done in <b>Settings - Security - Application permissions</b> menu");
			}
			else if (rc != SC_INIT_REJECTED_EULA)
				_HandleError(rc);
			return;
		}
		if (callback)
			(*callback)();
	}

	void Client::Shutdown()
	{
		if (scClient != NULL)
			SC_Client_Release(scClient);
	}

	bool Client::IsInitialized()
	{
		return scClient != NULL;
	}
}
