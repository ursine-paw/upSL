package com.ursinepaw.scoreloop;

import java.util.List;
import java.util.Map;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.opengl.GLSurfaceView;
import android.preference.PreferenceManager;

import com.scoreloop.client.android.core.controller.AchievementController;
import com.scoreloop.client.android.core.controller.AchievementsController;
import com.scoreloop.client.android.core.controller.RankingController;
import com.scoreloop.client.android.core.controller.RequestController;
import com.scoreloop.client.android.core.controller.RequestControllerException;
import com.scoreloop.client.android.core.controller.RequestControllerObserver;
import com.scoreloop.client.android.core.controller.ScoreController;
import com.scoreloop.client.android.core.controller.ScoresController;
import com.scoreloop.client.android.core.controller.TermsOfServiceController;
import com.scoreloop.client.android.core.controller.TermsOfServiceControllerObserver;
import com.scoreloop.client.android.core.controller.UserController;
import com.scoreloop.client.android.core.controller.UsersController;
import com.scoreloop.client.android.core.controller.UsersController.LoginSearchOperator;
import com.scoreloop.client.android.core.model.Achievement;
import com.scoreloop.client.android.core.model.Client;
import com.scoreloop.client.android.core.model.Score;
import com.scoreloop.client.android.core.model.Session;
import com.scoreloop.client.android.core.model.TermsOfService;
import com.scoreloop.client.android.core.model.User;


public class Scoreloop {
	private static Activity			mActivity = null;
	private static GLSurfaceView	mGLSurfaceView = null;
	
	private static AchievementController	mAchievementController = null;	// for achievement submit
	private static boolean					mShouldSynchronizeAchievements = true;
	
	
	public static void setActivity(Activity activity) {
		mActivity = activity;
		SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(mActivity);
		mShouldSynchronizeAchievements = prefs.getBoolean("scoreloop_should_synchronize_achievements", true);
	}
	
	public static void setGLSurfaceView(GLSurfaceView view) {
		mGLSurfaceView = view;
	}
	
	public static void init(Context context, String secret) {
		Client.init(context, secret, null);
	}	
	
	public static User sessionGetUser() {	
		return Session.getCurrentSession().getUser();	
	}
	
	public static UserController userControllerCreate(final long callback_address) {

		return new UserController(new RequestControllerObserver() {			
			@Override
			public void requestControllerDidFail(final RequestController controller, Exception exception) {
				final int error_code;	
				if (exception instanceof RequestControllerException) {
					RequestControllerException controller_exception = (RequestControllerException)exception;
					switch (controller_exception.getErrorCode()) {
					case 0:
						if (controller_exception.hasDetail(RequestControllerException.DETAIL_USER_UPDATE_REQUEST_EMAIL_TAKEN))
							error_code = 10;	// ErrorCode::EmailAlreadyTaken
						else if (controller_exception.hasDetail(RequestControllerException.DETAIL_USER_UPDATE_REQUEST_USERNAME_TAKEN))
							error_code = 11;	// ErrorCode::UsernameAlreadyTaken
						else if (controller_exception.hasDetail(RequestControllerException.DETAIL_USER_UPDATE_REQUEST_INVALID_EMAIL))
							error_code = 12;	// ErrorCode::EmailFormatInvalid
						else if (controller_exception.hasDetail(RequestControllerException.DETAIL_USER_UPDATE_REQUEST_USERNAME_TOO_SHORT))
							error_code = 13;	// ErrorCode::UsernameTooShort
						else if (controller_exception.hasDetail(RequestControllerException.DETAIL_USER_UPDATE_REQUEST_INVALID_USERNAME))
							error_code = 14;	// ErrorCode::UsernameFormatInvalid
						else
							error_code = -1;	// ErrorCode::UnknownError
						break;
					case RequestControllerException.CODE_BUDDY_ADD_REQUEST_ALREADY_ADDED:
					case RequestControllerException.CODE_BUDDY_REMOVE_REQUEST_ALREADY_REMOVED:
						error_code = 1;			// ErrorCode::RequestFailed
						break;
					default:
						error_code = -1;		// ErrorCode::UnknownError
					}						
				}
				else
					error_code = -1;	// ErrorCode::UnknownError
				
				mGLSurfaceView.queueEvent(new Runnable() {
		            @Override
		            public void run() {
		            	nativeRequestComplete(controller, error_code, callback_address);
		            }
		        });										
			}

			@Override
			public void requestControllerDidReceiveResponse(final RequestController controller) {
				mGLSurfaceView.queueEvent(new Runnable() {
		            @Override
		            public void run() {
		            	nativeRequestComplete(controller, 0, callback_address);
		            }
		        });				
			}
		});
	}
	
