#include "Direct.hpp"

#include "types.hpp"

#include "Action.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "Renderer.hpp"
#include "Sound.hpp"
#include "World.hpp"

namespace Direct
{
	void InitDDraw()
	{

		DDSCAPS2     ddscaps;
		HRESULT      ddrval;
		LPDIRECTDRAW pDD;

		// Create the main DirectDraw object
		ddrval = DirectDrawCreate(nullptr, &pDD, nullptr);
		if (ddrval != DD_OK)
			goto error;
		ddrval = pDD->QueryInterface(IID_IDirectDraw4, (LPVOID *)& lpDD);
		if (ddrval != DD_OK)
			goto error;

		// Get exclusive mode
		ddrval = lpDD->SetCooperativeLevel(hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
		if (ddrval != DD_OK)
			goto error;

		// Set the video mode to 800x600x16
		ddrval = lpDD->SetDisplayMode(MAXX, MAXY, 32, 0, 0);
		if (ddrval != DD_OK)
		{
			switch (ddrval)
			{
			case DDERR_GENERIC:
			case DDERR_INVALIDMODE:
			case DDERR_INVALIDOBJECT:
			case DDERR_INVALIDPARAMS:
			case DDERR_LOCKEDSURFACES:
			case DDERR_NOEXCLUSIVEMODE:
			case DDERR_SURFACEBUSY:
			case DDERR_UNSUPPORTED:
			case DDERR_UNSUPPORTEDMODE: {MessageBeep(MB_OK); break; }
			case DDERR_WASSTILLDRAWING:
				;
			}

			goto error;
		}

		ZeroMemory(&ddsd2, sizeof(ddsd2));
		ddsd2.dwSize = sizeof(ddsd2);

		// Create the primary surface with 1 back buffer
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
			DDSCAPS_FLIP |
			DDSCAPS_COMPLEX;
		ddsd.dwBackBufferCount = 1; // Anzahl ??
		ddrval = lpDD->CreateSurface(&ddsd, &lpDDSPrimary, nullptr);
		if (ddrval != DD_OK)
			goto error;

		// für gamma-ablenden
		lpDDSPrimary->QueryInterface(IID_IDirectDrawGammaControl, (void **)&lpDDGammaControl);
		lpDDGammaControl->GetGammaRamp(0, &DDGammaOld);
		lpDDGammaControl->GetGammaRamp(0, &DDGammaRamp);

		ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
		ddrval = lpDDSPrimary->GetAttachedSurface(&ddscaps, &lpDDSBack);

		if (ddrval != DD_OK)
			goto error;


		ddsd.dwSize = sizeof(ddsd);  // Tell DirectDraw which members are valid. 
		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		// In diese Surface sollen die Bausteine geladen werden
		lpDD->CreateSurface(&ddsd, &lpDDSMisc, nullptr);
		lpDDSMisc = DDLoadBitmap(lpDD, Misc, 0, 0);
		DDSetColorKey(lpDDSMisc, RGB(255, 0, 255));
		// In diese Surface sollen das Panel geladen werden
		lpDD->CreateSurface(&ddsd, &lpDDSPanel, nullptr);
		lpDDSPanel = DDLoadBitmap(lpDD, Panel, 0, 0);
		DDSetColorKey(lpDDSPanel, RGB(255, 0, 255));
		// In diese Surface sollen die Animation der Figur gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSGuyAni, nullptr);
		lpDDSGuyAni = DDLoadBitmap(lpDD, GuyAni, 0, 0);
		DDSetColorKey(lpDDSGuyAni, RGB(255, 0, 255));
		// In diese Surface sollen die Landschaftsanimationen gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSAnimation, nullptr);
		lpDDSAnimation = DDLoadBitmap(lpDD, Animation, 0, 0);
		DDSetColorKey(lpDDSAnimation, RGB(255, 0, 255));
		// In diese Surface soll die Schrift1 gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSSchrift1, nullptr);
		lpDDSSchrift1 = DDLoadBitmap(lpDD, Schrift1, 0, 0);
		DDSetColorKey(lpDDSSchrift1, RGB(255, 0, 255));
		// In diese Surface soll die Schrift2 gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSSchrift2, nullptr);
		lpDDSSchrift2 = DDLoadBitmap(lpDD, Schrift2, 0, 0);
		DDSetColorKey(lpDDSSchrift2, RGB(255, 0, 255));
		// In diese Surface soll das Papier gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSPapier, nullptr);
		lpDDSPapier = DDLoadBitmap(lpDD, Papier, 0, 0);
		DDSetColorKey(lpDDSPapier, RGB(255, 0, 255));
		// In diese Surface solln die Bäume gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSBaum, nullptr);
		lpDDSBaum = DDLoadBitmap(lpDD, Baum, 0, 0);
		DDSetColorKey(lpDDSBaum, RGB(255, 0, 255));
		// In diese Surface solln die Cursor gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSCursor, nullptr);
		lpDDSCursor = DDLoadBitmap(lpDD, Cursorbmp, 0, 0);
		DDSetColorKey(lpDDSCursor, RGB(255, 0, 255));
		// In diese Surface solln die Buttons gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSButtons, nullptr);
		lpDDSButtons = DDLoadBitmap(lpDD, Buttons, 0, 0);
		DDSetColorKey(lpDDSButtons, RGB(255, 0, 255));
		// In diese Surface solln das TextFeld gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSTextFeld, nullptr);
		lpDDSTextFeld = DDLoadBitmap(lpDD, TextFeld, 0, 0);
		DDSetColorKey(lpDDSTextFeld, RGB(255, 0, 255));
		// In diese Surface solln das Inventar gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSInventar, nullptr);
		lpDDSInventar = DDLoadBitmap(lpDD, Inventarbmp, 0, 0);
		DDSetColorKey(lpDDSInventar, RGB(255, 0, 255));
		// In diese Surface solln die Bauwerke gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSBau, nullptr);
		lpDDSBau = DDLoadBitmap(lpDD, Bau, 0, 0);
		DDSetColorKey(lpDDSBau, RGB(255, 0, 255));
		// In diese Surface solln die Credits gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSCredits, nullptr);
		lpDDSCredits = DDLoadBitmap(lpDD, Credits, 0, 0);
		DDSetColorKey(lpDDSCredits, RGB(0, 0, 0)); //Ausnahmsweise schwarz
		// In diese Surface solln das Logo gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSLogo, nullptr);
		lpDDSLogo = DDLoadBitmap(lpDD, Logo, 0, 0);


		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);  // Tell DirectDraw which members are valid. 
		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		// In diese Surface soll die MiniMap gespeichert werden
		ddsd.dwWidth = 2 * MAXXKACH;
		ddsd.dwHeight = 2 * MAXYKACH;
		lpDD->CreateSurface(&ddsd, &lpDDSKarte, nullptr);
		// In diese Surface soll die Landschaft gespeichert werden
		ddsd.dwWidth = MAXSCAPEX;
		ddsd.dwHeight = MAXSCAPEY;
		lpDD->CreateSurface(&ddsd, &lpDDSScape, nullptr);
		// In diese Surface soll die Schrift gespeichert werden
		ddsd.dwWidth = MAXX;
		ddsd.dwHeight = MAXY;
		lpDD->CreateSurface(&ddsd, &lpDDSSchrift, nullptr);
		DDSetColorKey(lpDDSSchrift, RGB(255, 0, 255));
		// In diese Surface soll die Schatzkarte gespeichert werden
		ddsd.dwWidth = SKARTEX;
		ddsd.dwHeight = SKARTEY;
		lpDD->CreateSurface(&ddsd, &lpDDSSchatzkarte, nullptr);


		ddbltfx.dwSize = sizeof(ddbltfx);
		ddpf.dwSize = sizeof(ddpf);
		lpDDSSchrift->GetPixelFormat(&ddpf);

	error:;
		/*
		finiObjects();
		MessageBox( hwnd, "DirectDraw Init FAILED", TITLE, MB_OK );
		DestroyWindow( hwnd );
		*/
	}

