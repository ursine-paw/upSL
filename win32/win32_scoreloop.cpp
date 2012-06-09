#include "../Scoreloop.h"

namespace Scoreloop
{	
	const char* User::GetLogin(User* user)
	{
		static std::string result;
		return result.c_str();
	}

	const char* User::GetEmail(User* user)
	{
		static std::string result;
		return result.c_str();
	}

	void User::SetLogin(User* user, const char* login)
	{
	}

	void User::SetEmail(User* user, const char* email)
	{
	}

	UserList* User::GetBuddyUsers(User* user)
	{
		return NULL;
	}

	User* Session::GetUser()
	{
		return NULL;
	}

	UserController* UserController::Create(RequestCompletionCallback callback)
	{
		return NULL;
	}

	void UserController::Release(UserController* self)
	{
	}

	User* UserController::GetUser(UserController* self)
	{
		return NULL;
	}

	void UserController::LoadUser(UserController* self)
	{
	}

	void UserController::SubmitUser(UserController* self)
	{
	}

	void UserController::SetUser(UserController* self, User* user)
	{
	}
	
	void UserController::AddAsBuddy(UserController* self)
	{
	}
	
	void UserController::RemoveAsBuddy(UserController* self)
	{
	}
	
	void UserController::RequestBuddies(UserController* self)
	{
	}

	double Score::GetResult(Score* score)
	{
		return 0.0;
	}

	double Score::GetMinorResult(Score* score)
	{
		return 0.0;
	}

	unsigned int Score::GetLevel(Score* score)
	{
		return 0;
	}

	unsigned int Score::GetMode(Score* score)
	{
		return 0;
	}

	unsigned int Score::GetRank(Score* score)
	{
		return 0;
	}

	User* Score::GetUser(Score* score)
	{
		return NULL;
	}

	const char* Score::GetContextString(Score* score, const char* key)
	{
		static std::string result;
		return result.c_str();
	}

	Score* ScoreList::GetScore (ScoreList* score_list, unsigned int idx)
	{
		return NULL;
	}

	unsigned int ScoreList::GetScoresCount(ScoreList* score_list)
	{
		return 0;
	}

	ScoreController* ScoreController::Create(RequestCompletionCallback callback)
	{
		return NULL;
	}

	void ScoreController::Release(ScoreController* self)
	{
	}

	Score* ScoreController::GetScore(ScoreController* self)
	{
		return NULL;
	}

	void ScoreController::SubmitScore(ScoreController* controller, double result, double minor, unsigned int level, unsigned int mode, std::map<std::string, std::string>* context)
	{
	}

	ScoresController* ScoresController::Create(RequestCompletionCallback callback)
	{
		return NULL;
	}

	void ScoresController::Release(ScoresController* self)
	{
	}

	void ScoresController::SetSearchCriteria(ScoresController* self, SearchCriteria::Enum criteria)
	{
	}

	void ScoresController::SetGameMode(ScoresController* self, unsigned int mode)
	{
	}

	void ScoresController::LoadRange(ScoresController* self, unsigned int range_start, unsigned int range_length)
	{
	}

	void ScoresController::LoadRangeForUser(ScoresController* self, User* user, unsigned int range_length)
	{
	}

	void ScoresController::LoadRangeAtRank(ScoresController* self, unsigned int rank, unsigned int range_length)
	{
	}

	void ScoresController::LoadRangeForScore(ScoresController* self, Score* score, unsigned int range_length)
	{
	}

	void ScoresController::LoadNextRange(ScoresController* self)
	{
	}

	void ScoresController::LoadPreviousRange(ScoresController* self)
	{
	}

	bool ScoresController::HasPreviousRange(ScoresController* self)
	{
		return false;
	}

	bool ScoresController::HasNextRange(ScoresController* self)
	{
		return false;
	}

	ScoreList* ScoresController::GetScores(ScoresController* self)
	{
		return NULL;
	}

	User* UserList::GetUser(UserList* self, unsigned int idx)
	{
		return NULL;
	}
	
	unsigned int UserList::GetUsersCount(UserList* self)
	{
		return 0;
	}
	
	UsersController* UsersController::Create(RequestCompletionCallback callback)
	{
		return NULL;
	}

	void UsersController::Release(UsersController* self)
	{
	}
	
