#ifndef BASE_APP_H
#define BASE_APP_H

#include "BaseCommon.h"
#include "../../HQEngine/Source/HQEngineApp.h"
#include "../../HQEngine/Source/HQCamera.h"

#include "../../ThirdParty-mod/MyGUI/include/MyGUI.h"
#include "../../ThirdParty-mod/MyGUI/include/MyGUI_HQEnginePlatform.h"



/*----------BaseApp-------------------------*/
class BaseApp : public HQEngineRenderDelegate, public HQEngineKeyListener, public HQEngineMouseListener {
public:
	BaseApp(const char* rendererAPI, 
			HQLogStream *logStream,
			const char * additionalAPISettings = NULL,
			hquint32 width = 600, hquint32 height = 600);
	//create app and render device using renderer API read from "API.txt" and default log file stream "log.txt"
	BaseApp(const char * additionalAPISettings = NULL, hquint32 width = 600, hquint32 height = 600);
	//create app and render device using renderer API read from "API.txt"
	BaseApp(HQLogStream* logStream, const char * additionalAPISettings = NULL, hquint32 width = 600, hquint32 height = 600);
	~BaseApp();

	void Run();

protected:
	virtual void Update(HQTime dt) = 0;
	virtual void RenderImpl(HQTime dt) = 0;///don't need to call BeginRender() & EndRender()

	virtual void MousePressed(HQMouseKeyCodeType button, const HQPointi &point);
	virtual void MouseReleased(HQMouseKeyCodeType button, const HQPointi &point);
	virtual void MouseMove(const HQPointi &point);

	char m_renderAPI_name[6];//"D3D9" or "GL"
	HQRenderAPI m_renderAPI_type;

	HQRenderDevice *m_pRDevice;
	HQEngineResManager *m_resManager;
	HQEngineEffectManager *m_effectManager;
	HQCamera * m_camera;
	HQSceneNode* m_scene;//the whole scene
	HQSceneNode* m_sceneController;//the scene's transformation controller
	HQSceneNode* m_world;//the whole world

	HQFloat3 m_cameraTransition;//camera's transition per second

	//GUI 
	MyGUI::HQEnginePlatform* m_guiPlatform;
	MyGUI::Gui *m_myGUI;
	MyGUI::TextBox* m_fpsTextBox;

	bool m_leftMousePressed;
	HQPointi m_prevMousePos;

private:
	void Init(const char* rendererAPI,
			HQLogStream *logStream,
			const char * additionalAPISettings,
			hquint32 width, hquint32 height);

	//implement HQEngineRenderDelegate
	virtual void Render(HQTime dt) __FINAL__;

	//implement HQEngineKeyListener
	virtual void KeyReleased(HQKeyCodeType keyCode);
};

#endif