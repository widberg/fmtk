#ifndef ___SETUP_H__
#define ___SETUP_H__

//---------------------------------------------------------------------
//	How To Use:
//---------------------------------------------------------------------
//
//	I've re-done the setup file so you no longer comment / uncomment lines of code.
//	Instead, you change values from 0 (off) to 1 (on) and vice-versa.
//
//	EG:
//
//		OPTION		1	// on
//		OPTION		0	// off
//
//	Also, the 0/1 value in the // comment description to the
//	right of the option is FUEL default setting.
//
//	EG:
//
//		OPTION		0	// 1 (FUEL default is 1 on, but it's currently 0 off)
//
//
//---------------------------------------------------------------------
//	Post Processing Effects ( Screen Fx done to final frames )
//---------------------------------------------------------------------
//
//	Post-processing effects can have a huge impact on FPS, b/c
//	they're done on a per-pixel basis for each pixel of your screen
//	resolution. So, turning off post-processing fx might help
//	on FPS, especially if you're ramping up screen resolution.	
//
//---------------------------------------------------------------------

#define	ANTI_ALIASING		0	// 0 - add FXAA-like trilinear AA ( useful when playing FUEL on a couch / TV setup where screen is further away, otherwise might look too blurry close-up)
#define	MOTION_BLUR			1	// 1 - do motion blur to give sense of speed as you go faster
#define	PAUSE_TINTING		0	// 1 - yellow-tinted pause screen (I find this annoying, so added flag to disable it)
#define	BLUER_NIGHTS		1	// 1 - wash out night colors to be more blue-black to simulate lack of red spectrum light at night
#define	BRIGHTER_COLORS		1	// 0 - greener greens, bluer blues ( makes overall screen colors more vivid / less dystopian/washed out )
#define	SCREEN_DIRT			0	// 1 - screen edges get dirty / clean as vehicle gets dirty / clean (dust-storm levels have their own screen dirt'ing, so even if this is disabled you'll still see dirt caked on the screen on some levels)
#define	SCREEN_WATER		1	// 1 - splash water on screen when going through water
#define	SCREEN_MUD			1	// 1 - fling mud on screen when off-roading
#define	DYNAMIC_LUMINANCE	1	// 1 - dynamic brightness (eye adaptation) when going in and out of shadows
#define	BLOOM_HAZE			1	// 1 - bloom lighting shimmers on bright colors


//---------------------------------------------------------------------
// Other Stuff
//---------------------------------------------------------------------

// terrain
#define	FADED_ROADS				1	// 0 - faded paint lines on asphalt, rivers leave wet area across roads, tire tracks on dirt roads blend with underlying dirt color better
#define DISTANCE_SMOKE			1	// 1 - horizon smoke transparency doesn't blend well, so this lets you turn it off if you find it distracting (note, also turns off fog banks on lake of Offshore Shack) ... turning off may help with FPS some
#define	BLUER_SKIES				1	// 0 - tweak horizon blue'ing / haze to use more sky blue coloring (good when paired up with BRIGHTER_COLORS)
#define	RAIN_SHEEN				1	// 0 - add reflective rain sheen to ground, vehicles, plants, etc to make it look wet in rain (might hit FPS on lower-end graphics cards)
#define	PATH_FINDER				0	// 0 - green-highlights main dirt/asphalt roads (but not trails) as air haze increases (basically a cheat for races like Dustbowl City: Dark Dust)

// vehicles
#define	CHEAPER_TEXTURE_CALCS	1	// 0 - Asobo used partial derivative formula to calculate the .z value on some normal / bump maps (eg: on vehicles). The purpose is to save VRAM by only storing the .xy of the normal map texture then using GPU processing to calc the .z. They used a euclidean distance (sqrt(dot(xy,xy))) to do so. But, we can use a cheaper manhattan distance (abs(x-y)) with very minor visual impact. Might help with FPS.

