#include "UEProjectPatrolNetwork.h"

#include <AI/NavigationModifier.h>
#include <Components/SplineComponent.h>
#include <NavigationSystem.h>


AUEProjectPatrolNetwork::AUEProjectPatrolNetwork(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , PatrolPath(nullptr)
	, bBidirectional(false)
    , bLoopPath(true)
    , bCurrentDirectionIsForward(true)
{
	PatrolPath = CreateDefaultSubobject<USplineComponent>("PatrolPath");
    RootComponent = PatrolPath;
}

bool AUEProjectPatrolNetwork::GetNextPatrolPoint(int32 InCurrentIndex, FVector& OutNextPoint)
{
    const bool bIsValidIndex = IsValidPointIndex(InCurrentIndex);

    if (bIsValidIndex)
    {
        const int32 NextIndex = GetNextPointIndex(InCurrentIndex);
        OutNextPoint = PatrolPath->GetLocationAtSplinePoint(NextIndex, ESplineCoordinateSpace::World);
    }

    return bIsValidIndex;
}

int32 AUEProjectPatrolNetwork::GetNextPointIndex(int32 CurrentIndex, bool bRandom /*= false*/)
{
    const int32 NumPoints = GetNumPoints();
    if (NumPoints == 0) return INDEX_NONE;

    if (bRandom)
    {
        // Exclude current point from random selection
        TArray<int32> ValidPoints;

        for (int32 i = 0; i < NumPoints; ++i)
        {
            if (i != CurrentIndex)
            {
                ValidPoints.Add(i);
            }
        }

        return ValidPoints[FMath::RandRange(0, ValidPoints.Num() - 1)];
    }

    if (bBidirectional)
    {
        if (CurrentIndex >= NumPoints - 1) 
        {
            bCurrentDirectionIsForward = false;
        }
        else if (CurrentIndex <= 0)
        {
            bCurrentDirectionIsForward = true;
        }

        return bCurrentDirectionIsForward ? CurrentIndex + 1 : CurrentIndex - 1;
    }

    return bLoopPath ? (CurrentIndex + 1) % NumPoints : FMath::Min(CurrentIndex + 1, NumPoints - 1);
}

bool AUEProjectPatrolNetwork::GetPointLocation(int32 InIndex, FVector& OutLocation) const
{
    const bool bIsValidIndex = IsValidPointIndex(InIndex);

    if (bIsValidIndex)
    {
        OutLocation = PatrolPath->GetLocationAtSplinePoint(
            InIndex, ESplineCoordinateSpace::World);
    }

    return bIsValidIndex;
}

bool AUEProjectPatrolNetwork::GetPointRotation(int32 InIndex, FRotator& OutRotation) const
{
    const bool bIsValidIndex = IsValidPointIndex(InIndex);

    if (bIsValidIndex)
    {
        OutRotation = PatrolPath->GetRotationAtSplinePoint(
            InIndex, ESplineCoordinateSpace::World);
    }

    return bIsValidIndex;
}

float AUEProjectPatrolNetwork::GetDistanceAlongPathToPoint(
    int32 TargetIndex, const FVector& CurrentLocation) const
{
    if (!IsValidPointIndex(TargetIndex)) return -1.0f;

    float Distance;
    int32 NearestIndex;
    if (!FindNearestPointOnPath(CurrentLocation, Distance, NearestIndex)) return -1.0f;

    return PatrolPath->GetDistanceAlongSplineAtSplinePoint(TargetIndex) - Distance;
}

bool AUEProjectPatrolNetwork::FindNearestPointOnPath(
    const FVector& InWorldLocation, float& OutDistance, int32& OutPointIndex) const
{
    if (!GetNumPoints()) return false;

    float ClosestDistanceSquared = MAX_FLT;
    int32 BestIndex = INDEX_NONE;

    // Iterate over the patrol points along the spline to find the closest
    for (int32 i = 0; i < GetNumPoints(); i++)
    {
        const FVector PointLocation = 
            PatrolPath->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
        const float DistanceSquared = FVector::DistSquared(InWorldLocation, PointLocation);
        if (DistanceSquared < ClosestDistanceSquared)
        {
            ClosestDistanceSquared = DistanceSquared;
            BestIndex = i;
        }
    }

    if (BestIndex == INDEX_NONE) return false;

    // Return the index of the closest spline point
    OutPointIndex = BestIndex;
    // Also return the distance along the spline at that discrete point
    OutDistance = PatrolPath->GetDistanceAlongSplineAtSplinePoint(BestIndex);

    return true;
}

bool AUEProjectPatrolNetwork::GetDirectionAtPoint(int32 InIndex, FVector& OutDirection) const
{
    const bool bIsValidIndex = IsValidPointIndex(InIndex);

    if (bIsValidIndex)
    {
        OutDirection = PatrolPath->GetDirectionAtSplinePoint(
            InIndex, ESplineCoordinateSpace::World);
    }

    return bIsValidIndex;
}

void AUEProjectPatrolNetwork::SetPathBidirectional(bool bNewBidirectional)
{
    bBidirectional = bNewBidirectional;
}

void AUEProjectPatrolNetwork::SetLoopPath(bool bNewLoopPath)
{
    bLoopPath = bNewLoopPath;
}

bool AUEProjectPatrolNetwork::IsValidPointIndex(int32 Index) const
{
    return Index >= 0 && Index < PatrolPath->GetNumberOfSplinePoints();
}

int32 AUEProjectPatrolNetwork::GetNumPoints() const
{
	return PatrolPath->GetNumberOfSplinePoints();
}
