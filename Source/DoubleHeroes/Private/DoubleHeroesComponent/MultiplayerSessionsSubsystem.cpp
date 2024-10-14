// Fill out your copyright notice in the Description page of Project Settings.


#include "DoubleHeroesComponent/MultiplayerSessionsSubsystem.h"

#include "Character/BlueHeroCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem():
	CreateSessionCompleteDelegate(
		FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
	FindSessionsCompleteDelegate(
		FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete)),
	DestroySessionCompleteDelegate(
		FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete))
// StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
{
	//PrintString("MSS Constructor");
	CreateServerAfterDestroy = false;
	DestroyServerName = "";
	ServerNameToFind = "";
	RoomIDToFind = "";
	MyServerName = "";
	MyRoomID = "";
	MyPassword = "";
	LastNumPublicConnections = 0;
	bIsLAN = false;
	MySessionName = FName("Co-op Adventure Session Name");
}

void UMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	//PrintString("MSS Initialize");
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		/*FString SubsystemName = OnlineSubsystem->GetSubsystemName().ToString();
		PrintString(SubsystemName);*/

		SessionInterface = OnlineSubsystem->GetSessionInterface();
		/*if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates
			                .AddUObject(this, &UMultiplayerSessionsSubsystem::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates
			                .AddUObject(this, &UMultiplayerSessionsSubsystem::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates
			                .AddUObject(this, &UMultiplayerSessionsSubsystem::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates
			                .AddUObject(this, &UMultiplayerSessionsSubsystem::OnJoinSessionComplete);
		}*/
		// MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
		// MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
		// MultiplayerOnJoinSessionComplete.AddDynamic(this, &ThisClass::OnJoinSession);
		MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		// MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
	}
}

/*
void UMultiplayerSessionsSubsystem::Deinitialize()
{
	UE_LOG(LogTemp, Warning, TEXT("MSS Deinitialize"));
}
*/

void UMultiplayerSessionsSubsystem::CreateServer(int32 ConnectionsNum, FString ServerName, FString Password, bool IsLAN)
{
	/*PrintString("CreateServer");

	if (ServerName.IsEmpty())
	{
		PrintString("Server name cannot be empty!");
		ServerCreateDel.Broadcast(false);
		return;
	}


	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(MySessionName);
	if (ExistingSession)
	{
		FString Msg = FString::Printf(
			TEXT("Session with name %s already exists, destroying it."), *MySessionName.ToString());
		PrintString(Msg);
		CreateServerAfterDestroy = true;
		DestroyServerName = ServerName;
		SessionInterface->DestroySession(MySessionName);
		return;
	}
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true;
	bool IsLAN = false;
	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		IsLAN = true;
	}
	SessionSettings.bIsLANMatch = IsLAN;

	SessionSettings.Set(FName("SERVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	//打印ServerName
	// PrintString(FString::Printf(TEXT("把ServerName存储:%s"), *ServerName));

	SessionInterface->CreateSession(0, MySessionName, SessionSettings);*/

	//判断MySessionName是否为空

	if (!SessionInterface.IsValid())
	{
		return;
	}

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(MySessionName);
	if (ExistingSession != nullptr)
	{
		// bCreateSessionOnDestroy = true;
		CreateServerAfterDestroy = true;
		NumPublicConnections = ConnectionsNum;
		MyServerName = ServerName;
		MyPassword = Password;
		bIsLAN = IsLAN;
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("ExistingSession!"));
		DestroySession();
		return; //用return防止异步回调还没执行完就开始创建了
	}
	if (CreateServerAfterDestroy == false)
	{
		MyRoomID = GenerateRoomId();
		
		// MySessionName = FName(*("SessionName_" + MyRoomID));
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("MySessionName:" + MySessionName.ToString()));

		const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
		//Store the delegate in a FDelegateHandle so we can later remove it from the delegate list
		CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
			CreateSessionCompleteDelegate);
		LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
		LastSessionSettings->bIsLANMatch = bIsLAN;
		LastSessionSettings->bIsDedicated = false;
		LastSessionSettings->NumPublicConnections = 2;
		LastSessionSettings->bAllowJoinInProgress = true;
		LastSessionSettings->bAllowJoinViaPresence = true;
		LastSessionSettings->bShouldAdvertise = true;
		LastSessionSettings->bUsesPresence = true;
		LastSessionSettings->bUseLobbiesIfAvailable = true;
		LastSessionSettings->BuildUniqueId = 1;
		LastSessionSettings->Set(FName("RoomID"), MyRoomID, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		//打印RoomID的值到屏幕
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("RoomID:%s"), *MyRoomID));
		//判断密码是否为空
		/*if(!Password.IsEmpty())
		{
			LastSessionSettings->Set(FName("Password"), Password, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		}*/
		//创建会话并返回结果
		if (!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), MySessionName,
		                                     *LastSessionSettings))
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("CreateSession failed"));
			//如果创建失败，则取消注册创建会话完成委托。OnCreateSessionComplete
			SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

			//OnCreateSession
			CreateServerDel.Broadcast(false);
		}
	}
	else
	{
		CreateServerDel.Broadcast(false);
	}
}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "Session create successfully!");
	if (bWasSuccessful)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FString Path = "/Game/Maps/Lobby";
			if (!GameMapPath.IsEmpty())
			{
				Path = FString::Printf(TEXT("%s?listen"), *GameMapPath);
			}
			
			World->ServerTravel(Path);
			// 设置旅行完成后的回调
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "Failed to create session!");
		CreateServerDel.Broadcast(bWasSuccessful);
	}
}


