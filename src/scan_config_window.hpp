#ifndef inc_gautier_ui_scan_config_window
#define inc_gautier_ui_scan_config_window

#include "config.h"
#include <gtkmm.h>
#include <glibmm.h>

#include "scan_config_record.hpp"

class scan_config_window : public Gtk::Window {
    public:
        scan_config_window();
        scan_config_record get_config();
        void set_config ( scan_config_record config );
    private:
        void on_window_show();
        void on_window_exit();

        scan_config_record configuration;

        Gtk::Box main_frame;
        Gtk::Box sameness_level_frame;
        Gtk::Box items_per_container_frame;
        Gtk::Box item_code_length_frame;
        Gtk::Box container_prefix_frame;
        Gtk::Box container_suffix_frame;
        Gtk::Box autoincrement_frame;
        Gtk::Box autoprint_label_frame;
        Gtk::Box export_format_frame;

        Gtk::Label group_label_sameness_level;
        Gtk::Label group_label_group_size;
        Gtk::Label group_label_container_definition;
        Gtk::Label group_label_export_format;

        Gtk::ToggleButton sameness_freeform;
        Gtk::ToggleButton sameness_unique;

        Gtk::Label items_per_container_label;
        Gtk::Entry items_per_container;

        Gtk::Label items_code_length_label;
        Gtk::Entry items_code_length_min;
        Gtk::Entry items_code_length_max;

        Gtk::Label container_prefix_label;
        Gtk::Entry container_prefix;

        Gtk::Label container_suffix_label;
        Gtk::Entry container_suffix;

        Gtk::CheckButton autoincrement_enabled;
        Gtk::CheckButton autoprint_enabled;

        Gtk::ToggleButton tab_delimited_format;
        Gtk::ToggleButton xml_format;
        Gtk::ToggleButton edi856_format;
};

#endif
