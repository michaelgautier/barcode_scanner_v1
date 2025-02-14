#include "application.hpp"

gautier_application::gautier_application()
{
    Glib::set_prgname ( "Gautier_Scanner_v1.E" );
    Glib::set_application_name ( "Gautier Scanner" );

    set_id ( "gautier.scanner.app_v01e" );
    set_flags ( Gio::Application::Flags::NON_UNIQUE );

    return;
}

void gautier_application::on_startup()
{
    Gtk::Application::on_startup();

    return;
}