// light
#define	CHEAPER_PLANTS			1	// 0 - use a compiler flag in the plants shader to switch to low-quality lighting on LOD trees (IE: no rain, specular, etc) unfortunately, this can be noticeable on tree leaves as they pop into place a bit more, but it might help performance
#define	GRASS_AMB_OCC			1	// 0 - Black Ambient Occlusion spot under vehicles darkens grass now, but can cause grass/tree flicker at start of races and when high above ground, b/c game engine uses a slower intermittent update on distant grass objects I think  ... not very noticeable, but disable this if you get annoyed
#define	SHADOW_HIGHLIGHTS		1	// 0 - Adds a tiny amount of reflective cubemap to shadows to add depth & enhance shadow detail (acts like ambient light bouncing around and going into shadows to reflect off of things even though shadows block direct light)
#define	FORCE_BILINEAR_SHADOWS	0	// 0 - force things that do (more expensive) trilinear shadows to do (cheaper & chunkier) bilinear instead (only do this if really desperate for FPS .. looks ugly)
#define	SUNLIGHT_AS_SHADOWLIGHT	0	// 0 - use sun color to make shadow color (half-baked... experimenting to see if I can get seemless day/night transition this way)

/*
	the following are amounts to tweak how much each is done.
	default value listed is for default FUEL shadows. But, if
	you make shadows softer / lighter, the shadow highlights
	and shadow specular ("sunny trail" bleeding into shadows)
	can get washed out. So, we can tweak these values here
	to punch them up again.

	the shadow highlights velour "frosting" can get over-done
	and still look ok.

	But, the specular shadow may need some fiddling. Too light
	and it won't be noticeable. Too much, and it will be stark
	seeing it show up on ground in shadows created by trees and
	buildings where it will look unnaturally bright. I prefer
	it to be faint enough to look ok sitting in a shadow, but
	where the "sunny trail" is noticeable when driving along
	looking into the sun. Have to remember that with bloom on,
	the bloom will help bleed some of the brightness around a
	shadow-generating object (eg: looking at a tree blocking sun),
	which will help blend the faint shadow specular into the scene.
	But, it's really a matter of personal taste.
*/

// light value tweaks
#define	SHADOW_HIGHLIGHTS_AMOUNT	0.5	// 0.3 works for default FUEL shadows, but 0.5 is better for soft/lighter shadows
#define	SPECULAR_SHADOW_AMOUNT		0.5	// 0.2 works for default FUEL shadows, but 0.5 is better for soft/lighter shadows

