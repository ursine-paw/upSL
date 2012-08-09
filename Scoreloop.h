// Copyright (c) 2012 ursinepaw.com
#pragma once

#include <map>

namespace Scoreloop
{ 
	typedef void (*RequestCompletionCallback)(int);
	typedef void (*TermsOfServiceAcceptionCallback)();

	struct ErrorCode
	{
		enum Enum
		{
			UnknownError = -1,
			Ok = 0,
			RequestFailed = 1,

			EmailAlreadyTaken = 10,
			UsernameAlreadyTaken,
			EmailFormatInvalid,
			UsernameTooShort,
			UsernameFormatInvalid
		};
	};

	struct SearchCriteria
	{
		enum Enum
		{
			Global = 0,
			TwentyFourHour = 1,
			UserCountry = 2,
			BuddyhoodOnly = 3
		};
	};
	
	struct UsersSearchOperator
	{
		enum Enum
		{
			Prefix = 0,
			ExactMatch = 1,
		};
	};

	struct User
	{
		/**
		* Returns the user's login.
		*
		* This method returns the login for the user. This property is distinct from the user identifier. 
		* The login for the user can be changed.
		*/
		static const char* GetLogin(User* self);
		
		/**
		* Returns the user's email address.
		*
		* This method returns the user's email address.
		*/
		static const char* GetEmail(User* self);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		/**
		* Returns the identifier of the user.
		* 
		* The identifier is generated and assigned by the Scoreloop servers.
		*/
		static const char* GetIdentifier(User* self);
#endif

		/** 
		* Sets the user's login.
		*
		* This method is used to set the login of the user. The login is the preferred "display name" for the user.
		* This method is available only to the session user.
		*/
		static void SetLogin(User* self, const char* login);

		/** 
		* Sets the user's email address.
		*
		* This method is used to set the email address of the user. This method is available only to the session user.
		*/
		static void SetEmail(User* self, const char* email);
		
		/**
		* Returns the user's list of buddies.
		*
		* This method returns a list of the user's buddies.
		*/
		static struct UserList* GetBuddyUsers(User* self);
	};

	struct Session
	{
		/**
		* Gets the session user.
		*
		* This function provides access to the current session user. 
		* Note that, to get the latest updated user attributes, you must make a server request by calling UserController::RequestUser().
		*/
		static User* GetUser();
	};

	struct UserController
	{
		/**
		* Creates a user controller, which can be used to retrieve information about an arbitrary user from the server.
		*
		* This function creates an instance of UserController. The controller is used to retrieve the information about
		* an arbitrary user from the Scoreloop servers
		*/
		static UserController* Create(RequestCompletionCallback callback);

		/**
		* Decrements the object's reference count, and deletes the object if the counter reaches 0.
		*
		* This function decrements the reference count for the current instance by 1.
		* The current controller instance will be automatically deleted when the reference count equals 0.
		*/
		static void Release(UserController* self);

		/**
		* Returns the user currently set for the controller.
		*
		* If the user is not explicitly set, the controller will deal with the current session user instead.
		*/
		static User* GetUser(UserController* self);

		/**
		* Requests attribute information for the user.
		*
		* If no user has been explicitly set, attributes for the current session user will be requested instead.
		* Note that this is an asynchronous call and a callback will be triggered, 
		* after which you can access the retrieved user by calling UserController::GetUser().
		*/
		static void LoadUser(UserController* self);

		/**
		* Pushes updates of the current session user's attributes to the server.
		*
		* This method updates a user's attributes on the Scoreloop server. Only the session user can be updated in this way.
		* Note that this is an asynchronous call and a callback will be triggered upon success or failure.
		*/
		static void SubmitUser(UserController* self);
		
		/**
		* Sets the user object that the controller should deal with.
		*
		* If the user is not explicitly set,the controller will deal with the current session user instead.
		*/
		static void SetUser(UserController* self, User* user);

		/**
		* Adds the user returned by UserController::GetUser() to the buddy list of the session user.
		*
		* This method adds the user that is returned by UserController::GetUser() to the buddy list of the current session user.
		*/
		static void AddAsBuddy(UserController* self);


