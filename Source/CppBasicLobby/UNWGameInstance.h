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
	/*�����������ʱ��������Ҫ�����ǵĴ���*/
	UUNWGameInstance();
public:
		/*
		* ����һ������ĺ���
		* @Param UserID �Ǹ��û����������
		* @Param SessionName ������
		* @Param bIsLAN �Ƿ������
		* @Param bIsPresence �Ƿ񷿼��Ѿ�����
		* @Param MaxNumPlayers ������������
		*/
		bool HostSession(TSharedPtr<const FUniqueNetId> UserID, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);
	
	/*�����Ѿ��������ʱ����õĴ�����*/
		FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;

	/*Session ��ʼ��ʱ����õĴ�����*/
		FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	/*������ ��ʼ Session�Ĵ�����*/
		FDelegateHandle OnCreateSessionCompleteDelegateHandle;
		FDelegateHandle OnStartSessionCompleteDelegateHandle;

	/*
	*�����������˴����� �ʹ��� ���� �� ��ʼ�Ĵ����� ����������Ҫһ����������Session�ı���
	* Lan MaxNumPlayers and so on
	*/
		TSharedPtr<class FOnlineSessionSettings> SessionSettings;

		/*
		* ���յ�Session������������ʱ�� �����������
		* @Param SessionName Session��ɴ�������
		* @Param bWasSuccessful true �첽������� false ���ִ���
		*/
		virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

		/*
		* ����Session�������ʱ �����������
		* @Param SessionName Session��ʼʱ������
		* @Param bWasSuccessful true �첽������� false ���ִ���
		*/
		void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);
		
		/*
		* �ҵ�һ�����ߵ�Session
		* @Param UserId ˭���������
		* @Param bIsLAN ����������
		* @Param bIsPresence �Ƿ���ڵ�Sessions
		*/
		void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);

		/*����Sessions�Ĵ���*/
		FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

		/*ע����������*/
		FDelegateHandle OnFindSessionsCompleteDelegateHandle;

		/*�������*/
		TSharedPtr<class FOnlineSessionSearch> SessionSearch;

		

		/*
		* ִ�д�������Session Search ���
		* @Param bWasSuccessful true �첽������� false �첽������������
		*/
		void OnFindSessionsComplete(bool bWasSuccessful);


		/**
		* ͨ��SearchResult ����һ��Session
		* @Param UserId ˭������������
		* @Param SessionName Session ����
		* @Param SearchResult �����Session
		* @return bool true ����ɹ� fasle ����ʧ��
		*/
		bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

		/*������ ����һ��Session*/
		FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

		/*�������Session ���֮����õĺ���������*/
		FDelegateHandle OnJoinSessionCompleteDelegateHandle;

		/*
		* ������Session���֮����ô˺���
		* @Param SessionName �����ĸ�SessionName
		* @Param bWasSuccessfun true �춯�������û�д��� false ������
		*/
		void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);


		virtual void OnDestroySessionComplete(FName SessiomName, bool bWasSuccessful);
		/*������ ����һ��Session*/
		FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

		/*ע�����ٴ����� ������һ��Session���*/
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