//-----------------------------------
//	Texture Downsampling
//-----------------------------------
//
//	Asobo used the default tex2D() function for texture sample pulls.
//	However, HLSL has a tex2Dbias() function that lets you include a
//	"bias" factor to sharpen or blur images. Essentially, it's trying
//	to use upsampling and downsampling of the texture to artificially
//	sharpen or degrade the texture quality by moving to the next mipmap
//	level.
//
//	Before you get too excited about "sharpen all the textures!"..
//	I haven't noticed tex2Dbias sharpening anything, regardless of bias.
//	Bias values greater then 1 start to push the mipmap level to the next
//	level which uses a lower res texture (creating a bit of bluriness).
//	But, using 0 or negative numbers for bias didn't get any kind of
//	texture sharpening effect. (There's a tex2Dlod function, but it
//	seems to create a noise pattern to "sharpen" things, so textures
//	look like they have a lot of tv static dancing around on them,
//	and it's very ugly and distracting. Hence not using that.)
//
//	Anyways, the better use for tex2Dbias is to downsample the textures
//	in order to improve game performance.
//
//	How can we do that you ask? Well, the game already tracks shadows
//	in frustrum bands (IE: bands / slices to do lower-res shadow maps
//	the further out from the player). You notice these frustrums as you
//	cruise around and watch the seam between the crisp shadows near you
//	transition into the smudgy shadows a bit further out. That's the
//	transition point between frustrum slices / bands.
//
//	The game has 4 bands / slices...
//
//		slice 0 = about 10' radius around player vehicle with best quality shadows
//		slice 1 = about 10' to 50' with smudgier shadows
//		slice 2 = about 50' to 200' with even smudgier shadows (about the distance trees transition from low to high quality models)
//		slice 3 = about 200'+ on to the horizon, where shadows are fuzzy blots
//
//	The shadow slice value is being calculated as shadows process. So, we can
//	simply capture that slice index value, and use it as the built-in bias
//	factor for tex2Dbias texture pulls... that way texture pulls in band
//	nearest the player vehicle will remain nice and sharp/clean, while
//	texture pulls in bands further out will downsample by some amount
//	since they will be harder to notice the downsampling on.
//
//	If we do it just the right amount, it's hardly noticeable. If we
//	do it too much, then distant textures are obviously smudgy and
//	blurred. But, the more downsampled the textures, the greater our
//	game performance (theoretically), b/c less burden on the graphics
//	card processing.
//
//	IE: what it's doing is reducing the mip-map level of the texture,
//	either by pulling a different mipmap level stored with the texture,
//	or processing the texture to reduce it's size / quality.
//
//	EG: if we take a 64x64 pixel texture, we can reduce it to 32x32,
//	then 16x16 then 8x8. Each of those can get stored as a mipmap level.
//	Or, we can use the GPU to resize the texture on-the-fly, downsizing
//	it in half each time to create mipmap levels on-the-fly.
//
//	I'm not quite sure which one the game is doing... all I know is
//	tex2Dbias works, and the bias amount definitely impacts texture
//	quality and (by extension) performance, since we're processing
//	smaller textures.
//
//-----------------------------------
//
//	Since the bias amount is based on the shadow band/slice, you'll
//	need to make sure shadows are on in the game setup options in
//	order to use this feature.
//
//	Also, we're just doing bias on shaders that already process
//	shadow bands / slices, b/c they're already calculating that.
//	So, all we're doing is taking advantage of that to cut some
//	overhead off the texture pulls on those shaders. Other
//	shaders that don't calculate shadows won't do bias processing.
//	But, there aren't too many of those, b/c most things process
//	shadows.
//
//	EG: tornadoes and sand storms don't process shadows, and they
//	need to look nice anyways, so no sense in adding extra
//	processing just to determine shadow band and bias them to make
//	them look blurrier, smudgier, uglier.
//
//	Also, since soft particles (like smoke, dust, etc) are already
//	smudgy / blurry, we automatically double the bias amount on those,
//	because we can get away with blurring them and reducing their quality
//	more since it's harder to notice. We can even do this on water
//	refraction, because it's under the water, and, at a distance,
//	isn't noticeable. So, no sense in processing high quality water
//	refraction that isn't noticed by the player unless right on top
//	of the water.
//
//	I also applied the bias to the AO spot under vehicles, since there's
//	a tex1Dbias function. And, I applied it to the chrome / clear-coat /
//	environment reflections, since there's a texCUBEbias function.
//
//	Basically, a lot of texture pulls now have the ability to use bias
//	if you want to try it.
//
//-----------------------------------
//
//	Since the biasing is based on what band / slice the object is
//	in, we use the DOWNSAMPLE_AMOUNT as a multiplier to adjust.
//
//	As far as I can tell from doing research, the bias level is
//	additive to the mipmap level. So, if something had a mipmap level
//	of 1 already, then a bias of 1 would = 1 + 1 = use 2nd mip-map level
//	(which has reduced quality).
//
//	The problem is I don't know if HLSL rounds to the nearest whole mip-map level.
//	So, if we use DOWNSAMPLE_AMOUNT as a multiplier of the shadow slice/band,
//	and we had a DOWNSAMPLE_AMOUNT of 0.5...
//
//		0 * 0.5	= +0.0 ... no bias .. keep doing full quality / visuals in circle around player
//		1 * 0.5	= +0.5 ... if compiler rounds down to 0, then it's original quality. If it rounds up, then it's next mip-map level of reduced quality
//		2 * 0.5 = +1.0 ... add 1 to mip-map level, so do next mip-map level of reduced quality
//		3 * 0.5 = +1.5 ... if compiler rounds down, will act like +1 mip-map level. If compiler rounds up, will act like +2 mip-map level for even more reduced quality.
//
//	Meanwhile, an easier, more obvious example... if we have DOWNSAMPLE_AMOUNT of 1
//
//		0 * 1 = +0 ... no bias .. keep doing full quality / visuals in circle around player
//		1 * 1 = +1 ... +1 mip-map level for reduced quality in next band away from player vehicle
//		2 * 1 = +2 ... +2 mip-map level for even more reduced quality in 3rd band away from player vehicle
//		3 * 1 = +3 ... +3 mip-map level for greatest quality reduction in last band and on to horizon
//
//	And, note, that since DOWNSAMPLE_AMOUNT is a multiplier..
//	if you set it greater then 1 then it starts to ramp up bias quickly
//	on the outer slices / bands.
//
//		0 * 1.5 = 0.0
//		1 * 1.5 = 1.5
//		2 * 1.5 = 3.0
//		3 * 1.5 = 4.5
//
//		0 * 2.0 = 0.0
//		1 * 2.0 = 2.0
//		2 * 2.0 = 4.0
//		3 * 2.0 = 6.0
//
//	... and so on.
//
//	So, you might have to experiment.
//	But, with some fooling around I noticed the following...
//
//-----------------------------------
//	DOWNSAMPLE_AMOUNT	RESULT
//-----------------------------------
//			0			all bias is 0 (anything x 0 = 0), so same as if not using it. Just set DOWNSAMPLE_TEXTURES to 0 to turn off.
//			0.5			nice trade-off between visuals & performance. Blurring of distant textures is vague, and blends in naturally.
//			0.75		noticeable blurring of mountains and other far-distant textures.
//			1			obvious blurring of mid-range textures transitioning to distant. mountains look like lumps of melting ice cream.
//			2			distant textures lose detail. obvious transition seam between band 0 and band 1 on some things (eg: roof ramp on ground)
//-----------------------------------
//
//	After doing some performance testing on this with MSI Afterburner,
//	I'm not sure I really noticed a difference. IE: multiple runs
//	would generate varying results, but narrow enough to seem like
//	if there was a difference, it wasn't very significant, like maybe
//	an extra frame on the Avg FPS and 0.1% low (faster frame delivery).
//
//	So, your mileage may vary.
//
//-----------------------------------


