// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

UMultiplayerSessionSubsystem::UMultiplayerSessionSubsystem()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();
	}

	CreateCompleteSessionDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete);
	FindSessionCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionComplete);
	JoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete);
	DestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete);
	StartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete);
}

void UMultiplayerSessionSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr)
	{
		SessionInterface->DestroySession(NAME_GameSession);
	}

	//Store the delegate in a FDelegate list so we cam later remove it from the delegate list
	CreateCompleteSessionDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateCompleteSessionDelegate);
	
	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	
	if (LastSessionSettings.IsValid())
	{
		LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "LAN" ? true : false;
		LastSessionSettings->NumPublicConnections = NumPublicConnections;
		LastSessionSettings->bAllowJoinInProgress = true;
		LastSessionSettings->bAllowJoinViaPresence = true;
		LastSessionSettings->bShouldAdvertise = true;
		LastSessionSettings->bUsesPresence = true;
		LastSessionSettings->bUseLobbiesIfAvailable = true;
		LastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		LastSessionSettings->BuildUniqueId = 1;
		
		const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
		if (LocalPlayer != nullptr)
		{
			if (!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
			{
				SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateCompleteSessionDelegateHandle);

				//Bind our own custom delegates
				MultiplayerOnCreateSessionComplete.Broadcast(false);
			}
		}
	}
}

void UMultiplayerSessionSubsystem::FindSessions(int32 MaxSearchResult)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	FindSessionCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (LastSessionSearch.IsValid())
	{
		LastSessionSearch->MaxSearchResults = MaxSearchResult;
		LastSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "LAN" ? true : false;
		LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		
		const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
		if (!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
		{
			SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegateHandle);
			MultiplayerOnFindSessionComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		}
	}
}

void UMultiplayerSessionSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult)
{
	if (!SessionInterface.IsValid())
	{
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

void UMultiplayerSessionSubsystem::DestroySession()
{
}

void UMultiplayerSessionSubsystem::StartSession()
{
}

void UMultiplayerSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateCompleteSessionDelegateHandle);
	}

	MultiplayerOnCreateSessionComplete.Broadcast(bWasSuccessful);
}

void UMultiplayerSessionSubsystem::OnFindSessionComplete(bool bWasSuccessful)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegateHandle);
	}

	if (LastSessionSearch->SearchResults.Num() <= 0)
	{
		MultiplayerOnFindSessionComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	MultiplayerOnFindSessionComplete.Broadcast(LastSessionSearch->SearchResults, bWasSuccessful);
}

void UMultiplayerSessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	MultiplayerOnJoinSessionComplete.Broadcast(Result);
}

void UMultiplayerSessionSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
}

void UMultiplayerSessionSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
}