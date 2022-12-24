AutoMesh
========

`ADDMESH: <mesh name> [<property> ...]`

Max 512 + 1 chars for token. max 17 tokens, 1 ADDMESH:, 1 mesh name, 15 properties.

Properties
----------

OnATVPark

OnStormed

OnIncinerated

OnSnowed

OnFlooded

OnSanded

NoStormed

NoIncinerated

NoSnowed

NoFlooded

NoSanded

OnFrozen

OnDesert

OnMountain

NoFrozen

NoDesert

NoMountain

NoOcclusion

ShortCut

MaxDistToRoad<int>

MinDistToRoad<int>

SideDist<int>

ExteriorCurveMinCurve<int>

ExteriorCurveMaxCurve<int>

InteriorCurveMinCurve<int>

InteriorCurveMaxCurve<int>

MiddleDist<int>

Probability<int>

// * 0.0099999998

CrossingDist<int>

Frequency<int> // max(int, 5)

Phase<int>

GroundMaxPente<int>

// * 0.0099999998

GroundMaxTrou<int>

// * 0.0099999998

HumidityMin<int>

// * 0.0099999998

HumidityMax<int>

// * 0.0099999998

RoughnessMin<int>

// * 0.0099999998

RoughnessMax<int>

// * 0.0099999998

LinePosition

BigTarmacRoad

SmallTarmacRoad

BigDirtRoad

SmallDirtRoad

NormalDirtRoad

NormalTarmacRoad

RunWay

HighWay

FieldRoad

GoatPath

OnHub

NoHub

LargeElectrical

InTheForest

OnForest

NoForest

AtTheWind

VeryFlatGround // GroundMaxPente = 0.050000001;

EdgyGround // GroundMaxPente = 1.5;

FlatGround // GroundMaxPente = 0.30000001;

VeryRare // Probability = 0.0099999998;

VeryFrequent // Probability = 1.0;

FairlyRare // Probability = 0.050000001;

FairlyFrequent // Probability = 0.2;

InDeepWater

InShallowWater

OnLakeBeach

PerturbCoord

FarFromRoad

OnTerra

TakeStoneColor

OnTerraMiddle<int>

// same as OnTerraMiddleMax<int>

OnTerraMiddleMax<int>

// if int <= 0.0099999998, then int = v20
// OnTerra = 1;
// v25 = 2;

OnTerraMiddleMin<int>

// if int <= 0.0099999998, then int = v20
// OnTerra = 1;
// v25 = 2;

CloseToRoad

OnRockyArea

OnTarmacRoad

GulchSide

RoadMiddle

GroundNormal

ParalelRoad

DownHill

WildField

CultivatedField

CityPopulation

TownPopulation

CountryPopulation

RandomRot

// if v25 == a4, do stuff