void UMultiplayerSessionsSubsystem::FindServer(int32 MaxSearchResults, FString RoomID, FString Password, bool IsLAN)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}
	MaxSearchResults = 10000;
	bIsLAN = IsLAN;
	FindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(
		FindSessionsCompleteDelegate);
	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = 10000;
	LastSessionSearch->bIsLanQuery = bIsLAN;
	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	RoomIDToFind = RoomID;

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("FindSessions failed"));
		//OnFindSessionsComplete
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);

		JoinServerDel.Broadcast(false);
	}
}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	// 检查 LastSessionSearch 是否有效
	if (!LastSessionSearch.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("LastSessionSearch is not valid"));
		JoinServerDel.Broadcast(false);
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "Session find Complete!");
	if (SessionInterface)
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	TArray<FOnlineSessionSearchResult> SessionResults = LastSessionSearch->SearchResults;
	
	if (!bWasSuccessful || SessionResults.Num() <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("Failed to find session"));
		JoinServerDel.Broadcast(false);
	}
	else
	{
		FOnlineSessionSearchResult* CorrectResult = nullptr;
		for (FOnlineSessionSearchResult Result : SessionResults)
		{
			if (Result.IsValid())
			{
				FString SettingValue = "RoomID";
				Result.Session.SessionSettings.Get(FName("RoomID"), SettingValue);
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
				                                 FString::Printf(TEXT("RoomID:%s"), *SettingValue));
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
				                                 FString::Printf(TEXT("RoomIDToFind:%s"), *RoomIDToFind));
				if (!SettingValue.IsEmpty())
				{
					if (SettingValue.Equals(RoomIDToFind))
					{
						// JoinServer(FName(*("SessionName_" + SettingValue)),Result);
						JoinServer(MySessionName, Result);
						// CorrectResult = &Result;
						break;
					}
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("Result is not valid"));
			}
		}
		/*if (CorrectResult)
		{
			JoinServer(MySessionName, *CorrectResult);
		}
		else
		{
			ServerNameToFind = "";
			JoinServerDel.Broadcast(false);
		}*/
		RoomIDToFind = "";
		JoinServerDel.Broadcast(false);
	}
	//OnFindSessions
	// MultiplayerOnFindSessionsComplete.Broadcast(SessionSearch->SearchResults, bWasSuccessful);
}

/*void UMultiplayerSessionsSubsystem::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	for (auto Result : SessionResults)
	{
		FString SettingValue;
		Result.Session.SessionSettings.Get(FName("RoomID"), SettingValue);
		if (SettingValue == RoomID)
		{
			JoinServer(Result);
			return;
		}
	}
	/*if (!bWasSuccessful || SessionResults.Num() == 0)
	{
		JoinButton->SetIsEnabled(true);
	}#1#
}*/


/*void UMultiplayerSessionsSubsystem::FindServer(FString ServerName)
{
	PrintString("FindingServer");

	if (ServerName.IsEmpty())
	{
		PrintString("Server name cannot be empty");
		ServerJoinDel.Broadcast(false);
		return;
	}

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	bool IsLAN = false;
	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		IsLAN = true;
	}
	SessionSearch->bIsLanQuery = IsLAN;
	SessionSearch->MaxSearchResults = 9999;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	ServerNameToFind = ServerName;
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}*/

