// Simon Sejer Nielsen 2018

#include "ChooseNextWaypoint.h"
#include "PatrolRouteComponent.h"


EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory)
{


	// Get Patrol Points
	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto PatrolRouteComponent = ControlledPawn->FindComponentByClass<UPatrolRouteComponent>();

	if (!ensure(PatrolRouteComponent)){	return EBTNodeResult::Failed; } //TODO remove ensure

	// Warn about Empty Patrol Routes
	auto PatrolledPoints = PatrolRouteComponent->GetPatrolPoints();

	if (PatrolledPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No patrol points added on: %s"), *ControlledPawn->GetName())
		return EBTNodeResult::Failed;
	}


	//Set Next Waypoint
	auto BlackBoard = OwnerComp.GetBlackboardComponent();
	auto Index = BlackBoard->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackBoard->SetValueAsObject(NextWaypoint.SelectedKeyName, PatrolledPoints[Index]);

	//Cycle Next Waypoint
	auto NextIndex = (Index + 1) % PatrolledPoints.Num();
	BlackBoard->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex); // Setting the Index rather than making a new variable (NextIndex) will result in a crash when it resets to 0!
	
	return EBTNodeResult::Succeeded;
}