	void InitDInput()
	{
		DirectInputCreate(g_hInst, DIRECTINPUT_VERSION, &g_pDI, nullptr);			// DirectInput
		g_pDI->CreateDevice(GUID_SysMouse, &g_pMouse, nullptr);						// Mousepointer
		g_pMouse->SetDataFormat(&c_dfDIMouse);										// MausDateninformation einstellen
		g_pMouse->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);	// Exklusive Maus

		g_pDI->CreateDevice(GUID_SysKeyboard, &g_pKey, nullptr);					// Keyboard einrichten
		g_pKey->SetDataFormat(&c_dfDIKeyboard);										// Datenformat auf KeyBoard umschalten
		g_pKey->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);	// KeyBoard geht nicht exklusive
	}

	void InitDSound()
	{
		HRESULT hr;
		WAVEFORMATEX Waveformat; // Zwischenspeicher des Bufferformats

		Soundzustand = 1; // Sound anschalten

		hr = DirectSoundCreate8(nullptr, &lpds, nullptr); // DirectSound-Objekt machen
		if (hr != DD_OK)
		{
			Soundzustand = -1;
			return;
		}
		hr = IDirectSound_SetCooperativeLevel(lpds, hwnd, DSSCL_PRIORITY); // Prioritäten setzen
		if (hr != DD_OK)
		{
			Soundzustand = -1;
			return;
		}

		// Primary Buffer
		ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
		dsbdesc.dwSize = sizeof(DSBUFFERDESC);
		dsbdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
		hr = lpds->CreateSoundBuffer(&dsbdesc, &lpdsbPrimary, nullptr);
		if (hr != DD_OK)
		{
			Soundzustand = -1;
			return;
		}

		memset(&Waveformat, 0, sizeof(WAVEFORMATEX));
		Waveformat.wFormatTag = WAVE_FORMAT_PCM;
		Waveformat.nChannels = 1; // stereo oder mono (1 oder 2) 
		Waveformat.nSamplesPerSec = 44100;
		Waveformat.wBitsPerSample = 16;
		Waveformat.nBlockAlign = Waveformat.wBitsPerSample / 8 * Waveformat.nChannels;
		Waveformat.nAvgBytesPerSec = Waveformat.nSamplesPerSec * Waveformat.nBlockAlign;
		hr = lpdsbPrimary->SetFormat(&Waveformat);
		if (hr != DD_OK)
		{
			Soundzustand = -1;
			return;
		}
	}

	void SetAcquire()
	{
		if (!MouseInit) return;
		if (bActive)
		{
			g_pMouse->Acquire();
			if (g_pKey)g_pKey->Acquire();

		}
		else
		{
			g_pMouse->Unacquire();
			if (g_pKey)g_pKey->Unacquire();

		}
	}

	void CheckMouse()
	{
		DIMOUSESTATE	dims;			// Da werden die Daten der Maus gespeichert
		short			Button;			// Welcher Knopf ist gedrückt worden			
		short			Push;			// Knopf gedrückt(1) oder losgelassen(-1) oder gedrückt(0) gehalten
		short			xDiff, yDiff;	// Die Differenz zur vorherigen Position ((Für Scrollen)

		g_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &dims);
		// Mausbewegung
		xDiff = MousePosition.x;
		MousePosition.x += (short)dims.lX;
		xDiff -= MousePosition.x;
		if (MousePosition.x < 0) MousePosition.x = 0;
		if (MousePosition.x > MAXX - 2) MousePosition.x = MAXX - 2;
		yDiff = MousePosition.y;
		MousePosition.y += (short)dims.lY;
		yDiff -= MousePosition.y;
		if (MousePosition.y < 0) MousePosition.y = 0;
		if (MousePosition.y > MAXY - 2) MousePosition.y = MAXY - 2;

		if (TwoClicks == -1)
		{
			if (Guy.Aktiv)
			{
				if (Math::InRect(MousePosition.x, MousePosition.y, Bmp[BUTTSTOP].rcDes) &&
					(Bmp[BUTTSTOP].Phase != -1)) CursorTyp = CUPFEIL;
				else CursorTyp = CUUHR;
			}
			else CursorTyp = CUPFEIL;
		}
		Button = -1;

		if (dims.rgbButtons[0] & 0x80)
		{
			Button = 0;
			if (Button0down) Push = 0;
			else
			{
				Push = 1;
				Button0down = true;
			}
		}
		else
		{
			if (Button0down)
			{
				Button = 0;
				Push = -1;
				Button0down = false;
			}
		}


		if (dims.rgbButtons[1] & 0x80)
		{
			Button = 1;
			if (Button1down) Push = 0;
			else
			{
				Push = 1;
				Button1down = true;
			}
		}
		else
		{
			if (Button1down)
			{
				Button = 1;
				Push = -1;
				Button1down = false;
			}
		}

		// Wenn ein Text steht, dann bei Mausdruck Text weg 
		if (PapierText != -1)
		{
			if (Frage == 0)
			{
				if (Math::InRect(MousePosition.x, MousePosition.y, Bmp[JA].rcDes))
				{
					CursorTyp = CUPFEIL;
					if ((Button == 0) && (Push == 1))
					{
						Frage = 1;
						Renderer::Textloeschen(TXTPAPIER);
						PapierText = -1;
						Guy.Aktiv = false;
						Sound::PlaySound(WAVKLICK2, 100);
					}
				}
				else if (Math::InRect(MousePosition.x, MousePosition.y, Bmp[NEIN].rcDes))
				{
					CursorTyp = CUPFEIL;
					if ((Button == 0) && (Push == 1))
					{
						Frage = 2;
						Renderer::Textloeschen(TXTPAPIER);
						PapierText = -1;
						Guy.Aktiv = false;
						Sound::PlaySound(WAVKLICK2, 100);
					}
				}
				else if ((Button == 0) && (Push == 1)) Sound::PlaySound(WAVKLICK, 100);
			}
			else if ((Button != -1) && (Push == 1))
			{
				Renderer::Textloeschen(TXTPAPIER);
				PapierText = -1;
				Guy.Aktiv = false;
			}
			return;

		}

		// Animationen und Text löschen (werden dann von den MouseIn.. Sachen neu angestellt
		Renderer::Textloeschen(TXTTEXTFELD);
		Math::ButtAniAus();

		// Wenn der Guy aktiv dann linke Mouse-Buttons ignorieren
		if ((Guy.Aktiv) && (Button == 0))
		{
			if ((Math::InRect(MousePosition.x, MousePosition.y, Bmp[BUTTSTOP].rcDes)) &&
				(Bmp[BUTTSTOP].Phase != -1));
			else Button = -1;
		}

		// die Maus ist in der Spielflaeche ->
		if (Math::InRect(MousePosition.x, MousePosition.y, rcSpielflaeche))
			Math::MouseInSpielflaeche(Button, Push, xDiff, yDiff);
		// die Maus ist im Panel ->
		if (Math::InRect(MousePosition.x, MousePosition.y, rcPanel))
			Math::MouseInPanel(Button, Push);
		/*
		dims.lX, dims.lY, dims.lZ,
		(dims.rgbButtons[0] & 0x80) ? '0' : ' ',
		(dims.rgbButtons[1] & 0x80) ? '1' : ' ',
		(dims.rgbButtons[2] & 0x80) ? '2' : ' ',
		(dims.rgbButtons[3] & 0x80) ? '3' : ' ');
		*/
	}

	short CheckKey()
	{
#define KEYDOWN(name,key) (name[key] & 0x80) 
		char     buffer[256];
		short	 x;

		g_pKey->GetDeviceState(sizeof(buffer), (LPVOID)&buffer);


		if (Spielzustand == SZLOGO)
		{
			if ((KEYDOWN(buffer, DIK_ESCAPE)) || (KEYDOWN(buffer, DIK_RETURN)) ||
				(KEYDOWN(buffer, DIK_SPACE))) // Logo Abbrechen
			{
				Sound::StopSound(WAVLOGO);
				Game::NeuesSpiel(false);
				return(2);
			}
		}
		else if (Spielzustand == SZINTRO)
		{
			if ((KEYDOWN(buffer, DIK_ESCAPE)) || (KEYDOWN(buffer, DIK_RETURN)) ||
				(KEYDOWN(buffer, DIK_SPACE)))	// Intro Abbrechen
			{
				Sound::StopSound(WAVSTURM);		// Sound hier sofort stoppen
				Sound::StopSound(WAVSCHWIMMEN);	// Sound hier sofort stoppen
				Guy.Aktiv = false;
				for (x = Guy.Pos.x; x<MAXXKACH; x++)
				{
					Guy.Pos.x = x;
					World::Entdecken();
					if (Scape[Guy.Pos.x][Guy.Pos.y].Art != 1) break;
				}
				Scape[Guy.Pos.x - 2][Guy.Pos.y].Objekt = WRACK;
				Scape[Guy.Pos.x - 2][Guy.Pos.y].ObPos.x = (short)Bmp[WRACK].rcDes.left;
				Scape[Guy.Pos.x - 2][Guy.Pos.y].ObPos.y = (short)Bmp[WRACK].rcDes.top;

				Guy.PosScreen.x =
					(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][0].x +
					Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][2].x) / 2;
				Guy.PosScreen.y =
					(Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][1].y +
					Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][3].y) / 2;
				RouteStart.x = -1;
				RouteStart.y = -1;
				RouteZiel.x = -1;
				RouteZiel.y = -1;
				Camera.x = Guy.PosScreen.x - rcSpielflaeche.right / 2;
				Camera.y = Guy.PosScreen.y - rcSpielflaeche.bottom / 2;
				if (BootsFahrt) World::ChangeBootsFahrt();
				Guy.Zustand = GUYLINKS;
				Guy.Aktion = Action::NOTHING;
				Spielzustand = SZSPIEL;
				Guy.PosAlt = Guy.PosScreen;
				Game::SaveGame();
				return(1);
			}
		}
		else if (Spielzustand == SZGERETTET)
		{
			if ((KEYDOWN(buffer, DIK_ESCAPE)) || (KEYDOWN(buffer, DIK_RETURN)) ||
				(KEYDOWN(buffer, DIK_SPACE)))
			{
				Spielzustand = SZABSPANN;
				return(1);
			}
		}
		else if (Spielzustand == SZSPIEL)
		{
			if (KEYDOWN(buffer, DIK_RIGHT)) Camera.x += 10;
			if (KEYDOWN(buffer, DIK_LEFT))  Camera.x -= 10;
			if (KEYDOWN(buffer, DIK_DOWN))	Camera.y += 10;
			if (KEYDOWN(buffer, DIK_UP))    Camera.y -= 10;
			if (KEYDOWN(buffer, DIK_ESCAPE))
			{
				Guy.AkNummer = 0;
				Guy.Aktiv = false;
				Guy.Aktion = Action::QUIT;
			}
			if (KEYDOWN(buffer, DIK_F11))
			{
				Guy.AkNummer = 0;
				Guy.Aktiv = false;
				Guy.Aktion = Action::RESTART;
			}
			if (KEYDOWN(buffer, DIK_G))
			{
				Gitter = !Gitter;
				World::Generate();
			}
			if (KEYDOWN(buffer, DIK_A))
			{
				LAnimation = !LAnimation;
				World::Generate();
			}

			/*
			if (KEYDOWN(buffer, DIK_C)) // Zum testen
			{
				short x,y;
				for (y=0;y<MAXYKACH;y++)
				for (x=0;x<MAXXKACH;x++)
				Scape[x][y].Entdeckt = true;
				World::Generate();
			}

			if (KEYDOWN(buffer, DIK_I))	// Zum testen
			{
				Guy.Inventar[ROHAST] = 10;
				Guy.Inventar[ROHSTEIN] = 10;
				Guy.Inventar[ROHBLATT] = 10;
				Guy.Inventar[ROHLIANE] = 10;
				Guy.Inventar[ROHSTAMM] = 9;
			}
			if (KEYDOWN(buffer, DIK_W))	// Zum testen
			{
				Guy.Inventar[ROHAXT]   = 1;
				Guy.Inventar[ROHEGGE]  = 1;
				Guy.Inventar[ROHANGEL]  = 1;
				Guy.Inventar[ROHHAMMER]   = 1;
				Guy.Inventar[ROHFERNROHR] = 1;
				Guy.Inventar[ROHSTREICHHOLZ] = 1;
				Guy.Inventar[ROHSCHAUFEL] = 1;
				Guy.Inventar[ROHKARTE] = 1;
				Guy.Inventar[ROHSCHLEUDER] = 1;

				Bmp[BUTTFAELLEN].Phase  = 0;
				Bmp[BUTTANGELN].Phase  = 0;
				Bmp[BUTTANZUENDEN].Phase  = 0;
				Bmp[BUTTAUSSCHAU].Phase = 0;
				Bmp[BUTTSCHATZKARTE].Phase = 0;
				Bmp[BUTTSCHATZ].Phase = 0;
				Bmp[BUTTSCHLEUDER].Phase = 0;
				Bmp[BUTTFELD].Phase  = 0;
				Bmp[BUTTBOOT].Phase  = 0;
				Bmp[BUTTROHR].Phase  = 0;
				Bmp[BUTTHAUS1].Phase = 0;
				Bmp[BUTTHAUS2].Phase = 0;
				Bmp[BUTTHAUS3].Phase = 0;
			}
			*/

			if (KEYDOWN(buffer, DIK_S))	// Sound ausschalten
			{
				if (Soundzustand == 0) Soundzustand = 1;
				else if (Soundzustand == 1) Soundzustand = 0;
			}
		}
		else if (Spielzustand == SZABSPANN)
		{
			if ((KEYDOWN(buffer, DIK_ESCAPE)) || (KEYDOWN(buffer, DIK_RETURN)) ||
				(KEYDOWN(buffer, DIK_SPACE)))
			{
				Sound::StopSound(WAVABSPANN);
				return(0);
			}
		}
		return(1);
	}

} // namespace Direct