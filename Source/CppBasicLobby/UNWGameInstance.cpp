// Fill out your copyright notice in the Description page of Project Settings.

#include "CppBasicLobby.h"
#include "UNWGameInstance.h"

UUNWGameInstance::UUNWGameInstance()
{
	/*绑定函数 当创建Session的时候*/
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UUNWGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UUNWGameInstance::OnStartOnlineGameComplete);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UUNWGameInstance::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UUNWGameInstance::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UUNWGameInstance::OnDestroySessionComplete);
}


bool UUNWGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserID, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
	//获取 OnlineSubsystem
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		//获取Session Interface 然后调用CreateSession
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid() && UserID.IsValid())
		{
			//填充SessionSetting 结构
			//地图或者模式类型
			SessionSettings = MakeShareable(new FOnlineSessionSettings());
			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;
			SessionSettings->Set(SETTING_MAPNAME, FString("NewMap"), EOnlineDataAdvertisementType::ViaOnlineService);

			//设置代理处理 SessionInterface
			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			//代理函数在CreateSession执行完成之后就会执行 不需要CreateSession函数执行成功
			return Sessions->CreateSession(*UserID, SessionName, *SessionSettings);

		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No OnlineSubsystem found!"));
	}
	return false;
}

void UUNWGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnCreateSessionComplete %s,%d"), *SessionName.ToString(), bWasSuccessful));
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		//获取Session Interface 调用StartSession
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			//清楚SessionComplete Delegate handle 当我们结束调用
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				//设置开启Session 代理
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
				//我们的StartSessionComplete 之后应该调用
				Sessions->StartSession(SessionName);
			}
		}
	}
}
void UUNWGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnStartSessionComplete %s %d"), *SessionName.ToString(), bWasSuccessful));
	
	//获取在线子系统
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		//同理 完成也要清理
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);

		}
		if (bWasSuccessful)
		{
			UGameplayStatics::OpenLevel(GetWorld(), "NewMap", true, "listen");

		}
	}
}

void UUNWGameInstance::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence)
{
	//获取在线子系统
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		//获取SessionsInterface
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid() && UserId.IsValid())
		{
			//填充SearchSettings
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 20;
			SessionSearch->PingBucketSize = 50;
			//当存在这个Session的时候 填充查询结果
			if (bIsPresence)
			{
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
			}
			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();
			//设置代理处理函数 绑定这个我们的代理
			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
			Sessions->FindSessions(*UserId, SearchSettingsRef);
		}
	}
	else
	{
		//错误处理
		OnFindSessionsComplete(false);
	}
	

}

void UUNWGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("OnFindSessionsComplete bSuccess: %d"), bWasSuccessful));
	//获取在线子系统
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		//获取SessionsInterface
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			//同理清楚 清理代理
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
			//Debug显示找到的Session
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num()));
			//找到了至少一个结果
			if (SessionSearch->SearchResults.Num() > 0)
			{
				for (int32 SearchIdx=0; SearchIdx< SessionSearch->SearchResults.Num();SearchIdx++)
				{
					//OwningUserName 是现在真正的SessionName 你可以创建你自己的服务器设置 和 游戏Sessionclass 服务器名字就是这个
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("SessionNumber : %d | SessionName : %s"), SearchIdx, *(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName)));
				}
			}
		}
	}

}
bool UUNWGameInstance::JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult)
{
	bool bSuccessful = false;
	//获取在线子系统
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		//获取Session Interface 
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
			bSuccessful = Sessions->JoinSession(*UserId, SessionName, SearchResult);
		}
	}
	return bSuccessful;
}
void UUNWGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<int32>(Result)));
	
	//获取在线子系统
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		//获取Session Interface 
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			//同理 清理 Join delegate
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			//我们要获取本地第一个PlayerController 我们调用ClientTravel 去到Servermap
			APlayerController* const PlayerController = GetFirstLocalPlayerController();

			//我们需要一个FString 类型的字符串 传递进ClientTravel 我们使用SessionInterface 构造这么一个字符串
			//因为每个不同的在线子系统有不同的Travel URLs
			FString TravelURL;
			if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
			{
				PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void UUNWGameInstance::OnDestroySessionComplete(FName SessiomName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnDestroySessionComplete %s, %d"), *SessiomName.ToString(), bWasSuccessful));

	//获取在线子系统
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		//获取Session Interface
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			//同理 清楚 Destroy Delegate
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				UGameplayStatics::OpenLevel(GetWorld(), "ThirdPersonExampleMap", true);
			}
		}
	}
}

void UUNWGameInstance::StartOnlineGame()
{
	//获取 UserId
	ULocalPlayer* const Player = GetFirstGamePlayer();
	// 调用我们的cpp 函数
	HostSession(Player->GetPreferredUniqueNetId(), GameSessionName, true, true, 4);
}

void UUNWGameInstance::FindOnlineGames()
{
	ULocalPlayer* const Player = GetFirstGamePlayer();
	FindSessions(Player->GetPreferredUniqueNetId(), true, true);
}

void UUNWGameInstance::JoinOnlineGame()
{
	ULocalPlayer* const Player = GetFirstGamePlayer();

	//搜索结果 这里只有一个 可是我们可能会搜索到多个
	FOnlineSessionSearchResult SearchResult;
	if (SessionSearch->SearchResults.Num() > 0)
	{
		for (int32 i=0;i<SessionSearch->SearchResults.Num();i++)
		{
			//防止这个Session是我们自己
			if (SessionSearch->SearchResults[i].Session.OwningUserId != Player->GetPreferredUniqueNetId())
			{
				SearchResult = SessionSearch->SearchResults[i];
				JoinSession(Player->GetPreferredUniqueNetId(), GameSessionName, SearchResult);
				break;
			}
		}
	}
}

void UUNWGameInstance::DestroySessionAndLeaveGame()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
			Sessions->DestroySession(GameSessionName);
		}
	}
}