	void UsersController::SearchByLogin(UsersController* self, const char* login, UsersSearchOperator::Enum search_operator, unsigned int limit, bool global)
	{
	}
	
	void UsersController::SearchByEmail(UsersController* self, const char* email, UsersSearchOperator::Enum search_operator, unsigned int limit, bool global)
	{
	}
	
	bool UsersController::IsOverLimit(UsersController* self)
	{
		return false;
	}
	
	UserList* UsersController::GetUsers(UsersController* self)
	{
		return NULL;
	}

	unsigned long Money::GetAmount(Money* self)
	{
		return 0;
	}

	const char* Award::GetIdentifier(Award* self)
	{
		static std::string result;
		return result.c_str();
	}

	int Award::GetInitialValue(Award* self)
	{
		return 0;
	}

	int Award::GetAchievingValue(Award* self)
	{
		return 0;
	}

	const char* Award::GetLocalizedDescription(Award* self)
	{
		static std::string result;
		return result.c_str();
	}
	const char* Award::GetLocalizedTitle(Award* self)
	{
		static std::string result;
		return result.c_str();
	}

	Money* Award::GetRewardMoney(Award* self)
	{
		return NULL;
	}

	bool Award::IsAchievedByValue(Award* self, int value)
	{
		return false;
	}

	bool Award::IsValidCounterValue(Award* self, int value)
	{
		return false;
	}


	Award* Achievement::GetAward(Achievement* self)
	{
		return NULL;
	}

	int Achievement::GetValue(Achievement* self)
	{
		return 0;
	}

	bool Achievement::IsAchieved(Achievement* self)
	{
		return false;
	}

	Achievement* AchievementList::GetAchievement(AchievementList* self, unsigned int idx)
	{
		return NULL;
	}

	unsigned int AchievementList::GetAchievementsCount(AchievementList* self)
	{
		return 0;
	}

	AchievementsController* AchievementsController::Create(RequestCompletionCallback callback)
	{
		return NULL;
	}

	void AchievementsController::Release(AchievementsController* self)
	{
	}

	void AchievementsController::SetUser(AchievementsController* self, User* user)
	{
	}

	User* AchievementsController::GetUser(AchievementsController* self)
	{
		return NULL;
	}

	void AchievementsController::LoadAchievements(AchievementsController* self)
	{
	}

	bool AchievementsController::ShouldSynchronizeAchievements(AchievementsController* self)
	{
		return false;
	}

	void AchievementsController::SynchronizeAchievements(AchievementsController* self)
	{
	}

	AchievementList* AchievementsController::GetAchievements(AchievementsController* self)
	{
		return NULL;
	}

	unsigned int AchievementsController::GetAchievedAwardsCount(AchievementsController* self)
	{
		return 0;
	}

	Achievement* AchievementsController::GetAchievementForAwardIdentifier(AchievementsController* self, const char* award_identifier)
	{
		return NULL;
	}

	bool AchievementsController::IsAchievedForAwardIdentifier(AchievementsController* self, const char* award_identifier)
	{
		return false;
	}

	int AchievementsController::GetValueForAwardIdentifier(AchievementsController* self, const char* award_identifier)
	{
		return 0;
	}
	
	void AchievementsController::SetValueForAwardIdentifier(AchievementsController* self, const char* award_identifier, int value)
	{
	}

	void AchievementsController::SetAchievedValueForAwardIdentifier(AchievementsController* self, const char* award_identifier)
	{
	}

	void AchievementsController::IncrementValueForAwardIdentifier(AchievementsController* self, const char* award_identifier)
	{
	}

	RankingController* RankingController::Create(RequestCompletionCallback callback)
	{
		return NULL;
	}

	void RankingController::Release(RankingController* self)
	{
	}

	void RankingController::SetSearchCriteria(RankingController* self, SearchCriteria::Enum criteria)
	{
	}

	void RankingController::LoadRankingForScore(RankingController* self, Score* score)
	{
	}
		
	unsigned int RankingController::GetRank(RankingController* self)
	{
		return 0;
	}

	unsigned int RankingController::GetTotal(RankingController* self)
	{
		return 0;
	}

	void Client::Initialize(TermsOfServiceAcceptionCallback callback)
	{
		if (callback)
			(*callback)();
	}

	void Client::Shutdown()
	{
	}

	bool Client::IsInitialized()
	{
		return true;
	}

}