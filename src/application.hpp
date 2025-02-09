#ifndef inc_gautier_application
#define inc_gautier_application

#include "config.h"
#include <gtkmm.h>
#include <glibmm.h>

class gautier_application : public Gtk::Application {
    public:
        gautier_application();
    protected:
        void on_startup() override;
};
#endif
