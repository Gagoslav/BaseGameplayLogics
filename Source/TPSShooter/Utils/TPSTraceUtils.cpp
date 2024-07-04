#include "TPSTraceUtils.h"
#include "DrawDebugHelpers.h"

// Copy parameters from World.h SweepSingle by channel and add const UWorld*, necessary properties for Capsule and also properties for debugging
bool TPSTraceUtils::SweepCapsuleSingleByChanel(const UWorld* InWorld, FHitResult& OutHit, const FVector& Start, const FVector& End, float CapsuleRadius, float CapsuleHalfHeight, const FQuat& Rot, ECollisionChannel TraceChannel, const FCollisionQueryParams& Params, const FCollisionResponseParams& ResponseParam, bool bDrawDebug, float DrawTime, FColor TraceColor, FColor HitColor)
{
	bool bResult = false;

	// As our function is designed for capsule particularly, create capsule
	FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
	bResult = InWorld->SweepSingleByChannel(OutHit, Start, End, Rot, TraceChannel, CollisionShape, Params, ResponseParam);

// Make sure we don't have any check of any draw logic in ship build
#if ENABLE_DRAW_DEBUG
	// Add debugging for Forward check for mantling
	if (bDrawDebug)
	{
		// Draw Forward check capsules at its start and end locations and connect them with line
		DrawDebugCapsule(InWorld, Start, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, TraceColor, false, DrawTime);
		DrawDebugCapsule(InWorld, End, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, TraceColor, false, DrawTime);
		DrawDebugLine(InWorld, Start, End, TraceColor, false, DrawTime);

		if (bResult)
		{
			// If we have collisions found by Forward check capsule, then draw the capsule and impact point
			DrawDebugCapsule(InWorld, OutHit.Location, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, HitColor, false, DrawTime);
			DrawDebugPoint(InWorld, OutHit.ImpactPoint, 10.0f, HitColor, false, DrawTime);
		}

	}
#endif
	return bResult;
}

bool TPSTraceUtils::SweepSphereSingleByChanel(const UWorld* InWorld, FHitResult& OutHit, const FVector& Start, const FVector& End, float SphereRadius, ECollisionChannel TraceChannel, const FCollisionQueryParams& Params, const FCollisionResponseParams& ResponseParam, bool bDrawDebug, float DrawTime, FColor TraceColor, FColor HitColor)
{
	bool bResult = false;

	// As our function is designed for sphere particularly, create sphere
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(SphereRadius);
	bResult = InWorld->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, TraceChannel, CollisionShape, Params, ResponseParam);

// Make sure we don't have any check of any draw logic in ship build
#if ENABLE_DRAW_DEBUG
	// Add debugging for Downward check for mantling
	if (bDrawDebug)
	{
		FVector DebugCapsuleLocation = (Start + End) * 0.5f;
		FVector TraceVector = End - Start;
		float DebugDrawCapsuleHalfHeight = TraceVector.Size() * 0.5f;

		//////
		FQuat DebugCapsuleRotation = FRotationMatrix::MakeFromZ(TraceVector).ToQuat();

		// Replace debug draw of two hypothetical spheres (start and end) by one Capsule with appropriate height
		DrawDebugCapsule(InWorld, DebugCapsuleLocation, DebugDrawCapsuleHalfHeight, SphereRadius, DebugCapsuleRotation, TraceColor, false, DrawTime);

		if (bResult)
		{
			DrawDebugSphere(InWorld, OutHit.Location, SphereRadius, 16, HitColor, false, DrawTime);
			DrawDebugPoint(InWorld, OutHit.ImpactPoint, 10.0f, HitColor, false, DrawTime);
		}

	}
#endif

	return bResult;
}



bool TPSTraceUtils::OverlapCapsuleBlockingByProfile(const UWorld* InWorld, const FVector& Position, float CapsuleRadius, float CapsuleHalfHeight, FQuat Rotation, FName CollisionProfileName, const FCollisionQueryParams& Params, bool bDrawDebug, float DrawTime, FColor TraceColor, FColor HitColor)
{
	bool bResult = false;

	// As our function is designed for capsule particularly, create capsule
	FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
	bResult = InWorld->OverlapBlockingTestByProfile(Position, Rotation, CollisionProfileName, CollisionShape, Params);


// Make sure we don't have any check of any draw logic in ship build
#if ENABLE_DRAW_DEBUG

	if (bDrawDebug && bResult)
	{
		// If overlap exists draw it and return false //WARNING SHOULD EDIT
		DrawDebugCapsule(InWorld, Position, CapsuleHalfHeight, CapsuleRadius, Rotation, bResult ? HitColor : TraceColor, false, DrawTime);
	}

#endif
	
	return bResult;
}
