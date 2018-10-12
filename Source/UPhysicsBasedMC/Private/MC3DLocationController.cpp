// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#include "MC3DLocationController.h"
#include "MC6DController.h"
#include "MC6DControllerOffset.h"

// Ctor
UMC3DLocationController::UMC3DLocationController()
{
	// Point update by default to NONE
	UpdateFunctionPointer = &UMC3DLocationController::Update_NONE;
}

// Dtor
UMC3DLocationController::~UMC3DLocationController()
{
}

// Init
void UMC3DLocationController::Init(UMC6DController* InMC,
	USkeletalMeshComponent* InSkelMesh,
	FTransform InOffset,
	EMCControlType ControlType,
	float P, float I, float D, float Max,
	bool bApplyToAllChildBodies)
{
	MC = InMC;
	SkeletalMesh = InSkelMesh;
	PIDController.Init(P, I, D, Max);

	// Bind the update function pointer
	if (InOffset.Equals(FTransform::Identity))
	{
		UE_LOG(LogTemp, Warning, TEXT(">> %s::%d"), TEXT(__FUNCTION__), __LINE__);
		switch (ControlType)
		{
		case EMCControlType::NONE:
			UpdateFunctionPointer = &UMC3DLocationController::Update_NONE;
			break;
		case EMCControlType::Position:
			UpdateFunctionPointer = &UMC3DLocationController::Update_Skel_Position;
			break;
		case EMCControlType::Velocity:
			UpdateFunctionPointer = &UMC3DLocationController::Update_Skel_Velocity;
			break;
		case EMCControlType::Acceleration:
			UpdateFunctionPointer = &UMC3DLocationController::Update_Skel_Acceleration;
			break;
		case EMCControlType::Force:
			UpdateFunctionPointer = &UMC3DLocationController::Update_Skel_Force;
			break;
		case EMCControlType::Impulse:
			UpdateFunctionPointer = &UMC3DLocationController::Update_Skel_Impulse;
			break;
		default:
			UpdateFunctionPointer = &UMC3DLocationController::Update_NONE;
			break;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(">> %s::%d"), TEXT(__FUNCTION__), __LINE__);
		Offset = InOffset.GetLocation() - SkeletalMesh->GetComponentLocation();
		switch (ControlType)
		{
		case EMCControlType::NONE:
			UpdateFunctionPointer = &UMC3DLocationController::Update_NONE;
			break;
		case EMCControlType::Position:
			UpdateFunctionPointer = &UMC3DLocationController::Update_Skel_Position_Offset;
			break;
		case EMCControlType::Velocity:
			UpdateFunctionPointer = &UMC3DLocationController::Update_Skel_Velocity_Offset;
			break;
		case EMCControlType::Acceleration:
			UpdateFunctionPointer = &UMC3DLocationController::Update_Skel_Acceleration_Offset;
			break;
		case EMCControlType::Force:
			UpdateFunctionPointer = &UMC3DLocationController::Update_Skel_Force_Offset;
			break;
		case EMCControlType::Impulse:
			UpdateFunctionPointer = &UMC3DLocationController::Update_Skel_Impulse_Offset;
			break;
		default:
			UpdateFunctionPointer = &UMC3DLocationController::Update_NONE;
			break;
		}
	}
}

// Init
void UMC3DLocationController::Init(UMC6DController* InMC,
	UStaticMeshComponent* InStaticMesh,
	FTransform InOffset,
	EMCControlType ControlType,
	float P, float I, float D, float Max,
	bool bApplyToAllChildBodies)
{
	MC = InMC;
	StaticMesh = InStaticMesh;

	PIDController.Init(P, I, D, Max);

	// Bind the update function pointer
	if (InOffset.Equals(FTransform::Identity))
	{
		UE_LOG(LogTemp, Warning, TEXT(">> %s::%d"), TEXT(__FUNCTION__), __LINE__);
		switch (ControlType)
		{
		case EMCControlType::NONE:
			UpdateFunctionPointer = &UMC3DLocationController::Update_NONE;
			break;
		case EMCControlType::Position:
			UpdateFunctionPointer = &UMC3DLocationController::Update_Static_Position;
			break;
		case EMCControlType::Velocity:
			UpdateFunctionPointer = &UMC3DLocationController::Update_Static_Velocity;
			break;
		case EMCControlType::Acceleration:
			UpdateFunctionPointer = &UMC3DLocationController::Update_Static_Acceleration;
			break;
		case EMCControlType::Force:
			UpdateFunctionPointer = &UMC3DLocationController::Update_Static_Force;
			break;
		case EMCControlType::Impulse:
			UpdateFunctionPointer = &UMC3DLocationController::Update_Static_Impulse;
			break;
		default:
			UpdateFunctionPointer = &UMC3DLocationController::Update_NONE;
			break;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(">> %s::%d"), TEXT(__FUNCTION__), __LINE__);
		Offset = InOffset.GetLocation() - StaticMesh->GetComponentLocation();
		switch (ControlType)
		{
		case EMCControlType::NONE:
			UpdateFunctionPointer = &UMC3DLocationController::Update_NONE;
			break;
		case EMCControlType::Position:
			UpdateFunctionPointer = &UMC3DLocationController::Update_Static_Position_Offset;
			break;
		case EMCControlType::Velocity:
			UpdateFunctionPointer = &UMC3DLocationController::Update_Static_Velocity_Offset;
			break;
		case EMCControlType::Acceleration:
			UpdateFunctionPointer = &UMC3DLocationController::Update_Static_Acceleration_Offset;
			break;
		case EMCControlType::Force:
			UpdateFunctionPointer = &UMC3DLocationController::Update_Static_Force_Offset;
			break;
		case EMCControlType::Impulse:
			UpdateFunctionPointer = &UMC3DLocationController::Update_Static_Impulse_Offset;
			break;
		default:
			UpdateFunctionPointer = &UMC3DLocationController::Update_NONE;
			break;
		}
	}
}

