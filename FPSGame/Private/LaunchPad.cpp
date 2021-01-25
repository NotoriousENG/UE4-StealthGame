// Fill out your copyright notice in the Description page of Project Settings.


#include "LaunchPad.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values
ALaunchPad::ALaunchPad()
{
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(120.0f, 120.0f, 500.0f));
	OverlapComp->SetupAttachment(RootComponent);

	OverlapComp->SetHiddenInGame(false);

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPad::HandleOverlap);

	LaunchPitchAngle = 35.0f;
	LaunchStrength = 1500.0f;
}

void ALaunchPad::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch += LaunchPitchAngle;
	FVector LaunchVelocity = LaunchDirection.Vector() * LaunchStrength;
	
	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	if (OtherCharacter)
	{
		OtherCharacter->LaunchCharacter(LaunchVelocity, true, true);
		PlayEffects();
		UE_LOG(LogTemp, Log, TEXT("Player"));
	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);
		PlayEffects();
		UE_LOG(LogTemp, Log, TEXT("Component"));
	}
}

void ALaunchPad::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, LaunchFX, GetActorLocation());
}