		/**
		* Removes the user returned by UserController::GetUser() from the buddy list of the current session user.
		*
		* This method removes the user that is returned by UserController::GetUser() from the buddy list of the current session user.
		*/
		static void RemoveAsBuddy(UserController* self);

		/**
		* Requests the buddy list of the user that is returned by UserController::GetUser().
		*
		* If no user has been explicitly set, the buddy list of the session user is requested instead.
		* Note that this is an asynchronous call and a callback will be triggered. 
		* After a successful request, you can access the buddy list by calling User::GetBuddies().
		*/
		static void RequestBuddies(UserController* self);
	};

	struct Score
	{
		/**
		* Gets the result of the score.
		*
		* This function returns the result property of the score instance.
		*/
		static double GetResult(Score* score);

		/**
		* Gets the minor result of the score.
		*
		* This function returns the minor result for the current score instance. 
		* The minor result is a secondary result that can be optionally used for score comparison purposes.
		*/
		static double GetMinorResult(Score* score);

		/**
		* Gets the level of the score.
		*
		* This function returns the level at which the current score instance was achieved.
		*/
		static unsigned int	GetLevel(Score* score);

		/**
		* Gets the mode of the score.
		*
		* This function returns the mode at which the score was achieved.
		*/
		static unsigned int	GetMode(Score* score);

		/**
		* Gets the rank of the score.
		*
		* This function returns the rank for the score. The rank refers to the score's position on a leaderboard and as such, 
		* this will only have a meaningful value if the score is returned using an ScoresController.
		*/
		static unsigned int	GetRank(Score* score);

		/**
		* Gets the user who submitted the score.
		*
		* This function returns the user who obtained and submitted the score.
		*/
		static User* GetUser(Score* score);

		/**
		* Gets the score context's key/value pair.
		*
		* Accessor method to get the score context's key/value pair.
		*/
		static const char* GetContextString(Score* score, const char* key);
	};

	struct ScoreList
	{
		/**
		* Retrieves a specific score from the list.
		*
		* This function returns the score at the given index in the list.
		*/
		static Score* GetScore(ScoreList* score_list, unsigned int idx);

		/**
		* Returns the number of scores on the list that is returned from the server.
		*
		* This function returns the number of scores that have been requested from the server by using one of the following load methods:
		* ScoresController::LoadRange(), ScoresController::LoadRangeForScore(), ScoresController::LoadRangeForUser().
		*/
		static unsigned int GetScoresCount(ScoreList* score_list);
	};	

	struct ScoreController
	{
		/**
		* Creates a score controller, which can be used to submit a score achieved by the player.
		*
		* This function creates an instance of ScoreController. The controller is used to submit scores to the server.
		*/
		static ScoreController* Create(RequestCompletionCallback callback);

		/** 
		* Decrements the object's reference count, and deletes the object if the counter reaches 0.
		*
		* This function decrements the reference count for the current controller instance. 
		* The current instance will be deleted when the reference count reaches 0.
		*/
		static void Release(ScoreController* self);

		/**
		* Provides access to the last score that was submitted.
		*
		* Returns the Score object that was submitted using the ScoreController::SubmitScore method.
		*/
		static Score* GetScore(ScoreController* self);

		/**
		* Submits a given score to the server.
		*
		* This function is used to submit a score to the server.
		* Note that this is an asynchronous call, and a callback will be triggered upon success or failure.
		*/
		static void SubmitScore(ScoreController* self, double result, double minor_result = 0.0, unsigned int level = 0, unsigned int mode = 0, std::map<std::string, std::string>* context = NULL);
	};

	struct ScoresController
	{
		/**
		* Creates a scores controller, which can be used to retrieve a high scores list.
		*
		* This function creates an instance of ScoresController. The controller is used to retrieve lists of scores from the server.
		*/
		static ScoresController* Create(RequestCompletionCallback callback);
		
		/** 
		* Decrements the object's reference count, and deletes the object if the counter reaches 0.
		*
		* This function decrements the reference count for the current controller instance. 
		* The current instance will be deleted when the reference count reaches 0.
		*/
		static void Release(ScoresController* self);

