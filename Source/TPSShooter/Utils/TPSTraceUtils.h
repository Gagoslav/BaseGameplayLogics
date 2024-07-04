#pragma once


namespace TPSTraceUtils
{
	// This method is a wrapper for GetWorld()->SweepSingle by channel but only for CAPSULE component
	// and it also includes a debugging functionality for capsule, including DrawTime, TraceColor and HitColor
	// We just don't want to use UKismetSystemLibrary::SweepSingle as it's considered to be used by BLUEPRINTS and is more redundant
	bool SweepCapsuleSingleByChanel(const UWorld* InWorld, struct FHitResult& OutHit, const FVector& Start, const FVector& End, float CapsuleRadius, float CapsuleHalfHeight, const FQuat& Rot, ECollisionChannel TraceChannel , const FCollisionQueryParams& Params = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& ResponseParam = FCollisionResponseParams::DefaultResponseParam, bool bDrawDebug = false, float DrawTime = -1.0f, FColor TraceColor = FColor::Black, FColor HitColor= FColor::Red);
	
	// This method is a wrapper for GetWorld()->SweepSingle by channel but only for SPHERE component
	// and it also includes a debugging functionality for sphere, including DrawTime, TraceColor and HitColor
	// We just don't want to use UKismetSystemLibrary::SweepSingle as it's considered to be used by BLUEPRINTS and is more redundant
	bool SweepSphereSingleByChanel(const UWorld* InWorld, struct FHitResult& OutHit, const FVector& Start, const FVector& End, float ShperRadius, ECollisionChannel TraceChannel, const FCollisionQueryParams& Params = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& ResponseParam = FCollisionResponseParams::DefaultResponseParam, bool bDrawDebug = false, float DrawTime = -1.0f, FColor TraceColor = FColor::Black, FColor HitColor = FColor::Red);


	bool OverlapCapsuleBlockingByProfile(const UWorld* InWorld, const FVector& Position, float CapsuleRadius, float CapsuleHalfHeight, FQuat Rotation,  FName CollisionProfileName, const FCollisionQueryParams& Params, bool bDrawDebug = false, float DrawTime = -1.0f, FColor TraceColor = FColor::Transparent, FColor HitColor = FColor::Red);
}