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
#if defined(BB10)
		SC_Error_t rc = SC_UserController_LoadUser(bundle->Controller);
#else
		SC_Error_t rc = SC_UserController_RequestUser(bundle->Controller);
#endif
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
#if defined(BB10)
		SC_Error_t rc = SC_UserController_LoadBuddies(bundle->Controller);
#else
		SC_Error_t rc = SC_UserController_RequestBuddies(bundle->Controller);
#endif
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
#if defined(BB10)
		SC_Context_h context = SC_Score_GetContext((SC_Score_h)score);
		if (context != NULL)
		{
			SC_Error_t rc = SC_Context_Get(context, key, &value);
		if (rc != SC_OK)
		{
			result = "";
			_HandleError(rc);
		}
		else
			result = value != NULL ? SC_String_GetData(value) : "";
		}
		else
			result = "";
#else
		SC_Error_t rc = SC_Score_GetContextString((SC_Score_h)score, key, &value);
		if (rc != SC_OK)
		{
			result = "";
			_HandleError(rc);
		}
		else
			result = value != NULL ? SC_String_GetData(value) : "";
#endif
		return result.c_str();
	}

	Score* ScoreList::GetScore(ScoreList* score_list, unsigned int idx)
	{
#if defined(BB10)
		return (Score*)SC_ScoreList_GetAt((SC_ScoreList_h)score_list, idx);
#else
		return (Score*)SC_ScoreList_GetScore((SC_ScoreList_h)score_list, idx);
#endif
	}

	unsigned int ScoreList::GetScoresCount(ScoreList* score_list)
	{
#if defined(BB10)
		return SC_ScoreList_GetCount((SC_ScoreList_h)score_list);
#else
		return SC_ScoreList_GetScoresCount((SC_ScoreList_h)score_list);
#endif
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
#if defined(BB10)
		rc = SC_Client_CreateScore(scClient, &score);
#else
		rc = SC_Score_New(&score);
#endif
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
#if defined(BB10)
			SC_Context_h sc_context = NULL;
			rc = SC_Context_New(&sc_context);
			if (rc == SC_OK)
			{
#endif
			for (std::map<std::string, std::string>::iterator it = context->begin(); it != context->end(); ++it)
			{
				SC_String_h value = NULL;
				rc = SC_String_New(&value, it->second.c_str());
				if (rc == SC_OK)
				{
#if defined(BB10)
					rc = SC_Context_Put(sc_context, it->first.c_str(), value);
#else
					rc = SC_Score_SetContextString(score, it->first.c_str(), value);
#endif
					SC_String_Release(value);
					if (rc != SC_OK)
					{
						_HandleError(rc);
						return;
					}
				}
				else
				{
					_HandleError(rc);
					return;
				}
			}
#if defined(BB10)
				rc = SC_Score_SetContext(score, sc_context);
				if (rc != SC_OK)
				{
					_HandleError(rc);
					return;
				}
			}
			else
			{
				_HandleError(rc);
				return;
			}
#endif
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

	void ScoresController::SetMode(ScoresController* self, unsigned int mode)
	{
		SC_Error_t rc = SC_ScoresController_SetMode((SC_ScoresController_h)self, mode);
		if (rc != SC_OK)
			_HandleError(rc);
	}

	void ScoresController::LoadRange(ScoresController* self, unsigned int range_start, unsigned int range_length)
	{
#if defined(BB10)
		SC_Range_t range = {range_start, range_length };
		SC_Error_t rc = SC_ScoresController_LoadRange((SC_ScoresController_h)self, range);
#else
		SC_Error_t rc = SC_ScoresController_LoadRange((SC_ScoresController_h)self, range_start, range_length);
#endif
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
#if defined(BB10)
		return (User*)SC_UserList_GetAt((SC_UserList_h)self, idx);
#else
		return (User*)SC_UserList_GetUser((SC_UserList_h)self, idx);
#endif
	}

	unsigned int UserList::GetUsersCount(UserList* self)
	{
#if defined(BB10)
		return SC_UserList_GetCount((SC_UserList_h)self);
#else
		return SC_UserList_GetUsersCount((SC_UserList_h)self);
#endif
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
#if defined(BB10)
		return (Achievement*)SC_AchievementList_GetAt((SC_AchievementList_h)self, idx);
#else
		return (Achievement*)SC_AchievementList_GetAchievement((SC_AchievementList_h)self, idx);
#endif
	}

	unsigned int AchievementList::GetAchievementsCount(AchievementList* self)
	{
#if defined(BB10)
		return SC_AchievementList_GetCount((SC_AchievementList_h)self);
#else
		return SC_AchievementList_GetAchievementsCount((SC_AchievementList_h)self);
#endif
	}


#if defined(BB10)
	struct AchievementsControllerImpl
	{
		SC_LocalAchievementsController_h	LocalAchievementsController;
		SC_AchievementsController_h			AchievementsController;
	};

	namespace Internal
	{
		void _LocalAchievementsRequestComplete(void* userData, SC_Error_t result)
		{
			// stub
		}
	}
#endif

	AchievementsController* AchievementsController::Create(RequestCompletionCallback callback)
	{
#if defined(BB10)
		AchievementsControllerImpl* impl = new AchievementsControllerImpl();
		SC_Error_t rc = SC_Client_CreateAchievementsController(scClient, &impl->AchievementsController, Internal::_RequestComplete, (void*)callback);
		if (rc == SC_OK)
		{
			SC_Error_t rc = SC_Client_CreateLocalAchievementsController(scClient, &impl->LocalAchievementsController, Internal::_LocalAchievementsRequestComplete, 0);
			if (rc == SC_OK)
				return (AchievementsController*)impl;
			else
				_HandleError(rc);
		}
		else
			_HandleError(rc);
		delete impl;
		return NULL;
#else
		SC_AchievementsController_h controller = NULL;
		SC_Error_t rc = SC_Client_CreateAchievementsController(scClient, &controller, Internal::_RequestComplete, (void*)callback);
		if (rc != SC_OK)
			_HandleError(rc);
		return (AchievementsController*)controller;
#endif
	}

	void AchievementsController::Release(AchievementsController* self)
	{
#if defined(BB10)
		AchievementsControllerImpl* impl = (AchievementsControllerImpl*)self;
		SC_LocalAchievementsController_Release(impl->LocalAchievementsController);
		SC_AchievementsController_Release(impl->AchievementsController);
		delete impl;
#else
		SC_AchievementsController_Release((SC_AchievementsController_h)self);
#endif
	}

	User* AchievementsController::GetUser(AchievementsController* self)
	{
#if defined(BB10)
		return (User*)SC_AchievementsController_GetUser(((AchievementsControllerImpl*)self)->AchievementsController);
#else
		return (User*)SC_AchievementsController_GetUser((SC_AchievementsController_h)self);
#endif
	}
	
	void AchievementsController::LoadAchievements(AchievementsController* self, User* user)
	{
#if defined(BB10)
		SC_User_h sc_user = (SC_User_h)user;
		if (sc_user == NULL)
		{
			SC_Session_h session = SC_Client_GetSession(scClient);
			sc_user = SC_Session_GetUser(session);
		}
		SC_Error_t rc = SC_AchievementsController_LoadAchievementsForCurrentGame(((AchievementsControllerImpl*)self)->AchievementsController, sc_user);
		if (rc != SC_OK)
			_HandleError(rc);
#else
		SC_Error_t rc == SC_OK;
		if (user != NULL)
			rc = SC_AchievementsController_SetUser((SC_AchievementsController_h)self, (SC_User_h)user);
		if (rc == SC_OK)
		{
			rc = SC_AchievementsController_LoadAchievements((SC_AchievementsController_h)self);
			if (rc != SC_OK)
				_HandleError(rc);
		}
		else
			_HandleError(rc);

#endif

	}

	bool AchievementsController::ShouldSynchronizeAchievements(AchievementsController* self)
	{
#if defined(BB10)
		return SC_LocalAchievementsController_ShouldSynchronizeAchievements(((AchievementsControllerImpl*)self)->LocalAchievementsController) == SC_TRUE;
#else
		return SC_AchievementsController_ShouldSynchronizeAchievements((SC_AchievementsController_h)self) == SC_TRUE;
#endif
	}

	void AchievementsController::SynchronizeAchievements(AchievementsController* self)
	{
#if defined(BB10)
		SC_Error_t rc = SC_LocalAchievementsController_SynchronizeAchievements(((AchievementsControllerImpl*)self)->LocalAchievementsController);
#else
		SC_Error_t rc = SC_AchievementsController_SynchronizeAchievements((SC_AchievementsController_h)self);
#endif
		if (rc != SC_OK)
			_HandleError(rc);
	}

	AchievementList* AchievementsController::GetAchievements(AchievementsController* self)
	{
#if defined(BB10)
		return (AchievementList*)SC_LocalAchievementsController_GetAchievements(((AchievementsControllerImpl*)self)->LocalAchievementsController);
#else
		return (AchievementList*)SC_AchievementsController_GetAchievements((SC_AchievementsController_h)self);
#endif
	}

	unsigned int AchievementsController::GetAchievedAwardsCount(AchievementsController* self)
	{
#if defined(BB10)
		return 0;
		// @todo Scoreloop engineers forgot about this, so we must comment out it
		//return SC_LocalAchievementsController_GetAchievedCount(((AchievementsControllerImpl*)self)->LocalAchievementsController);
#else
		return SC_AchievementsController_CountAchievedAwards((SC_AchievementsController_h)self);
#endif
	}

	Achievement* AchievementsController::GetAchievementForAwardIdentifier(AchievementsController* self, const char* award_identifier)
	{
		SC_Achievement_h result = NULL;
#if defined(BB10)
		SC_Error_t rc = SC_LocalAchievementsController_GetAchievementForAwardIdentifier(((AchievementsControllerImpl*)self)->LocalAchievementsController, award_identifier, &result);
#else
		SC_Error_t rc = SC_AchievementsController_GetAchievementForAwardIdentifier((SC_AchievementsController_h)self, award_identifier, &result);
#endif
		if (rc != SC_OK)
			_HandleError(rc);
		return (Achievement*)result;
	}

	bool AchievementsController::IsAchievedForAwardIdentifier(AchievementsController* self, const char* award_identifier)
	{
		SC_Bool_t result = SC_FALSE;
#if defined(BB10)
		SC_Error_t rc = SC_LocalAchievementsController_IsAchievedForAwardIdentifier(((AchievementsControllerImpl*)self)->LocalAchievementsController, award_identifier, &result);
#else
		SC_Error_t rc = SC_AchievementsController_IsAchievedForAwardIdentifier((SC_AchievementsController_h)self, award_identifier, &result);
#endif
		if (rc != SC_OK)
			_HandleError(rc);
		return result == SC_TRUE;
	}

	int AchievementsController::GetValueForAwardIdentifier(AchievementsController* self, const char* award_identifier)
	{
		int result = 0;
#if defined(BB10)
		SC_Error_t rc = SC_LocalAchievementsController_GetValueForAwardIdentifier(((AchievementsControllerImpl*)self)->LocalAchievementsController, award_identifier, &result);
#else
		SC_Error_t rc = SC_AchievementsController_GetValueForAwardIdentifier((SC_AchievementsController_h)self, award_identifier, &result);
#endif
		if (rc != SC_OK)
			_HandleError(rc);
		return result;
	}

	void AchievementsController::SetValueForAwardIdentifier(AchievementsController* self, const char* award_identifier, int value)
	{
		SC_Bool_t is_achieved = SC_FALSE;
#if defined(BB10)
		SC_Error_t rc = SC_LocalAchievementsController_SetValueForAwardIdentifier(((AchievementsControllerImpl*)self)->LocalAchievementsController, award_identifier, value, &is_achieved);
#else
		SC_Error_t rc = SC_AchievementsController_SetValueForAwardIdentifier((SC_AchievementsController_h)self, award_identifier, value, &is_achieved);
#endif
		if (rc != SC_OK)
			_HandleError(rc);
	}

	void AchievementsController::SetAchievedValueForAwardIdentifier(AchievementsController* self, const char* award_identifier)
	{
		SC_Bool_t success = SC_FALSE;
#if defined(BB10)
		SC_Error_t rc = SC_LocalAchievementsController_SetAchievedValueForAwardIdentifier(((AchievementsControllerImpl*)self)->LocalAchievementsController, award_identifier, &success);
#else
		SC_Error_t rc = SC_AchievementsController_SetAchievedValueForAwardIdentifier((SC_AchievementsController_h)self, award_identifier, &success);
#endif
		if (rc != SC_OK)
			_HandleError(rc);
	}

	void AchievementsController::IncrementValueForAwardIdentifier(AchievementsController* self, const char* award_identifier)
	{
		SC_Bool_t is_achieved = SC_FALSE;
#if defined(BB10)
		SC_Error_t rc = SC_LocalAchievementsController_IncrementValueForAwardIdentifier(((AchievementsControllerImpl*)self)->LocalAchievementsController, award_identifier, &is_achieved);
#else
		SC_Error_t rc = SC_AchievementsController_IncrementValueForAwardIdentifier((SC_AchievementsController_h)self, award_identifier, &is_achieved);
#endif
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
#if defined(BB10)
		SC_Error_t rc = SC_RankingController_LoadRankingForScore((SC_RankingController_h)self, (SC_Score_h)score);
#else
		SC_Error_t rc = SC_RankingController_RequestRankingForScore((SC_RankingController_h)self, (SC_Score_h)score);
#endif
		if (rc != SC_OK)
			_HandleError(rc);
	}

	void RankingController::LoadRankingForUserInMode(RankingController* self, User* user, unsigned int mode)
	{
#if defined(BB10)
		SC_Error_t rc = SC_RankingController_LoadRankingForUserInMode((SC_RankingController_h)self, (SC_User_h)user, mode);
#else
		SC_Error_t rc = SC_RankingController_RequestRankingForUserInGameMode((SC_RankingController_h)self, (SC_User_h)user, mode);
#endif
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
			else
#if !defined(BB10)			
			 if (rc != SC_INIT_REJECTED_EULA)				
#endif
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