		/**
		* Sets the search criteria used for subsequent requests.
		*
		* This function specifies the search criteria that will be used during the server request made by the controller.
		*/
		static void SetSearchCriteria(ScoresController* self, SearchCriteria::Enum criteria);

		/**
		* Set the game mode used for subsequent requests.
		*
		* This function sets the mode for the controller. The controller will only retrieve scores whose modes match this value.
		* If no mode is explicitly set for the controller, it will retrieve the scores for mode 0.
		*/
		static void SetMode(ScoresController* self, unsigned int mode);

		/**
		* Starts loading the given range of scores.
		*
		* This function requests the list of scores, defined by the supplied range, from the server. Note that this is an asynchronous call and a callback will be triggered, after which
		* you can access the retrieved scores by calling ScoresController::GetScores().
		*/
		static void LoadRange(ScoresController* self, unsigned int range_start, unsigned int range_length);

		/**
		* Requests the list of scores that includes the high score of the user in the middle.
		*
		* This function returns the list of scores that includes the high score of the user in the middle. Note that this is an asynchronous call and a callback will be triggered, after which 
		* you can access the retrieved scores by calling ScoresController::GetScores().
		*/
		static void LoadRangeForUser(ScoresController* self, User* user, unsigned int range_length);

		/**
		* Starts loading the range of scores beginning at a given rank.
		*
		* Requests the list beginning at the given rank. The rank refers to a score's position on the global leaderboard.
		* All lists start at 1, therefore the minimum rank possible is 1. Note that this is an asynchronous call and a callback will be triggered, after which
		* you can access the retrieved scores by calling ScoresController::GetScores().
		*/
		static void LoadRangeAtRank(ScoresController* self, unsigned int rank, unsigned int range_length);

		/**
		* Requests the list of scores that includes the given score in the middle.
		*
		* This method returns the list of scores that includes the given score in the middle. The score is included even if it did not make it into the high score list. 
		* Note that this is an asynchronous call and a callback will be triggered, after which you can access the retrieved scores by calling ScoresController::GetScores().
		*/
		static void LoadRangeForScore(ScoresController* self, Score* score, unsigned int range_length);

		/**
		* Starts loading the next range of scores.
		*
		* This function requests the next available range of scores from the server. 
		* Note that this is an asynchronous call and a callback will be triggered, after which you can access the retrieved scores by calling ScoresController::GetScores().
		*/
		static void LoadNextRange(ScoresController* self);

		/**
		* Starts loading the previous range of scores.
		*
		* This function requests the previous range of scores from the server. This function is used for backward pagination.
		* Note that this is an asynchronous call and a callback will be triggered, after which you can access the retrieved scores by calling ScoresController::GetScores().
		*/
		static void LoadPreviousRange(ScoresController* self);

		/**
		* Checks whether there is a previous range (page) of scores.
		*
		* This function checks if a "previous range" of scores is available from the server.
		* This function is used to check whether backward pagination through a a list of scores is possible.
		*/
		static bool HasPreviousRange(ScoresController* self);

		/**
		* Checks whether there is a next range (page) of scores.
		*
		* This function checks whether there is a "next range" of scores available from the server. 
		* This function is used to check whether forward pagination through a list of scores is possible.
		*/
		static bool HasNextRange(ScoresController* self);

		/**
		* Returns the retrieved list of scores.
		*
		* This function returns the list of scores that were returned by the server. 
		* Call this function only after receiving notification of a successful server request via delegate callbacks.
		*/
		static ScoreList* GetScores(ScoresController* self);
	};
	
	struct UserList
	{
		/**
		* Retrieves a specific user from the list.
		*
		* This function returns the user at the given index in the list.
		*/
		static User* GetUser(UserList* self, unsigned int idx);

		/**
		* Returns the number of users on the list.
		*
		* This function returns the total number of users on the list.
		*/
		static unsigned int GetUsersCount(UserList* self);
	};
	
	struct UsersController
	{
		/**
		* Creates a users controller, which can be used to retrieve lists of User objects from the server.
		*
		* This function creates an instance of UsersController. The controller is used to lists of User objects from the server based on various criteria.
		*/
		static UsersController* Create(RequestCompletionCallback callback);