	public static void userControllerLoadUser(final UserController controller) {	
		
		mActivity.runOnUiThread(new Runnable() {			
			@Override
			public void run() {
				controller.loadUser();				
			}
		});
		
	}
	
	public static void userControllerSubmitUser(final UserController controller) {
		mActivity.runOnUiThread(new Runnable() {			
			@Override
			public void run() {
				controller.submitUser();
			}
		});
	}
	
	public static void userControllerAddAsBuddy(final UserController controller) {
		mActivity.runOnUiThread(new Runnable() {			
			@Override
			public void run() {
				controller.addAsBuddy();
			}
		});
	}
	
	public static void userControllerRemoveAsBuddy(final UserController controller) {
		mActivity.runOnUiThread(new Runnable() {			
			@Override
			public void run() {
				controller.removeAsBuddy();
			}
		});
	}

	public static void userControllerRequestBuddies(final UserController controller) {
		mActivity.runOnUiThread(new Runnable() {			
			@Override
			public void run() {
				controller.loadBuddies();
			}
		});
	}
	
	public static ScoreController scoreControllerCreate(final long callback_address) {

		return new ScoreController(new RequestControllerObserver() {			
			@Override
			public void requestControllerDidFail(final RequestController controller, Exception exception) {	
				mGLSurfaceView.queueEvent(new Runnable() {
		            @Override
		            public void run() {
		            	nativeRequestComplete(controller, -1, callback_address);
		            }
		        });										
			}
	
			@Override
			public void requestControllerDidReceiveResponse(final RequestController controller) {
				mGLSurfaceView.queueEvent(new Runnable() {
		            @Override
		            public void run() {
		            	nativeRequestComplete(controller, 0, callback_address);
		            }
		        });				
			}
		});
	}
	
	public static void scoreControllerSubmitScore(final ScoreController controller, double result, double minor_result, int level, int mode, Map<String, Object> context) {
		final Score score = new Score(result, context);
		score.setMinorResult(minor_result);
		score.setLevel(level);
		score.setMode(mode);
		mActivity.runOnUiThread(new Runnable() {			
			@Override
			public void run() {				
				controller.submitScore(score);
			}
		});
	}
	
	public static ScoresController scoresControllerCreate(final long callback_address) {
		
		return new ScoresController(new RequestControllerObserver() {			
			@Override
			public void requestControllerDidFail(final RequestController controller, Exception exception) {	
				mGLSurfaceView.queueEvent(new Runnable() {
		            @Override
		            public void run() {
		            	nativeRequestComplete(controller, -1, callback_address);
		            }
		        });										
			}
	
			@Override
			public void requestControllerDidReceiveResponse(final RequestController controller) {
				mGLSurfaceView.queueEvent(new Runnable() {
		            @Override
		            public void run() {
		            	nativeRequestComplete(controller, 0, callback_address);
		            }
		        });				
			}
		});
	}

	public static void scoresControllerLoadRange(final ScoresController controller, int range_start, int range_length) {
		controller.setRangeLength(range_length);
		final int _rank = range_start  + 1;
		mActivity.runOnUiThread(new Runnable() {			
			@Override
			public void run() {	
				controller.loadRangeAtRank(_rank);
			}
		});
	}

	public static void scoresControllerLoadRangeForUser(final ScoresController controller, User user, int range_length) {
		controller.setRangeLength(range_length);
		final User _user = user;
		mActivity.runOnUiThread(new Runnable() {			
			@Override
			public void run() {	
				
				controller.loadRangeForUser(_user);
			}
		});
	}