#define	DOWNSAMPLE_TEXTURES		0	// 0 - turn on/off downsampling textures the further in the distance they are from vehicle
#define	DOWNSAMPLE_AMOUNT		0.5	// 0.5 is my recommendation, 1.0 if you're ok with smudgies, 2.0 if really hurting for performance


//-----------------------------------
//	post-processing also does tex2D texture pulls (of the screen / frames).
//	we can try doing some tex2Dbias on certain things on them, like dynamic
//	eye adaptation / brightness, bloom, etc.. things that already pull
//	blurry / smudgy / AA'ed screen samples to create special effects.
//	The main screen sample will remain un-biased, but the overlay of
//	post-processing fx on it can get biased to see if they speed up
//	rendering performance. We can just use a default bias amount of 2
//	for this. So, all we need is a flag to turn it off and on.
//
//	In experimenting, this seemed to kill performance a little rather
//	than increase it. Hence, I have it switched off by default.
//
//-----------------------------------

#define	DOWNSAMPLE_POSTPROCESSING	0	// 0 - turn on downsampling of screen samples for certain post-processing fx



//-----------------------------------
//	Darker Nights
//-----------------------------------
//
// Multiplier: number impacts things in multiplicative fashion
//
//	with this set to 1.0 and SUNLIGHT_AS_SHADOW
//	it seems to have a seemless day/night transition.
//	But, i'm still experimenting with it. Once we darken
//	nights here, then it's not seemless. Need to find
//	a way to make the light value decay to blend into night
//	better.
//-----------------------------------


#define	NIGHT_MULTIPLIER		0.2	// 1 = default // 0.5 = full moon night // 0.25 = new moon night // 0.0 = pitch-black


