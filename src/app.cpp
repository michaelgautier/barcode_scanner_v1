#include "application.hpp"
#include "main_window.hpp"

int main ( int argc, char** argv )
{
    Glib::RefPtr<gautier_application> app = Glib::make_refptr_for_instance<gautier_application> ( new gautier_application() );

    return app->make_window_and_run<main_window> ( argc, argv );
}