		/** 
		* Decrements the object's reference count, and deletes the object if the counter reaches 0.
		*
		* This function decrements the reference count for the current controller instance. 
		* The current instance will be deleted when the reference count reaches 0.
		*/
		static void Release(UsersController* self);
		
		/**
		* Requests user objects from the server for the user whose login matches the supplied string.
		*
		* This method requests user objects from the server for the user whose login matches the supplied string.
		* Note that this is an asynchronous call and a callback will be triggered. After a successful server request, the user objects that are retrieved can be accessed by calling UsersController::GetUsers().
		* If the number of matched user objects exceeds the limit set by the developer, then the matched users will not be accessible. This means that calling UsersController::GetUsers() in this case will return an empty list.
		*/
		static void SearchByLogin(UsersController* self, const char* login, UsersSearchOperator::Enum search_operator = UsersSearchOperator::ExactMatch, unsigned int limit = 15, bool global = false);

		/**
		* Requests user objects from the server for the user whose email address matches the supplied string.
		*
		* This method requests user objects from the server for the user whose email address matches the supplied string.
		* Note that this is an asynchronous call and a callback will be triggered. After a successful server request, the user objects that are retrieved can be accessed by calling UsersController::GetUsers().
		* If the number of matched user objects exceeds the limit set by the developer, then the matched users will not be accessible. This means that calling UsersController::GetUsers() in this case will return an empty list.
		*/
		static void SearchByEmail(UsersController* self, const char* email, UsersSearchOperator::Enum search_operator = UsersSearchOperator::ExactMatch, unsigned int limit = 15, bool global = false);
		
		/**
		* Reports whether the search limit was exceeded or not.
		*
		* This method will return the value "true" if the number of positive matches found on the server exceeds the "limit" value. Otherwise, it will have the value "false".
		*/
		static bool IsOverLimit(UsersController* self);

		/**
		* Returns the list of user objects that have been successfully retrieved from the server after calling one of the search methods.
		*
		* After a successful server request, the user objects that are retrieved will normally be accessible using this accessor.
		* In cases where the number of positive matches found on the server exceeds the "limit" value, then this method will return an empty list.
		*/
		static UserList* GetUsers(UsersController* self);
	};

	struct Money
	{
		/**
		* Returns the amount of game-specific currency represented by the money object.
		*
		* This function returns the amount of currency represented by the instance of Money.
		*/
		static unsigned long GetAmount(Money* self);
	};

	struct Award
	{
		/**
		* Returns the identifier that uniquely identifies the award.
		*
		* When you configure an award at https://developer.scoreloop.com, you choose a unique reverse DNS style name to identify it, for example, com.mycompany.myfirstaward.
		* After the awards are configured on the developer website, Scoreloop generates an award bundle which you must to your game application. This provides local access to all award information, including award identifiers.
		*/
		static const char* GetIdentifier(Award* self);

		/**
		* Returns the initial value for the Award.
		*
		* This method returns the initial value for the Award.
		*/
		static int GetInitialValue(Award* self);

		/**
		* Returns the achieving value for the Award instance.
		*
		* This method returns the achieving value for the Award instance. The achieving value is the value that a user must reach before they achieve the Award.
		*/
		static int GetAchievingValue(Award* self);

		/**
		* Returns the localized description for the Award.
		*
		* A short description of the Award that you write. The localized description should be used to display additional information about the Award to users.
		* This property is configured at https:://developer.scoreloop.com, and is accessed locally using the SC_Awards bundle.
		*/
		static const char* GetLocalizedDescription(Award* self);

		/**
		* Returns the localized title for the Award.
		*
		* In addition to the reverse DNS identifier, Award objects can also store a localized title.
		* The localized title provides a more meaningful or natural name for the SC_Award object and should be used for display purposes.
		* This property is configured at https://developer.scoreloop.com and is accessed locally using the SC_Awards bundle.
		*/
		static const char* GetLocalizedTitle(Award* self);

		/**
		* Returns an SC_Money object corresponding to the reward that the user will receive if they achieve the Award.
		*
		* The amount of money that is credited to the user's account once the Award has been achieved. 
		* This property is configured at https://developer.scoreloop.com and is accessed locally using the SC_Awards bundle.
		*/
		static Money* GetRewardMoney(Award* self);

