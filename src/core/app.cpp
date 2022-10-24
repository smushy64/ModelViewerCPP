#include "global.hpp"
#include "debug.hpp"

#include "platform/event.hpp"
#include "platform/renderer.hpp"
#include "platform/io.hpp"
#include "platform/time.hpp"

#include "app.hpp"
#include "text.hpp"
#include "ui.hpp"
#include "collision.hpp"

#include <glm/vec4.hpp>

Core::FontAtlas DEFAULT_FONT;
bool LoadDefaultFont();

struct AppUI {
    Core::UI::Label*       versionLabel;
    Core::UI::LabelButton* loadButton;
    Core::UI::LabelButton* quitButton;
};
AppUI USER_INTERFACE;
void UICleanUp( AppUI ui );
void InitializeApp( Core::AppData& data );

void UpdateApp( Core::AppData& data ) {
    USER_INTERFACE.loadButton->UpdateState( data.input );
    USER_INTERFACE.quitButton->UpdateState( data.input );
}

void RenderScene( Platform::Renderer* ) {
}

void RenderUI( Platform::Renderer* renderer ) {
    renderer->RenderText( *USER_INTERFACE.versionLabel );
    renderer->RenderTextButton( *USER_INTERFACE.loadButton );
    renderer->RenderTextButton( *USER_INTERFACE.quitButton );
}

void RenderApp( Platform::Renderer* renderer ) {
    renderer->ClearBuffer();{

        RenderScene( renderer );
        RenderUI( renderer );

    } renderer->SwapBuffer();
}

void HandleEvents( Core::AppData& data );
void Core::RunApp( AppData& data ) {
    InitializeApp( data );
    
    f32 lastElapsedTime = 0.0f;
    while( data.isRunning ) {
        f32 elapsedTime = Platform::ElapsedTime();
        data.deltaTime = elapsedTime - lastElapsedTime;

        Platform::ProcessWindowEvents( data );
        HandleEvents( data );

        UpdateApp( data );
        RenderApp( data.renderer );
    }

    // cleanup
    UICleanUp(USER_INTERFACE);
    delete( data.renderer );
}

void HandleEvents( Core::AppData& data ) {
    using namespace Core;

    while( !data.events.IsEmpty() ) {
        Event* event = data.events.Pop();

    switch( event->GetType() ) {
        case Event::Type::QUIT: {
            data.isRunning = false;
            LOG_INFO("App > Quit Event Handled");
        } break;
        case Event::Type::WINDOW_RESIZE: {
            WindowResizeEvent* resizeEvent = (WindowResizeEvent*)event;
            data.screenResolution = resizeEvent->GetResolution();
            data.renderer->SetViewport( data.screenResolution );

            USER_INTERFACE.loadButton->UpdateBounds(); 
            USER_INTERFACE.quitButton->UpdateBounds(); 
        } break;
        case Event::Type::KEY_DOWN: {
            KeyDown* keydown = (KeyDown*)event;
            data.input.keys[ (usize)keydown->GetKeycode() ] = true;
        } break;
        case Event::Type::KEY_UP: {
            KeyUp* keyup = (KeyUp*)event;
            data.input.keys[ (usize)keyup->GetKeycode() ] = false;
        } break;
        case Event::Type::MOUSE_POSITION: {
            MousePositionEvent* mousePos = (MousePositionEvent*)event;
            data.input.pixelMouse = mousePos->GetPixelPosition();
            data.input.screenSpaceMouse = mousePos->GetScreenPosition();
        } break;
        case Event::Type::MOUSE_DOWN: {
            MouseDown* mouseDown = (MouseDown*)event;
            data.input.mouseButtons[(usize)mouseDown->GetMouseCode()] = true;
        } break;
        case Event::Type::MOUSE_UP: {
            MouseUp* mouseDown = (MouseUp*)event;
            data.input.mouseButtons[(usize)mouseDown->GetMouseCode()] = false;
        } break;
        case Event::Type::MOUSE_SCROLL: {
            MouseScroll* mouseScroll = (MouseScroll*)event;
            data.input.mouseScroll = mouseScroll->GetMouseScrollDirection();
        } break;

        default: break;
    }

        delete( event );
    }
}

bool LoadDefaultFont() {
    Platform::File openSansFile = Platform::LoadFile( "./resources/open_sans/OpenSans-Regular.ttf" );
    Core::FontAtlasSettings openSansSettings = {};
    openSansSettings.fromChar  = ' ';
    openSansSettings.toChar    = '~';
    openSansSettings.pointSize = 64.0f;
    DEFAULT_FONT = {};
    bool fontResult = Core::CreateFontAtlas( openSansFile, openSansSettings, DEFAULT_FONT );
    Platform::FreeFile( openSansFile );

    return fontResult;
}

void UICleanUp( AppUI ui ) {
    LOG_INFO( "App > Cleaning up UI . . ." );
    delete( ui.versionLabel );
    delete( ui.loadButton );
    delete( ui.quitButton );
}

void QuitCallback( void* param ) {

#ifdef DEBUG
    if( param == nullptr ) {
        LOG_ERROR( "Quit Button Callback > Parameter is null!" );
        return;
    }
#endif

    Core::AppData* appData = (Core::AppData*)param;
    appData->isRunning = false;
}

void InitializeApp( Core::AppData& data ) {

    if( !Platform::InitializeTimer() ) {
        LOG_ERROR("App > Failed to initialize timer!");
        data.isRunning = false;
        return;
    }

    if(!data.renderer->Initialize()) {
        LOG_ERROR("App > Failed to initialize renderer!");
        data.isRunning = false;
        return;
    }

    if( !LoadDefaultFont() ) {
        LOG_ERROR( "App > Failed to load default font!" );
        data.isRunning = false;
        return;
    }

    USER_INTERFACE.versionLabel = new Core::UI::Label( PROGRAM_TITLE, DEFAULT_FONT );
    USER_INTERFACE.versionLabel->SetAnchorY( Core::YAnchor::BOTTOM );
    USER_INTERFACE.versionLabel->SetPosition( glm::vec2(0.01f, 0.0125f) );
    USER_INTERFACE.versionLabel->SetScale( 0.35f );

    f32 menuXOffset = 0.99f;
    f32 menuScale = 0.65f;

    USER_INTERFACE.loadButton = new Core::UI::LabelButton( "Load Mesh", DEFAULT_FONT );
    USER_INTERFACE.loadButton->SetAnchorX( Core::XAnchor::RIGHT );
    USER_INTERFACE.loadButton->SetPosition( glm::vec2( menuXOffset, 0.1f ) );
    USER_INTERFACE.loadButton->SetScale( menuScale );

    USER_INTERFACE.quitButton = new Core::UI::LabelButton( "Quit Program", DEFAULT_FONT );
    USER_INTERFACE.quitButton->SetAnchorX( Core::XAnchor::RIGHT );
    USER_INTERFACE.quitButton->SetPosition( glm::vec2( menuXOffset, 0.05f ) );
    USER_INTERFACE.quitButton->SetScale( menuScale );
    USER_INTERFACE.quitButton->SetCallback( QuitCallback );
    USER_INTERFACE.quitButton->SetCallbackParameter( (void*)&data );

    data.renderer->LoadFontAtlasBitmap( DEFAULT_FONT );
    data.renderer->UseFontAtlas( DEFAULT_FONT );
    data.renderer->SetClearColor( glm::vec4( 0.15f ) );
}
