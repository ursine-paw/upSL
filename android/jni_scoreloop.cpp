// Copyright (c) 2012 ursinepaw.com
#include "../Scoreloop.h"

#include "android/jni/JniHelper.h"

namespace Scoreloop
{
	const char* User::GetLogin(User* self)
	{
		static std::string result;
		result = "";

		JniMethodInfo t;
		if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/User",
            "getLogin",
            "()Ljava/lang/String;"))
		{			
			jstring ret = (jstring)t.env->CallObjectMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);			
			if (ret)
			{
				jboolean is_copy = 0;
				const char* str = t.env->GetStringUTFChars(ret, &is_copy);
				if (str)
					result = str;
				if (is_copy)
					t.env->ReleaseStringUTFChars(ret, str);			
				t.env->DeleteLocalRef(ret);				
			}
		}
		return result.c_str();
	}

	const char* User::GetEmail(User* self)
	{
		static std::string result;
		result = "";

		JniMethodInfo t;
		if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/User",
            "getEmailAddress",
            "()Ljava/lang/String;"))
		{
			
			jstring ret = (jstring)t.env->CallObjectMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			if (ret)
			{
				jboolean is_copy = 0;
				const char* str = t.env->GetStringUTFChars(ret, &is_copy);
				if (str)
					result = str;
				if (is_copy)
					t.env->ReleaseStringUTFChars(ret, str);			
				t.env->DeleteLocalRef(ret);			
			}
		}		
		return result.c_str();
	}

	const char* User::GetIdentifier(User* self)
	{
		static std::string result;
		result = "";

		JniMethodInfo t;
		if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/User",
            "getIdentifier",
            "()Ljava/lang/String;"))
		{
			
			jstring ret = (jstring)t.env->CallObjectMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			if (ret)
			{
				jboolean is_copy = 0;
				const char* str = t.env->GetStringUTFChars(ret, &is_copy);
				if (str)
					result = str;
				if (is_copy)
					t.env->ReleaseStringUTFChars(ret, str);			
				t.env->DeleteLocalRef(ret);				
			}
		}
		return result.c_str();
	}

	void User::SetLogin(User* self, const char* login)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/User",
            "setLogin",
            "(Ljava/lang/String;)V"))
		{
			jstring arg = t.env->NewStringUTF(login);
			t.env->CallVoidMethod((jobject)self, t.methodID, arg);
			t.env->DeleteLocalRef(arg);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void User::SetEmail(User* self, const char* email)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/User",
            "setEmailAddress",
            "(Ljava/lang/String;)V"))
		{
			jstring arg = t.env->NewStringUTF(email);
			t.env->CallVoidMethod((jobject)self, t.methodID, arg);
			t.env->DeleteLocalRef(arg);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	UserList* User::GetBuddyUsers(User* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/User",
            "getBuddyUsers",
            "()Ljava/util/List;"))
		{
			jobject ret = t.env->CallObjectMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			return (UserList*)ret;
		}
		return NULL;
	}

	User* Session::GetUser()
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "sessionGetUser",
            "()Lcom/scoreloop/client/android/core/model/User;"))
		{
			jobject ret = t.env->CallStaticObjectMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			return (User*)ret;
		}
		return NULL;
	}

	UserController* UserController::Create(RequestCompletionCallback callback)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "userControllerCreate",
            "(J)Lcom/scoreloop/client/android/core/controller/UserController;"))
		{
			jlong arg = (long)(void*)callback;
			jobject ret = t.env->CallStaticObjectMethod(t.classID, t.methodID, arg);
			t.env->DeleteLocalRef(t.classID);
			return (UserController*)t.env->NewGlobalRef(ret);
		}
		return NULL;
	}

	void UserController::Release(UserController* self)
	{
		JNIEnv* env = JniHelper::getEnv();
		if (env)
			env->DeleteGlobalRef((jobject)self);
	}

	User* UserController::GetUser(UserController* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/controller/UserController",
            "getUser",
            "()Lcom/scoreloop/client/android/core/model/User;"))
		{
			jobject ret = t.env->CallObjectMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);			
			return (User*)ret;
		}
		return NULL;
	}

	void UserController::LoadUser(UserController* self)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "userControllerLoadUser",
            "(Lcom/scoreloop/client/android/core/controller/UserController;)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)self);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void UserController::SubmitUser(UserController* self)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "userControllerSubmitUser",
            "(Lcom/scoreloop/client/android/core/controller/UserController;)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)self);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void UserController::SetUser(UserController* self, User* user)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/controller/UserController",
            "setUser",
            "(Lcom/scoreloop/client/android/core/model/Entity;)V"))
		{
			t.env->CallVoidMethod((jobject)self, t.methodID, (jobject)user);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void UserController::AddAsBuddy(UserController* self)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "userControllerAddAsBuddy",
            "(Lcom/scoreloop/client/android/core/controller/UserController;)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)self);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void UserController::RemoveAsBuddy(UserController* self)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "userControllerRemoveAsBuddy",
            "(Lcom/scoreloop/client/android/core/controller/UserController;)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)self);
			t.env->DeleteLocalRef(t.classID);
		}
	}
	
	void UserController::RequestBuddies(UserController* self)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "userControllerRequestBuddies",
            "(Lcom/scoreloop/client/android/core/controller/UserController;)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)self);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	double Score::GetResult(Score* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/Score",
            "getResult",
            "()Ljava/lang/Double;"))
		{
			jobject value = t.env->CallObjectMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);

			if (value != NULL)
			{
				jdouble ret = 0.0;
				if (JniHelper::getMethodInfo(t,
						"java/lang/Double",
						"doubleValue",
						"()D"))
				{
					ret = t.env->CallDoubleMethod(value, t.methodID);
					t.env->DeleteLocalRef(t.classID);
				}
				t.env->DeleteLocalRef(value);
				return ret;
			}
		}
		return 0.0;
	}

	double Score::GetMinorResult(Score* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/Score",
            "getMinorResult",
            "()Ljava/lang/Double;"))
		{
			jobject value = t.env->CallObjectMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);

			if (value != NULL)
			{ 
				jdouble ret = 0.0;
				if (JniHelper::getMethodInfo(t,
						"java/lang/Double",
						"doubleValue",
						"()D"))
				{
					ret = t.env->CallDoubleMethod(value, t.methodID);
					t.env->DeleteLocalRef(t.classID);
				}
				t.env->DeleteLocalRef(value);
				return ret;
			}
		}
		return 0.0;
	}

	unsigned int Score::GetLevel(Score* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/Score",
            "getLevel",
            "()Ljava/lang/Integer;"))
		{
			jobject value = t.env->CallObjectMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);

			if (value != NULL)
			{
				jint ret = 0;
				if (JniHelper::getMethodInfo(t,
						"java/lang/Integer",
						"intValue",
						"()I"))
				{
					ret = t.env->CallIntMethod(value, t.methodID);
					t.env->DeleteLocalRef(t.classID);
				}
				t.env->DeleteLocalRef(value);
				return ret;
			}
		}
		return 0;
	}

	unsigned int Score::GetMode(Score* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/Score",
            "getMode",
            "()Ljava/lang/Integer;"))
		{
			jobject value = t.env->CallObjectMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);

			if (value != NULL)
			{
				jint ret = 0;
				if (JniHelper::getMethodInfo(t,
						"java/lang/Integer",
						"intValue",
						"()I"))
				{
					ret = t.env->CallIntMethod(value, t.methodID);
					t.env->DeleteLocalRef(t.classID);
				}
				t.env->DeleteLocalRef(value);
				return ret;
			}
		}
		return 0;
	}

	unsigned int Score::GetRank(Score* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/Score",
            "getRank",
            "()Ljava/lang/Integer;"))
		{
			jobject value = t.env->CallObjectMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);

			if (value != NULL)			
			{
				jint ret = 0;
				if (JniHelper::getMethodInfo(t,
						"java/lang/Integer",
						"intValue",
						"()I"))
				{
					ret = t.env->CallIntMethod(value, t.methodID);
					t.env->DeleteLocalRef(t.classID);					
				}
				t.env->DeleteLocalRef(value);
				return ret;
			}			
		}
		return 0;
	}

	User* Score::GetUser(Score* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/Score",
            "getUser",
            "()Lcom/scoreloop/client/android/core/model/User;"))
		{
			jobject ret = t.env->CallObjectMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			return (User*)ret;
		}
		return NULL;
	}

	const char* Score::GetContextString(Score* self, const char* key)
	{
		static std::string result;
		result = "";

		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/Score",
            "getContext",
            "()Ljava/util/Map;"))
		{
			jobject context = t.env->CallObjectMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			if (context && 
				JniHelper::getMethodInfo(t,
					"java/util/Map",
					"get",
					"(Ljava/lang/Object;)Ljava/lang/Object;"))
			{
				jstring arg = t.env->NewStringUTF(key);			
				jstring ret = (jstring)t.env->CallObjectMethod(context, t.methodID, arg);
				t.env->DeleteLocalRef(arg);
				t.env->DeleteLocalRef(t.classID);
				if (ret)
				{
					jboolean is_copy = 0;
					const char* str = t.env->GetStringUTFChars(ret, &is_copy);
					if (str)
						result = str;
					if (is_copy)
						t.env->ReleaseStringUTFChars(ret, str);			
					t.env->DeleteLocalRef(ret);
				}
			}
		}
		return result.c_str();
	}

	Score* ScoreList::GetScore(ScoreList* self, unsigned int idx)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "java/util/List",
            "get",
            "(I)Ljava/lang/Object;"))
		{
			jobject ret = t.env->CallObjectMethod((jobject)self, t.methodID, (jint)idx);
			t.env->DeleteLocalRef(t.classID);
			return (Score*)ret;
		}
		return NULL;
	}

	unsigned int ScoreList::GetScoresCount(ScoreList* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "java/util/List",
            "size",
            "()I"))
		{
			jint ret = t.env->CallIntMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			return ret;
		}
		return 0;
	}

	ScoreController* ScoreController::Create(RequestCompletionCallback callback)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "scoreControllerCreate",
            "(J)Lcom/scoreloop/client/android/core/controller/ScoreController;"))
		{
			jlong arg = (long)(void*)callback;
			jobject ret = t.env->CallStaticObjectMethod(t.classID, t.methodID, arg);
			t.env->DeleteLocalRef(t.classID);
			return (ScoreController*)t.env->NewGlobalRef(ret);
		}
		return NULL;
	}

	void ScoreController::Release(ScoreController* self)
	{
		JNIEnv* env = JniHelper::getEnv();
		if (env)
			env->DeleteGlobalRef((jobject)self);
	}

	Score* ScoreController::GetScore(ScoreController* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/controller/ScoreController",
            "getScore",
            "()Lcom/scoreloop/client/android/core/model/Score;"))
		{
			jobject ret = t.env->CallObjectMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			return (Score*)ret;
		}
		return NULL;
	}

	void ScoreController::SubmitScore(ScoreController* self, double result, double minor_result, unsigned int level, unsigned int mode, std::map<std::string, std::string>* context)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "scoreControllerSubmitScore",
            "(Lcom/scoreloop/client/android/core/controller/ScoreController;DDIILjava/util/Map;)V"))
		{
			jdouble arg1 = result;
			jdouble arg2 = minor_result;
			jint arg3 = level;
			jint arg4 = mode;
			jobject arg5 = 0;
			if (context)
			{
				jclass mapClass = t.env->FindClass("java/util/HashMap");
				if (mapClass)
				{
					jsize map_size = context->size();
					jmethodID init = t.env->GetMethodID(mapClass, "<init>", "(I)V");
					jobject hash_map = t.env->NewObject(mapClass, init, map_size);
					jmethodID put = t.env->GetMethodID(mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
					
					for (std::map<std::string, std::string>::const_iterator it = context->begin(); it != context->end(); ++it)
					{
						jstring key = t.env->NewStringUTF(it->first.c_str());
						jstring value = t.env->NewStringUTF(it->second.c_str());
						t.env->CallObjectMethod(hash_map, put, key, value);
						t.env->DeleteLocalRef(value);
						t.env->DeleteLocalRef(key);
					}				
					arg5 = hash_map;
					t.env->DeleteLocalRef(mapClass);
				}
			}
			t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)self, arg1, arg2, arg3, arg4, arg5);
			if (arg5)
				t.env->DeleteLocalRef(arg5);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	ScoresController* ScoresController::Create(RequestCompletionCallback callback)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "scoresControllerCreate",
            "(J)Lcom/scoreloop/client/android/core/controller/ScoresController;"))
		{
			jlong arg = (long)(void*)callback;
			jobject ret = t.env->CallStaticObjectMethod(t.classID, t.methodID, arg);
			t.env->DeleteLocalRef(t.classID);
			return (ScoresController*)t.env->NewGlobalRef(ret);
		}
		return NULL;
	}

	void ScoresController::Release(ScoresController* self)
	{
		JNIEnv* env = JniHelper::getEnv();
		if (env)
			env->DeleteGlobalRef((jobject)self);
	}

	void ScoresController::SetSearchCriteria(ScoresController* self, SearchCriteria::Enum criteria)
	{
		char method_name[256] = "";
		switch (criteria)
		{
		case SearchCriteria::Global:
			strcpy(method_name, "getGlobalScoreSearchList");
			break;
		case SearchCriteria::TwentyFourHour:
			strcpy(method_name, "getTwentyFourHourScoreSearchList");
			break;
		case SearchCriteria::UserCountry:
			strcpy(method_name, "getUserCountryLocationScoreSearchList");
			break;
		case SearchCriteria::BuddyhoodOnly:
			strcpy(method_name, "getBuddiesScoreSearchList");
			break;
		}

		CC_ASSERT(strlen(method_name) > 0);
		
		jobject search_list = NULL;
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/scoreloop/client/android/core/model/SearchList",
            method_name,
            "()Lcom/scoreloop/client/android/core/model/SearchList;"))
		{
			search_list = t.env->CallStaticObjectMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}

		if (search_list != NULL &&
			JniHelper::getMethodInfo(t,
					"com/scoreloop/client/android/core/controller/ScoresController",
					"setSearchList",
					"(Lcom/scoreloop/client/android/core/model/SearchList;)V"))
		{
			t.env->CallVoidMethod((jobject)self, t.methodID, search_list);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void ScoresController::SetMode(ScoresController* self, unsigned int mode)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/controller/ScoresController",
            "setMode",
            "(Ljava/lang/Integer;)V"))
		{
			jclass intClass = t.env->FindClass("java/lang/Integer");
			jmethodID intConstructor = t.env->GetMethodID(intClass, "<init>", "(I)V");
			jobject arg = t.env->NewObject(intClass, intConstructor, (jint)mode);
			t.env->CallVoidMethod((jobject)self, t.methodID, arg);
			t.env->DeleteLocalRef(arg);
			t.env->DeleteLocalRef(intClass);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void ScoresController::LoadRange(ScoresController* self, unsigned int range_start, unsigned int range_length)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "scoresControllerLoadRange",
            "(Lcom/scoreloop/client/android/core/controller/ScoresController;II)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)self, (jint)range_start, (jint)range_length);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void ScoresController::LoadRangeForUser(ScoresController* self, User* user, unsigned int range_length)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "scoresControllerLoadRangeForUser",
            "(Lcom/scoreloop/client/android/core/controller/ScoresController;Lcom/scoreloop/client/android/core/model/User;I)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)self, (jobject)user, (jint)range_length);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void ScoresController::LoadRangeAtRank(ScoresController* self, unsigned int rank, unsigned int range_length)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "scoresControllerLoadRangeAtRank",
            "(Lcom/scoreloop/client/android/core/controller/ScoresController;II)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)self, (jint)rank, (jint)range_length);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void ScoresController::LoadRangeForScore(ScoresController* self, Score* score, unsigned int range_length)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "scoresControllerLoadRangeForScore",
            "(Lcom/scoreloop/client/android/core/controller/ScoresController;Lcom/scoreloop/client/android/core/model/Score;I)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)self, (jobject)score, (jint)range_length);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void ScoresController::LoadNextRange(ScoresController* self)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "scoresControllerLoadNextRange",
            "(Lcom/scoreloop/client/android/core/controller/ScoresController;)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)self);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void ScoresController::LoadPreviousRange(ScoresController* self)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "scoresControllerLoadPreviousRange",
            "(Lcom/scoreloop/client/android/core/controller/ScoresController;)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)self);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	bool ScoresController::HasPreviousRange(ScoresController* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/controller/ScoresController",
            "hasPreviousRange",
            "()Z"))
		{
			jboolean ret = t.env->CallBooleanMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			return ret == JNI_TRUE;
		}
		return false;
	}

	bool ScoresController::HasNextRange(ScoresController* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/controller/ScoresController",
            "hasNextRange",
            "()Z"))
		{
			jboolean ret = t.env->CallBooleanMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			return ret == JNI_TRUE;
		}
		return false;
	}

	ScoreList* ScoresController::GetScores(ScoresController* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/controller/ScoresController",
            "getScores",
            "()Ljava/util/List;"))
		{
			jobject ret = t.env->CallObjectMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			return (ScoreList*)ret;
		}
		return NULL;
	}

	User* UserList::GetUser(UserList* self, unsigned int idx)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "java/util/List",
            "get",
            "(I)Ljava/lang/Object;"))
		{
			jobject ret = t.env->CallObjectMethod((jobject)self, t.methodID, (jint)idx);
			t.env->DeleteLocalRef(t.classID);
			return (User*)ret;
		}
		return NULL;

	}

	unsigned int UserList::GetUsersCount(UserList* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "java/util/List",
            "size",
            "()I"))
		{
			jint ret = t.env->CallIntMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			return ret;
		}
		return 0;
	}

	UsersController* UsersController::Create(RequestCompletionCallback callback)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "usersControllerCreate",
            "(J)Lcom/scoreloop/client/android/core/controller/UsersController;"))
		{
			jlong arg = (long)(void*)callback;
			jobject ret = t.env->CallStaticObjectMethod(t.classID, t.methodID, arg);
			t.env->DeleteLocalRef(t.classID);
			return (UsersController*)t.env->NewGlobalRef(ret);;
		}
		return NULL;
	}

	void UsersController::Release(UsersController* self)
	{
		JNIEnv* env = JniHelper::getEnv();
		if (env)
			env->DeleteGlobalRef((jobject)self);
	}
	
	void UsersController::SearchByLogin(UsersController* self, const char* login, UsersSearchOperator::Enum search_operator, unsigned int limit, bool global)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "usersControllerSearchByLogin",
            "(Lcom/scoreloop/client/android/core/controller/UsersController;Ljava/lang/String;IIZ)V"))
		{
			jstring arg = t.env->NewStringUTF(login);
			t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)self, arg, (jint)search_operator, (jint)limit, (jboolean)global);
			t.env->DeleteLocalRef(arg);
			t.env->DeleteLocalRef(t.classID);			
		}
	}

	void UsersController::SearchByEmail(UsersController* self, const char* email, UsersSearchOperator::Enum search_operator, unsigned int limit, bool global)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "usersControllerSearchByEmail",
            "(Lcom/scoreloop/client/android/core/controller/UsersController;Ljava/lang/String;IIZ)V"))
		{
			jstring arg = t.env->NewStringUTF(email);
			t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)self, arg, (jint)search_operator, (jint)limit, (jboolean)global);
			t.env->DeleteLocalRef(arg);
			t.env->DeleteLocalRef(t.classID);			
		}
	}
	
	bool UsersController::IsOverLimit(UsersController* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/controller/UsersController",
            "isOverLimit",
            "()Z"))
		{
			jboolean ret = t.env->CallBooleanMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			return ret == JNI_TRUE;
		}
		return false;
	}
	
	UserList* UsersController::GetUsers(UsersController* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/controller/UsersController",
            "getUsers",
            "()Ljava/util/List;"))
		{
			jobject ret = t.env->CallObjectMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			return (UserList*)ret;
		}
		return NULL;
	}

	unsigned long Money::GetAmount(Money* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/Money",
            "getAmount",
            "()Ljava/math/BigDecimal"))
		{
			jobject value = t.env->CallObjectMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);

			if (value != NULL)
			{
				jlong ret = 0;
				if (JniHelper::getMethodInfo(t,
						"java/math/BigDecimal",
						"longValue",
						"()J"))
				{
					ret = t.env->CallLongMethod(value, t.methodID);
					t.env->DeleteLocalRef(t.classID);
				}
				t.env->DeleteLocalRef(value);
				return ret;
			}
		}
		return 0;
	}

	const char* Award::GetIdentifier(Award* self)
	{
		static std::string result;
		result = "";

		JniMethodInfo t;
		if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/Award",
            "getIdentifier",
            "()Ljava/lang/String;"))
		{			
			jstring ret = (jstring)t.env->CallObjectMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			if (ret)
			{
				jboolean is_copy = 0;
				const char* str = t.env->GetStringUTFChars(ret, &is_copy);
				if (str)
					result = str;
				if (is_copy)
					t.env->ReleaseStringUTFChars(ret, str);			
				t.env->DeleteLocalRef(ret);				
			}
		}
		return result.c_str();
	}

	int Award::GetInitialValue(Award* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/Award",
            "getInitialValue",
            "()I"))
		{
			jint ret = t.env->CallIntMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			return ret;
		}
		return 0;
	}

	int Award::GetAchievingValue(Award* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/Award",
            "getAchievingValue",
            "()I"))
		{
			jint ret = t.env->CallIntMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			return ret;
		}
		return 0;
	}

	const char* Award::GetLocalizedDescription(Award* self)
	{
		static std::string result;
		result = "";

		JniMethodInfo t;
		if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/Award",
            "getLocalizedDescription",
            "()Ljava/lang/String;"))
		{
			
			jstring ret = (jstring)t.env->CallObjectMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			if (ret)
			{
				jboolean is_copy = 0;
				const char* str = t.env->GetStringUTFChars(ret, &is_copy);
				if (str)
					result = str;
				if (is_copy)
					t.env->ReleaseStringUTFChars(ret, str);			
				t.env->DeleteLocalRef(ret);
			}			
		}
		return result.c_str();
	}

	const char* Award::GetLocalizedTitle(Award* self)
	{
		static std::string result;
		result = "";

		JniMethodInfo t;
		if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/Award",
            "getLocalizedTitle",
            "()Ljava/lang/String;"))
		{
			
			jstring ret = (jstring)t.env->CallObjectMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			if (ret)
			{
				jboolean is_copy = 0;
				const char* str = t.env->GetStringUTFChars(ret, &is_copy);
				if (str)
					result = str;
				if (is_copy)
					t.env->ReleaseStringUTFChars(ret, str);			
				t.env->DeleteLocalRef(ret);
			}			
		}
		return result.c_str();
	}

	Money* Award::GetRewardMoney(Award* self)
	{
		JniMethodInfo t;
		if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/Award",
            "getRewardMoney",
            "()Lcom/scoreloop/client/android/core/model/Money;"))
		{			
			jobject ret = t.env->CallObjectMethod((jobject)self, t.methodID);	
			t.env->DeleteLocalRef(t.classID);
			return (Money*)ret;
		}
		return NULL;
	}

	bool Award::IsAchievedByValue(Award* self, int value)
	{
		JniMethodInfo t;
		if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/Award",
            "isAchievedByValue",
            "(I)Z"))
		{			
			jboolean ret = t.env->CallBooleanMethod((jobject)self, t.methodID, (jint)value);	
			t.env->DeleteLocalRef(t.classID);
			return ret;
		}
		return false;		
	}

	bool Award::IsValidCounterValue(Award* self, int value)
	{
		JniMethodInfo t;
		if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/Award",
            "isValidCounterValue",
            "(I)Z"))
		{			
			jboolean ret = t.env->CallBooleanMethod((jobject)self, t.methodID, (jint)value);	
			t.env->DeleteLocalRef(t.classID);
			return ret;
		}
		return false;	
	}

	Award* Achievement::GetAward(Achievement* self)
	{
		JniMethodInfo t;
		if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/Achievement",
            "getAward",
            "()Lcom/scoreloop/client/android/core/model/Award;"))
		{			
			jobject ret = t.env->CallObjectMethod((jobject)self, t.methodID);	
			t.env->DeleteLocalRef(t.classID);
			return (Award*)ret;
		}
		return NULL;
	}
	
	int Achievement::GetValue(Achievement* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/Achievement",
            "getValue",
            "()I"))
		{
			jint ret = t.env->CallIntMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			return ret;
		}
		return 0;
	}

	bool Achievement::IsAchieved(Achievement* self)
	{
		JniMethodInfo t;
		if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/model/Achievement",
            "isAchieved",
            "()Z"))
		{			
			jboolean ret = t.env->CallBooleanMethod((jobject)self, t.methodID);	
			t.env->DeleteLocalRef(t.classID);
			return ret;
		}
		return false;
	}

	Achievement* AchievementList::GetAchievement(AchievementList* self, unsigned int idx)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "java/util/List",
            "get",
            "(I)Ljava/lang/Object;"))
		{
			jobject ret = t.env->CallObjectMethod((jobject)self, t.methodID, (jint)idx);
			t.env->DeleteLocalRef(t.classID);
			return (Achievement*)ret;
		}
		return NULL;
	}

	unsigned int AchievementList::GetAchievementsCount(AchievementList* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "java/util/List",
            "size",
            "()I"))
		{
			jint ret = t.env->CallIntMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			return ret;
		}
		return 0;
	}		

	AchievementsController* AchievementsController::Create(RequestCompletionCallback callback)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "achievementsControllerCreate",
            "(J)Lcom/scoreloop/client/android/core/controller/AchievementsController;"))
		{
			jlong arg = (long)(void*)callback;
			jobject ret = t.env->CallStaticObjectMethod(t.classID, t.methodID, arg);
			t.env->DeleteLocalRef(t.classID);
			return (AchievementsController*)t.env->NewGlobalRef(ret);
		}
		return NULL;
	}

	void AchievementsController::Release(AchievementsController* self)
	{
		JNIEnv* env = JniHelper::getEnv();
		if (env)
			env->DeleteGlobalRef((jobject)self);
	}

	User* AchievementsController::GetUser(AchievementsController* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/controller/AchievementsController",
            "getUser",
            "()Lcom/scoreloop/client/android/core/model/User;"))
		{
			jobject ret = t.env->CallObjectMethod((jobject)self, t.methodID);
			return (User*)ret;
		}
		return NULL;
	}

	void AchievementsController::LoadAchievements(AchievementsController* self, User* user)
	{
		JniMethodInfo t;

		if (user != NULL)
		{
			if (JniHelper::getMethodInfo(t,
				"com/scoreloop/client/android/core/controller/AchievementsController",
				"setUser",
				"(Lcom/scoreloop/client/android/core/model/User;)V"))
			{
				t.env->CallVoidMethod((jobject)self, t.methodID, (jobject)user);
			}
		}

		if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/controller/AchievementsController",
            "setForceInitialSync",
            "(Z)V"))
		{
			t.env->CallVoidMethod((jobject)self, t.methodID, (jboolean)false);
			t.env->DeleteLocalRef(t.classID);
		}

        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "achievementsControllerLoadAchievements",
            "(Lcom/scoreloop/client/android/core/controller/AchievementsController;)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)self);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	bool AchievementsController::ShouldSynchronizeAchievements(AchievementsController* self)
	{

		bool result = true;

		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "achievementsControllerShouldSynchronizeAchievements",
            "()Z"))
		{
			result = t.env->CallStaticBooleanMethod(t.classID, t.methodID) == JNI_TRUE;
			t.env->DeleteLocalRef(t.classID);
		}
		return result;
	}

	void AchievementsController::SynchronizeAchievements(AchievementsController* self)
	{
		JniMethodInfo t;
		if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/controller/AchievementsController",
            "setForceInitialSync",
            "(Z)V"))
		{
			t.env->CallVoidMethod((jobject)self, t.methodID, (jboolean)true);
			t.env->DeleteLocalRef(t.classID);
		}

        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "achievementsControllerLoadAchievements",
            "(Lcom/scoreloop/client/android/core/controller/AchievementsController;)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)self);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	AchievementList* AchievementsController::GetAchievements(AchievementsController* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/controller/AchievementsController",
            "getAchievements",
            "()Ljava/util/List;"))
		{
			jobject ret = t.env->CallObjectMethod((jobject)self, t.methodID);
			return (AchievementList*)ret;
		}
		return NULL;
	}

	unsigned int AchievementsController::GetAchievedAwardsCount(AchievementsController* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/controller/AchievementsController",
            "countAchievedAwards",
            "()I"))
		{
			jint ret = t.env->CallIntMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			return ret;
		}
		return 0;
	}

	Achievement* AchievementsController::GetAchievementForAwardIdentifier(AchievementsController* self, const char* award_identifier)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/controller/AchievementsController",
            "getAchievementForAwardIdentifier",
            "(Ljava/lang/String;)Lcom/scoreloop/client/android/core/model/Achievement;"))
		{
			jstring arg = t.env->NewStringUTF(award_identifier);
			jobject ret = t.env->CallObjectMethod((jobject)self, t.methodID, arg);
			t.env->DeleteLocalRef(arg);
			t.env->DeleteLocalRef(t.classID);
			return (Achievement*)ret;
		}
		return NULL;
	}

	bool AchievementsController::IsAchievedForAwardIdentifier(AchievementsController* self, const char* award_identifier)
	{	
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/controller/AchievementsController",
            "getAchievementForAwardIdentifier",
            "(Ljava/lang/String;)Lcom/scoreloop/client/android/core/model/Achievement;"))
		{
			jstring arg = t.env->NewStringUTF(award_identifier);
			jobject achievement = t.env->CallObjectMethod((jobject)self, t.methodID, arg);
			t.env->DeleteLocalRef(arg);
			t.env->DeleteLocalRef(t.classID);

			return Achievement::IsAchieved((Achievement*)achievement);
		}
		return false;
	}

	int AchievementsController::GetValueForAwardIdentifier(AchievementsController* self, const char* award_identifier)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/controller/AchievementsController",
            "getAchievementForAwardIdentifier",
            "(Ljava/lang/String;)Lcom/scoreloop/client/android/core/model/Achievement;"))
		{
			jstring arg = t.env->NewStringUTF(award_identifier);
			jobject achievement = t.env->CallObjectMethod((jobject)self, t.methodID, arg);
			t.env->DeleteLocalRef(arg);
			t.env->DeleteLocalRef(t.classID);

			return Achievement::GetValue((Achievement*)achievement);
		}
		return 0;
	}
	
	void AchievementsController::SetValueForAwardIdentifier(AchievementsController* self, const char* award_identifier, int value)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/controller/AchievementsController",
            "getAchievementForAwardIdentifier",
            "(Ljava/lang/String;)Lcom/scoreloop/client/android/core/model/Achievement;"))
		{
			jstring arg = t.env->NewStringUTF(award_identifier);
			jobject achievement = t.env->CallObjectMethod((jobject)self, t.methodID, arg);
			t.env->DeleteLocalRef(arg);
			t.env->DeleteLocalRef(t.classID);

			if (JniHelper::getMethodInfo(t,
				"com/scoreloop/client/android/core/model/Achievement",
				"setValue",
				"(I)V"))
			{
				t.env->CallVoidMethod((jobject)achievement, t.methodID, (jint)value);
				t.env->DeleteLocalRef(t.classID);

				bool needsSubmit = false;
				if (JniHelper::getMethodInfo(t,
					"com/scoreloop/client/android/core/model/Achievement",
					"needsSubmit",
					"()Z"))
				{
					needsSubmit |= t.env->CallBooleanMethod((jobject)achievement, t.methodID) == JNI_TRUE;
					t.env->DeleteLocalRef(t.classID);
				}

				if (needsSubmit &&
					JniHelper::getStaticMethodInfo(t,
						"com/ursinepaw/scoreloop/Scoreloop",
						"scheduleAchievementsSynchronization",
						"()V"))
				{
					t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)self);
					t.env->DeleteLocalRef(t.classID);
				}
			}
		}
	}

	void AchievementsController::SetAchievedValueForAwardIdentifier(AchievementsController* self, const char* award_identifier)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/controller/AchievementsController",
            "getAchievementForAwardIdentifier",
            "(Ljava/lang/String;)Lcom/scoreloop/client/android/core/model/Achievement;"))
		{
			jstring arg = t.env->NewStringUTF(award_identifier);
			jobject achievement = t.env->CallObjectMethod((jobject)self, t.methodID, arg);
			t.env->DeleteLocalRef(arg);
			t.env->DeleteLocalRef(t.classID);

			if (JniHelper::getMethodInfo(t,
				"com/scoreloop/client/android/core/model/Achievement",
				"setAchieved",
				"()V"))
			{
				t.env->CallVoidMethod((jobject)achievement, t.methodID);
				t.env->DeleteLocalRef(t.classID);

				bool needsSubmit = false;
				if (JniHelper::getMethodInfo(t,
					"com/scoreloop/client/android/core/model/Achievement",
					"needsSubmit",
					"()Z"))
				{
					needsSubmit |= t.env->CallBooleanMethod((jobject)achievement, t.methodID) == JNI_TRUE;
					t.env->DeleteLocalRef(t.classID);
				}

				if (needsSubmit &&
					JniHelper::getStaticMethodInfo(t,
						"com/ursinepaw/scoreloop/Scoreloop",
						"scheduleAchievementsSynchronization",
						"()V"))
				{
					t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)self);
					t.env->DeleteLocalRef(t.classID);
				}
			}
		}
	}

	void AchievementsController::IncrementValueForAwardIdentifier(AchievementsController* self, const char* award_identifier)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/controller/AchievementsController",
            "getAchievementForAwardIdentifier",
            "(Ljava/lang/String;)Lcom/scoreloop/client/android/core/model/Achievement;"))
		{
			jstring arg = t.env->NewStringUTF(award_identifier);
			jobject achievement = t.env->CallObjectMethod((jobject)self, t.methodID, arg);
			t.env->DeleteLocalRef(arg);
			t.env->DeleteLocalRef(t.classID);

			if (JniHelper::getMethodInfo(t,
				"com/scoreloop/client/android/core/model/Achievement",
				"incrementValue",
				"()V"))
			{
				t.env->CallVoidMethod((jobject)achievement, t.methodID);
				t.env->DeleteLocalRef(t.classID);

				bool needsSubmit = false;
				if (JniHelper::getMethodInfo(t,
					"com/scoreloop/client/android/core/model/Achievement",
					"needsSubmit",
					"()Z"))
				{
					needsSubmit |= t.env->CallBooleanMethod((jobject)achievement, t.methodID) == JNI_TRUE;
					t.env->DeleteLocalRef(t.classID);
				}

				if (needsSubmit &&
					JniHelper::getStaticMethodInfo(t,
						"com/ursinepaw/scoreloop/Scoreloop",
						"scheduleAchievementsSynchronization",
						"()V"))
				{
					t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)self);
					t.env->DeleteLocalRef(t.classID);
				}
			}
		}
	}


	RankingController* RankingController::Create(RequestCompletionCallback callback)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "rankingControllerCreate",
            "(J)Lcom/scoreloop/client/android/core/controller/RankingController;"))
		{
			jlong arg = (long)(void*)callback;
			jobject ret = t.env->CallStaticObjectMethod(t.classID, t.methodID, arg);
			t.env->DeleteLocalRef(t.classID);
			return (RankingController*)t.env->NewGlobalRef(ret);
		}
		return NULL;
	}

	void RankingController::Release(RankingController* self)
	{
		JNIEnv* env = JniHelper::getEnv();
		if (env)
			env->DeleteGlobalRef((jobject)self);
	}

	void RankingController::SetSearchCriteria(RankingController* self, SearchCriteria::Enum criteria)
	{
		char method_name[256] = "";
		switch (criteria)
		{
		case SearchCriteria::Global:
			strcpy(method_name, "getGlobalScoreSearchList");
			break;
		case SearchCriteria::TwentyFourHour:
			strcpy(method_name, "getTwentyFourHourScoreSearchList");
			break;
		case SearchCriteria::UserCountry:
			strcpy(method_name, "getUserCountryLocationScoreSearchList");
			break;
		case SearchCriteria::BuddyhoodOnly:
			strcpy(method_name, "getBuddiesScoreSearchList");
			break;
		}

		CC_ASSERT(strlen(method_name) > 0);
		
		jobject search_list = NULL;
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/scoreloop/client/android/core/model/SearchList",
            method_name,
            "()Lcom/scoreloop/client/android/core/model/SearchList;"))
		{
			search_list = t.env->CallStaticObjectMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}

		if (search_list != NULL &&
			JniHelper::getMethodInfo(t,
					"com/scoreloop/client/android/core/controller/RankingController",
					"setSearchList",
					"(Lcom/scoreloop/client/android/core/model/SearchList;)V"))
		{
			t.env->CallVoidMethod((jobject)self, t.methodID, search_list);
			t.env->DeleteLocalRef(t.classID);
		}
	}
	
	void RankingController::LoadRankingForScore(RankingController* self, Score* score)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "rankingControllerLoadRankingForScore",
            "(Lcom/scoreloop/client/android/core/controller/RankingController;Lcom/scoreloop/client/android/core/model/Score;)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)self, (jobject)score);
			t.env->DeleteLocalRef(t.classID);
		}
	}
	
	
	void RankingController::LoadRankingForUserInMode(RankingController* self, User* user, unsigned int mode)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "rankingControllerLoadRankingForUserInMode",
            "(Lcom/scoreloop/client/android/core/controller/RankingController;Lcom/scoreloop/client/android/core/model/User;I)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)self, (jobject)user, mode);
			t.env->DeleteLocalRef(t.classID);
		}
	}
	
	unsigned int RankingController::GetRank(RankingController* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/controller/RankingController",
            "getRanking",
            "()Lcom/scoreloop/client/android/core/model/Ranking;"))
		{
			jobject ranking = t.env->CallObjectMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);

			if (ranking != NULL)
			{
				 if (JniHelper::getMethodInfo(t,
					"com/scoreloop/client/android/core/model/Ranking",
					"getRank",
					"()Ljava/lang/Integer;"))
				{
					jobject value = t.env->CallObjectMethod(ranking, t.methodID);
					t.env->DeleteLocalRef(t.classID);

					jint ret = 0;
					if (value != NULL)
					{						
						if (JniHelper::getMethodInfo(t,
								"java/lang/Integer",
								"intValue",
								"()I"))
						{
							ret = t.env->CallIntMethod(value, t.methodID);
							t.env->DeleteLocalRef(t.classID);
						}
						t.env->DeleteLocalRef(value);						
					}
					return ret;
				}
			}
		}

		return 0;
	}

	unsigned int RankingController::GetTotal(RankingController* self)
	{
		JniMethodInfo t;
        if (JniHelper::getMethodInfo(t,
            "com/scoreloop/client/android/core/controller/RankingController",
            "getRanking",
            "()Lcom/scoreloop/client/android/core/model/Ranking;"))
		{
			jobject ranking = t.env->CallObjectMethod((jobject)self, t.methodID);
			t.env->DeleteLocalRef(t.classID);

			if (ranking != NULL)
			{
				 if (JniHelper::getMethodInfo(t,
					"com/scoreloop/client/android/core/model/Ranking",
					"getTotal",
					"()Ljava/lang/Integer;"))
				{
					jobject value = t.env->CallObjectMethod(ranking, t.methodID);
					t.env->DeleteLocalRef(t.classID);

					jint ret = 0;
					if (value != NULL)
					{						
						if (JniHelper::getMethodInfo(t,
								"java/lang/Integer",
								"intValue",
								"()I"))
						{
							ret = t.env->CallIntMethod(value, t.methodID);
							t.env->DeleteLocalRef(t.classID);
						}
						t.env->DeleteLocalRef(value);						
					}
					return ret;
				}
			}
		}

		return 0;
	}

	void Client::Initialize(TermsOfServiceAcceptionCallback callback)
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "initialize",
            "(J)V"))
		{
			jlong arg = (long)(void*)callback;
			t.env->CallStaticVoidMethod(t.classID, t.methodID, arg);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void Client::Shutdown()
	{
	}

	bool Client::IsInitialized()
	{
		JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t,
            "com/ursinepaw/scoreloop/Scoreloop",
            "isInitialized",
            "()Z"))
		{
			jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			return ret == JNI_TRUE;
		}
		return false;
	}
}

extern "C"
{
	void Java_com_ursinepaw_scoreloop_Scoreloop_nativeTOSAccepted(JNIEnv* env, jobject thiz, jlong callback_address)
	{
		if (callback_address)
			(*reinterpret_cast<Scoreloop::TermsOfServiceAcceptionCallback>(callback_address))();
	}

	void Java_com_ursinepaw_scoreloop_Scoreloop_nativeRequestComplete(JNIEnv* env, jobject thiz, jobject controller, jint error_code, jlong callback_address)
	{
		if (callback_address)
			(*reinterpret_cast<Scoreloop::RequestCompletionCallback>(callback_address))(error_code);
	}
}