		/**
		* Checks if the Award is achieved for the value supplied.
		*
		* This method checks whether the Award is achieved for the supplied value.
		*/
		static bool IsAchievedByValue(Award* self, int value);

		/**
		* Checks if a value lies within the counter range specified.
		*
		* This method checks whether a given value lies within the counter range specified for the Award.
		*/
		static bool IsValidCounterValue(Award* self, int value);
	};

	struct Achievement
	{
		/**
		* Returns the Award object that this achievement refers to.
		*
		* The award object that this achievement refers to.
		*/
		static Award* GetAward(Achievement* self);

		/**
		* Returns the value of the score obtained by the user.
		*
		* This method returns the value of the score that is obtained by the user. 
		* If this property matches or exceeds the achievingvalue of the Award, the user achieves the award.
		*/
		static int GetValue(Achievement* self);

		/**
		* Returns whether or not the Award has been achieved.
		*
		* This method returns a boolean value that indicates if the Award has been achieved or not.
		*/
		static bool IsAchieved(Achievement* self);
	};

	struct AchievementList
	{
		/**
		* Retrieves a specific Achievement from the list of achievements.
		*
		* This function returns the Achievement at the given index in the AchievementList.
		*/
		static Achievement* GetAchievement(AchievementList* self, unsigned int idx);

		/**
		* Returns the number of Achievement objects in the AchievementList.
		*
		* This method returns the number of Achievement objects in the AchievementList.
		*/
		static unsigned int GetAchievementsCount(AchievementList* self);
	};

	struct AchievementsController
	{
		/**
		* Creates an achievements controller, which is used to manage Achievements.
		*
		* This function creates an instance of AchievementsController. The controller can be used to retrieve lists of achievements
		* from the server or to set an Award as being achieved for a user and to synchronize information stored locally with that on the server.
		*/
		static AchievementsController* Create(RequestCompletionCallback callback);

		/** 
		* Decrements the object's reference count, and deletes the object if the counter reaches 0.
		*
		* This function decrements the reference count for the current controller instance. 
		* The current instance will be deleted when the reference count reaches 0.
		*/
		static void Release(AchievementsController* self);

		/**
		* Gets the user for whom the achievements are requested.
		*
		* This method gets the user for whom the achievements are requested.
		*/
		static User* GetUser(AchievementsController* self);

		/**
		* Loads the achievements for the specified user.
		*
		* This method loads achievements for the specified user.
		* Achievements for the session owner are loaded from local storage, achievements for other users are retrieved from the Scoreloop server.
		* This method loads all achievements, whether they have been achieved or not. Note that this is an asynchronous call and a callback will be triggered, after which you can call SC_AchievementsController_GetAchievements() to access the list that is retrieved.
		* The awards bundle will be loaded transparently.
		*/
		static void LoadAchievements(AchievementsController* self, User* user = NULL);

		/**
		* Checks whether AchievementsController::SynchronizeAchievements() should be called or not.
		*
		* This method checks whether AchievementsController::SynchronizeAchievements() should be called or not. 
		* Synchronization should occur if the local database has never been synchronized with the server, or if an award has been achieved since the last synchronization.
		*/
		static bool ShouldSynchronizeAchievements(AchievementsController* self);

		/**
		* Synchronizes the local database with the Scoreloop server.
		*
		* This method synchronizes information about achievements that is stored locally on the device with the Scoreloop server. This method should be called if:
		* - The local database has never been synchronized with the Scoreloop server
		* - An award has been unlocked since the last synchronization call
		*
		* Use AchievementsController::ShouldSynchronizeAchievements() to check when this method should be called.
		* Note that this method raises an assertion if the user is not the local session-user. Also, note that this is an asynchronous call and a callback will be triggered.
		*/
		static void SynchronizeAchievements(AchievementsController* self);

		/**
		* Gets the achievements which are loaded by calling AchievementsController::LoadAchievements()
		*
		* This method gets Achievements loaded by AchievementsController::LoadAchievements().
		* Please make sure that AchievementsController::LoadAchievements() has been called before calling this accessor function.
		*/
		static AchievementList* GetAchievements(AchievementsController* self);

