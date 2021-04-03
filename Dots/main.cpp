#include "demopch.h"

#include "./headers/effects.h"
#include "./headers/MathUtils.h"
#include "./headers/Sequencer.h"
#include "./headers/ColorSequencer.h"

void PrintFPS(voxie_frame_t& vf, voxie_wind_t& vw, float dt)
{
	const float fontSize = 0.05f;
	float fps = 1 / dt;
	char str[256];
	sprintf(str, "%1.1f", fps);
	voxie_printalph(&vf, vPtr(vec3(-1.f, -1.f, vw.aspz)), vPtr(vec3(fontSize, 0.f, 0.f)), vPtr(vec3(0.f, 0.f, -fontSize)), 0xffffff, str);
}

void DrawDebugFrame(voxie_frame_t& vf)
{
	const float len = 0.3f;
	voxie_drawlin(&vf, 0.f, 0.f, 0.f, len, 0.f, 0.f, 0xff0000);
	voxie_drawlin(&vf, 0.f, 0.f, 0.f, 0.f, len, 0.f, 0x00ff00);
	voxie_drawlin(&vf, 0.f, 0.f, 0.f, 0.f, 0.f, len, 0x0000ff);
}

std::vector<std::string> pages = {
										("Thank you for watching\nHolo Grail by Holo Sapiens\nfor the revision 2021\nMade with love in canada\nSpecial thanks to:\nGavin and Matthew from Voxon\nAnd to my \"demoscene\" Friends\nAtc barti codac defklf Elgogo\nenitalp fks guille hellflip jamie"),
										("Karl Kortal Made Maxxout Mike24\nMoby Moonnove Niko Nop Orion\nPatapom Rahow Readywolf\nRicks SgtSlayer shadeone sigfrid\nSoundy Spk Zappy\nall my \"shadertoy\" friends\nEidos Montreal & Sherbrooke Teams\nall Square-Enix\nSideFx Derivative"),
										("--The End--\nDo you like the music?\nGo to Spotify \"Volkor X\" !\n------\nDance Animation from Mixamo\nUse Fade3d for delaunay meshing\n------\nSee you soon...\nMajor-X and Volkor-X 2021"),
};


