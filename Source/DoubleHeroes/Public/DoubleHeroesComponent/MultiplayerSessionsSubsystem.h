// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "MultiplayerSessionsSubsystem.generated.h"

/*DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerCreateDelegate, bool, WasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerJoinDelegate, bool, WasSuccessful);*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool, bWasSuccessful);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnStartSessionComplete, bool, bWasSuccessful);


/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UMultiplayerSessionsSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	// virtual void Deinitialize() override;

	//创建CreateSession方法,传入参数为服务器名称、最大连接数、匹配类型
	UFUNCTION(BlueprintCallable)
	void CreateServer(int32 ConnectionsNum, FString ServerName, FString Password, bool bIsLAN);
	UFUNCTION(BlueprintCallable)
	void FindServer(int32 MaxSearchResults, FString RoomID, FString Password, bool bIsLAN);
	void JoinServer(FName SessionName, const FOnlineSessionSearchResult& SessionResult);
	void DestroySession();
	// void StartSession();

	// UFUNCTION(BlueprintCallable)
	// void FindServer(FString ServerName);
	
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	// void OnJoinSession(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION()
	FString GenerateRoomId();

	//自定义委托回调
	UPROPERTY(BlueprintAssignable)
	FMultiplayerOnCreateSessionComplete CreateServerDel;
	UPROPERTY(BlueprintAssignable)
	FMultiplayerOnJoinSessionComplete JoinServerDel;
	FMultiplayerOnDestroySessionComplete MultiplayerOnDestroySessionComplete;
	// FMultiplayerOnStartSessionComplete MultiplayerOnStartSessionComplete;
	
	/*void OnCreateSessionComplete(FName SessionName, bool WasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool WasSuccessful);
	void OnFindSessionsComplete(bool WasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);*/

	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;
	// FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	// FDelegateHandle StartSessionCompleteDelegateHandle;

	bool CreateServerAfterDestroy{false};

	

	/*UPROPERTY(BlueprintAssignable)
	FServerCreateDelegate ServerCreateDel;

	UPROPERTY(BlueprintAssignable)
	FServerJoinDelegate ServerJoinDel;*/

	/*UPROPERTY(BlueprintAssignable)
	FMultiplayerOnCreateSessionComplete ServerCreateDel;

	UPROPERTY(BlueprintAssignable)
	FMultiplayerOnJoinSessionComplete ServerJoinDel;*/

	UPROPERTY(BlueprintReadWrite)
	FString GameMapPath;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

private:
	FString DestroyServerName;
	FString ServerNameToFind;
	FName MySessionName;
	int32 NumPublicConnections{4};
	FString MyServerName;
	FString MyRoomID;
	FString MyPassword;
	bool bIsLAN;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	// bool bCreateSessionOnDestroy{false};
	int32 LastNumPublicConnections;
	FString RoomIDToFind;
	FString LastPassword;
	
};