//-----------------------------------
//	HUD / UI Transparency
//-----------------------------------
//
// Multipliers: numbers impact things in multiplicative fashion
//
//	Saw S. Victor's Steam Guide "Custom Resolution & Shader Tweak"
//	that showed toning down UI elements to make game more immersive
//	or better for screen shots. Decided to implement it.
//
//	However, implementing it in vertex shaders gives better control
//	over what we impact, will do the calculations per-vertex instead
//	of per-pixel, and thus maybe save on calculations.
//
//	Likewise, we can separate out radar vs other UI elements to make
//	transparent.
//
//-----------------------------------
//
//	UI_TRANSPARENCY tones down vignetting and screen dirt not controlled
//	by SCREEN_DIRT flag above (IE: in dust storms), along with other
//	UI elements, like compass, speed gauge, menu screen (lots of UI stuff).
//
//	RADAR_TRANSPARENCY tones down the radar map lines & gray circle
//	around it. Doesn't impact the symbols on radar, though (that's
//	controlled with UI_TRANSPARENCY).
//
//	GPS_TRANSPARENCY tones down GPS navigation arrows in races, and
//	the lines pointing down from compass to waypoints.
//
//	decimal values from 0.0 (fully transparent) to 1.0 (fully opaque)
//	default is 1.0 which has them fully opaque like normal.
//	As they make them more transparent, the black background of the
//	game shows through, making it seem like title screens are darker.
//
//-----------------------------------

#define	UI_TRANSPARENCY			1.0	// vignette, dust storm screen dirt, menu UI
#define	RADAR_TRANSPARENCY		1.0	// radar
#define	GPS_TRANSPARENCY		1.0	// GPS nav arrows and waypoint lines from compass


//-----------------------------------
//	BRIGHTER_GPS makes GPS arrows & waypoint lines white,
//	which makes them easier to see when more transparent.
//
//	0 = off		1 = on
//-----------------------------------

#define	BRIGHTER_GPS			0	// 0 - makes GPS arrows & waypoint lines white (good when more transparent)



//-----------------------------------
//	SOFTEN SHADOWS & AMBIENT OCCLUSION (AO)
//-----------------------------------
//
//	The lighting function is additive...
//		(
//		  sunlight blocked by shadows	to create shaded areas of "skylight"
//		+ skylight blocked by occlusion to create darker AO areas in shaded areas
//		)
//		* all of that blocked by "normals dot" which gives normals / bump maps their body, but also acts as AO
//		+ AMBIENT_OCCLUSION_COLOR as final base color to keep AO areas from being pitch black.
//
//	So, "shadow" & "occlusion" are multipliers from 0 (fully blocked) to 1 (not blocks).
//
//	With that in mind, we can easily brighten up shadows & AO areas by just
//	using a max( value, min amount ) function to cap how much they block.
//
//		EG; shadow = max(shadow, 1 - SHADOW_AMOUNT)
//
//	SHADOW_AMOUNT & AO_AMOUNT are 0.0 to 1.0 to let the 
//	game know how much of each to do. Behind the scenes
//	they invert (1 - amount).
//
//	EG:
//
//	SHADOW_AMOUNT = 1.0 = max(shadow, 1 - 1.0) ... full shadows
//	SHADOW_AMOUNT = 0.5 = max(shadow, 1 - 0.5) ... cuts shadows in 1/2
//	SHADOW_AMOUNT = 0.0 = max(shadow, 1 - 0.0) ... no shadows
//
//	SHADOW_SOFTEN & AO_SOFTEN act as switches to turn
//	amount softening on (1) / off (0)
//
//	I recommend that if you turn SHADOW_SOFTEN on, you turn
//	AO_SOFTEN on, too, b/c dark AO will stick out like a sore
//	thumb in faint shadows. However, faint AO won't stick out
//	in dark shadows.. but you'll miss some body/depth to the shadows.
//
//	As you make shadows more faint, the sunlight will seem brighter,
//	(because more light values are getting through). So, you might
//	want to adjust the in-game Brightness setting once you find
//	a shadow / ao amount you like.
//
//	NOTE - softening AO makes shadows show up in near-distance on
//	objects, b/c he sun dot is impacted by the softening. not sure
//	why the game does this. Basically, it's like the closet band
//	of shadows that surrounds the player vehicle works fine, but
//	the secondary band beyond that is cutting off and letting light
// 	leak through directly in areas that should be shadowed. So, you'll
//	notice shadow bands filling in sometimes. Not sure how I can fix this.
//
//	If it gets too annoying, I recommend disabling AO_SOFTEN, and
//	just ramping up AMBIENT_OCCLUSION_COLOR amount to 1.5 or so. It will
//	soften the AO areas, but will also wash-out the vehicles in
//	the select screen. So, there's a draw-back to either way.
//
//-----------------------------------

