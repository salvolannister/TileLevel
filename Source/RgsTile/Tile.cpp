// Copyright(c) Forge Reply. All Rights Reserved.

#include "Tile.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Materials/MaterialInstance.h"


ATile::ATile()
{
	if (!IsValid(RootComponent))
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	}

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Game/Geometry/Meshes/ChamferBox"));
	MeshComponent->SetStaticMesh(CubeMesh.Object);
	MeshComponent->SetRelativeScale3D(FVector(2.0f, 2.0f, 1.0f));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> TimeMaterialInstance(TEXT("/Game/Materials/TileMaterialInstance"));
	MeshComponent->SetMaterial(0, TimeMaterialInstance.Object);
	MeshComponent->SetupAttachment(RootComponent);

	TileCoordinateText = CreateDefaultSubobject<UTextRenderComponent>(FName("TextCoordinates"));
	TileCoordinateText->SetRelativeLocationAndRotation(FVector(0.f, 30.f, 60.f), FRotator(90.f, 90.f, 90.f));
	TileCoordinateText->SetupAttachment(RootComponent);
	TileCoordinateText->SetTextRenderColor(FColor::Black);

	PrimaryActorTick.bCanEverTick = true;
}

void ATile::BeginPlay()
{
	Super::BeginPlay();
	
	MeshComponent->SetVectorParameterValueOnMaterials(FName("TileBaseColor"), FVector(FColor::Silver));
	MeshComponent->SetScalarParameterValueOnMaterials(FName("TileEmission"), 0.0f);

	bVisited = false;
	TilePosX = TilePosY = -1;
}


void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATile::SetType(ETileType Type)
{
	TileType = Type;
}

ETileType ATile::GetType() const
{
	return TileType;
}

void ATile::StepOn()
{

}

void ATile::StepOff()
{
	
}


void ATile::SetRenderText(const int32 x, const int32 y)
{
	TileCoordinateText->SetText(FText::Format(NSLOCTEXT("Coordinates","Coordinates","(X:{0}, Y:{1})"), 
								FText::AsNumber(x),
								FText::AsNumber(y))
	);
}

void ATile::ShowTileEffect(bool bShowEffect)
{
	if (bShowEffect)
	{
		StepOn();
	}
	else
	{
		StepOff();
	}
}

bool ATile::HasBeenVisited() const
{
	return bVisited;
}


void ATile::StoreTileGridPosition(const int32 x, const int32 y)
{
	TilePosX = x;
	TilePosY = y;
}