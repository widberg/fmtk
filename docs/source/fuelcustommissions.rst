Custom Missions
===============

.. code-block:: c

   EMD_Begin

.. code-block:: c

   EMD_End

.. code-block:: c

   EMD_ExportVehicleName [filename]

   VehicleNames.txt %i -> "%s" where %i is the vehicle ID and %s is the vehicle name from the trans text

.. code-block:: c

   EMD_ForceStartPosition x y

.. code-block:: c

   EMD_HideStartEnd

The start area will not show up in free ride

.. code-block:: c

   EMD_Save [filename]

miss.tsc writes all the mission commands for the existing missions to a file
save never writes EMD_SetCheckpointTimerBonus

.. code-block:: c

   EMD_SetAIElasticParam x y z

.. code-block:: c

   EMD_SetCheckpoint x y z [rx ry rz rw] [type] [w]

ww = w * pi * 180 * 1/2, (0, sin(ww), 0, cos(ww)), overrides rx ry rz rw

.. code-block:: c

   EMD_SetCheckpointTimerBonus bonus

Used for blits sets the time added to the timer for each checkpoint. One of these command for each checkpoint command. Otherwise uses EMD_SetTimerCheckBonus.

.. code-block:: c

   EMD_SetEndPos x y z

.. code-block:: c

   EMD_SetEndRot <w | rx ry rz rw>

ww = w * pi * 180 * 1/2, (0, sin(ww), 0, cos(ww)), sets rx ry rz rw

.. code-block:: c

   EMD_SetFilterIA id...

.. code-block:: c

   EMD_SetName name

at most 31 characters

.. code-block:: c

   EMD_SetNbIA num

at most 32

.. code-block:: c

   EMD_SetNbLoop num

.. code-block:: c

   EMD_SetPlayerAllowedClassFilter id...

at most 10 ids

.. code-block:: c

   EMD_SetPlayerAllowedGroupFilter id...

at most 10 ids

.. code-block:: c

   EMD_SetPlayerAllowedVehiculeFilter id...

at most 32 ids

.. code-block:: c

   EMD_SetPlayerForbiddenClassFilter id...

.. code-block:: c

   EMD_SetPlayerForbiddenGroupFilter id...

.. code-block:: c

   EMD_SetPlayerForbiddenVehiculeFilter [id...]

.. code-block:: c

   EMD_SetRaceMode mode

0 < mode < 7

.. code-block:: c

   EMD_SetRaceScenario scenario

scenario < 15

.. code-block:: c

   EMD_SetStartPos x y z

.. code-block:: c

   EMD_SetStartRot <w | rx ry rz rw>

ww = w * pi * 180 * 1/2, (0, sin(ww), 0, cos(ww)), sets rx ry rz rw

.. code-block:: c

   EMD_SetTimer time

   Sets the time attack time out

.. code-block:: c

   EMD_SetTimerCheckBonus bonus

.. code-block:: c

   EMD_SetWeatherScenario

stub, does nothing

.. code-block:: c

    AddHubCareerRaces hub_name mission_name tt_name tt_description

careful with this one, it'll fuck up your save file

tt_description does not do anything, should be NONE

.. code-block:: c

    AddMissionSurfaceType mission_name tt_surface_type tt_vehicle_type

.. code-block:: c

    AddMissionParam mission_name ai_vehicle_id_string start_time_of_day end_time_of_day start_weather_string start_fog end_weather_string end_fog start_weather_auxiliary end_weather_auxiliary

Weather String
--------------

SUN
TOR
W# wind
S# snow
C# cloud
R# rain
G# sand - unused
``TOR_W#100_S#50_C#75_R#10``

.. code-block:: c

    AddMissionCarOnline mission_name vehicle_id_string

.. code-block:: c

    AddMissionNeededVehicle mission_name vehicle_name vehicle_surface_string vehicle_type_string some_vehicle_string

.. code-block:: c

    AddMissionFuel mission_name legend expert rookie

.. code-block:: c

    AddMissionTime mission_name legend expert rookie time_limit

.. code-block:: c

    AddMissionIASpeed mission_name x y

.. code-block:: c

    AddNbMaxOnlineVehicle mission_name num