// for lighter / soft shadows I recommend either:
//
// 	SHADOW_AMOUNT 			0.5		// very soft shadows
// 	AMBIENT_OCCLUSION_COLOR 2.5		// very soft AO (and very washed out vehicle menu)
//
// ... or ...
//	
// 	SHADOW_AMOUNT 			0.6		// soft shadows
// 	AMBIENT_OCCLUSION_COLOR 2.0		// soft AO (washed out vehicle menu)
//
// ... or ...
//	
// 	SHADOW_AMOUNT 			0.7		// reasonable compromise between default FUEL & soft shadows
// 	AMBIENT_OCCLUSION_COLOR 1.5		// ditto.. and vehicle menu not as washed out
//
// vehicles in select menu will be washed out, but shadows will look soft and nice
// The side-benefit of softening the shadows is that the
// dynamic luminance / brightness / eye adaptation won't
// go bonkers and blind you when in shadows. There will be
// a much more faint transition.. and this will also tone
// down bloom, since bloom highlights bright spots on screen.

#define	SHADOW_SOFTEN		1		// 0 = off, 1 = process shadow softening amount
#define	SHADOW_AMOUNT		0.70	// 1.0 = full shadows, 0.75 = light shadows, 0.5 = very faint, 0.0 = no shadows (just turn them off in game options in this case)


// recommend keeping this disabled and ramping up AMBIENT_OCCLUSION_COLOR instead
#define	AO_SOFTEN			0		// 0 = off, 1 = process AO softening amount
#define	AO_AMOUNT			0.90	// 1.0 = full AO, 0.5 = half (faint), 0.0 = no AO

//-----------------------------------
//	AMBIENT_OCCLUSION_COLOR
//-----------------------------------
//
//	Additive: base amount of lighting to add when doing lighting.
//
//	The lighting function is additive...
//
//		  sunlight blocked by shadows	to create shaded areas of "skylight"
//		+ skylight blocked by occlusion to create darker AO areas in shaded areas
//		+ AMBIENT_OCCLUSION_COLOR as final base color to keep AO areas from being pitch black.
//
//	FUEL's original lighting model did a lot of funky stuff to
//	basically make the AO light color 0.65. We can adjust it
//	here to make AO spots in shadows lighter or darker. EG:
//	the dark spot under vehicles. If you set it to a very high
//	amount the AO will "wash out" and no longer add depth to the
//	scene. If you make it too low, then AO areas will look really
//	dark and stick out.
//
//	Since this is used to soften / lighten AO areas caused by
//	the normals dot cancelling out light on the shadow-side of
//	objects and the occlusion in the game darkening spots under
//	trees and on vehicles, we can use it to help lighten those
//	areas instead of AO_AMOUNT / AO_SOFTEN above.
//
//	The problem is that since this is an additive lighting amount,
//	it will start to wash out the colors, as if you're turning up
//	the brightness on the scene. And, the vehicles in the menu
//	will show up bright and having detail washed out on them
//	(as if seen through an over-exposed camera).
//
//	So, there's a catch-22.. you can use AO_SOFTEN to lighten
//	AO areas, but it will make shadows act funky on things. Or,
//	you can use AMBIENT_OCCLUSION_COLOR, but it will wash out
//	things.
//
//	Personally, I prefer using AMBIENT_OCCLUSION_COLOR, and then
//	using an ENB injector (like ReShade) to add in a color enhancing
//	shader to boost the colors back up.
//
//	The AO_SOFTEN will help blend shadows much better then
//	AMBIENT_OCCLUSION_COLOR, but causing shadow clipping is
//	annoying.
//
//	So, there's a catch-22 on both methods.
//
//-----------------------------------

#define	AMBIENT_OCCLUSION_COLOR		1.5// 0.5 = approx FUEL default // 0 = pitch black AO areas // 2.0 = Light AO & washed out vehicle menu




#endif