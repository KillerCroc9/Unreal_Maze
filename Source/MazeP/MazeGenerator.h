// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeGenerator.generated.h"

UCLASS()
class MAZEP_API AMazeGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMazeGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void GenerateAndPrintMaze();
	void SpawnActorAtLocation(TSubclassOf<AActor> ActorClass, const FVector& SpawnLocation);
	UPROPERTY(EditDefaultsOnly, Category = "Maze")
	TSubclassOf<AActor> IntersectionWall;
	
	UPROPERTY(EditDefaultsOnly, Category = "Maze")
	TSubclassOf<AActor> SideWalls;
	
	UPROPERTY(EditDefaultsOnly, Category = "Maze")
	TSubclassOf<AActor> CentreWalls;

	

	//UPROPERTY()
	TArray<TArray<char>> MazeArray;


};