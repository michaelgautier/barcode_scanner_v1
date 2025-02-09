#ifndef inc_gautier_ui_main_window
#define inc_gautier_ui_main_window

#include "config.h"
#include <gtkmm.h>
#include <glibmm.h>

#include "barcode_record.hpp"
#include "scan_config_record.hpp"
#include "scan_config_window.hpp"

class main_window : public Gtk::ApplicationWindow {
    public:
        main_window();
    private:
        //Header
        Glib::RefPtr<Gio::Menu> main_menu;

        Gtk::PopoverMenu main_menu_popover;
        Gtk::HeaderBar main_header;
        Gtk::MenuButton main_menu_button;

        //Header Menu Actions
        Glib::RefPtr<Gio::SimpleActionGroup> HeaderActions;

        void on_export();
        void on_configure();
        void on_reset_clicked();
        void on_erase_all_clicked();

        //Contains all visuals
        Gtk::Box main_frame;
        Gtk::Box left_frame;

        //Barcode text entry
        Gtk::Box input_frame; //Contains the input fields
        Gtk::Label barcode_field_label;
        Gtk::Entry barcode_field;

        //Completed Scans
        guint container_count;
        Gtk::ScrolledWindow scanned_barcodes_scroller;
        Gtk::ListBox scanned_barcodes;
        Gtk::CenterBox scan_options_center_frame;
        Gtk::Box scan_options_frame;
        Gtk::Button scan_reset_button;
        Gtk::Button scan_erase_all_button;

        //Barcode List - Active scan
        Gtk::ScrolledWindow barcodes_scroller;
        Gtk::ColumnView barcodes;
        Glib::RefPtr<Gtk::ColumnViewColumn> barcode_column;

        //Barcode List - Data model
        Glib::RefPtr<Gio::ListStore<barcode_record>> barcode_list;
        Glib::RefPtr<barcode_record> barcode_row;//Goes into the barcode_list

        //Signals
        Glib::RefPtr<Gtk::SignalListItemFactory> barcode_list_signal_factory;
        void on_barcode_bind ( const Glib::RefPtr<Gtk::ListItem>& list_item );
        void on_barcode_activate();

        //Scanner Configuration
        scan_config_window* scan_config_win;
        void scan_config_on_close();

        scan_config_record scan_config_data;
};
#endif
