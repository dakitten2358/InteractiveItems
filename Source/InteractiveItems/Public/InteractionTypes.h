#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "InteractionTypes.generated.h"

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
	Instant,
	Duration
};

UENUM(BlueprintType)
enum class EInteractionStimulusType : uint8
{
	Show,
	Update,
	Hide,
};

UENUM(BlueprintType)
enum class EInteractionTraceMethod : uint8
{
	Camera,
	CameraAndPawn,
	Pawn,
};

USTRUCT(BlueprintType)
struct INTERACTIVEITEMS_API FInteractionStimulus
{
	GENERATED_USTRUCT_BODY()

	enum FResult
	{
		SensingSucceeded,
		SensingFailed
	};

public:
	FInteractionStimulus();
	FInteractionStimulus(TWeakObjectPtr<class AActor> InteractionTargetActor, EInteractionStimulusType InteractionType, float InteractionProgress, FResult Result = SensingSucceeded);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<class AActor> InteractionTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector InteractionLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D InteractionPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EInteractionStimulusType InteractionType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InteractionProgress;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint32 bIsInteractionAvailable : 1;

private:
	void GetInteractionLocation(TWeakObjectPtr<class AActor> ForActor, FVector& WorldLocation);
};

USTRUCT(BlueprintType)
struct INTERACTIVEITEMS_API FInteractionStatus
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<class AActor> InteractionTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InteractionPercentComplete;

	FInteractionStatus();
	FInteractionStatus(TWeakObjectPtr<class AActor> Target, float Elapsed, float Duration);
};

USTRUCT(BlueprintType, Blueprintable)
struct INTERACTIVEITEMS_API FInteractionDetails
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<class UTexture2D> Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Details;
};