int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hpinst, LPSTR cmdline, int ncmdshow)
{
	voxie_wind_t vw;
	if (voxie_load(&vw) < 0) { return(-1); }
	//vw.usecol = 1; //Override color rendering
	if (voxie_init(&vw) < 0) { return(-1); }

#define WHOLEDEMO 1

#if WHOLEDEMO
	ColorSequencer colorSequencer(
		{
			{0.,{0.4f, 1.f, 0.4f}},			// Disolve
			{21.86,{0.25f, 0.25f, 1.f}},	// Water
			{41.72,{1.f, 0.75f, 0.75f}},	// BouncingDots
			{74.82,{1.f, 0.25f, 0.25f}},	// CollapsingTiles
			{88.06,{0.35f, 1.f, 0.35f}},	// Wave
			{101.31,{0.25f, 1.0f, 1.0f}},	// Greeting
			{127.79,{0.35f, 0.35f, 1.f}},	// Plexus
			{154.27,{0.35f, 1.f, 0.35f}},	// Nuke
		}
	, 0.625f);
#else
	ColorSequencer colorSequencer(
		{
			{0.,{0.35f, 1.f, 0.35f}},
		}
	, 0.625f);
#endif

#if WHOLEDEMO
	Sequencer sequencer({
		{ 2., new Dissolve(.2f, 2.0f, 0.5f, 4.2f, 48000, 1.0f, radians(30.0f), David) },
		{ 8.62, new Dissolve(.2f, 2.0f, 0.5f, 4.2f, 48000, 1.25f, radians(30.0f), VolkorXIntro) },
		{ 15.24, new Dissolve(.2f, 2.0f, 0.5f, 4.2f, 48000, 1.6f, radians(30.0f), Present) },
		{ 21.86, new Water(512, { HoloGrail, Grail }, 10.f, 2.f, 1.0f) },
		{ 41.72, new BouncingDots(.5, 34.1f, 1.5f, 1.1f,Octahedron) },
		{ 74.82, new CollapsingTiles(32, 13.24f / 3.0f, 13.24, 0.5f) },
		{ 88.06, new Wave(64, -0.17f, 13.27f, 0.5f, 13.24f / 3.0f) },
		{ 101.31, new Greeting({ {&CNCD, 0.9f }, {&Revision, 1.1f}, {&Cocoon, 1.0f } ,{&Deadliner, 0.9f}, {&Commodore, 1.3f}, {&EidosSherbooke, 1.0f}, {&Canada, 1.1f}, {&Eko, 0.9f}, {&Atari, 1.3f}, {&Fairlight, 0.9f}, {&Farbrausch, 0.9f}, {&FTA, 1.5f}, {&FutureCrew, 1.0f}, {&Hemoroids, 1.0f}, {&Kalisto, 1.2f}, {&Lemon, 1.0f},{&Matilde, 1.1f}, {&Melon, 1.2f}, {&Nooon, 0.9f}, {&Orange, 0.9f}, {&Overlanders, 1.1f}, {&Oxygene, 1.1f}, {&Quebec, 1.2f}, {&Replicants, 0.9f}, {&Sanity, 0.9f}, {&Scoopex, 0.9f}, {&Skarla, 0.9f}, {&Spaceballs, 0.6f}, {&TheUnion, 0.4f}, {&Vectrex, 1.6f}, {&VolkorX, 1.2f} ,{&Voxon, 1.1f}, {&Xmen, 0.9f}}, 200000, 1.0f, 1.655 / 2.0f) },
		{ 127.79, new Plexus(26.48, 1.0f) },
		{ 154.27, new Nuke(-0.18f, 0.30f, 39.72f, 41.5f) },
		{ 194.00, new EndCredit(pages, 13.24f) },
	});
#else
	Sequencer sequencer({
		//{ 0., new Dissolve(.5f, 300.f, 2.f, 3.5f, 32000, 1.0f, David) },
		//{ 0., new Water(512, { HoloGrail, Grail }, 10.f, 2.f, 1.0f) },
		//{ 0., new BouncingDots(.5, 34.1f, 1.5f, 1.1f,Octahedron) },
		//{ 0., new CollapsingTiles(32, 13.24f / 3.0f, 13.24, 0.5f) },
		//{ 0., new Wave(64, -0.17f, 13.27f, 0.5f, 13.27f / 3.0f) },
		//{ 0., new Greeting({ {&CNCD, 0.9f }, {&Revision, 1.1f}, {&Cocoon, 1.0f } ,{&Deadliner, 0.9f}, {&Commodore, 1.3f}, {&EidosSherbooke, 1.0f}, {&Canada, 1.1f}, {&Eko, 0.9f}, {&Atari, 1.3f}, {&Fairlight, 0.9f}, {&Farbrausch, 0.9f}, {&FTA, 1.5f}, {&FutureCrew, 1.0f}, {&Hemoroids, 1.0f}, {&Kalisto, 1.2f}, {&Lemon, 1.0f},{&Matilde, 1.1f}, {&Melon, 1.2f}, {&Nooon, 0.9f}, {&Orange, 0.9f}, {&Overlanders, 1.1f}, {&Oxygene, 1.1f}, {&Quebec, 1.2f}, {&Replicants, 0.9f}, {&Sanity, 0.9f}, {&Scoopex, 0.9f}, {&Skarla, 0.9f}, {&Spaceballs, 0.6f}, {&TheUnion, 0.4f}, {&Vectrex, 1.6f}, {&VolkorX, 1.2f} ,{&Voxon, 1.1f}, {&Xmen, 0.9f}}, 200000, 1.0f, 1.655 / 2.0f) },
		//{ 0., new Lorentz(1000.f) },
		//{ 0., new Plexus(26.48, 1.0f) },
		{ 0., new Nuke(-0.18f, 0.30f, 39.72f, 41.5f) },
		//{ 0., new EndCredit(pages, 13.24f) },
		});
#endif

	sequencer.Init(vw);

/*	
	HyperGeometry hyperGeometry(0.5f, 0.18f,HyperSoccerBall);
	hyperGeometry.Init(vw);
*/
	float speedMultiplier = 1.0f;

	int musicSId = voxie_playsound("./data/Voxon.wav", -1, 100, 100, speedMultiplier);

	double oldTime = voxie_klock() * speedMultiplier;
	double startTime = oldTime;
	voxie_inputs_t in;
	while (!voxie_breath(&in))
	{
		voxie_frame_t vf;
		double	currentTime = voxie_klock() * speedMultiplier;
		float	deltaTime = min(float(currentTime - oldTime), 1/10.f);
		oldTime = currentTime;

		if (voxie_keystat(0x1) || (voxie_keystat(0x38) && voxie_keystat(0x3E)))	 // ESC or Alt-F4: quit
		{ 
			voxie_quitloop();
		}

		voxie_frame_start(&vf);
		// Set view to have a right handed axis:
		// X -> Right
		// Y -> Forward
		// Z -> Up
		voxie_setview(&vf, -vw.aspx, +vw.aspy, +vw.aspz, +vw.aspx, -vw.aspy, -vw.aspz);

		// draw wireframe box around volume if running on emulator
		if (vw.useemu)
		{
			voxie_drawbox(&vf, -vw.aspx, -vw.aspy, -vw.aspz, +vw.aspx, +vw.aspy, +vw.aspz, 1, 0xffffff);
			//DrawDebugFrame(vf);
			// Print frame rate
			PrintFPS(vf, vw, deltaTime);
		}

		sequencer.Tick(currentTime - startTime, deltaTime, &vf);
		colorSequencer.Tick(currentTime - startTime, deltaTime);

		voxie_frame_end(); 
		voxie_getvw(&vw);
	}

	voxie_uninit(0); //Close window and unload voxiebox.dll
	return(0);
}

#if 0
!endif
#endif




