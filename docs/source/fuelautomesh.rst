AutoMesh
========

`FUEL Fandom Wiki Edit Landscape Object Frequency entry <https://fuel.fandom.com/wiki/Edit_Landscape_Object_Frequency>`_

`ADDMESH: <mesh name> [<property> ...]`

Properties
----------

Properties can be either boolean or numeric. A boolean property is true if it is included and false if it is excluded. A numeric property has a value of the integer concatenated with it after the property name.

Boolean
-------

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

// For all properties
// if v25 == a4, do stuff

Integer
-------

MaxDistToRoad

MinDistToRoad

SideDist

ExteriorCurveMinCurve

ExteriorCurveMaxCurve

InteriorCurveMinCurve

InteriorCurveMaxCurve

MiddleDist

Probability

// * 0.0099999998

CrossingDist

Frequency // max(int, 5)

Phase

GroundMaxPente

// * 0.0099999998

GroundMaxTrou

// * 0.0099999998

HumidityMin

// * 0.0099999998

HumidityMax

// * 0.0099999998

RoughnessMin

// * 0.0099999998

RoughnessMax

// * 0.0099999998

OnTerraMiddle

// same as OnTerraMiddleMax

OnTerraMiddleMax

// if int <= 0.0099999998, then int = v20
// OnTerra = 1;
// v25 = 2;

OnTerraMiddleMin

// if int <= 0.0099999998, then int = v20
// OnTerra = 1;
// v25 = 2;

Limitations
-----------

Max 512 chars + 1 null terminator for token. Max 17 tokens, 1 ADDMESH:, 1 mesh name, 15 properties.