/*void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool WasSuccessful)
{
	PrintString(FString::Printf(TEXT("OnCreateSessionComplete: %d"), WasSuccessful));

	ServerCreateDel.Broadcast(WasSuccessful);

	if (WasSuccessful)
	{
		FString Path = "/Game/ThirdPerson/Maps/ThirdPersonMap";

		if (!GameMapPath.IsEmpty())
		{
			Path = FString::Printf(TEXT("%s?listen"), *GameMapPath);
		}

		GetWorld()->ServerTravel(Path);
	}
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool WasSuccessful)
{
	FString Msg = FString::Printf(
		TEXT("OnDestroySessionComplete, SessionName: %s, Success: %d"), *SessionName.ToString(), WasSuccessful);
	PrintString(Msg);

	if (CreateServerAfterDestroy)
	{
		CreateServerAfterDestroy = false;
	}

	CreateServer(2, DestroyServerName);
}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool WasSuccessful)
{
	if (!WasSuccessful)
	{
		return;
	}
	if (ServerNameToFind.IsEmpty())
	{
		return;
	}

	TArray<FOnlineSessionSearchResult> Results = SessionSearch->SearchResults;
	FOnlineSessionSearchResult* CorrectResult = nullptr;
	if (Results.Num() > 0)
	{
		FString Msg = FString::Printf(TEXT("%d sessions found."), Results.Num());
		PrintString(Msg);

		for (FOnlineSessionSearchResult Result : Results)
		{
			if (Result.IsValid())
			{
				FString ServerName = "No-name";
				Result.Session.SessionSettings.Get(FName("SERVER_NAME"), ServerName);

				PrintString(FString::Printf(TEXT("获取的ServerName:%s"), *ServerName));
				if (ServerName.Equals(ServerNameToFind))
				{
					CorrectResult = &Result;
					FString Msg2 = FString::Printf(TEXT("相等的ServerName:%s"), *ServerName);
					PrintString(Msg2);
					break;
				}
			}
		}

		if (CorrectResult)
		{
			PrintString(FString::Printf(TEXT("正在加入")));
			SessionInterface->JoinSession(0, MySessionName, *CorrectResult);
		}
		else
		{
			PrintString(FString::Printf(TEXT("Coundn't find server with name: %s"), *ServerNameToFind));
			ServerNameToFind = "";
			ServerJoinDel.Broadcast(false);
		}
	}
	else
	{
		PrintString("Zero sessions found.");
		ServerJoinDel.Broadcast(false);
	}
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	ServerJoinDel.Broadcast(Result == EOnJoinSessionCompleteResult::Success);

	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		FString Msg = FString::Printf(TEXT("Successfully joined session %s"), *SessionName.ToString());
		PrintString(Msg);

		FString Address = "";
		bool Success = SessionInterface->GetResolvedConnectString(MySessionName, Address);
		if (Success)
		{
			PrintString(FString::Printf(TEXT("Address: %s"), *Address));
			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(Address, TRAVEL_Absolute);
			}
		}
		else
		{
			PrintString("GetResolvedConnectString returned false!");
		}
	}
	else
	{
		PrintString("OnJoinSessionComplete failed");
	}
}*/

/*void UMultiplayerSessionsSubsystem::OnCreateSession(bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "Session create successfully!");
	if (bWasSuccessful)
	{
		UWorld* World = GetWorld();
		if(World)
		{
			World->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap");
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "Failed to create session!");
	}
}*/

void UMultiplayerSessionsSubsystem::JoinServer(FName SessionName, const FOnlineSessionSearchResult& SessionResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "JoinServer");
	if (!SessionInterface.IsValid())
	{
		JoinServerDel.Broadcast(false);
		return;
	}

	JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(
		JoinSessionCompleteDelegate);
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), MySessionName, SessionResult))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "Join Failed!");
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

		//
		JoinServerDel.Broadcast(false);
	}
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "OnJoinSessionComplete");
	if (SessionInterface)
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		FString Address;
		bool Success = SessionInterface->GetResolvedConnectString(MySessionName, Address);
		if (Success)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("Address: ") + Address);
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "JoinSessionName:" + SessionName.ToString());
			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "ClientTravel");
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "Failed to get connect string");
		}
	}
	JoinServerDel.Broadcast(Result == EOnJoinSessionCompleteResult::Success);
}

void UMultiplayerSessionsSubsystem::OnDestroySession(bool bWasSuccessful)
{
}

void UMultiplayerSessionsSubsystem::DestroySession()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "DestroySession");
	if (!SessionInterface.IsValid())
	{
		MultiplayerOnDestroySessionComplete.Broadcast(false);
		return;
	}
	SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

	//销毁
	if (!SessionInterface->DestroySession(MySessionName))
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
		MultiplayerOnDestroySessionComplete.Broadcast(false);
	}
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName,
                                                             bool bWasSuccessful)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}
	if (bWasSuccessful && CreateServerAfterDestroy)
	{
		CreateServerAfterDestroy = false;
		CreateServer(NumPublicConnections, MyServerName, MyPassword, bIsLAN);
	}
}

FString UMultiplayerSessionsSubsystem::GenerateRoomId()
{
	// 获取当前时间戳
	time_t now = std::time(nullptr);
	unsigned int seed = static_cast<unsigned int>(now);

	// 生成随机数
	srand(seed);
	int randomNum = rand() % 1000;

	// 构造房间 ID
	FString RoomId = FString::Printf(TEXT("%lu%d"), now, randomNum);

	return RoomId;
}