	public static void scoresControllerLoadRangeAtRank(final ScoresController controller, int rank, int range_length) {
		controller.setRangeLength(range_length);
		final int _rank = rank;
		mActivity.runOnUiThread(new Runnable() {			
			@Override
			public void run() {	
				controller.loadRangeAtRank(_rank);
			}
		});
	}

	public static void scoresControllerLoadRangeForScore(final ScoresController controller, final Score score, int range_length) {
		controller.setRangeLength(range_length);
		mActivity.runOnUiThread(new Runnable() {			
			@Override
			public void run() {	
				controller.loadRangeForScore(score);
			}
		});
	}

	public static void scoresControllerLoadNextRange(final ScoresController controller) {
		mActivity.runOnUiThread(new Runnable() {			
			@Override
			public void run() {	
				controller.loadNextRange();
			}
		});
	}

	public static void scoresControllerLoadPreviousRange(final ScoresController controller) {
		mActivity.runOnUiThread(new Runnable() {			
			@Override
			public void run() {	
				controller.loadPreviousRange();
			}
		});
	}

	public static UsersController usersControllerCreate(final long callback_address) {
		return new UsersController(new RequestControllerObserver() {			
			@Override
			public void requestControllerDidFail(final RequestController controller, Exception exception) {	
				mGLSurfaceView.queueEvent(new Runnable() {
		            @Override
		            public void run() {
		            	nativeRequestComplete(controller, -1, callback_address);
		            }
		        });										
			}
	
			@Override
			public void requestControllerDidReceiveResponse(final RequestController controller) {
				mGLSurfaceView.queueEvent(new Runnable() {
		            @Override
		            public void run() {
		            	nativeRequestComplete(controller, 0, callback_address);
		            }
		        });				
			}
		});
	}
	
	public static void usersControllerSearchByLogin(final UsersController controller, final String login, int search_operator, int limit, boolean global) {
		
		switch (search_operator) {
		case 0:
			controller.setSearchOperator(LoginSearchOperator.PREFIX);
			break;
		case 1:
			controller.setSearchOperator(LoginSearchOperator.EXACT_MATCH);
		}
		controller.setSearchLimit(limit);
		controller.setSearchesGlobal(global);
		
		mActivity.runOnUiThread(new Runnable() {			
			@Override
			public void run() {	
				controller.searchByLogin(login);
			}
		});
	}
	
	public static void usersControllerSearchByEmail(final UsersController controller, final String email, int search_operator, int limit, boolean global) {
		
		switch (search_operator) {
		case 0:
			controller.setSearchOperator(LoginSearchOperator.PREFIX);
			break;
		case 1:
			controller.setSearchOperator(LoginSearchOperator.EXACT_MATCH);
		}
		controller.setSearchLimit(limit);
		controller.setSearchesGlobal(global);
		
		mActivity.runOnUiThread(new Runnable() {			
			@Override
			public void run() {	
				controller.searchByEmail(email);
			}
		});
	}
	
	public static void scheduleAchievementsSynchronization() {
		if (mShouldSynchronizeAchievements == false) {
			mShouldSynchronizeAchievements = true;
    		SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(mActivity);
    		SharedPreferences.Editor editor = prefs.edit();
    		editor.putBoolean("scoreloop_should_synchronize_achievements", true);
    		editor.commit();
		}
	}
	
