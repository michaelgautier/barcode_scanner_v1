#ifndef inc_gautier_ui_main_window
#define inc_gautier_ui_main_window

#include <gtkmm.h>
#include <glibmm.h>

#include "barcode_record.hpp"

class main_window : public Gtk::ApplicationWindow {
    public:
        main_window();
    private:
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
};
#endif
