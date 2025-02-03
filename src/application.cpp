#include "application.hpp"

gautier_application::gautier_application()
{
    Glib::set_application_name ( "Gautier Scanner" );

    set_id ( "gautier.scanner.app_v01c" );
    set_flags ( Gio::Application::Flags::NON_UNIQUE );

    return;
}