	public static AchievementsController achievementsControllerCreate(final long callback_address) {

		return new AchievementsController(new RequestControllerObserver() {			
			@Override
			public void requestControllerDidFail(final RequestController controller, Exception exception) {	
				mGLSurfaceView.queueEvent(new Runnable() {
		            @Override
		            public void run() {
		            	nativeRequestComplete(controller, -1, callback_address);
		            }
		        });										
			}
	
			@Override
			public void requestControllerDidReceiveResponse(final RequestController controller) {
				mGLSurfaceView.queueEvent(new Runnable() {
		            @Override
		            public void run() {
		            	final AchievementsController achievementsController = (AchievementsController)controller;
		            	if (achievementsController.getForceInitialSync() == true) {
		            		mActivity.runOnUiThread(new Runnable() {			
		            			@Override
		            			public void run() {	
				            		List<Achievement> achievements = achievementsController.getAchievements();
				            		for (Achievement achievement : achievements) {
				            			if (achievement.needsSubmit()) {
				            				if (mAchievementController == null)
				            					mAchievementController = new AchievementController(new RequestControllerObserver() {
				            						@Override
				            						public void requestControllerDidReceiveResponse(RequestController controller) {
				            						}
				            						
				            						@Override
				            						public void requestControllerDidFail(RequestController controller, Exception exception) {					
				            						}
				            					});
				            			
				            				mAchievementController.setAchievement(achievement);
				            				mAchievementController.submitAchievement();
				            			}
				            		}
		            			}
		            		});
		            		
		            		if (mShouldSynchronizeAchievements == true) {
			            		mShouldSynchronizeAchievements = false;
			            		SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(mActivity);
			            		SharedPreferences.Editor editor = prefs.edit();
			            		editor.putBoolean("scoreloop_should_synchronize_achievements", false);
			            		editor.commit();
			            	}
		            	}		         
		            	
		            	nativeRequestComplete(controller, 0, callback_address);
		            }
		        });				
			}
		});
	}
	
	public static boolean achievementsControllerShouldSynchronizeAchievements() {
		return mShouldSynchronizeAchievements;
	}
	
	public static void achievementsControllerLoadAchievements(final AchievementsController controller) {
		mActivity.runOnUiThread(new Runnable() {			
			@Override
			public void run() {					
				controller.loadAchievements();
			}
		});
	}
	
	public static RankingController rankingControllerCreate(final long callback_address) {

		return new RankingController(new RequestControllerObserver() {			
			@Override
			public void requestControllerDidFail(final RequestController controller, Exception exception) {	
				mGLSurfaceView.queueEvent(new Runnable() {
		            @Override
		            public void run() {
		            	nativeRequestComplete(controller, -1, callback_address);
		            }
		        });										
			}
	
			@Override
			public void requestControllerDidReceiveResponse(final RequestController controller) {
				mGLSurfaceView.queueEvent(new Runnable() {
		            @Override
		            public void run() {
		            	nativeRequestComplete(controller, 0, callback_address);
		            }
		        });				
			}
		});
	}
	
	public static void rankingControllerLoadRankingForScore(final RankingController controller, final Score score) {
		mActivity.runOnUiThread(new Runnable() {			
			@Override
			public void run() {
				controller.loadRankingForScore(score);
			}
		});
	}
	
	public static void rankingControllerLoadRankingForUserInMode(final RankingController controller, final User user, final int mode) {
		mActivity.runOnUiThread(new Runnable() {			
			@Override
			public void run() {
				controller.loadRankingForUserInGameMode(user, mode);
			}
		});
	}
	public static void initialize(final long callback_address) {	
		TermsOfService tos = Session.getCurrentSession().getUsersTermsOfService();
		TermsOfService.Status status = tos.getStatus();
		if (status != TermsOfService.Status.ACCEPTED) {
			final TermsOfServiceController controller = new TermsOfServiceController(
				new TermsOfServiceControllerObserver() {
					@Override
					public void termsOfServiceControllerDidFinish(final TermsOfServiceController controller, final Boolean accepted) {
						if (accepted != null && accepted.booleanValue() == true)
							mGLSurfaceView.queueEvent(new Runnable() {
					            @Override
					            public void run() {
					            	nativeTOSAccepted(callback_address);
					            }
							});
		    	    }
		    	 });
			mActivity.runOnUiThread(new Runnable() {			
				@Override
				public void run() {
					controller.query(mActivity);
				}
			});
		}
		else
			mGLSurfaceView.queueEvent(new Runnable() {
	            @Override
	            public void run() {
	            	nativeTOSAccepted(callback_address);
	            }
	        });	
			
	}
	
	public static boolean isInitialized() {
		TermsOfService tos = Session.getCurrentSession().getUsersTermsOfService();
		TermsOfService.Status status = tos.getStatus();		
		return status == TermsOfService.Status.ACCEPTED;
	}
	
	private static native void nativeTOSAccepted(long callback_address);
	private static native void nativeRequestComplete(RequestController controller, int error_code, long callback_address);
}
