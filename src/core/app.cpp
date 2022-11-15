/**
 * Description:  Application Definition
 * Author:       Alicia Amarilla (smushy) 
 * File Created: November 14, 2022 
*/
#include "app.hpp"

void Render( Core::AppContext* app ) {
    app->renderer.api.ClearBuffer();
    app->renderer.api.SwapBuffers();
}

void Core::OnUpdate( AppContext* app ) {
    Render(app);
}

Core::AppContext Core::CreateContext() {
    AppContext app = {};
    return app;
}
void Core::OnClose( AppContext* app ) {
    app->isRunning = false;
}

void Core::OnResolutionUpdate( AppContext* app, i32 width, i32 height ) {
    app->renderer.api.SetViewport( width, height );
    Render( app );
}
