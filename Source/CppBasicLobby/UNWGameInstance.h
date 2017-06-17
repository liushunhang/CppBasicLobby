// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"

#include "UNWGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CPPBASICLOBBY_API UUNWGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	/*当对象产生的时候我们需要绑定我们的代理*/
	UUNWGameInstance();
public:
		/*
		* 创建一个房间的函数
		* @Param UserID 那个用户发起的请求
		* @Param SessionName 房间名
		* @Param bIsLAN 是否局域网
		* @Param bIsPresence 是否房间已经存在
		* @Param MaxNumPlayers 房间最大玩家数
		*/
		bool HostSession(TSharedPtr<const FUniqueNetId> UserID, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);
	
	/*房间已经创建完成时候调用的代理函数*/
		FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;

	/*Session 开始的时候调用的代理函数*/
		FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	/*处理创建 开始 Session的代理函数*/
		FDelegateHandle OnCreateSessionCompleteDelegateHandle;
		FDelegateHandle OnStartSessionCompleteDelegateHandle;

	/*
	*现在我们有了代理函数 和处理 创建 和 开始的处理器 现在我们需要一个设置我们Session的变量
	* Lan MaxNumPlayers and so on
	*/
		TSharedPtr<class FOnlineSessionSettings> SessionSettings;

		/*
		* 当收到Session创建完成请求的时候 调用这个函数
		* @Param SessionName Session完成创建名称
		* @Param bWasSuccessful true 异步请求完成 false 出现错误
		*/
		virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

		/*
		* 启动Session请求完成时 调用这个函数
		* @Param SessionName Session开始时的名称
		* @Param bWasSuccessful true 异步请求完成 false 出现错误
		*/
		void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);
		
		/*
		* 找到一个在线的Session
		* @Param UserId 谁发起的请求
		* @Param bIsLAN 局域网搜索
		* @Param bIsPresence 是否存在的Sessions
		*/
		void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);

		/*查找Sessions的代理*/
		FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

		/*注册搜索代理*/
		FDelegateHandle OnFindSessionsCompleteDelegateHandle;

		/*搜索结果*/
		TSharedPtr<class FOnlineSessionSearch> SessionSearch;

		

		/*
		* 执行代理函数当Session Search 完成
		* @Param bWasSuccessful true 异步搜索完成 false 异步搜索发生错误
		*/
		void OnFindSessionsComplete(bool bWasSuccessful);


		/**
		* 通过SearchResult 加入一个Session
		* @Param UserId 谁发起的这个请求
		* @Param SessionName Session 名字
		* @Param SearchResult 加入的Session
		* @return bool true 加入成功 fasle 否则失败
		*/
		bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

		/*代理函数 加入一个Session*/
		FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

		/*处理加入Session 完成之后调用的函数处理器*/
		FDelegateHandle OnJoinSessionCompleteDelegateHandle;

		/*
		* 当加入Session完成之后调用此函数
		* @Param SessionName 加入哪个SessionName
		* @Param bWasSuccessfun true 异动操作完成没有错误 false 错误发生
		*/
		void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);


		virtual void OnDestroySessionComplete(FName SessiomName, bool bWasSuccessful);
		/*代理函数 销毁一个Session*/
		FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

		/*注册销毁代理函数 当销毁一个Session完成*/
		FDelegateHandle OnDestroySessionCompleteDelegateHandle;


		/*UFUNCTION  BP_call*/

		/*Create Session*/
		UFUNCTION(BlueprintCallable, Category = "Network|Test")
			void StartOnlineGame();

		/*Search and Finding Session*/
		UFUNCTION(BlueprintCallable, Category = "Network|Test")
			void FindOnlineGames();

		/*Join Session*/
		UFUNCTION(BlueprintCallable, Category = "Network|Test")
			void JoinOnlineGame();

		/*Destroy Session*/
		UFUNCTION(BlueprintCallable, Category = "Network|Test")
			void DestroySessionAndLeaveGame();

};
