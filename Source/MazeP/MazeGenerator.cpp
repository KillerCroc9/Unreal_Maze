// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeGenerator.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeGen.h"
#include "TimerManager.h"

// Sets default values
AMazeGenerator::AMazeGenerator()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMazeGenerator::BeginPlay()
{
    Super::BeginPlay();
    FTimerHandle Handle;

    GetWorld()->GetTimerManager().SetTimer(Handle,this,&AMazeGenerator::GenerateAndPrintMaze,5.0f);
    // Call the function to generate and print the maze
   // GenerateAndPrintMaze();
}

// Called every frame
void AMazeGenerator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}
void AMazeGenerator::GenerateAndPrintMaze()
{
    //     // Generate the maze

    // // Print the maze to the log
     //MazeString= MazeGen::Mazer();
    float time = 0.1f;
    int Pillar = 0;
    int Path = 0;
    MazeGen maze;
    MazeArray = maze.MazeGrid();
    FVector CellSize(400.0f, 400.0f, 400.0f); // Adjust as needed
    
    

    for (int32 Row = 0; Row < MazeArray.Num(); Row++)
    {
        for (int32 Column = 0; Column < MazeArray[Row].Num(); Column++)
        {
            char CellValue = MazeArray[Row][Column];
            AActor* SpawnedActor = nullptr;
            if (CellValue == '+' && Pillar %2==0)
            {
                // Spawn an intersection wall at the current location
                FVector SpawnLocation = FVector(Column * CellSize.X, Row * CellSize.Y, 200.0f); // Adjust Z as needed
                // Create a lambda function to handle the actor spawning
                FTimerDelegate TimerDelegate;
                TimerDelegate.BindLambda([this, SpawnLocation]() {
                    SpawnActorAtLocation(IntersectionWall, SpawnLocation);
                    });
                FTimerHandle Handle;

                GetWorld()->GetTimerManager().SetTimer(Handle,TimerDelegate, time,false);
                Pillar += 1;
                //SpawnedActor = GetWorld()->SpawnActor<AActor>(IntersectionWall, SpawnLocation, FRotator::ZeroRotator);

            }
            else if (CellValue == '+' && Pillar % 1 == 0)
            {
                // Spawn an intersection wall at the current location
                FVector SpawnLocation = FVector(Column * CellSize.X, Row * CellSize.Y, 200.0f); // Adjust Z as needed
                // Create a lambda function to handle the actor spawning
                FTimerDelegate TimerDelegate;
                TimerDelegate.BindLambda([this, SpawnLocation]() {
                    SpawnActorAtLocation(IntersectionWallOdd, SpawnLocation);
                    });
                FTimerHandle Handle;

                GetWorld()->GetTimerManager().SetTimer(Handle, TimerDelegate, time, false);
                Pillar += 1;
                //SpawnedActor = GetWorld()->SpawnActor<AActor>(IntersectionWall, SpawnLocation, FRotator::ZeroRotator);

            }

            else if (CellValue == '-')
            {
                // Spawn a horizontal wall at the current location
                FVector SpawnLocation = FVector(Column * CellSize.X, Row * CellSize.Y, 200.0f); // Adjust Z as needed
                
                // Create a lambda function to handle the actor spawning
                FTimerDelegate TimerDelegate;

                TimerDelegate.BindLambda([this, SpawnLocation]() {
                    SpawnActorAtLocation(SideWalls, SpawnLocation);
                    });
                FTimerHandle Handle;

                GetWorld()->GetTimerManager().SetTimer(Handle, TimerDelegate, time, false);
                
                //SpawnedActor = GetWorld()->SpawnActor<AActor>(SideWalls, SpawnLocation, FRotator::ZeroRotator);
            }
            else if (CellValue == '|')
            {
                // Spawn a vertical wall at the current location
                FVector SpawnLocation = FVector(Column * CellSize.X, Row * CellSize.Y, 200.0f); // Adjust Z as needed
                
                // Create a lambda function to handle the actor spawning
                FTimerDelegate TimerDelegate;

                TimerDelegate.BindLambda([this, SpawnLocation]() {
                    SpawnActorAtLocation(CentreWalls, SpawnLocation);
                    });
                FTimerHandle Handle;

                GetWorld()->GetTimerManager().SetTimer(Handle, TimerDelegate, time, false);
                
                //SpawnedActor = GetWorld()->SpawnActor<AActor>(CentreWalls, SpawnLocation, FRotator::ZeroRotator);
            }
            
          /*  else if (CellValue == 'X' && Path % 2 == 1) {
                Path += 1;
            }*/

            time += 0.005f;
        }
    }
    for (int32 Row = 0; Row < MazeArray.Num(); Row++)
    {
        for (int32 Column = 0; Column < MazeArray[Row].Num(); Column++)
        {
            char CellValue = MazeArray[Row][Column];
            if (CellValue == 'X' /*&& Path % 2 == 0*/) {
                FVector SpawnLocation = FVector(Column * CellSize.X, Row * CellSize.Y, 250.0f); // Adjust Z as needed

                //Create a lambda function to handle the actor spawning
                FTimerDelegate TimerDelegate;

                TimerDelegate.BindLambda([this, SpawnLocation]() {
                    SpawnActorAtLocation(Light, SpawnLocation);
                    });
                FTimerHandle Handle;

                GetWorld()->GetTimerManager().SetTimer(Handle, TimerDelegate, time, false);
                Path += 1;
                PathLocation.Add(SpawnLocation);
                time += 0.1f;

            }
        }

    }

    FVector SpawnLocation = FVector(0 * CellSize.X, 0 * CellSize.Y, 250.0f); // Adjust Z as needed

    //Create a lambda function to handle the actor spawning
    FTimerDelegate TimerDelegate;

    TimerDelegate.BindLambda([this, SpawnLocation]() {
        SpawnActorAtLocation(Camera, SpawnLocation);
        });
    FTimerHandle Handle;

    GetWorld()->GetTimerManager().SetTimer(Handle, TimerDelegate, time, false);
    Path += 1;
            
            time += 0.01f;

}

void AMazeGenerator::SpawnActorAtLocation(TSubclassOf<AActor> ActorClass, const FVector& SpawnLocation)
{

        AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass, SpawnLocation, FRotator::ZeroRotator);
    

}