// Update
void UMC3DLocationController::Update(float DeltaTime)
{
	(this->*UpdateFunctionPointer)(DeltaTime);
}


/* Skeletal mesh update functions */
void UMC3DLocationController::Update_Skel_Position(float DeltaTime)
{
	SkeletalMesh->SetWorldLocation(MC->GetComponentLocation(),
		false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);	
	// TODO test SkeletalMesh->SetAllPhysicsPosition()
	UE_LOG(LogTemp, Warning, TEXT(">> %s::%d MC Loc=%s"),
		TEXT(__FUNCTION__), __LINE__,
		*MC->GetComponentLocation().ToString());
}

void UMC3DLocationController::Update_Skel_Velocity(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT(">> %s::%d Not implemented.."), TEXT(__FUNCTION__), __LINE__);
}

void UMC3DLocationController::Update_Skel_Acceleration(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT(">> %s::%d Not implemented.."), TEXT(__FUNCTION__), __LINE__);
}

void UMC3DLocationController::Update_Skel_Force(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT(">> %s::%d Not implemented.."), TEXT(__FUNCTION__), __LINE__);
}

void UMC3DLocationController::Update_Skel_Impulse(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT(">> %s::%d Not implemented.."), TEXT(__FUNCTION__), __LINE__);
}

// With offset
void UMC3DLocationController::Update_Skel_Position_Offset(float DeltaTime)
{
	SkeletalMesh->SetWorldLocation(MC->GetComponentLocation() + Offset,
		false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
}

void UMC3DLocationController::Update_Skel_Velocity_Offset(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT(">> %s::%d Not implemented.."), TEXT(__FUNCTION__), __LINE__);
}

void UMC3DLocationController::Update_Skel_Acceleration_Offset(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT(">> %s::%d Not implemented.."), TEXT(__FUNCTION__), __LINE__);
}

void UMC3DLocationController::Update_Skel_Force_Offset(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT(">> %s::%d Not implemented.."), TEXT(__FUNCTION__), __LINE__);
}

void UMC3DLocationController::Update_Skel_Impulse_Offset(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT(">> %s::%d Not implemented.."), TEXT(__FUNCTION__), __LINE__);
}


/* Static mesh update functions */
void UMC3DLocationController::Update_Static_Position(float DeltaTime)
{
	StaticMesh->SetWorldLocation(MC->GetComponentLocation(),
		false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
	UE_LOG(LogTemp, Warning, TEXT(">> %s::%d MC Loc=%s"),
		TEXT(__FUNCTION__), __LINE__,
		*MC->GetComponentLocation().ToString());
}

void UMC3DLocationController::Update_Static_Velocity(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT(">> %s::%d Not implemented.."), TEXT(__FUNCTION__), __LINE__);
}

void UMC3DLocationController::Update_Static_Acceleration(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT(">> %s::%d Not implemented.."), TEXT(__FUNCTION__), __LINE__);
}

void UMC3DLocationController::Update_Static_Force(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT(">> %s::%d Not implemented.."), TEXT(__FUNCTION__), __LINE__);
}

void UMC3DLocationController::Update_Static_Impulse(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT(">> %s::%d Not implemented.."), TEXT(__FUNCTION__), __LINE__);
}

// With offset
void UMC3DLocationController::Update_Static_Position_Offset(float DeltaTime)
{
	StaticMesh->SetWorldLocation(MC->GetComponentLocation() + Offset,
		false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
	UE_LOG(LogTemp, Warning, TEXT(">> %s::%d MC Loc=%s"),
		TEXT(__FUNCTION__), __LINE__,
		*(MC->GetComponentLocation() + Offset).ToString());
}

void UMC3DLocationController::Update_Static_Velocity_Offset(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT(">> %s::%d Not implemented.."), TEXT(__FUNCTION__), __LINE__);
}

void UMC3DLocationController::Update_Static_Acceleration_Offset(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT(">> %s::%d Not implemented.."), TEXT(__FUNCTION__), __LINE__);
}

void UMC3DLocationController::Update_Static_Force_Offset(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT(">> %s::%d Not implemented.."), TEXT(__FUNCTION__), __LINE__);
}

void UMC3DLocationController::Update_Static_Impulse_Offset(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT(">> %s::%d Not implemented.."), TEXT(__FUNCTION__), __LINE__);
}