		/**
		* Counts how many awards the user has achieved and returns the total.
		*
		* Before calling this method, ensure that the achievements array has been filled. This is done by first calling AchievementsController::LoadAchievements()
		*/
		static unsigned int GetAchievedAwardsCount(AchievementsController* self);

		/**
		* Returns the achievement related to the award identifier specified in the achievements list.
		*
		* This method gets the achievement related to the award from the achievements list.
		*/
		static Achievement* GetAchievementForAwardIdentifier(AchievementsController* self, const char* award_identifier);

		/**
		* Checks the status of the award with the given identifier to see if it has been achieved.
		*
		* This method checks the status of the award with the given identifier to see if it has been achieved.
		*/
		static bool IsAchievedForAwardIdentifier(AchievementsController* self, const char* award_identifier);

		/**
		* Returns the value of the award specified by the given identifier.
		*
		* This method gets the value of the award specified by the given identifier. 
		* The award is identified using the unique reverse DNS identifier that you chose for it when you configured the award at https://developer.scoreloop.com.
		*/
		static int GetValueForAwardIdentifier(AchievementsController* self, const char* award_identifier);
		
		/**
		* Sets the value of a local achievement.
		*
		* This method sets the value of a specific award. The award is identified by using the unique reverse DNS identifier that you chose when you configured the award at https://developer.scoreloop.com.
		* The method sets values locally only and allows you to set increasing values only.
		*/
		static void SetValueForAwardIdentifier(AchievementsController* self, const char* award_identifier, int value);

		/**
		* Sets the status of the given award as "achieved".
		*
		* This method:
		* - Sets the value of the award to be equal to its achieving value
		* - Sets the achieved property of the award to be equal to "true"
		*/
		static void SetAchievedValueForAwardIdentifier(AchievementsController* self, const char* award_identifier);

		/**
		* Increments the value of the specified award by one.
		*
		* This method increments the value of the specified award by one. If the incremented value reaches the achieving value for the award, the user achieves the award.
		*/
		static void IncrementValueForAwardIdentifier(AchievementsController* self, const char* award_identifier);
	};

	struct RankingController
	{
		/**
		* Creates a ranking controller, which can be used to retrieve a rank.
		*
		* This function creates an instance of RankingController. The controller is used to retrieve ranks from the server.
		*/
		static RankingController* Create(RequestCompletionCallback callback);

		/** 
		* Decrements the object's reference count, and deletes the object if the counter reaches 0.
		*
		* This function decrements the reference count for the current controller instance. 
		* The current instance will be deleted when the reference count reaches 0.
		*/
		static void Release(RankingController* self);

		/**
		* Set the search criteria used for subsequent requests.
		*
		* This function sets the search criteria for the current controller instance.
		*/
		static void SetSearchCriteria(RankingController* self, SearchCriteria::Enum criteria);

		/**
		* Requests the ranking of a given score.
		*
		* This function is used to request the ranking for a given score.
		* Note that this is an asynchronous call and a callback will be triggered after which you could access the retrieved ranking by calling RankingController::GetRank().
		*/
		static void LoadRankingForScore(RankingController* self, Score* score);
		
		/**
		* Requests the ranking of a given user for a given mode.
		*
		* This function is used to request the ranking of a given user.
		* Note that this is an asynchronous call and a callback will be triggered after which you could access the retrieved ranking by calling RankingController::GetRank().
		*/
		static void LoadRankingForUserInMode(RankingController* self, User* user, unsigned int mode);
		
		/**
		* Returns the rank that is retrieved after making a successful server request.
		*
		* After a successful server request, use this function to return the rank that was requested.
		*/
		static unsigned int GetRank(RankingController* self);

		/**
		* Returns the received total number of scores.
		*
		* This method is used to get the total number of scores used for ranking purposes.
		*/
		static unsigned int GetTotal(RankingController* self);
	};

	struct Client
	{
		static void Initialize(TermsOfServiceAcceptionCallback callback);
		static void Shutdown();
		static bool IsInitialized();		
	};
};
