#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "UEProjectPatrolNetwork.generated.h"


class USplineComponent;

/**
 * A patrol network that defines a path for AI to follow using spline points.
 * Supports bidirectional movement, random point selection, and navmesh integration.
 * Can be placed in level and configured via editor or at runtime.
 */
UCLASS(Blueprintable, Category = "AI|Patrol", meta = (DisplayName = "Patrol Network"))
class AUEProjectPatrolNetwork : public AActor
{
    GENERATED_BODY()

public:

    AUEProjectPatrolNetwork(
        const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /**
     * Get the next patrol point in the network based on current index.
     * 
     * @param InCurrentIndex - Current point index in patrol network.
     * @param OutNextPoint - Location of next patrol point.
     * @return Whether the returned point is valid.
     */
    UFUNCTION(BlueprintCallable, Category = "Patrol")
    bool GetNextPatrolPoint(int32 InCurrentIndex, FVector& OutNextPoint);
    
    /**
     * Calculate next point index based on patrol configuration.
     * 
     * @param CurrentIndex - Current point index.
     * @param bRandom - If true, select random next point excluding the current one.
     * @return Next point index or INDEX_NONE if invalid.
     */
    UFUNCTION(BlueprintCallable, Category = "Patrol")
    int32 GetNextPointIndex(int32 CurrentIndex, bool bRandom = false);
    
    /**
     * Get world location of point at given index.
     * 
     * @param InIndex - Point index to query.
     * @param OutLocation - World location of the point.
     * @return Whether the point index to query is valid.
     */
    UFUNCTION(BlueprintCallable, Category = "Patrol")
    bool GetPointLocation(int32 InIndex, FVector& OutLocation) const;
    
    /**
     * Get rotation at given point index.
     * 
     * @param InIndex - Point index to query.
     * @param OutRotation - Rotation at the point.
     * @return Whether the point index to query is valid.
     */
    UFUNCTION(BlueprintCallable, Category = "Patrol")
    bool GetPointRotation(int32 InIndex, FRotator& OutRotation) const;

    /**
     * Calculate distance along spline to reach target point.
     * 
     * @param TargetIndex - Index of the target point.
     * @param CurrentLocation - Current world location.
     * @return Distance along path to target, or -1 if invalid.
     */
    UFUNCTION(BlueprintCallable, Category = "Patrol")
    float GetDistanceAlongPathToPoint(int32 TargetIndex, const FVector& CurrentLocation) const;
    
    /**
     * Find closest point on patrol path to given location.
     * 
     * @param InWorldLocation - Location to find nearest point to.
     * @param OutDistance - Distance along spline of the nearest point.
     * @param OutPointIndex - Index of the nearest spline point.
     * @return Whether a valid point was found.
     */
    UFUNCTION(BlueprintCallable, Category = "Patrol")
    bool FindNearestPointOnPath(const FVector& InWorldLocation, float& OutDistance, 
                                int32& OutPointIndex) const;
    

    /**
     * Get forward direction at given point.
     * 
     * @param InIndex - Point index to query.
     * @param OutDirection - Unit direction vector at point.
     * @return Whether the point index to query is valid.
     */
    UFUNCTION(BlueprintCallable, Category = "Patrol")
    bool GetDirectionAtPoint(int32 InIndex, FVector& OutDirection) const;

    /**
     * Set whether path should be traversed bidirectionally.
     * 
     * @param bNewBidirectional - New bidirectional setting.
     */
    UFUNCTION(BlueprintCallable, Category = "Patrol")
    void SetPathBidirectional(bool bNewBidirectional);

    /**
     * Set whether path should be traversed looping at the end of it.
     * 
     * @param bNewLoopPath - New loop setting.
     */
    UFUNCTION(BlueprintCallable, Category = "Patrol")
    void SetLoopPath(bool bNewLoopPath);
    
    /**
     * Check if point index is valid for this path.
     * 
     * @param Index - Index to validate.
     * @return Whether index is valid.
     */
    UFUNCTION(BlueprintCallable, Category = "Patrol")
    bool IsValidPointIndex(int32 Index) const;

    /**
     * Returns the number of points in the spline.
     * 
     * @return The number of points.
     */
    UFUNCTION(BlueprintCallable, Category = "Patrol")
    int32 GetNumPoints() const;

protected:

    /** Spline component defining the patrol path points and curves. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Patrol")
    USplineComponent* PatrolPath;

    /** If true, AI will patrol back and forth along path instead of looping. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol",
              meta = (DisplayName = "Is Bidirectional"))
    bool bBidirectional;

    /** If true, patrol will continue from first point after reaching last. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol",
              meta = (DisplayName = "Loop Path"))
    bool bLoopPath;

private:

    // Tracks patrol direction: true => forward, false => backward
    bool bCurrentDirectionIsForward;
};
